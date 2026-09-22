// Write a C++ function that computes priority-based scheduling metrics for a set of processes. The function takes an array of process IDs, their priority numbers (where higher priority value means higher scheduling importance), and burst times (CPU execution times), along with the number of processes. It must sort the processes in descending order of priority, then compute the waiting time (time spent ready before execution) and turnaround time (waiting time + burst time) for each process, assuming non-preemptive scheduling and that the first process in the sorted order has zero waiting time. The function should return a struct or custom data type containing: a vector/sorted list of process IDs in the execution order (highest priority first), individual waiting times, individual turnaround times, the average waiting time (as a double), and the average turnaround time (as a double). The input may have duplicate priorities, and in such cases the relative order of tied-priority processes in the input should be preserved (stable sort). The function must handle edge cases like zero processes (return zeros and empty vectors) and single process (waiting time 0, turnaround = burst time). Include all necessary headers and write the solution as a free function named `computePriorityScheduling`.

// The algorithm follows the classic non-preemptive priority scheduling approach. First, validate the input: if the number of processes is zero, return a result with empty vectors and zero averages. For non-zero counts, create a temporary array of structures (or use a vector of pairs) holding process ID, priority, burst time, and an index to preserve original input order for stable sorting. Sort this array by priority in descending order; for equal priorities, maintain the original input order using the index as a tiebreaker (simulating a stable sort). After sorting, compute waiting times iteratively: the first process (highest priority) has waiting time 0; for each subsequent process `i`, waiting time `[i] = waitingTime[i-1] + burstTime[i-1]` because all previous processes must finish before it starts. Then compute turnaround time for each process as `waitingTime[i] + burstTime[i]`. Accumulate the sums of waiting and turnaround times to calculate averages, casting to double for precision. Edge cases: single process gives waiting time 0 and turnaround equal to its burst time; duplicate priorities must keep input order; no negative burst times are expected (assume positive integers, but handle zero burst times gracefully). Time complexity is O(n log n) due to sorting, space complexity is O(n) for the temporary structures and output vectors.

#include <vector>
#include <algorithm>
#include <numeric>

// Structure to hold scheduling results
struct SchedulingResult {
    std::vector<int> processIds;      // Execution order (highest priority first)
    std::vector<int> waitingTimes;    // Waiting time for each process in order
    std::vector<int> turnaroundTimes; // Turnaround time for each process in order
    double averageWaitingTime = 0.0;
    double averageTurnaroundTime = 0.0;
};

// Compute priority-based scheduling metrics
SchedulingResult computePriorityScheduling(
    const std::vector<int>& processIds,
    const std::vector<int>& priorities,
    const std::vector<int>& burstTimes) {
    
    SchedulingResult result;
    int n = processIds.size();
    
    // Handle empty input
    if (n == 0) {
        return result;
    }
    
    // Create temporary array for sorting with original index for stable tie-breaking
    struct TempProcess {
        int pid;
        int priority;
        int burst;
        int originalIndex;
    };
    
    std::vector<TempProcess> processes(n);
    for (int i = 0; i < n; ++i) {
        processes[i] = {processIds[i], priorities[i], burstTimes[i], i};
    }
    
    // Sort by priority descending, then by original index ascending for stable order
    std::sort(processes.begin(), processes.end(),
        [](const TempProcess& a, const TempProcess& b) {
            if (a.priority != b.priority) {
                return a.priority > b.priority;
            }
            return a.originalIndex < b.originalIndex;
        });
    
    // Extract execution order
    result.processIds.reserve(n);
    for (const auto& p : processes) {
        result.processIds.push_back(p.pid);
    }
    
    // Compute waiting times
    std::vector<int> waiting(n, 0);
    for (int i = 1; i < n; ++i) {
        waiting[i] = waiting[i-1] + processes[i-1].burst;
    }
    
    // Compute turnaround times
    std::vector<int> turnaround(n);
    for (int i = 0; i < n; ++i) {
        turnaround[i] = waiting[i] + processes[i].burst;
    }
    
    // Store results
    result.waitingTimes = std::move(waiting);
    result.turnaroundTimes = std::move(turnaround);
    
    // Compute averages as double
    result.averageWaitingTime = 
        static_cast<double>(std::accumulate(result.waitingTimes.begin(), result.waitingTimes.end(), 0)) / n;
    result.averageTurnaroundTime = 
        static_cast<double>(std::accumulate(result.turnaroundTimes.begin(), result.turnaroundTimes.end(), 0)) / n;
    
    return result;
}

#include <cassert>
#include <cmath>

int main() {
    // Test case 1: Basic example with unique priorities
    {
        std::vector<int> pids = {1, 2, 3};
        std::vector<int> prio = {3, 1, 2};  // Order: pid1 (prio3), pid3 (prio2), pid2 (prio1)
        std::vector<int> bt = {10, 5, 8};
        auto res = computePriorityScheduling(pids, prio, bt);
        assert(res.processIds == std::vector<int>({1, 3, 2}));
        assert(res.waitingTimes == std::vector<int>({0, 10, 18}));
        assert(res.turnaroundTimes == std::vector<int>({10, 18, 23}));
        assert(std::abs(res.averageWaitingTime - (0+10+18)/3.0) < 1e-9);
        assert(std::abs(res.averageTurnaroundTime - (10+18+23)/3.0) < 1e-9);
    }
    
    // Test case 2: Duplicate priorities (stable sort preserving input order)
    {
        std::vector<int> pids = {10, 20, 30};
        std::vector<int> prio = {5, 5, 3};  // pid10 and pid20 tied at priority 5
        std::vector<int> bt = {2, 4, 1};
        auto res = computePriorityScheduling(pids, prio, bt);
        assert(res.processIds == std::vector<int>({10, 20, 30}));  // stable order preserved
        assert(res.waitingTimes == std::vector<int>({0, 2, 6}));
        assert(res.turnaroundTimes == std::vector<int>({2, 6, 7}));
        assert(std::abs(res.averageWaitingTime - (0+2+6)/3.0) < 1e-9);
        assert(std::abs(res.averageTurnaroundTime - (2+6+7)/3.0) < 1e-9);
    }
    
    // Test case 3: Single process
    {
        std::vector<int> pids = {42};
        std::vector<int> prio = {10};
        std::vector<int> bt = {7};
        auto res = computePriorityScheduling(pids, prio, bt);
        assert(res.processIds == std::vector<int>({42}));
        assert(res.waitingTimes == std::vector<int>({0}));
        assert(res.turnaroundTimes == std::vector<int>({7}));
        assert(res.averageWaitingTime == 0.0);
        assert(res.averageTurnaroundTime == 7.0);
    }
    
    // Test case 4: Empty input
    {
        std::vector<int> pids, prio, bt;
        auto res = computePriorityScheduling(pids, prio, bt);
        assert(res.processIds.empty());
        assert(res.waitingTimes.empty());
        assert(res.turnaroundTimes.empty());
        assert(res.averageWaitingTime == 0.0);
        assert(res.averageTurnaroundTime == 0.0);
    }
    
    // Test case 5: All equal priorities (preserve input order)
    {
        std::vector<int> pids = {5, 6, 7, 8};
        std::vector<int> prio = {2, 2, 2, 2};
        std::vector<int> bt = {1, 3, 2, 4};
        auto res = computePriorityScheduling(pids, prio, bt);
        assert(res.processIds == std::vector<int>({5, 6, 7, 8}));
        assert(res.waitingTimes == std::vector<int>({0, 1, 4, 6}));
        assert(res.turnaroundTimes == std::vector<int>({1, 4, 6, 10}));
        assert(std::abs(res.averageWaitingTime - (0+1+4+6)/4.0) < 1e-9);
        assert(std::abs(res.averageTurnaroundTime - (1+4+6+10)/4.0) < 1e-9);
    }
    
    // Test case 6: Reversed priority order
    {
        std::vector<int> pids = {1, 2, 3};
        std::vector<int> prio = {1, 2, 3};  // Sorted: pid3, pid2, pid1
        std::vector<int> bt = {5, 5, 5};
        auto res = computePriorityScheduling(pids, prio, bt);
        assert(res.processIds == std::vector<int>({3, 2, 1}));
        assert(res.waitingTimes == std::vector<int>({0, 5, 10}));
        assert(res.turnaroundTimes == std::vector<int>({5, 10, 15}));
        assert(std::abs(res.averageWaitingTime - (0+5+10)/3.0) < 1e-9);
        assert(std::abs(res.averageTurnaroundTime - (5+10+15)/3.0) < 1e-9);
    }
    
    return 0;
}
