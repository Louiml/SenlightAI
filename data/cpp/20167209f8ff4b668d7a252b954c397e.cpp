// Write a C++ function `int totalSkips(int N)` that, for a positive integer `N`, returns the value of a sequence defined as follows: the first term is 1, and for each subsequent term from 2 up to `N`, the term equals the previous term plus twice the value of `((i - 1) / 2 + 1)` (integer division). The function must handle any positive `N` (including 1) and must not rely on global variables or user input. Assume `N` is within the range of a standard 32-bit signed integer.

#include <cassert>

int main() {
    // Base case N = 1
    assert(totalSkips(1) == 1);
    // Small values
    assert(totalSkips(2) == 3);
    assert(totalSkips(3) == 5);
    assert(totalSkips(4) == 9);
    assert(totalSkips(5) == 13);
    // Larger consistent check
    assert(totalSkips(10) == 61);
    // Verify a few more: 6, 7, 8, 9
    assert(totalSkips(6) == 19);
    assert(totalSkips(7) == 25);
    assert(totalSkips(8) == 33);
    assert(totalSkips(9) == 41);
    return 0;
}

// Return the value of the sequence at position N.
// Sequence: dp[1] = 1; dp[i] = dp[i-1] + 2 * ((i-1)/2 + 1) for i >= 2.
int totalSkips(int N) {
    if (N <= 1) {
        return 1;  // Handles N == 1 (and defensively N < 1).
    }
    int previous = 1;
    for (int i = 2; i <= N; ++i) {
        int increment = 2 * ((i - 1) / 2 + 1);
        previous += increment;
    }
    return previous;
}

// The sequence grows linearly. Starting from `dp[1] = 1`, for each `i` from 2 to `N`, the increment is `2 * ((i-1)/2 + 1)`. We can compute this iteratively in a loop, maintaining only the previous term (since only `dp[i-1]` is needed to compute `dp[i]`). The increment pattern is: for i=2, increment=2; i=3, increment=2; i=4, increment=4; i=5, increment=4; i=6, increment=6; etc. Edge case: if `N == 1`, the answer is simply 1. No special handling for negative values because the task guarantees positive `N`. Time complexity is O(N), and auxiliary space is O(1) if we use a single loop variable; if we store the whole array it would be O(N) space, but a simple loop is more efficient.
