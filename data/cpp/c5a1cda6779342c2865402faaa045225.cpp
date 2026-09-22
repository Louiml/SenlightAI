Write a C++ function `compute_nth_term` that takes three integers `a`, `b`, and `c` as input and returns the value of the `c`-th term of an arithmetic sequence starting at `a` with common difference `b`. The function should return `a + (c-1)*b` for any valid integer inputs, including negative values for `a`, `b`, and `c`. The task is to implement this function without using a loop, using direct arithmetic computation, and handle the edge case where `c` is zero or negative by returning `a` (as the original loop would never execute and `i` would remain `a` if `c <= 0`). The function must be `const`-correct, take parameters by value, and be self-contained with no reliance on external libraries beyond standard headers.

// The original code uses a loop that starts with `i = a`, increments `i` by `b` after each iteration, and breaks when `j` reaches `c`. For `c >= 1`, the number of increments (iterations) is exactly `c-1`, so the result is `a + (c-1)*b`. For `c < 1` (e.g., `c = 0` or negative), the loop condition `j == c` is never true (since `j` starts at 1 and only increases), so the loop would run forever in the original code—but we can define a sane behavior: return `a` (the initial value) for any `c <= 0`. The direct arithmetic formula works for all positive `c`. Edge cases: `c = 1` gives `a`; `b` can be negative, so `c-1` times negative `b` subtracts; large values of `c` could cause integer overflow, but we assume the problem expects normal `int` arithmetic. Time complexity is O(1), space complexity is O(1). The solution avoids the loop entirely, using a simple formula.

// Compute the c-th term (1-indexed) of arithmetic sequence starting at a, difference b.
// For c <= 0, return a (safer behavior than infinite loop).
int compute_nth_term(int a, int b, int c) {
    if (c <= 0) {
        return a;
    }
    return a + (c - 1) * b;
}

#include <cassert>

int compute_nth_term(int a, int b, int c); // Declaration for testing

int main() {
    // Basic positive terms
    assert(compute_nth_term(1, 2, 1) == 1);   // 1st term
    assert(compute_nth_term(1, 2, 2) == 3);   // 2nd term
    assert(compute_nth_term(1, 2, 5) == 9);   // 5th term

    // Negative starting value
    assert(compute_nth_term(-3, 4, 3) == 5);  // -3, 1, 5

    // Negative common difference
    assert(compute_nth_term(10, -3, 4) == 1); // 10, 7, 4, 1

    // Edge: c = 0 or negative
    assert(compute_nth_term(5, 7, 0) == 5);
    assert(compute_nth_term(5, 7, -2) == 5);

    // c = 1 returns start
    assert(compute_nth_term(100, -50, 1) == 100);

    // Mixed signs
    assert(compute_nth_term(-2, -3, 3) == -8); // -2, -5, -8

    // Large c (but within int range)
    assert(compute_nth_term(0, 1, 10) == 9);

    return 0;
}
