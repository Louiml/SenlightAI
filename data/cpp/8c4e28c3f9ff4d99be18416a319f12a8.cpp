// Write a C++ function named `rectCoverWays` that takes a non-negative integer `n` (representing the width of a 2×n rectangle) and returns the number of distinct ways to tile it completely using 2×1 dominoes (which can be placed either vertically or horizontally), without overlaps or gaps. If `n` is 0, there is exactly one way (the empty tiling). The function must not use recursion or dynamic programming arrays; instead it must compute the result iteratively in constant auxiliary space. The function signature should be `int rectCoverWays(int n)`. Assume `n` fits within an `int` and the result does not overflow.

#include <cassert>

int main() {
    // Base cases
    assert(rectCoverWays(0) == 1);
    assert(rectCoverWays(1) == 1);
    assert(rectCoverWays(2) == 2);
    // Small cases from the Fibonacci sequence shifted: n=3 -> 3, n=4 -> 5
    assert(rectCoverWays(3) == 3);
    assert(rectCoverWays(4) == 5);
    // Larger known values: n=5 -> 8, n=6 -> 13
    assert(rectCoverWays(5) == 8);
    assert(rectCoverWays(6) == 13);
    // A moderately large value: n=10 -> 89
    assert(rectCoverWays(10) == 89);
    // Ensure invalid negative input returns 0 (if we choose to guard)
    assert(rectCoverWays(-1) == 0);
    return 0;
}

#include <cassert>

// Returns the number of ways to tile a 2*n rectangle with 2*1 dominoes.
// n must be non-negative. Uses iterative Fibonacci-style computation.
int rectCoverWays(int n) {
    if (n < 0) return 0; // invalid input guard
    if (n == 0) return 1; // empty board: one trivial tiling

    int prev = 1; // f(0)
    int curr = 1; // f(1)
    for (int i = 2; i <= n; ++i) {
        int next = prev + curr;
        prev = curr;
        curr = next;
    }
    return curr;
}

// This is the classic Fibonacci-style problem. For a 2×n board, the first placement is either a vertical domino (covering the leftmost column completely, leaving a 2×(n-1) board) or two horizontal dominoes stacked (covering the leftmost two columns, leaving a 2×(n-2) board). Thus the number of ways `f(n) = f(n-1) + f(n-2)`, with base cases `f(0)=1` (empty board, one way) and `f(1)=1` (single vertical domino). For `n=2`, the formula gives `f(2)=f(1)+f(0)=2`, matching the two possible tilings (two vertical or two horizontal). We compute iteratively from `f(0)` and `f(1)` up to `n`, maintaining only the two most recent values. Edge cases: `n=0` must return 1; `n=1` returns 1; `n=2` returns 2; negative inputs are not expected but we can guard by returning 0 if `n<0` (or assume valid input). Time complexity is O(n) and space complexity is O(1).
