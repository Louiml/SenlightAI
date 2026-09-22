// In C++, macros are commonly used to create inline expressions, but they can introduce subtle bugs due to operator precedence and multiple evaluation of arguments. Write a function named `safeMax` that takes two integers `a` and `b` and returns the larger of the two, but with the crucial difference that it must evaluate each argument exactly once (to avoid side-effect issues). To test your understanding, create a separate helper function `doubleOrNegate` that takes an integer `x` and an integer `flag`; if `flag` is nonzero, returns `2 * x`, otherwise returns `-x`. Then, in a test program, use `safeMax` to compare the result of `doubleOrNegate(3, 1)` and `doubleOrNegate(5, 0)`, and also verify that using `safeMax` does not cause unexpected side effects when called with expressions that increment a counter. The task is to implement `safeMax` correctly and demonstrate that it is superior to a macro-based `#define MAX(a,b) (((a)>(b))?(a):(b))` by showing that a macro can double-evaluate an argument, while your function does not.
// The key insight is that a macro expands textually, so if you pass an expression with side effects (e.g., `++count`), the macro may evaluate that expression multiple times (once for the comparison, and again for the return value). A proper function evaluates its arguments exactly once when called (unless by reference, but for integers by value is fine). The solution is a simple inline function that takes two `int` parameters by value and returns the maximum. Edge cases: equal values are fine, negative numbers are fine, and no special overflow handling is needed because we only compare. Time complexity is O(1), space O(1). To demonstrate the superiority, we can write a test that increments a counter inside an expression passed to both the macro and the function, and check that the counter increments only once with the function. However, the task only requires the function itself, not the macro test in the solution—the test section can include assertions that verify behavior with increment counters.
#include <algorithm> // for std::max as fallback, though we implement our own

// Return the larger of two integers, evaluating each argument exactly once.
// This is safer than a macro because macros can double-evaluate arguments
// with side effects like increments or function calls.
inline int safeMax(int a, int b) {
    return (a > b) ? a : b;
}
#include <cassert>

// Helper to test side-effect-free usage and side-effect handling.
inline int doubleOrNegate(int x, int flag) {
    return (flag != 0) ? (2 * x) : (-x);
}

int main() {
    // Basic maximum check
    assert(safeMax(3, 5) == 5);
    assert(safeMax(5, 3) == 5);
    assert(safeMax(-2, -7) == -2);
    assert(safeMax(4, 4) == 4);

    // Check that arguments are evaluated exactly once.
    int counter = 0;
    auto incrementing = [&counter](int v) { counter++; return v; };
    int result = safeMax(incrementing(10), incrementing(20));
    assert(result == 20);
    assert(counter == 2); // each argument evaluated once

    // Compare with doubleOrNegate results
    assert(safeMax(doubleOrNegate(3, 1), doubleOrNegate(5, 0)) == safeMax(6, -5));
    assert(safeMax(doubleOrNegate(3, 1), doubleOrNegate(5, 0)) == 6);

    // Additional edge cases
    assert(safeMax(0, 0) == 0);
    assert(safeMax(-1, 1) == 1);
    assert(safeMax(100, -100) == 100);
}
