Write a C++ function named `isZero` that takes an integer parameter and returns a `bool` indicating whether the integer is equal to zero. The function should be designed so that it can be used in a program that reads an integer from standard input and prints its zero-status using the same logic as the provided snippet. The function must be const-correct (mark the parameter as `const` where appropriate) and include a descriptive comment. Do not include a `main` function in your solution; the test section will provide the entry point. The function should handle all valid `int` values, including negative numbers, positive numbers, and zero itself, with no special edge cases beyond standard integer equality.

The task is straightforward: check whether an integer equals zero. The main algorithm is a single equality comparison between the input integer and the literal `0`, returning the result as a `bool`. Since the function does not modify its parameter, the parameter should be declared `const int` to enforce read-only semantics. No error handling or special cases are needed because all `int` values are valid inputs, and equality with zero is a well-defined operation. The time complexity is O(1) — a single comparison — and the space complexity is O(1), as no auxiliary data structures are used. The function can be directly used in a program like the snippet: read an integer, call `isZero`, and print the boolean result (which automatically outputs `1` for true and `0` for false in C++ when using `cout`).

#include <cstdbool> // Not strictly needed for bool, but included for clarity

// Returns true if the given integer is zero, false otherwise.
bool isZero(const int value) {
    return value == 0;
}

#include <cassert>

// The solution function is declared above (or included from a header)
bool isZero(const int value);

int main() {
    // Test positive numbers
    assert(isZero(1) == false);
    assert(isZero(42) == false);
    assert(isZero(2147483647) == false); // INT_MAX

    // Test negative numbers
    assert(isZero(-1) == false);
    assert(isZero(-42) == false);
    assert(isZero(-2147483648) == false); // INT_MIN (implementation-defined but valid on most)

    // Test zero itself
    assert(isZero(0) == true);

    // Test edge case: zero passed as a const expression
    const int zero = 0;
    assert(isZero(zero) == true);

    // Test explicit bool conversion matches logical not
    assert(isZero(0) == !isZero(5));

    return 0;
}
