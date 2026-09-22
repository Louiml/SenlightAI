// Write a C++ function that simulates a robust integer division calculator. The function should take two integers as parameters (numerator and denominator) and return the integer result of the division. If the denominator is zero, the function must throw an exception (any type) that can be caught by a caller. The function must also handle the edge case where the numerator is the smallest possible 32-bit integer (`INT_MIN`) and the denominator is `-1`, which would cause an overflow in a normal division operation — in this case, the function should also throw an exception with a clear message, rather than producing undefined behavior. The function must be `const`-correct if applicable (it has no mutable state, so it can be a free function or a static member). Provide a clean, self-contained function with a descriptive name like `safeIntegerDivision`.

The core problem is to safely perform integer division while guarding against two exceptional conditions: division by zero and overflow. For division by zero, we check if the denominator is zero and throw an exception (e.g., a `std::invalid_argument` or a custom exception object). For the overflow case, we check if the numerator equals `std::numeric_limits<int>::min()` (which is `INT_MIN`) and the denominator equals `-1`; in that case, the mathematical result would be `2147483648`, which exceeds `INT_MAX` for a 32-bit `int`, causing undefined behavior in C++. We throw an exception (e.g., `std::overflow_error`) to signal this. The normal path simply performs `numerator / denominator`. Edge cases: if denominator is zero, we throw before doing division; if overflow, we throw before doing division. No other special cases are needed since integer division truncates toward zero, which is standard. Time complexity is O(1) as it's a constant-time operation. Space complexity is O(1) as no extra data structures are used. The solution includes necessary headers: `<stdexcept>` for exceptions, `<limits>` for `std::numeric_limits<int>::min()`, and `<iostream>` for any debug output if needed, but not required for the function itself.

#include <stdexcept>
#include <limits>

// Perform integer division with safety checks for division by zero and overflow.
int safeIntegerDivision(int numerator, int denominator) {
    // Division by zero is undefined.
    if (denominator == 0) {
        throw std::invalid_argument("Division by zero is not allowed.");
    }

    // Check for overflow when INT_MIN is divided by -1.
    if (numerator == std::numeric_limits<int>::min() && denominator == -1) {
        throw std::overflow_error("Integer division overflow: INT_MIN / -1 is not representable.");
    }

    // Safe division.
    return numerator / denominator;
}

#include <cassert>
#include <stdexcept>

int main() {
    // Normal division cases.
    assert(safeIntegerDivision(10, 2) == 5);
    assert(safeIntegerDivision(-10, 2) == -5);
    assert(safeIntegerDivision(10, -2) == -5);
    assert(safeIntegerDivision(-10, -2) == 5);
    assert(safeIntegerDivision(7, 3) == 2); // truncates toward zero
    assert(safeIntegerDivision(-7, 3) == -2); // truncates toward zero

    // Division by zero should throw.
    bool threw = false;
    try {
        safeIntegerDivision(5, 0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Overflow case: INT_MIN / -1.
    threw = false;
    try {
        safeIntegerDivision(std::numeric_limits<int>::min(), -1);
    } catch (const std::overflow_error&) {
        threw = true;
    }
    assert(threw);

    // Edge but safe: INT_MIN / 1 is fine.
    assert(safeIntegerDivision(std::numeric_limits<int>::min(), 1) == std::numeric_limits<int>::min());

    // Edge but safe: any number / -1 (except INT_MIN) is fine.
    assert(safeIntegerDivision(42, -1) == -42);
}
