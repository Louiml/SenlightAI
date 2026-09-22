Design a C++ function `assign_gpu_resources` that simulates GPU resource assignment for a set of jobs. Each job has an ID, a required GPU usage amount (a positive integer for full-instance jobs, or a fractional value between 0 and 1 for fractional jobs), and a list of GPU instance exclusions (indices that job cannot use). The system has a fixed number of GPU instances (e.g., 4). The function must process jobs in priority order (the input vector is already sorted by priority, highest first), and for each job: if the job is marked as "running" (meaning it already has an assignment from a previous call), verify that its existing assignment still fits without exceeding capacity on any instance, and if so, keep it; otherwise, remove the job from the list. For new jobs (not running), attempt to assign instances: fractional jobs should prefer an instance already partially used (if adding the fractional usage does not exceed 1.0), else a completely free instance; integer jobs require enough free instances (usage=0) and should prefer non-pending instances (ones not used by running jobs) first, then pending instances. The function returns the list of successfully scheduled jobs (with their assigned instance indices stored in a field), and prunes (removes) jobs that cannot be assigned. The GPU instances start with zero usage; running jobs contribute pending usage that must be respected when assigning new jobs. The function must not assign a job to an excluded instance, and must enforce that the total usage per instance never exceeds 1.0. The function should be `const`-correct where possible and avoid global state (the GPU state is passed as a reference to a struct). Provide clear comments explaining the algorithm.
The algorithm simulates a greedy assignment scheduler. First, it initializes usage and pending_usage for all instances to 0. Then it iterates through the list of jobs; for each job marked as running, it increments pending_usage for each instance the job already uses (using its stored coproc_indices), because those instances are reserved for existing running jobs. Next, it processes jobs in priority order. For a running job, it checks if its current assignment still fits: for each assigned instance, verify that `usage[instance] + fractional_part <= 1.0`; if all fit, it confirms the assignment by adding usage and subtracting pending_usage; otherwise, it erases the job from the list. For a non-running job with fractional usage (<1.0), it first scans all usable instances (not excluded) to find one where `(usage[i] > 0 || pending_usage[i] > 0)` and `usage[i] + pending_usage[i] + usage <= 1.0`, and assigns to that; if none, it looks for any instance with `usage[i] == 0` and assigns. For an integer job (usage >= 1), it counts the number of usable instances with `usage[i] == 0`; if that count is less than the required integer usage, the job is pruned. Otherwise, it first assigns as many instances as needed from the set that have `usage[i] == 0 && pending_usage[i] == 0` (non-pending free), then if more are needed, it assigns from the remaining instances with `usage[i] == 0` (which may have pending usage). Each assignment sets the job's coproc_indices to the assigned instance indices. The algorithm ensures that a job is either fully assigned or not at all, and that no instance exceeds capacity. Time complexity is O(J * I) where J is number of jobs and I is number of instances (since each job may scan all instances multiple times); space complexity is O(I) for usage arrays plus O(J) for the output list (if we copy). Edge cases include: running jobs whose assignments become invalid due to over-subscription (must be pruned), fractional jobs that can share an instance with other fractional jobs as long as total <=1.0, integer jobs that must be contiguous in terms of capacity but can use any instances (not necessarily contiguous), and exclusion lists that prevent certain jobs from using certain instances.
#include <vector>
#include <algorithm>
#include <cmath>

// GPU instance state
struct GPUResource {
    int count;                       // number of GPU instances
    std::vector<double> usage;       // current usage per instance (0.0 to 1.0)
    std::vector<double> pending_usage; // usage reserved by running jobs

    GPUResource(int n) : count(n), usage(n, 0.0), pending_usage(n, 0.0) {}

    void clearUsage() {
        std::fill(usage.begin(), usage.end(), 0.0);
        std::fill(pending_usage.begin(), pending_usage.end(), 0.0);
    }
};

// Job representation
struct Job {
    int id;
    double usage;                    // requested usage: <1 fractional, >=1 integer
    bool is_running;                 // true if already assigned from previous call
    std::vector<int> coproc_indices; // assigned instance indices (for running jobs, current; for new, to be filled)
    std::vector<bool> exclusions;    // true if this instance cannot be used

    Job(int id_, double usage_, bool running, const std::vector<bool>& excl)
        : id(id_), usage(usage_), is_running(running), exclusions(excl) {}
};

// Check if job can use instance i
static bool canUseGPU(const Job& job, int i) {
    return !job.exclusions[i];
}

// Increment pending usage for a running job's existing assignment
static void incrementPendingUsage(Job& job, GPUResource& gpu) {
    double x = (job.usage < 1.0) ? job.usage : 1.0;
    for (int i = 0; i < job.usage; ++i) {
        int idx = job.coproc_indices[i];
        gpu.pending_usage[idx] += x;
    }
}

// Check if current running assignment still fits
static bool currentAssignmentOK(const Job& job, const GPUResource& gpu) {
    double x = (job.usage < 1.0) ? job.usage : 1.0;
    for (int i = 0; i < job.usage; ++i) {
        int idx = job.coproc_indices[i];
        if (gpu.usage[idx] + x > 1.0) return false;
    }
    return true;
}

// Confirm current running assignment by adding usage and removing pending
static void confirmCurrentAssignment(Job& job, GPUResource& gpu) {
    double x = (job.usage < 1.0) ? job.usage : 1.0;
    for (int i = 0; i < job.usage; ++i) {
        int idx = job.coproc_indices[i];
        gpu.usage[idx] += x;
        gpu.pending_usage[idx] -= x;
    }
}

// Try to assign a fractional job
static bool getFractionalAssignment(Job& job, GPUResource& gpu) {
    // First: try an instance already partially assigned
    for (int i = 0; i < gpu.count; ++i) {
        if (!canUseGPU(job, i)) continue;
        if ((gpu.usage[i] > 0.0 || gpu.pending_usage[i] > 0.0)
            && (gpu.usage[i] + gpu.pending_usage[i] + job.usage <= 1.0)) {
            job.coproc_indices = {i};
            gpu.usage[i] += job.usage;
            return true;
        }
    }
    // Then: try a completely free instance
    for (int i = 0; i < gpu.count; ++i) {
        if (!canUseGPU(job, i)) continue;
        if (gpu.usage[i] == 0.0) {
            job.coproc_indices = {i};
            gpu.usage[i] += job.usage;
            return true;
        }
    }
    return false;
}

// Try to assign an integer job
static bool getIntegerAssignment(Job& job, GPUResource& gpu) {
    int needed = static_cast<int>(job.usage);
    // Count free instances (usage == 0)
    int nfree = 0;
    for (int i = 0; i < gpu.count; ++i) {
        if (!canUseGPU(job, i)) continue;
        if (gpu.usage[i] == 0.0) ++nfree;
    }
    if (nfree < needed) return false;

    int assigned = 0;
    // First assign non-pending instances (no pending usage)
    for (int i = 0; i < gpu.count && assigned < needed; ++i) {
        if (!canUseGPU(job, i)) continue;
        if (gpu.usage[i] == 0.0 && gpu.pending_usage[i] == 0.0) {
            gpu.usage[i] = 1.0;
            job.coproc_indices.push_back(i);
            ++assigned;
        }
    }
    // Then assign remaining free instances (even if pending)
    for (int i = 0; i < gpu.count && assigned < needed; ++i) {
        if (!canUseGPU(job, i)) continue;
        if (gpu.usage[i] == 0.0) {
            gpu.usage[i] = 1.0;
            job.coproc_indices.push_back(i);
            ++assigned;
        }
    }
    return assigned == needed;
}

// Main function: assign GPU resources to jobs in priority order.
// Input: vector<Job*> sorted by decreasing priority.
// Output: same vector but pruned of jobs that cannot be scheduled.
void assignGPUResources(std::vector<Job*>& jobs, GPUResource& gpu) {
    gpu.clearUsage();

    // Step 1: record pending usage for running jobs
    for (Job* job : jobs) {
        if (job->is_running) {
            incrementPendingUsage(*job, gpu);
        }
    }

    // Step 2: process jobs in priority order
    auto it = jobs.begin();
    while (it != jobs.end()) {
        Job* job = *it;
        if (job->is_running) {
            if (currentAssignmentOK(*job, gpu)) {
                confirmCurrentAssignment(*job, gpu);
                ++it;
            } else {
                it = jobs.erase(it);
            }
        } else {
            if (job->usage < 1.0) {
                if (getFractionalAssignment(*job, gpu)) {
                    ++it;
                } else {
                    it = jobs.erase(it);
                }
            } else {
                if (getIntegerAssignment(*job, gpu)) {
                    ++it;
                } else {
                    it = jobs.erase(it);
                }
            }
        }
    }
}
#include <cassert>
#include <vector>

// Include the solution code here (or link it)
// The following is a standalone test program.

int main() {
    // Setup: 4 GPU instances
    GPUResource gpu(4);

    // Test 1: Simple integer jobs with no exclusions
    std::vector<Job*> jobs1;
    jobs1.push_back(new Job(1, 2.0, false, {false, false, false, false}));
    jobs1.push_back(new Job(2, 1.0, false, {false, false, false, false}));
    jobs1.push_back(new Job(3, 2.0, false, {false, false, false, false}));
    assignGPUResources(jobs1, gpu);
    assert(jobs1.size() == 2); // Jobs 1 and 2 fit; job 3 needs 2 but only 1 free left
    assert(jobs1[0]->coproc_indices.size() == 2);
    assert(jobs1[1]->coproc_indices.size() == 1);
    for (auto* j : jobs1) delete j;

    // Test 2: Fractional sharing
    gpu.clearUsage();
    std::vector<Job*> jobs2;
    jobs2.push_back(new Job(1, 0.3, false, {false, false, false, false}));
    jobs2.push_back(new Job(2, 0.4, false, {false, false, false, false}));
    jobs2.push_back(new Job(3, 0.4, false, {false, false, false, false}));
    jobs2.push_back(new Job(4, 0.5, false, {false, false, false, false}));
    assignGPUResources(jobs2, gpu);
    assert(jobs2.size() == 3); // 1,2,3 can share one instance (0.3+0.4+0.4=1.1 >1? Actually 0.3+0.4=0.7, then 3 fails? Let's check: after 1 & 2 on instance0 usage=0.7, job3 needs 0.4 -> 0.7+0.4=1.1 fails, so it fails; job4 can use instance1 => 3 jobs total)
    for (auto* j : jobs2) delete j;

    // Test 3: Exclusions
    gpu.clearUsage();
    std::vector<Job*> jobs3;
    jobs3.push_back(new Job(1, 1.0, false, {true, false, false, false})); // cannot use instance 0
    jobs3.push_back(new Job(2, 1.0, false, {false, true, false, false})); // cannot use instance 1
    jobs3.push_back(new Job(3, 1.0, false, {false, false, false, false}));
    assignGPUResources(jobs3, gpu);
    assert(jobs3.size() == 3); // All fit: job1 uses instance1, job2 uses instance0, job3 uses instance2
    assert(jobs3[0]->coproc_indices[0] != 0);
    assert(jobs3[1]->coproc_indices[0] != 1);
    for (auto* j : jobs3) delete j;

    // Test 4: Running job must keep assignment, new job can use free instance
    gpu.clearUsage();
    std::vector<Job*> jobs4;
    Job* running = new Job(1, 1.0, true, {false, false, false, false});
    running->coproc_indices = {0}; // already on instance 0
    jobs4.push_back(running);
    Job* newjob = new Job(2, 1.0, false, {false, false, false, false});
    jobs4.push_back(newjob);
    assignGPUResources(jobs4, gpu);
    assert(jobs4.size() == 2);
    assert(jobs4[0]->coproc_indices[0] == 0);
    assert(jobs4[1]->coproc_indices[0] != 0);
    delete newjob; // running job still owned? We'll delete both carefully
    delete running;

    // Test 5: Running job that no longer fits gets pruned
    gpu.clearUsage();
    std::vector<Job*> jobs5;
    Job* run1 = new Job(1, 1.0, true, {false, false, false, false});
    run1->coproc_indices = {0};
    Job* run2 = new Job(2, 1.0, true, {false, false, false, false});
    run2->coproc_indices = {1};
    Job* run3 = new Job(3, 1.0, true, {false, false, false, false});
    run3->coproc_indices = {0}; // conflict
    jobs5.push_back(run1);
    jobs5.push_back(run2);
    jobs5.push_back(run3);
    assignGPUResources(jobs5, gpu);
    assert(jobs5.size() == 2); // run3 pruned due to conflict
    delete run1;
    delete run2;
    delete run3;

    return 0;
}
