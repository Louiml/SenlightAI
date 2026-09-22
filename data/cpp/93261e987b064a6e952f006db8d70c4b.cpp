Write a C++ function `maxProductAfterBreak(int n)` that, given an integer `n` where `2 <= n <= 58`, returns the maximum product you can obtain by breaking `n` into the sum of at least two positive integers. The order of the summands does not matter; you only need the maximum possible product of the summands. For example, for `n = 2`, the only break is `1 + 1`, product `1`. For `n = 3`, possible breaks are `1+2` (product `2`) and `1+1+1` (product `1`), so the maximum is `2`. For `n = 4`, breaks include `1+3` (3), `2+2` (4), `1+1+2` (2), etc., so the answer is `4`. The function must handle edge cases where the optimal break may include a summand equal to the original number? No—you must break into *at least two* positive integers, so a single summand equal to `n` is not allowed. The solution should use dynamic programming with memoization as a core technique, but you may implement it iteratively or recursively as long as the function is correct and efficient.

#include <cassert>

int main() {
    assert(maxProductAfterBreak(2) == 1);
    assert(maxProductAfterBreak(3) == 2);
    assert(maxProductAfterBreak(4) == 4);
    assert(maxProductAfterBreak(5) == 6);  // 3*2
    assert(maxProductAfterBreak(6) == 9);  // 3*3
    assert(maxProductAfterBreak(7) == 12); // 3*2*2 or 3*4
    assert(maxProductAfterBreak(8) == 18); // 3*3*2
    assert(maxProductAfterBreak(10) == 36); // 3*3*4
    assert(maxProductAfterBreak(58) == 1549681956); // known LeetCode result
    return 0;
}

#include <vector>
#include <algorithm>

// Returns the maximum product from breaking n into at least two positive integers.
int maxProductAfterBreak(int n) {
    // dp[i][s] = maximum product when we are allowed to use summands up to i,
    // and we need to sum to s. -1 means not computed yet.
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(n + 1, -1));
    
    // Recursive helper as a lambda for clean code.
    // Note: We use a reference to dp to avoid copying.
    const auto solve = [&](auto&& self, int i, int sum) -> int {
        if (i == 1) {
            // Only 1's left; product of all remaining summands is 1.
            return 1;
        }
        if (dp[i][sum] != -1) {
            return dp[i][sum];
        }
        // Option 1: Do not use i as a summand.
        int notPick = self(self, i - 1, sum);
        // Option 2: Use i at least once, if possible.
        int pick = 0;
        if (sum >= i) {
            // Multiply i by the result of further breaking (sum - i) still using i.
            pick = i * self(self, i, sum - i);
        }
        return dp[i][sum] = std::max(notPick, pick);
    };
    
    // Start with largest allowed summand = n-1 (to force at least two parts).
    return solve(solve, n - 1, n);
}

// The problem is identical to LeetCode 343 "Integer Break". The given code uses a recursive memoized DP approach. The state is `(i, sum)` where `i` is the maximum integer we are allowed to use as a summand, and `sum` is the remaining sum we must partition. The base case is when `i == 1`, meaning we can only use 1's, so the product of all remaining summands is `1` (since we need at least two summands overall, and if we reach `i==1` we are forced to fill with 1's). However, note that the given code calls `solve(n-1, n)` and does not enforce "at least two parts" directly; it implicitly handles it because if `n` is broken into a single part, that would require `i == n` and `sum == n`, but the recursion starts at `i=n-1`, so a single part `n` is not possible. At each state, we can either not pick `i` (npick) and recurse on `i-1` with same sum, or if `sum >= i`, we pick `i` and multiply `i` by the product of the remaining `sum-i` using the same `i` (allowing multiple copies of `i`). The answer is the maximum of pick and npick. Edge cases: `n=2` -> solve(1,2) returns 1. `n=3` -> solve(2,3): npick = solve(1,3)=1; pick = 2 * solve(2,1) but sum=1 < 2, so pick=0; result=1. That gives 1, but the correct answer for 3 is 2. Wait—the given code seems to have a flaw? Let's trace: For n=3, `solve(2,3)`:
// - npick = solve(1,3) -> base case returns 1.
// - pick: sum>=2? yes, pick = 2 * solve(2,1). But solve(2,1): i=2, sum=1, base case i==1? No, i=2, so npick=solve(1,1)=1, pick=0 because sum<2, so dp[2][1]=max(1,0)=1. So pick = 2*1 = 2. Then dp[2][3] = max(1,2)=2. So the answer is 2. Good. So the code works for n=3. For n=4, we get 4. The algorithm works. However, note that the code does not explicitly force "at least two parts"—but because we start at `i=n-1`, we cannot use the single part `n`. However, there is a subtle case: For n=2, starting at i=1, solve(1,2) returns 1, correct. For n=3, it works. So it's fine. The key insight: At any state, we can choose to either not use the current integer `i` as a summand, or if the remaining sum is large enough, use it one or more times. The product is maximized by considering all possible partitions with summands <= n-1. Time complexity: O(n^2) states, each O(1) work, so O(n^2) time and O(n^2) space. We can also find a simpler mathematical solution (break into 3's and 2's), but the DP is the requested approach. For the solution, we'll implement a clean recursive memoized function or an iterative DP. We'll produce a function `maxProductAfterBreak` that uses a 2D memo table. Important edge cases: n=2 and n=3 return 1 and 2 respectively. Also note that the product can be large (for n=58, maximum product is 1549681956, fits in 32-bit int). So int is fine. We'll implement with `std::vector` and memoization. We'll also add a lambda or helper function.
