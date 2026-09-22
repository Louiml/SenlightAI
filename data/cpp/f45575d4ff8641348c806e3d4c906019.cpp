A university has `n` student workers, each of whom can be assigned tasks that take either 1 hour or 2 hours to complete. There are `m` total hourly tasks to finish, and each task takes exactly 1 hour. A worker can spend at most `t` hours total working. However, a worker who works for `x` hours on 1-hour tasks (where `x` is an integer ≤ `t`) has the remaining `(t - x)` hours to split into 2-hour slots (each slot counts as 2 hours of work toward completing one 2-hour task, but only whole slots count). Given `n`, `m`, and a list of how many 1-hour tasks each worker is *specifically* required to do (each of the `m` tasks is pre-assigned to a specific worker, given by worker indices in the input), write a C++ function `long long minimumTotalHours(long long n, long long m, const std::vector<long long>& assignment)`. The function should return the **minimum integer `t`** such that all `m` tasks can be completed within `t` hours per worker under the rule: for each worker, if they are assigned `f` specific 1-hour tasks, they first do `min(t, f)` of them in 1 hour each, and then they can use `(t - min(t, f)) / 2` additional 2-hour slots (each slot works on a task that ultimately requires 1 hour but counts as 2 hours of their time, though we only count tasks completed, so a worker contributes `min(t,f) + (t - min(t,f))/2` total tasks). The total number of completed tasks across all workers must be at least `m`. The input worker indices are 1-based (so index `i` refers to worker `i-1` in 0-based). The function must handle `m` up to 2×10^5 and `n` up to 2×10^5. Note that `t` can be as large as `2*m` in the worst case. Ensure the function uses efficient binary search.
We are given a multiset of counts `freq[i]` representing how many 1-hour tasks are assigned to worker `i` (indices from `0` to `n-1` after converting 1-based input to 0-based). For a given candidate `t` (total hours per worker), we need to check if all `m` tasks can be completed. For each worker with `f` specific tasks, they can do at most `min(t, f)` of those specific tasks in 1 hour each, and then they have `(t - min(t, f))` leftover hours which they can split into 2-hour blocks, each block completing one additional task (of any type, but we just count tasks). So the total tasks they can complete is `min(t, f) + (t - min(t, f)) / 2`. Summing this over all workers gives the maximum total tasks completable in `t` hours. If this sum is at least `m`, then `t` is feasible. The feasible condition is monotonic: if `t` works, any larger `t` also works (because we can always do less). We binary search the smallest `t` in `[1, 2*m]` such that the sum is `>= m`. Edge cases: when `f` is large and `t` is small, `min(t,f)` is `t`, leaving zero leftover, so a worker contributes exactly `t` tasks. When `f` is small, they finish their specific tasks quickly and then use leftover hours in 2-hour blocks. Note that the division is integer division, so leftover hours not enough for a 2-hour block are wasted. The binary search uses standard `lo` and `hi` with the invariant that `check(lo)` is false (or `lo` is a candidate), and `check(hi)` is true. We start with `lo=1`, `hi=2*m`. Time complexity: `O(log m)` iterations, each `check` iterates over `n` workers, so `O(n log m)` total, which is fine for `n` up to 2×10^5. Space complexity: `O(n)` to store the frequency array.
#include <vector>
#include <algorithm>

// Returns the minimum total hours t such that all m tasks can be completed.
// Each worker i has freq[i] specific 1-hour tasks. For a given t, a worker can do
// min(t, freq[i]) specific tasks in 1 hour each, plus (t - min(t, freq[i]))/2 additional tasks.
long long minimumTotalHours(long long n, long long m, const std::vector<long long>& assignment) {
    // Build frequency array for each worker (0-based).
    std::vector<long long> freq(n, 0);
    for (long long worker : assignment) {
        // Input is 1-based, convert to 0-based.
        freq[worker - 1]++;
    }
    
    // Check if all tasks can be done when each worker has t hours.
    auto canDo = [&](long long t) -> bool {
        long long totalTasks = 0;
        for (long long f : freq) {
            long long oneHour = std::min(t, f);
            long long twoHourParts = (t - oneHour) / 2;
            totalTasks += oneHour + twoHourParts;
            if (totalTasks >= m) {
                return true; // early exit if enough tasks already
            }
        }
        return totalTasks >= m;
    };
    
    // Binary search for the smallest feasible t in [1, 2*m].
    // Invariant: canDo(lo) is false, canDo(hi) is true (hi is always feasible because
    // with t >= 2*m, each worker can do at least their specific tasks or enough).
    long long lo = 1, hi = 2 * m;
    while (lo < hi) {
        long long mid = lo + (hi - lo) / 2;
        if (canDo(mid)) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}
#include <cassert>
#include <vector>

// Function under test (declared above, include the solution code here).
long long minimumTotalHours(long long n, long long m, const std::vector<long long>& assignment);

int main() {
    // Basic case: 1 worker, 1 task, they have 1 hour.
    assert(minimumTotalHours(1, 1, {1}) == 1);
    
    // 1 worker, 3 tasks, all assigned to same worker -> need 3 hours.
    assert(minimumTotalHours(1, 3, {1, 1, 1}) == 3);
    
    // 2 workers, each has 1 task, each can do in 1 hour.
    assert(minimumTotalHours(2, 2, {1, 2}) == 1);
    
    // 3 workers, one has 5 tasks, others have 0. Need 5 hours for that worker.
    assert(minimumTotalHours(3, 5, {1, 1, 1, 1, 1}) == 5);
    
    // 2 workers, each has 2 tasks (total 4 tasks). With t=2, each does min(2,2)=2 tasks => total 4, so answer 2.
    assert(minimumTotalHours(2, 4, {1, 1, 2, 2}) == 2);
    
    // One worker has 3 tasks, another has 0, total 3 tasks. With t=2, worker does min(2,3)=2, leftover 0 => 2 tasks, not enough. t=3 does 3 => answer 3.
    assert(minimumTotalHours(2, 3, {1, 1, 1}) == 3);
    
    // Edge: many tasks, one worker can use 2-hour slots. Worker has 1 specific task, others have 0, total 3 tasks. With t=2: min(2,1)=1, leftover (2-1)/2=0 => total 1. t=3: min(3,1)=1, leftover (3-1)/2=1 => total 2. t=4: min(4,1)=1, leftover (4-1)/2=1 => total 2. t=5: min(5,1)=1, leftover (5-1)/2=2 => total 3. So answer 5.
    assert(minimumTotalHours(2, 3, {1, 1, 2}) == 5);
    
    // Large case: n=100000, m=200000, all tasks to worker 1. Need t=200000.
    std::vector<long long> big_assign(200000, 1);
    assert(minimumTotalHours(100000, 200000, big_assign) == 200000);
    
    // Large case: n=100000, m=200000, each task to a different worker (only 100000 workers, so each gets 2). t=2 each worker does 2 tasks => total 200000, answer 2.
    std::vector<long long> even_assign;
    for (int i = 0; i < 100000; ++i) {
        even_assign.push_back(i + 1);
        even_assign.push_back(i + 1);
    }
    assert(minimumTotalHours(100000, 200000, even_assign) == 2);
    
    return 0;
}
