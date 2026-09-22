// Write a C++ function named `absoluteValue` that takes an integer and returns its absolute value. The absolute value of a non-negative integer is the integer itself, while the absolute value of a negative integer is its positive counterpart (e.g., `absoluteValue(-5)` returns `5`, `absoluteValue(5)` returns `5`, and `absoluteValue(0)` returns `0`). The function must not use any standard library functions like `std::abs`; instead, implement the logic manually using a conditional expression. The function should be const-correct (though integers are passed by value, the function itself should not modify any external state).
// The solution is straightforward: if the input integer is greater than or equal to zero, return it unchanged; otherwise, return the negation of the input (since negating a negative integer yields a positive value). The critical edge case is the integer `0`, which should return `0` — the condition `i >= 0` correctly handles this. Another edge case is the most negative integer (`INT_MIN` on typical two's-complement systems). Negating `INT_MIN` causes signed integer overflow, which is undefined behavior in C++. To be robust, the function should avoid negation when the value is zero; however, for typical teaching tasks, handling only ordinary negative values is acceptable. The condition can be written as `return (i >= 0) ? i : -i;` — this works for all values except `INT_MIN` where overflow occurs. If robustness is required, one could use `unsigned` arithmetic or check for `INT_MIN` separately, but for this task we assume typical inputs. Time complexity is O(1) and space complexity is O(1).
// Returns the absolute value of an integer without using std::abs.
// For negative input, returns the positive counterpart; otherwise returns input unchanged.
int absoluteValue(int value) {
    // If value is non-negative, return it as is; otherwise negate it.
    return (value >= 0) ? value : -value;
}
#include <cassert>
#include <climits>

int main() {
    // Positive number
    assert(absoluteValue(42) == 42);
    // Negative number
    assert(absoluteValue(-7) == 7);
    // Zero
    assert(absoluteValue(0) == 0);
    // Large positive
    assert(absoluteValue(1000000) == 1000000);
    // Large negative (safe if not INT_MIN)
    assert(absoluteValue(-2147483647) == 2147483647);
    // Negative with small magnitude
    assert(absoluteValue(-1) == 1);
    // Positive with small magnitude
    assert(absoluteValue(1) == 1);
    // Edge case: INT_MAX
    assert(absoluteValue(INT_MAX) == INT_MAX);
    // Edge case: INT_MIN is intentionally not tested to avoid overflow UB
    return 0;
}
