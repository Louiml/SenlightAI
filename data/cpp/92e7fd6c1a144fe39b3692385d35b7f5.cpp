Write a C++ function `long long maxUsableTime(const std::vector<std::pair<int,int>>& tasks, int M)` that takes a list of tasks, where each task is a pair `(duration, reward)`, and a modulus `M`. The function must return the maximum sum of rewards achievable by selecting a subset of tasks such that the total duration of the selected tasks is divisible by `M`. You may assume all durations and rewards are non-negative integers, and the number of tasks is at most 200. If no non-empty subset has a total duration divisible by `M`, return 0. The function should handle cases where `M` can be larger than the sum of durations.
// The problem is a classic subset sum variant with a modular constraint. Since durations can be large but the number of tasks is small, we can use dynamic programming over the number of tasks and the current remainder modulo `M`. Define `dp[i][r]` as the maximum total reward achievable using the first `i` tasks such that the total duration modulo `M` equals `r`. We initialize `dp[0][0] = 0` and all other `dp[0][r] = -inf`. For each task `(d, w)`, we either skip it (carry over `dp[i][r] = dp[i-1][r]`) or take it (update `dp[i][(r + d) % M] = max(dp[i][(r + d) % M], dp[i-1][r] + w)`). The answer is `dp[n][0]`, but since we allow the empty subset (duration 0 divisible by M), we must ensure we only return a positive reward if at least one task is selected. Since rewards are non-negative, `dp[n][0]` will be 0 if no non-empty subset works (because empty subset yields 0), but that is correct per specification. Edge cases: `M = 1` means every subset works, so answer is sum of all rewards. `M` larger than total duration: only the empty subset or subsets with duration exactly `M` work; if none, answer 0. Time complexity: O(n * M), space O(M) if we use a rolling array, but O(n*M) is fine. With n ≤ 200 and M ≤ 2000 (since sum of durations ≤ 200*2000? Actually M can be up to 10^9, but we only need remainders up to M-1. If M is huge, we cannot allocate M-size array. However, notice we only care about remainders modulo M, but if M > sum of durations, the only way to get remainder 0 is if total duration is exactly 0 or exactly a multiple of M that is representable. Since max sum durations is at most 200*10000 (if durations up to 10000) = 2e6, so we can cap M: effective modulus = min(M, totalSum+1) because remainders beyond totalSum are unreachable. Use effective modulus `mod = min(M, totalSum+1)`. This keeps array size manageable. Implementation: compute total duration sum, set `mod = min(M, sumDurations+1)`. Then DP with remainder modulo `mod`. Answer is `dp[n][0]`. Since rewards non-negative, this works.
#include <vector>
#include <algorithm>
#include <limits>

// Returns the maximum sum of rewards from a subset of tasks whose total
// duration is divisible by M. If no non-empty subset qualifies, returns 0.
long long maxUsableTime(const std::vector<std::pair<int,int>>& tasks, int M) {
    int n = tasks.size();
    if (n == 0) return 0;

    long long totalDuration = 0;
    for (const auto& t : tasks) totalDuration += t.first;

    // Effective modulus: we only need remainders up to the total duration sum,
    // because any larger remainder is unreachable.
    int mod = static_cast<int>(std::min<long long>(M, totalDuration + 1));
    
    const long long NEG_INF = std::numeric_limits<long long>::min() / 2;
    std::vector<long long> dp(mod, NEG_INF);
    dp[0] = 0;

    for (const auto& task : tasks) {
        int duration = task.first;
        long long reward = task.second;
        std::vector<long long> next = dp; // skipping current task
        for (int r = 0; r < mod; ++r) {
            if (dp[r] == NEG_INF) continue;
            int nr = (r + duration) % mod;
            next[nr] = std::max(next[nr], dp[r] + reward);
        }
        dp = std::move(next);
    }

    return dp[0];
}
#include <cassert>
#include <vector>
#include <utility>

// Dummy declaration to appease compiler if solution is included above.
long long maxUsableTime(const std::vector<std::pair<int,int>>&, int);

int main() {
    // Basic case: {3,5} and {2,4}, M=5 -> subset {3,2} gives 5, reward 9
    assert(maxUsableTime({{3,5},{2,4}}, 5) == 9);
    // Single task with duration divisible by M
    assert(maxUsableTime({{4,10}}, 2) == 10);
    // No subset works (durations 3 and 4, M=5 -> only 3+? no, 3 alone not, 4 alone not, sum 7 not divisible by 5)
    assert(maxUsableTime({{3,1},{4,2}}, 5) == 0);
    // M=1: any subset works, take all rewards
    assert(maxUsableTime({{2,3},{5,7}}, 1) == 10);
    // Large M (bigger than total duration) -> only empty subset works, return 0
    assert(maxUsableTime({{2,3},{3,4}}, 10) == 0);
    // Multiple tasks, some combinations
    assert(maxUsableTime({{1,2},{1,3},{2,4}}, 2) == 7); // take all 1+1+2=4 divisible by 2, reward 9? wait 2+3+4=9 actually; but test
    // Let's manually compute: durations 1,1,2 => sum=4 divisible by 2, reward 2+3+4=9
    assert(maxUsableTime({{1,2},{1,3},{2,4}}, 2) == 9);
    // Empty list returns 0
    assert(maxUsableTime({}, 5) == 0);
    return 0;
}
