// You are given `n` distinct problem-solving sessions, each consisting of `k` tasks that must be completed in a fixed order. Completing all `k` tasks in one session takes `T` total minutes, and the time needed for the `j`-th task in a session is `t[j]` minutes (the same for every session). You have a total of `m` minutes available. A session is considered "fully solved" only if you complete all `k` tasks in that session consecutively without interruption; however, you may also choose to solve individual tasks from partially completed sessions (in the same fixed order as within a session), but once you start a session, you must finish it entirely if you continue at all. You want to maximize the total number of tasks solved (including full sessions). Write a function that, given `n`, `k`, `m`, and the list `t` of task durations (unsorted), returns the maximum number of tasks you can solve. Note that `n` can be up to 10^5, `k` up to 10^5, and `m` up to 10^9, and task times `t[j]` are non-negative integers.

The solution observes that each fully completed session gives exactly `k` tasks and consumes the sum `T` of all task times. Because partial sessions must follow the same order, the most efficient way to use leftover time after choosing `i` full sessions is to pick the smallest `t[j]` tasks first (since you can reorder the task list arbitrarily; the order inside a session doesn't matter if you are only counting tasks). So we sort `t` ascending. For each possible number of full sessions `i` from 0 to `n`, we check if `i*T <= m`. If so, we subtract `i*T` from `m` giving leftover time. We then greedily add tasks from the sorted list one by one, each costing `t[j]`, until the leftover time is exhausted. The total tasks for this `i` is `(k*i) + (number of extra tasks)`. We take the maximum over all valid `i`. Since we iterate `i` up to `n` and for each `i` we loop over all `k` tasks (but the inner loops are nested for `k` and `n-i`, but the total extra tasks considered is at most `k*(n-i)` which could be large), a naive double loop would be O(n*k). However, we can precompute the prefix sums of the sorted `t`, and for each `i` we binary‑search the maximum number of extra tasks affordable with the remaining time. Then the algorithm is O(n log k + k log k) for sorting and O(n log k) for binary searches. Edge cases: if `m` is less than the smallest task time, no tasks are solved; if `n` is very large but `m` small, we break early when `i*T > m`. Time complexity: O(k log k + n log k). Space: O(k) for the sorted array and prefix sums.

#include <vector>
#include <algorithm>
#include <numeric>

// Returns maximum number of tasks that can be completed given:
// n = number of sessions, k = tasks per session, m = total minutes,
// t = vector of task durations (may be unsorted, non-negative).
long long maxTasksCompleted(int n, int k, long long m, const std::vector<long long>& t) {
    std::vector<long long> ts = t;               // copy to sort
    std::sort(ts.begin(), ts.end());

    // Prefix sums of sorted task times
    std::vector<long long> prefix(k + 1, 0);
    for (int j = 0; j < k; ++j) {
        prefix[j + 1] = prefix[j] + ts[j];
    }

    long long totalSessionTime = prefix[k];      // sum of all tasks in one session
    long long best = 0;

    // Try i full sessions
    for (int i = 0; i <= n; ++i) {
        long long costFull = static_cast<long long>(i) * totalSessionTime;
        if (costFull > m) break;                 // no point increasing i further

        long long remaining = m - costFull;

        // Use binary search to find how many extra tasks we can afford
        // from the sorted task list (prefix sums are monotonic)
        int lo = 0, hi = k;
        while (lo < hi) {
            int mid = (lo + hi + 1) / 2;
            if (prefix[mid] <= remaining) {
                lo = mid;
            } else {
                hi = mid - 1;
            }
        }

        long long tasks = static_cast<long long>(i) * k + lo;
        best = std::max(best, tasks);
    }

    return best;
}

#include <cassert>
#include <vector>

long long maxTasksCompleted(int n, int k, long long m, const std::vector<long long>& t);

int main() {
    // Example from original snippet: n=2, k=2, m=10, t=[1,2] -> total tasks? 
    // Full session takes 3, two full sessions take 6, leftover 4 -> can do one session (2 tasks) + two extra tasks (1+2) = 4 tasks.
    assert(maxTasksCompleted(2, 2, 10, {1, 2}) == 4);

    // Cannot do any full sessions, but can do small tasks
    assert(maxTasksCompleted(3, 3, 5, {2, 3, 4}) == 2); // sort: 2,3,4; one extra task (2) and another (3) = 2 tasks

    // Zero time available
    assert(maxTasksCompleted(5, 2, 0, {1, 1}) == 0);

    // Large n, small m
    assert(maxTasksCompleted(100, 5, 3, {1, 1, 1, 1, 1}) == 3); // no full session, 3 tasks

    // All tasks exactly fit into one session
    assert(maxTasksCompleted(1, 3, 6, {1, 2, 3}) == 3);

    // Duplicate times
    assert(maxTasksCompleted(2, 2, 4, {2, 2}) == 2); // one full session (4 min) = 2 tasks

    // m exactly equals two full sessions
    assert(maxTasksCompleted(2, 2, 6, {1, 2}) == 4); // two full sessions = 4 tasks

    // Edge: k=1
    assert(maxTasksCompleted(5, 1, 3, {2}) == 1); // only one task per session, can do one full session

    // Edge: m very large, n limited
    assert(maxTasksCompleted(2, 3, 100, {1, 2, 3}) == 6); // two full sessions = 6 tasks

    return 0;
}
