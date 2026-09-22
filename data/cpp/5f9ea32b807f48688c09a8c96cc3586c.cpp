Write a C++ function that, given a positive integer `n` (with `2 <= n <= 58`), returns the maximum product you can obtain by splitting `n` into at least two positive integers that sum to `n`. The function should be named `maxProductAfterSplit` and take an `int` parameter. For example, for `n = 10`, possible splits include `3 + 3 + 4` with product `36`, or `2 + 2 + 2 + 2 + 2` with product `32`; the correct answer is `36`. The function must compute the result efficiently without iterating over all possible splits, and must handle the base cases for small `n` correctly.
int main() {
    assert(maxProductAfterSplit(2) == 1);
    assert(maxProductAfterSplit(3) == 2);
    assert(maxProductAfterSplit(4) == 4);
    assert(maxProductAfterSplit(5) == 6);
    assert(maxProductAfterSplit(6) == 9);
    assert(maxProductAfterSplit(7) == 12);   // 3 + 4
    assert(maxProductAfterSplit(8) == 18);   // 3 + 3 + 2
    assert(maxProductAfterSplit(9) == 27);   // 3 + 3 + 3
    assert(maxProductAfterSplit(10) == 36);  // 3 + 3 + 4
    assert(maxProductAfterSplit(58) == 1549681956); // 3*19 + 1 → but actually 3^18 * 4? Check: 58 = 3*18 + 4 → 3^18 * 4 = 1549681956
    return 0;
}
#include <cassert>

// Returns the maximum product from splitting n into at least two positive integers summing to n.
// Precondition: n >= 2 and n <= 58.
int maxProductAfterSplit(int n) {
    if (n == 2) return 1;      // 1 + 1
    if (n == 3) return 2;      // 1 + 2
    if (n == 4) return 4;      // 2 + 2
    if (n == 5) return 6;      // 3 + 2
    if (n == 6) return 9;      // 3 + 3
    // For n >= 7, always split off a 3 and recurse.
    return 3 * maxProductAfterSplit(n - 3);
}
// The key insight is that to maximize the product when splitting an integer into positive parts, you should use as many 3's as possible, because 3 is the most efficient multiplicative base (since `3 > 2 * 1` and for larger numbers, splitting into 3's gives higher product than splitting into 2's or leaving large numbers intact). However, there are special cases: if `n` is 2, the only split is `1 + 1` giving product 1. If `n` is 3, the only split is `1 + 2` giving product 2 (since `1 + 1 + 1` gives 1). If `n` is 4, the best split is `2 + 2` giving product 4 (since `3 + 1` gives 3). For `n >= 5`, the optimal strategy is to take a 3 and then recursively solve for `n - 3`. This works because for `n >= 5`, splitting off a 3 yields a product that is at least as good as splitting off a 2 or 4. The recursion terminates at the base cases. Edge cases include `n = 2`, `n = 3`, `n = 4`, `n = 5`, and `n = 6`, which are handled directly. Time complexity is `O(n)` due to linear recursion depth (each call reduces `n` by 3), and space complexity is `O(n)` on the call stack due to recursion depth, though this can be optimized to `O(1)` with iteration, but recursion is acceptable for the given constraints.
