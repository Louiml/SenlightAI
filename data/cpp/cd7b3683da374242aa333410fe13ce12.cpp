Write a C++ function `int maxEarnedMoney(const std::vector<std::pair<int,int>>& jobs)` that takes a vector of jobs, where each pair contains `(money, deadline)` with `money` being the profit earned and `deadline` being the number of days from today to complete the job (deadline ≥ 1). Each job takes exactly one day to complete, and you can schedule at most one job per day. The goal is to maximize total profit by selecting a subset of jobs that can all be completed before their respective deadlines without conflicts. Return the maximum total profit achievable. If the input vector is empty, return 0. The deadlines are positive integers and can be up to any size. The order of jobs in the input does not matter.
#include <cassert>
#include <vector>
#include <utility>

int maxEarnedMoney(const std::vector<std::pair<int,int>>& jobs); // declaration

int main() {
    // Basic case
    assert(maxEarnedMoney({{100,2}, {50,1}, {10,1}}) == 150);
    // Empty input
    assert(maxEarnedMoney({}) == 0);
    // Single job
    assert(maxEarnedMoney({{5,3}}) == 5);
    // All deadlines same, only one can be selected per day
    assert(maxEarnedMoney({{10,1}, {20,1}}) == 20);
    // Deadlines allow all jobs
    assert(maxEarnedMoney({{10,3}, {20,2}, {30,1}}) == 60);
    // Larger deadline with many jobs
    assert(maxEarnedMoney({{1,5}, {2,5}, {3,5}, {4,5}, {5,5}}) == 15); // take top 5 profits? Actually only 5 days, all can be done: 1+2+3+4+5=15
    // Jobs with far deadlines but limited days
    assert(maxEarnedMoney({{100,10}, {1,1}, {1,2}, {1,3}}) == 103); // take 100 on day 10, 1 on day 3,1 on day2,1 on day1
    // Mixed and duplicates
    assert(maxEarnedMoney({{50,2}, {40,2}, {30,1}, {30,1}}) == 120); // 50 on day2, 40 on day1? Actually day1 has 30,30; best is pick 50 day2 and 30 day1 = 80? Wait check: day2 can take 50 or 40, day1 can take 30 or 30. Best is 50+30=80. Let's correct: 50+30=80. So assert 80.
    // Correction from above
    assert(maxEarnedMoney({{50,2}, {40,2}, {30,1}, {30,1}}) == 80);
    // Large deadline gaps
    assert(maxEarnedMoney({{10,100}, {20,1}}) == 30); // take 20 on day1, 10 on day100
    return 0;
}
#include <vector>
#include <queue>
#include <algorithm>

// Given a list of jobs as (money, deadline) pairs, return the maximum total profit
// achievable by scheduling at most one job per day, each job taking one day.
int maxEarnedMoney(const std::vector<std::pair<int,int>>& jobs) {
    if (jobs.empty()) return 0;
    
    // Sort jobs by deadline descending
    std::vector<std::pair<int,int>> sortedJobs = jobs;
    std::sort(sortedJobs.begin(), sortedJobs.end(),
              [](const std::pair<int,int>& a, const std::pair<int,int>& b) {
                  return a.second > b.second; // higher deadline first
              });
    
    int maxDeadline = 0;
    for (const auto& job : sortedJobs) {
        maxDeadline = std::max(maxDeadline, job.second);
    }
    
    std::priority_queue<int> maxHeap; // max-heap of available profits
    int total = 0;
    int idx = 0;
    int n = sortedJobs.size();
    
    // Iterate days from maxDeadline down to 1
    for (int day = maxDeadline; day >= 1; --day) {
        // Add all jobs with deadline >= current day
        while (idx < n && sortedJobs[idx].second >= day) {
            maxHeap.push(sortedJobs[idx].first);
            ++idx;
        }
        // Schedule the most profitable available job on this day
        if (!maxHeap.empty()) {
            total += maxHeap.top();
            maxHeap.pop();
        }
    }
    return total;
}
// This is a classic scheduling problem that can be solved greedily using a max-heap (priority queue). The key insight is to iterate days from the largest deadline down to day 1. For each day `i`, we consider all jobs whose deadline is at least `i` (meaning they can be scheduled on day `i` or earlier). Among those available jobs, we schedule the one with the highest money on day `i` if any are available. This ensures that we always reserve earlier days for jobs with stricter deadlines and use each day to its maximum profit potential. To implement this efficiently, we first sort jobs by deadline in descending order. Then for each day from `maxDeadline` down to 1, we add all jobs with deadline ≥ current day into a max-heap (keyed by money). If the heap is non-empty, we pop the highest-money job and add its profit to the total. This greedy choice is optimal because any job we take on day `i` cannot be taken later, and taking the highest profit among feasible jobs maximizes gain. Edge cases include empty input (return 0), multiple jobs with same deadline, and deadlines that are very large (the loop must go from the maximum deadline down to 1). Time complexity is O(n log n) due to sorting and heap operations, and space complexity is O(n) for the heap.
