// Write a C++ function `countWaysToEndAtX(int n, int k, int x)` that counts the number of arrays of length `n` (with positions numbered `1` to `n`) satisfying: the first element is `1`, the last element is `x`, every element is an integer from `1` to `k`, and every pair of adjacent elements must be different. Since the answer can be huge, return it modulo `1e9+7`. The function must handle all constraints where `2 <= n <= 10^5`, `2 <= k <= 10^5`, and `1 <= x <= k`.

// The problem is a classic dynamic programming counting problem. Let `dp[i][state]` represent the number of valid ways to build the first `i` elements (0-indexed positions) such that the last element falls into one of three categories:
// - state `0`: the last element is exactly `1`
// - state `1`: the last element is exactly `x` (but if `x == 1`, this state is defined as "the second special value", but we handle by using only states 0 and 1 as actual target values, and state 2 for "any other value")
// - state `2`: the last element is neither `1` nor `x` (or if `x == 1`, state 2 means "any value other than 1")
//
// We initialize `dp[0][0] = 1` because the first element is fixed to `1`. For each subsequent position `i` (from 1 to n-1), we compute transitions based on the rule that adjacent elements differ. If the previous element was `1` (state 0), then the next element cannot be `1`. It can be `x` (if `x != 1`) or "other" (state 2). If the previous element was `x` (state 1, and `x != 1`), the next can be `1` or "other". If the previous element was "other" (state 2), the next can be any of the `k` values except the previous specific value. But because state 2 groups all "other" values together, we need to account for the count correctly: from a state-2 element (which represents `k-2` distinct values when `x != 1`, or `k-1` distinct values when `x == 1`), the number of ways to go to state 0 (value 1) is exactly 1 (place 1), to state 1 (value x) is exactly 1 (if `x != 1`), and to state 2 is `(k-2)` choices (any value except 1 and the specific previous value? Actually careful: from a state-2 element that is a specific "other" value, we cannot repeat that value but we can choose any other value among the remaining `k-1` values. Among those `k-1` values, exactly one is 1, exactly one is x (if `x != 1`), and the rest are `k-3` other values if `x != 1`, or `k-2` other values if `x == 1`.)
//
// To avoid the state-2 element being a specific value, we use the aggregated DP where `dp[i][2]` sums over all "other" values. Because the transitions from state 2 to state 2 depend on the actual previous value, but the aggregated count works because each specific "other" value has exactly the same number of transitions to each category. So the recurrence is:
// - `dp[i][0]` (ends with 1) = `dp[i-1][1] + dp[i-1][2]` if `x != 1` (since from state 1 (value x) we can place 1, and from any "other" value we can place 1). If `x == 1`, then state 1 is not used; we treat `dp[i-1][1]` as 0 and we still have `dp[i-1][2]`.
// - `dp[i][1]` (ends with x) = `dp[i-1][0] + dp[i-1][2]` if `x != 1`; if `x == 1`, this state is not used and can be ignored.
// - `dp[i][2]` (ends with neither) = `(dp[i-1][0] + dp[i-1][1]) * (k-2) + dp[i-1][2] * (k-3)` when `x != 1` (because from state 0: cannot place 1, can place any of the `k-1` other values, but among those, one is x which is state 1, so the remaining `k-2` go to state 2; similarly from state 1: `k-2` to state 2; from state 2 (which represents one specific "other" value): can place any of the `k-1` values except itself, but among those, one is 1, one is x, so `k-3` go to state 2). When `x == 1`, the formula simplifies: `dp[i][2] = dp[i-1][0]*(k-1) + dp[i-1][2]*(k-2)` (since there is no state 1, from state 0 we cannot place 1 but can place any of the `k-1` others, all of which are "other"; from a specific "other" we cannot repeat it, so `k-1` choices remain, but one of those is 1, so `k-2` are "other").
//
// At the end, if `x == 1`, the answer is `dp[n-1][0]`; otherwise the answer is `dp[n-1][1]`. Base case: `dp[0][0] = 1`, `dp[0][1] = 0`, `dp[0][2] = 0` (since first element is forced to 1).
//
// Edge cases: `n=1` is not allowed by constraints (n>=2), but even if it were, the answer would be 1 if x==1 else 0. `k=2` means only values 1 and 2 exist; if x is the other value, then state 2 is empty (k-2=0), and the recurrences still work. The modulo is applied at each step to avoid overflow.
//
// Time complexity: O(n) with O(1) space because we only keep the previous row. The original code uses O(n) space, but we can optimize to O(1). For clarity, we’ll use O(n) space in the solution but mention O(1) possible. Space complexity: O(n) for the DP table (or O(1) if optimized). We'll provide O(1) space implementation.

#include <vector>

const int MOD = 1000000007;

// Count arrays of length n with first element 1, last element x,
// values in [1,k], and adjacent elements different.
// Return count modulo 1e9+7.
long long countWaysToEndAtX(int n, int k, int x) {
    // dp0: ends with 1, dp1: ends with x (only if x != 1), dp2: ends with other
    long long dp0 = 1; // first element is 1
    long long dp1 = 0;
    long long dp2 = 0;

    for (int i = 1; i < n; ++i) {
        long long new0, new1, new2;
        if (x == 1) {
            // Only states 0 and 2 exist; state 1 is unused (always 0)
            new0 = dp2; // from "other" we can place 1
            new1 = 0;
            // from state0 (value 1) we can place k-1 others; from state2 (specific other) we can place k-2 others (excluding itself and 1)
            new2 = (dp0 * (k - 1) + dp2 * (k - 2)) % MOD;
        } else {
            // x != 1
            new0 = (dp1 + dp2) % MOD; // from x or other
            new1 = (dp0 + dp2) % MOD; // from 1 or other
            // from 1: k-2 others (excluding 1 and x); from x: k-2 others; from other: k-3 others (excluding itself, 1, x)
            new2 = ((dp0 + dp1) * (k - 2) + dp2 * (k - 3)) % MOD;
        }
        dp0 = new0;
        dp1 = new1;
        dp2 = new2;
    }

    return (x == 1) ? dp0 : dp1;
}

#include <cassert>
#include <iostream>

// copy the solution function here or include it

int main() {
    // n=2: [1, x], adjacent must differ, so if x==1 impossible, else exactly 1 way
    assert(countWaysToEndAtX(2, 3, 1) == 0);
    assert(countWaysToEndAtX(2, 3, 2) == 1);
    assert(countWaysToEndAtX(2, 2, 2) == 1);

    // n=3, k=2: only values 1 and 2, sequence alternating
    // x=1: [1,2,1] -> 1
    // x=2: [1,2,1,?] no, n=3: [1,? ,2], must be [1,? ,2] with ? not equal to neighbors -> ? cannot be 1 or 2, impossible -> 0
    assert(countWaysToEndAtX(3, 2, 1) == 1);
    assert(countWaysToEndAtX(3, 2, 2) == 0);

    // n=3, k=3: 
    // x=1: sequences [1, a, 1] with a != 1 => a can be 2 or 3 => 2 ways
    // x=2: sequences [1, a, 2] with a !=1 and a !=2 => a can be 3 => 1 way
    // x=3: similarly 1 way
    assert(countWaysToEndAtX(3, 3, 1) == 2);
    assert(countWaysToEndAtX(3, 3, 2) == 1);
    assert(countWaysToEndAtX(3, 3, 3) == 1);

    // n=4, k=3, x=1: known small case
    // List all? Instead compare with brute force mental: 
    // Let's brute: [1, a, b, 1] with a!=1, b!=a, b!=1. a can be 2 or 3.
    // If a=2: b can be 3 (cannot be 2 or 1) => [1,2,3,1]
    // If a=3: b can be 2 => [1,3,2,1]
    // So 2 ways.
    assert(countWaysToEndAtX(4, 3, 1) == 2);

    // n=4, k=3, x=2: sequences [1,a,b,2] with a!=1, b!=a, b!=2.
    // a=2: b can be 3 (since b!=2 and b!=a? b!=a is b!=2, so b=3 works) => [1,2,3,2]
    // a=3: b can be 1 or 2? b!=3 and b!=2 => b=1 works, b=2 doesn't because b!=2? Actually b!=2 is required, so b=1 works. Also b=2 fails. So [1,3,1,2] works. Also b=3? no.
    // So 2 ways.
    assert(countWaysToEndAtX(4, 3, 2) == 2);

    // Large values to check modulo behavior (not exhaustive but sanity)
    long long result = countWaysToEndAtX(100000, 100000, 50000);
    assert(result >= 0 && result < MOD);

    std::cout << "All tests passed!\n";
    return 0;
}
