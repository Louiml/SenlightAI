// Given a positive integer `n` representing the length of a rope (where `n >= 2`), write a C++ function `int maxProductAfterCutting(int n)` that returns the maximum possible product of the lengths of the pieces when the rope is cut into at least two positive integer-length segments. For example, a rope of length 8 can be cut as 3+3+2 giving product 18, or 2+2+2+2 giving product 16, so the answer is 18. The solution must handle all `n` from 2 up to 58 (the problem constraints from the original LeetCode problem), and the function should work efficiently, using dynamic programming or a mathematical greedy approach. You are expected to implement the function in a self-contained manner, with no external dependencies beyond the standard library.

This is a classic integer partition maximization problem, equivalent to a "cutting rope" problem. The key observation is that for any segment length `k`, the product is maximized by breaking lengths into factors of 3 as much as possible, because 3 is the most efficient integer factor (since 3 > e ≈ 2.718, and 3 is the closest integer to e that yields higher product than 2 or 4 when split repeatedly). Specifically, for any `n >= 2`, the optimal strategy is:
- If `n` is 2, the only cut is 1+1, product = 1.
- If `n` is 3, the only cut is 1+2, product = 2 (since 1+1+1 gives product 1, but we must cut at least once, so 1*2=2 is better).
- For `n >= 4`, repeatedly subtract 3 from `n` and multiply the result by 3, until `n` becomes 4, 2, or 0. If the remaining `n` is 4, multiply by 4 (because 4 is better than 3+1 = 3*1=3), if remaining is 2, multiply by 2, if 0, do nothing. This yields the maximum product.

The dynamic programming alternative: `dp[i]` = maximum product for rope length `i`, initialized `dp[0]=0`, `dp[1]=1`. For each `i` from 2 to `n`, for each cut length `j` from 1 to `i-1`, compute `max(dp[i], max(j * (i-j), j * dp[i-j]))`. The term `j * (i-j)` covers cutting into two pieces (no further cuts), and `j * dp[i-j]` covers cutting the remaining piece further. The time complexity for DP is O(n^2) and space O(n). The greedy approach is O(n) time worst-case (or O(1) with modulo math) and O(1) space. Edge cases include `n=2` and `n=3` where the rope must be cut into at least two pieces but the product is small. The greedy approach is simpler and more efficient, and works for all `n` up to 58 (fits in int).

#include <algorithm> // for std::max

// Returns the maximum product obtainable by cutting a rope of length n into at least two positive integer pieces.
// The function uses the greedy optimal strategy: prefer pieces of length 3.
int maxProductAfterCutting(int n) {
    // Base cases: must cut into at least two pieces, so product is forced.
    if (n == 2) return 1; // 1+1 product = 1
    if (n == 3) return 2; // 1+2 product = 2

    int product = 1;
    while (n > 4) {
        product *= 3;
        n -= 3;
    }
    // Now n is 4, 3, or 2 (but n cannot be 3 here because we would have exited earlier)
    // If n == 4, multiply by 4 (better than 3+1). If n == 2, multiply by 2. If n == 0 (when original n was multiple of 3), multiply by nothing.
    if (n != 0) {
        product *= n;
    }
    return product;
}

#include <cassert>

int main() {
    // Base cases
    assert(maxProductAfterCutting(2) == 1);
    assert(maxProductAfterCutting(3) == 2);
    // Simple cases
    assert(maxProductAfterCutting(4) == 4); // 2+2 product=4 (or 1+3 product=3)
    assert(maxProductAfterCutting(5) == 6); // 3+2 product=6
    assert(maxProductAfterCutting(6) == 9); // 3+3 product=9 (or 2+2+2 product=8)
    assert(maxProductAfterCutting(7) == 12); // 3+4 product=12 (better than 3+2+2=12, or 3+3+1=9)
    assert(maxProductAfterCutting(8) == 18); // 3+3+2 product=18
    assert(maxProductAfterCutting(9) == 27); // 3+3+3 product=27
    assert(maxProductAfterCutting(10) == 36); // 3+3+4 product=36
    // Larger value
    assert(maxProductAfterCutting(58) == 1549681956); // 19 threes and one 1? Actually 58 = 3*19 + 1, but we adjust: 3^18 * 4 = 387420489 * 4? Let's compute correctly: 58 mod 3 = 1 => use 3^18 * 4 = 387420489 * 4 = 1549681956
    return 0;
}
