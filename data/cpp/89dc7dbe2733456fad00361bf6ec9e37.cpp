/*
Write a C++ function that counts the number of distinct ways to climb a staircase of `N` steps, where you can take either 1 or 2 steps at a time. However, some specific steps are designated as "broken" and cannot be landed on. You start at step 0 (ground) and must reach exactly step `N`. The function should take two parameters: `int N` (total number of steps) and a `std::vector<int>` containing the indices of broken steps (indices are 1-based, meaning step number 1 is the first step). The function returns the number of valid ways to reach the top modulo 1,000,000,007 (to handle large results). For `N = 0`, return 1 (the starting position counts as one way). Note that broken steps cannot be used, but you may jump over them (e.g., from step `i-2` to `i` even if `i-1` is broken).
*/
#include <vector>
#include <unordered_set>

// Count ways to climb N steps with 1- or 2-step moves, avoiding broken steps.
// Returns the result modulo 1,000,000,007.
int countWaysToClimb(int N, const std::vector<int>& brokenSteps) {
    const int MOD = 1000000007;
    if (N == 0) {
        return 1;
    }
    
    std::unordered_set<int> broken(brokenSteps.begin(), brokenSteps.end());
    std::vector<long long> dp(N + 1, 0);
    dp[0] = 1;
    
    for (int i = 1; i <= N; ++i) {
        if (broken.find(i) != broken.end()) {
            dp[i] = 0;
        } else {
            long long ways = dp[i - 1];
            if (i >= 2) {
                ways += dp[i - 2];
            }
            dp[i] = ways % MOD;
        }
    }
    
    return static_cast<int>(dp[N]);
}
#include <cassert>
#include <vector>

int main() {
    // Basic case: 5 steps, no broken steps -> Fibonacci-like: 8 ways
    assert(countWaysToClimb(5, {}) == 8);
    
    // N=0 trivial case
    assert(countWaysToClimb(0, {}) == 1);
    
    // One broken step at position 2: from 0->1->3->4->5 or 0->1->3->5 etc.
    // Total ways for N=4 with step 2 broken: sequences: 1,1,1,1; 1,1,2; 2,1,1; 2,2? But 2,2 goes 0->2 broken, invalid. So valid: 1,1,1,1; 1,1,2; 1,2,1? 1->2 broken invalid. So 2 ways? Let's enumerate: step1, step3, step4 (1+1+1), step1, step3, step4? Actually with N=4 broken step 2: 0->1->3->4 (ways=1), 0->1->3->? no 2-step from 3 to 4? from 3 to4 is 1 step, from 2 to4 is 2 steps but 2 broken. So only one way? Let's compute: dp[0]=1, dp[1]=1, dp[2]=0 (broken), dp[3]=dp[2]+dp[1]=1, dp[4]=dp[3]+dp[2]=1. So result=1.
    assert(countWaysToClimb(4, {2}) == 1);
    
    // All steps broken except first and last? N=3 with step 1 and 2 broken -> cannot reach step 3 (only moves from 0 to 1 or 2, both broken) => 0
    assert(countWaysToClimb(3, {1, 2}) == 0);
    
    // Consecutive broken steps in middle: N=5, broken at 3 and 4 -> from 0->1->2->? can't go to 3 or4, from 2 can jump to 5? 2-step from 2 to4 broken, so no. Only way? 0->1->2 then stuck. So result 0.
    assert(countWaysToClimb(5, {3, 4}) == 0);
    
    // Large N to check modulo: N=1000, no broken -> result should be Fibonacci(1001) mod 1e9+7, known value? We just check it's not overflowing.
    int result = countWaysToClimb(1000, {});
    assert(result >= 0 && result < 1000000007);
    
    // Step 1 broken, N=2: can only go 0->2 (2-step) => 1 way
    assert(countWaysToClimb(2, {1}) == 1);
    
    // Step 1 broken, N=3: 0->2->3 (1 step from 2 to3) => 1 way
    assert(countWaysToClimb(3, {1}) == 1);
    
    // Broken steps not in range (e.g., step 0 or N+1) should be ignored
    assert(countWaysToClimb(3, {0, 4}) == 3); // Fibonacci(3+1)=3 ways: 111, 12, 21
    
    // Duplicate broken indices should not affect result
    assert(countWaysToClimb(4, {2, 2}) == 1); // same as {2} for N=4
    
    return 0;
}
// This is a dynamic programming problem on a linear sequence. Let `dp[i]` represent the number of ways to reach step `i`. We initialize `dp[0] = 1` (there is one way to stand at the start). For each step `i` from 1 to `N`, we consider:
// - If step `i` is broken (in the broken set), then `dp[i] = 0` because we cannot land there.
// - Otherwise, `dp[i] = (dp[i-1] + dp[i-2])` because the last move could be a 1-step from `i-1` or a 2-step from `i-2`. For `i = 1`, `dp[-1]` is treated as 0, so we only use `dp[0]`.
// The answer is `dp[N]`. Edge cases include: `N = 0` (return 1), all steps broken except the start (result is 0), and broken consecutive steps (prevents combinations that rely on them). Time complexity is `O(N)` since we iterate once over all steps. Space complexity is `O(N)` for the DP array, but we only need the last two values, so we can reduce it to `O(1)` auxiliary space by using two variables. However, for clarity in a teaching task, an `O(N)` array is acceptable; either approach is valid. The modulo operation is applied after each addition to prevent overflow.
