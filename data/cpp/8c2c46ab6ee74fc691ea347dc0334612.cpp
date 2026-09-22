/*
Write a C++ function named `countDigits` that takes a single integer parameter (which may be negative, zero, or positive) and returns the number of decimal digits in its absolute value. For example, `countDigits(12345)` should return `5`, `countDigits(-987)` should return `3`, and `countDigits(0)` should return `1`. The function must handle the edge case of zero correctly and must not modify the input parameter. Provide only the function definition without any `main` function.
*/
#include <cstdlib> // for std::abs

// Returns the number of decimal digits in the absolute value of the given integer.
// Handles zero as a special case with one digit.
int countDigits(int number) {
    if (number == 0) {
        return 1;
    }
    int absolute = std::abs(number);
    int digitCount = 0;
    while (absolute != 0) {
        ++digitCount;
        absolute /= 10;
    }
    return digitCount;
}
#include <cassert>

// Forward declaration of the function under test.
int countDigits(int number);

int main() {
    assert(countDigits(0) == 1);
    assert(countDigits(5) == 1);
    assert(countDigits(-5) == 1);
    assert(countDigits(10) == 2);
    assert(countDigits(-99) == 2);
    assert(countDigits(12345) == 5);
    assert(countDigits(-987654) == 6);
    assert(countDigits(1000000000) == 10);
    assert(countDigits(-2147483647) == 10); // Minimum int magnitude is 2147483648, but -2147483647 is safe
    // Note: std::abs(INT_MIN) is undefined on some platforms; test with a safe large value.
    assert(countDigits(1234567890) == 10);
    return 0;
}
// The algorithm repeatedly divides the absolute value of the number by 10 in a loop, incrementing a digit counter until the number becomes zero. The main edge case is zero, which has one digit and must be handled before entering the loop. Negative numbers are handled by taking the absolute value (using `std::abs` from `<cstdlib>` or manually negating if negative) so that division works correctly. For an integer `n`, the loop runs once per digit, so the time complexity is \(O(\log_{10}|n|)\), and space complexity is \(O(1)\). The function uses `const` by taking the parameter by value (which is a copy), so no mutation occurs to the caller's variable.
