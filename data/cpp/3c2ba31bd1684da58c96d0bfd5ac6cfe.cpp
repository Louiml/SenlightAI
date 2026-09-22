You are given a list of \(N\) jobs, each described by a start time, a finish time, and a weight (profit). Write a standalone C++ function `long long maxWeightSubset(const std::vector<Job>& jobs)` that returns the maximum total weight obtainable by selecting a subset of non-overlapping jobs (i.e., no two selected jobs can overlap in time; a job finishing at time \(t\) and another starting at time \(t\) are considered non-overlapping). The input vector may contain zero jobs (in which case return 0), jobs may be given in any order, and all times and weights are non-negative integers. The solution must efficiently handle up to \(N = 10^5\) jobs with times up to \(10^9\). The function must be `const`-correct and not mutate the input. Do not include a `main` function in the solution code; instead, provide a free function with the specified signature. The test section will include a `main` function that calls your function with various cases and checks results using `assert`.

#include <cassert>
#include <vector>

// Assume Job and maxWeightSubset are defined as in the solution above.

int main() {
    // Test 1: Empty input
    std::vector<Job> empty;
    assert(maxWeightSubset(empty) == 0);

    // Test 2: Single job
    std::vector<Job> single = {{0, 5, 10}};
    assert(maxWeightSubset(single) == 10);

    // Test 3: Two non-overlapping jobs
    std::vector<Job> nonOverlap = {{0, 3, 5}, {4, 7, 6}};
    assert(maxWeightSubset(nonOverlap) == 11);

    // Test 4: Two overlapping jobs, pick larger weight
    std::vector<Job> overlap = {{0, 5, 100}, {2, 4, 50}};
    assert(maxWeightSubset(overlap) == 100);

    // Test 5: Jobs given in arbitrary order
    std::vector<Job> shuffled = {{5, 9, 20}, {0, 2, 10}, {3, 4, 15}};
    // Best: take {0,2,10} and {5,9,20} => 30, or {3,4,15} and {5,9,20} => 35? Wait, {3,4} ends at 4, {5,9} starts at 5, so yes 15+20=35.
    assert(maxWeightSubset(shuffled) == 35);

    // Test 6: Back-to-back jobs (finish == start allowed)
    std::vector<Job> backToBack = {{0, 2, 3}, {2, 5, 4}};
    assert(maxWeightSubset(backToBack) == 7);

    // Test 7: Large weights and zero weights
    std::vector<Job> mixedWeights = {{0, 1, 0}, {1, 2, 5}, {3, 4, 0}};
    assert(maxWeightSubset(mixedWeights) == 5);

    // Test 8: All jobs overlap, choose max
    std::vector<Job> allOverlap = {{0, 10, 5}, {1, 9, 7}, {2, 8, 3}};
    assert(maxWeightSubset(allOverlap) == 7);

    // Test 9: Long chain
    std::vector<Job> chain = {{0, 1, 1}, {1, 2, 1}, {2, 3, 1}, {3, 4, 1}};
    assert(maxWeightSubset(chain) == 4);

    // Test 10: Large case with many jobs (just sanity check performance)
    std::vector<Job> large;
    for (int i = 0; i < 100000; ++i) {
        large.push_back({i, i + 1, i});
    }
    assert(maxWeightSubset(large) == (long long)99999 * 100000 / 2); // sum 0..99999

    return 0;
}

#include <vector>
#include <algorithm>
#include <cstdint>

// A job descriptor with start time, finish time, and weight (profit).
struct Job {
    int start;
    int finish;
    long long weight;
};

// Returns the maximum total weight of a set of non-overlapping jobs.
long long maxWeightSubset(const std::vector<Job>& jobs) {
    int n = static_cast<int>(jobs.size());
    if (n == 0) return 0;

    // Sort by finish time (ascending). If equal, any order works for correctness.
    std::vector<Job> sorted = jobs;
    std::sort(sorted.begin(), sorted.end(),
              [](const Job& a, const Job& b) { return a.finish < b.finish; });

    // Extract finish times for binary search.
    std::vector<long long> finishTimes;
    finishTimes.reserve(n);
    for (const Job& j : sorted) {
        finishTimes.push_back(j.finish);
    }

    // dp[i] = max weight using first i jobs (1-indexed: dp[0] = 0, dp[i] for i=1..n)
    std::vector<long long> dp(n + 1, 0);

    for (int i = 1; i <= n; ++i) {
        const Job& current = sorted[i - 1]; // 0-based index

        // Find the last job (index in sorted, 0-based) with finish <= current.start.
        // Use upper_bound to get first index with finish > current.start, then subtract 1.
        int pos = static_cast<int>(
            std::upper_bound(finishTimes.begin(), finishTimes.end(), current.start)
            - finishTimes.begin()
        ) - 1; // pos is the 0-based index of the last non-overlapping job, or -1 if none

        // dp index for that job is pos+1 (since dp is 1-indexed). If pos == -1, we use dp[0] = 0.
        long long includeWeight = current.weight + dp[pos + 1];
        long long excludeWeight = dp[i - 1];

        dp[i] = std::max(includeWeight, excludeWeight);
    }

    return dp[n];
}

// The problem is a classic weighted interval scheduling (also known as job scheduling with profits) problem. The main algorithm follows these steps:  
// 1. Sort the jobs by their finish time in ascending order. If two jobs have the same finish time, their order relative to each other does not matter for correctness, but sorting consistently helps.  
// 2. For each job, find the latest job that finishes before or at the start time of the current job. Since the jobs are sorted by finish time, we can use binary search (e.g., `std::upper_bound` on finish times to get the first job with finish > start, then subtract 1 index) to find this previous non-overlapping job index.  
// 3. Use dynamic programming: Let `dp[i]` be the maximum total weight obtainable from the first `i` jobs (after sorting). Then `dp[0] = 0`, and for each job `i` (using 1-based indexing for convenience), we have:  
//    `dp[i] = max(dp[i-1], jobs[i].weight + dp[p[i]])`, where `p[i]` is the index (in the sorted list) of the last job that ends before job `i` starts. The first term corresponds to not taking job `i`, the second to taking it.  
// 4. The answer is `dp[N]`.  
// Edge cases: empty input (return 0), jobs with identical start/finish times (they overlap unless start==finish? Actually start <= finish always; if start == finish, then it's a zero-length job and can be taken with any job that starts after or finishes before; but for non-overlap condition, a job with start==finish can coexist with any job that doesn't overlap that single point, but our condition is finish <= next start, so it's fine). Duplicate finish times are handled by the sorting and binary search correctly because we find the last index with finish <= start, which may be an earlier duplicate.  
// Time complexity: Sorting takes \(O(N \log N)\). For each job, binary search costs \(O(\log N)\), so total \(O(N \log N)\). Space complexity: \(O(N)\) for the DP array and possibly for storing finish times for binary search. This is efficient for \(N = 10^5\).
