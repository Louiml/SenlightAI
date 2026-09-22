// Write a C++ function named `calculateWithFlag` that takes two integers `a` and `b` and a boolean `flag`. The function must return `a + b` if `flag` is `true`, and return `a - b` if `flag` is `false`. Use the ternary operator for a concise, single-expression implementation. The function should be `const`-correct by not modifying its parameters (they are passed by value, so no modification is needed). Ensure the function handles all integer values, including negative numbers and zero, without any special edge cases beyond the boolean condition.

// The solution is straightforward branching: check the boolean condition `flag`. If `true`, compute the sum; otherwise, compute the difference. The ternary operator `flag ? a + b : a - b` directly expresses this logic. Important edge cases include negative integers, zero, and large values causing integer overflow (though C++ signed overflow is undefined, so for typical exercise purposes we assume inputs do not overflow). The function does not need to handle any other conditions or validate input since all parameters are valid by type contract. Time complexity is O(1), space complexity is O(1) — no extra storage beyond the return value.

#include <string> // Not strictly needed but included for completeness if needed elsewhere.

// Returns a + b when flag is true, otherwise returns a - b.
int calculateWithFlag(const int a, const int b, const bool flag) {
    return flag ? a + b : a - b;
}
(Note: The include is optional; the function needs no additional headers. I've kept it minimal.)

#include <cassert>

int main() {
    // Basic true and false cases
    assert(calculateWithFlag(3, 5, true) == 8);
    assert(calculateWithFlag(3, 5, false) == -2);

    // Zero values
    assert(calculateWithFlag(0, 0, true) == 0);
    assert(calculateWithFlag(0, 0, false) == 0);

    // Negative numbers
    assert(calculateWithFlag(-7, 2, true) == -5);
    assert(calculateWithFlag(-7, 2, false) == -9);

    // Mixed signs
    assert(calculateWithFlag(-4, -6, true) == -10);
    assert(calculateWithFlag(-4, -6, false) == 2);

    // Larger integers (within int range, no overflow)
    assert(calculateWithFlag(100000, 200000, true) == 300000);
    assert(calculateWithFlag(100000, 200000, false) == -100000);

    // Edge case: first parameter negative, second positive, false
    assert(calculateWithFlag(-10, 10, false) == -20);

    return 0;
}
