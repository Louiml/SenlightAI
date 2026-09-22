Write a C++ function named `isLeapYear` that takes an integer year as input and returns a `char` value: `'S'` (for "Sí", meaning yes) if the year is a leap year, and `'N'` (for "No", meaning no) otherwise. Use the standard Gregorian calendar rules: a year is a leap year if it is divisible by 4, except that years divisible by 100 are not leap years unless they are also divisible by 400. The function should handle negative years (treat them as historical years using the same rules) and zero as a valid input, returning appropriate results without errors. The function must be pure (no external I/O) and should work for all integer values within the range of `int`.
The solution directly applies the leap-year condition using boolean logic: `(year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))`. The ternary operator returns `'S'` if the condition is true, otherwise `'N'`. Edge cases include years like 1900 (divisible by 100 but not 400 → `'N'`), 2000 (divisible by 400 → `'S'`), and 0 (divided by 4, 100, and 400 leaves remainder 0, so it is a leap year under this rule). Negative years use `%` which in C++ yields negative remainders for negative operands, but since we only check equality to 0, the results are correct (e.g., -4 % 4 == 0). The algorithm runs in constant time O(1) and uses constant auxiliary space O(1). No loops or additional data structures are needed.
#include <cstddef>

// Return 'S' if the given year is a leap year, 'N' otherwise.
char isLeapYear(const int year) {
    const bool leap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
    return leap ? 'S' : 'N';
}
#include <cassert>

// Forward declaration of the function under test.
char isLeapYear(int year);

int main() {
    // Standard leap years
    assert(isLeapYear(2020) == 'S');
    assert(isLeapYear(2000) == 'S');
    assert(isLeapYear(1600) == 'S');
    
    // Non-leap years
    assert(isLeapYear(2021) == 'N');
    assert(isLeapYear(1900) == 'N');
    assert(isLeapYear(100) == 'N');
    
    // Edge cases: zero and negative years
    assert(isLeapYear(0) == 'S');        // 0 divisible by 4, 100, and 400
    assert(isLeapYear(-4) == 'S');       // -4 divisible by 4 and not by 100
    assert(isLeapYear(-100) == 'N');     // -100 divisible by 100 but not 400
    assert(isLeapYear(-400) == 'S');     // -400 divisible by 400
    
    return 0;
}
