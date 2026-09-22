Write a C++ function named `minimumPossibleCookingTime` that takes a vector of integers representing durations of cooking tasks (all positive) and returns the minimum possible maximum cooking time when the tasks are optimally distributed between two cooking devices. The function must replicate the logic of the given snippet: use dynamic programming to determine which sums of cooking durations (up to the total sum) are achievable by assigning a subset of tasks to the first device, then compute the minimal value of `max(sum_assigned, total_sum - sum_assigned)`. The input vector may be empty (return 0), and the total sum fits within the constraints of the original problem (N ≤ 100, T ≤ 100 each, so sum ≤ 10000). The function should be `const`-correct and operate on a `const std::vector<int>&` without modifying the input.
The solution follows a classic subset-sum dynamic programming approach. Let `N` be the number of tasks and `S` the total sum of all durations. For each task index `i` from 1 to N, maintain a boolean table `dp[i][s]` indicating whether a subset of the first `i` tasks can sum exactly to `s` (where `s` ranges 0..S). Initialize `dp[0][0] = true` and all other `dp[0][s] = false`. For each task `T[i]`, update `dp[i][s]` as `dp[i-1][s]` (not taking the task) OR `dp[i-1][s-T[i]]` (taking the task, if `s >= T[i]`). This is the standard 0/1 knapsack subset-sum recurrence. After processing all tasks, `dp[N][s]` tells us whether a sum `s` is achievable for device A. The time for device A is `s`, and for device B is `S - s`. The maximum of these two is the makespan for that assignment; we want the minimum such maximum over all achievable `s`. Edge cases: empty input (S=0, dp[0][0]=true, answer=0); if no tasks, the loop over `s` yields only `s=0`, giving max(0,0)=0. The 2D DP table can be optimized to 1D using `vector<bool>`, but here we implement with a 2D vector for clarity. Time complexity is O(N * S) where S ≤ 10000, and space complexity is O((N+1)*(S+1)) ≈ 101*10001 ≈ 1,010,101 booleans, which is acceptable but can be reduced. The algorithm correctly handles all positive durations and duplicate values.
#include <vector>
#include <algorithm>

// Returns the minimum possible maximum cooking time when tasks are split between two devices.
int minimumPossibleCookingTime(const std::vector<int>& durations) {
    int n = durations.size();
    if (n == 0) return 0;

    int sum = 0;
    for (int d : durations) sum += d;

    // dp[i][s] = true if subset of first i tasks can sum to s
    std::vector<std::vector<bool>> dp(n + 1, std::vector<bool>(sum + 1, false));
    dp[0][0] = true;

    for (int i = 1; i <= n; ++i) {
        int t = durations[i - 1];
        for (int s = 0; s <= sum; ++s) {
            bool take = (s >= t) ? dp[i - 1][s - t] : false;
            bool skip = dp[i - 1][s];
            dp[i][s] = take || skip;
        }
    }

    int answer = sum; // worst case if all on one device
    for (int s = 0; s <= sum; ++s) {
        if (dp[n][s]) {
            int makespan = std::max(s, sum - s);
            answer = std::min(answer, makespan);
        }
    }
    return answer;
}
#include <cassert>
#include <vector>

// Declaration (optional, since solution is in same translation unit)
int minimumPossibleCookingTime(const std::vector<int>&);

int main() {
    assert(minimumPossibleCookingTime({}) == 0);
    assert(minimumPossibleCookingTime({1}) == 1);
    assert(minimumPossibleCookingTime({1, 2}) == 2);
    assert(minimumPossibleCookingTime({1, 2, 3}) == 3);
    assert(minimumPossibleCookingTime({2, 3, 4}) == 5); // e.g., {4} vs {2,3} -> max(4,5)=5
    assert(minimumPossibleCookingTime({5, 5, 5, 5}) == 10); // two devices each get 10
    assert(minimumPossibleCookingTime({10, 1, 1, 1, 1}) == 10); // put 10 alone, rest sum 4
    assert(minimumPossibleCookingTime({100, 100}) == 100);
    assert(minimumPossibleCookingTime({1, 1, 1, 1, 1}) == 3); // 3 and 2
    assert(minimumPossibleCookingTime({3, 3, 3, 3, 100}) == 100); // 100 vs 12
    return 0;
}
