/*
Write a C++ function that takes a non-negative integer `n` (representing the number of steps in a staircase) and returns the number of distinct ways to climb to the top, where at each step you can climb either 1 or 2 steps. The function must handle edge cases for `n = 0` and `n = 1` appropriately, and should not use recursion (to avoid exponential time). Use dynamic programming with a simple iterative approach, and avoid memory leaks by not allocating raw arrays if possible — if you do use a raw array, ensure proper cleanup. The function signature should be `int waysToClimb(int n)`. The input `n` will be within the range `0 ≤ n ≤ 45` (so the result fits within a 32‑bit signed integer). The function should be `const`‑correct (mark parameters and the function appropriately if applicable).
*/

// Returns the number of distinct ways to climb a staircase of n steps,
// taking either 1 or 2 steps at a time. n must be non-negative.
int waysToClimb(int n) {
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }

    int prev2 = 1; // ways to reach step 0 (conceptually, one way)
    int prev1 = 1; // ways to reach step 1
    int current = 0;

    for (int i = 2; i <= n; ++i) {
        current = prev1 + prev2;
        prev2 = prev1;
        prev1 = current;
    }

    return current;
}

#include <cassert>

int main() {
    // Base cases
    assert(waysToClimb(0) == 0);
    assert(waysToClimb(1) == 1);

    // Small values
    assert(waysToClimb(2) == 2);
    assert(waysToClimb(3) == 3);
    assert(waysToClimb(4) == 5);
    assert(waysToClimb(5) == 8);

    // Larger value (Fibonacci number)
    assert(waysToClimb(10) == 89);
    assert(waysToClimb(15) == 987);

    // Upper bound of the problem
    assert(waysToClimb(45) == 1836311903);

    return 0;
}

// The problem is a classic Fibonacci‑like sequence: the number of ways to reach step `i` is the sum of ways to reach step `i-1` (by taking a 1‑step from there) and step `i-2` (by taking a 2‑step from there). Base cases: for `n=0` there are zero ways (or one way if you consider standing on the ground as one way, but here the original solution returns 0 for `n=0`), and for `n=1` there is exactly one way (a single 1‑step). We iterate from `2` to `n`, maintaining the last two computed values to avoid storing the whole array. This yields `O(n)` time and `O(1)` auxiliary space. Edge case: `n=0` returns 0, `n=1` returns 1, and `n=2` returns 2 (1‑step + 1‑step, or a single 2‑step). The iterative approach avoids stack overflow and redundant computation typical of naive recursion.
