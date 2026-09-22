// Given a vector of jobs where each job is represented as a vector of three integers `{job_id, deadline, profit}`, write a C++ function `jobScheduling` that returns a vector of two integers: the maximum number of jobs that can be scheduled and the maximum total profit achievable, under the constraint that each job takes exactly one unit of time and must be completed before or on its deadline (deadlines are 1-indexed). You may schedule at most one job per time slot (integer time from 1 to the maximum deadline). The function should handle up to 10^5 jobs, deadlines up to 10^5, and profits up to 10^3. All jobs are independent; you may choose any subset and order them to maximize profit. Return the result as `{number_of_jobs, total_profit}`.

#include <cassert>
#include <vector>
#include <iostream>

int main() {
    // Basic test: two jobs, different deadlines and profits
    std::vector<std::vector<int>> jobs1 = {{1, 4, 20}, {2, 1, 10}, {3, 1, 40}, {4, 1, 30}};
    assert(jobScheduling(jobs1) == std::vector<int>({3, 90}));  // pick job1(20), job3(40), job4(30)

    // All jobs have same deadline, pick highest profits
    std::vector<std::vector<int>> jobs2 = {{1, 2, 100}, {2, 2, 19}, {3, 2, 27}};
    assert(jobScheduling(jobs2) == std::vector<int>({2, 127})); // pick 100 and 27

    // Empty input
    std::vector<std::vector<int>> jobs3 = {};
    assert(jobScheduling(jobs3) == std::vector<int>({0, 0}));

    // Single job
    std::vector<std::vector<int>> jobs4 = {{1, 5, 50}};
    assert(jobScheduling(jobs4) == std::vector<int>({1, 50}));

    // Deadlines larger than number of jobs, still schedule all
    std::vector<std::vector<int>> jobs5 = {{1, 10, 5}, {2, 10, 6}, {3, 10, 7}};
    assert(jobScheduling(jobs5) == std::vector<int>({3, 18}));

    // Zero profit jobs should still count if scheduled
    std::vector<std::vector<int>> jobs6 = {{1, 1, 0}, {2, 2, 0}, {3, 2, 10}};
    assert(jobScheduling(jobs6) == std::vector<int>({2, 10})); // schedule job3(10) at t=2, job1(0) at t=1

    // Multiple jobs with same deadline, choose best subset
    std::vector<std::vector<int>> jobs7 = {{1, 3, 30}, {2, 3, 20}, {3, 2, 10}, {4, 2, 40}};
    // slots: t=3 choose 30, t=2 choose 40, t=1 choose 10 or 20 => 30+40+20=90? Wait
    // Actually heap approach: t=3 push 30,20 -> pop 30 ; t=2 push 10,40 -> pop 40 ; t=1 push nothing -> pop 20 => total 3 jobs, profit 90
    assert(jobScheduling(jobs7) == std::vector<int>({3, 90}));

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <vector>
#include <map>
#include <queue>
#include <algorithm>

// Returns {max_jobs, max_profit} for job sequencing with deadlines.
// Each job is {id, deadline, profit}. Deadline is 1-indexed, each job takes 1 time unit.
std::vector<int> jobScheduling(const std::vector<std::vector<int>>& jobs) {
    if (jobs.empty()) return {0, 0};

    int maxDeadline = 0;
    std::map<int, std::vector<int>> jobsByDeadline;
    for (const auto& job : jobs) {
        int deadline = job[1];
        int profit = job[2];
        jobsByDeadline[deadline].push_back(profit);
        maxDeadline = std::max(maxDeadline, deadline);
    }

    std::priority_queue<int> maxHeap;
    int totalJobs = 0;
    int totalProfit = 0;

    // Process time slots from latest to earliest.
    for (int t = maxDeadline; t >= 1; --t) {
        // Add all jobs with this deadline to the heap.
        auto it = jobsByDeadline.find(t);
        if (it != jobsByDeadline.end()) {
            for (int profit : it->second) {
                maxHeap.push(profit);
            }
        }
        // If any job is available, schedule the most profitable one now.
        if (!maxHeap.empty()) {
            totalProfit += maxHeap.top();
            maxHeap.pop();
            ++totalJobs;
        }
    }

    return {totalJobs, totalProfit};
}

// This is the classic job sequencing with deadlines problem. The optimal greedy strategy sorts jobs by profit in descending order, then for each job, tries to assign it to the latest available time slot before its deadline. If no slot is free, the job is skipped. However, a naive O(N * maxDeadline) approach may time out. An efficient approach uses a max-heap (priority queue) and processes time slots from the maximum deadline down to 1. For each time slot `t`, push all jobs whose deadline equals `t` into a max-heap (keyed by profit). Then, if the heap is not empty, pop the job with the highest profit from the heap, schedule it at time `t`, and add its profit to the total. This works because any job with deadline ≥ t can be scheduled at t, and by processing t descending, we consider all jobs that are "due" at or after t. We always pick the most profitable available job for the latest slot, which is optimal due to the exchange argument. Edge cases: empty input (return `{0,0}`), multiple jobs with same deadline, jobs with zero profit (still count if scheduled), and deadlines larger than number of jobs (some slots may be empty). Time complexity: O(M + N log N) where M is max deadline and N is number of jobs (each job is pushed and popped at most once). Space: O(M + N) for the map and heap.
