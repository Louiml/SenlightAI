/*
Write a C++ function named `isLeapYear` that takes an integer `year` as input and returns a `bool` value indicating whether the given year is a leap year according to the Gregorian calendar rules. A year is a leap year if it is divisible by 4, except for years that are divisible by 100 but not by 400. In other words, a year is a leap year if it is divisible by 4 and (either not divisible by 100, or divisible by 400). The function must handle all integer inputs, including negative years (which are not leap years), zero, and positive years up to the limits of `int`. The function should be `const`-correct and self-contained with only necessary headers included. Do not include a `main` function in the solution; provide only the function definition in the solution section.
*/
#include <cstdint> // not strictly needed, but for clarity

// Return true if the given year is a leap year according to the Gregorian calendar.
// Negative years and zero are not leap years.
bool isLeapYear(int year) {
    if (year <= 0) {
        return false;
    }
    if (year % 400 == 0) {
        return true;
    }
    if (year % 100 == 0) {
        return false;
    }
    return (year % 4 == 0);
}
#include <cassert>

// Declare the function (or include the header where it is defined)
bool isLeapYear(int year);

int main() {
    assert(isLeapYear(2000) == true);   // divisible by 400
    assert(isLeapYear(1900) == false);  // century but not divisible by 400
    assert(isLeapYear(2024) == true);   // divisible by 4 and not a century
    assert(isLeapYear(2023) == false);  // not divisible by 4
    assert(isLeapYear(0) == false);     // zero is not leap
    assert(isLeapYear(-4) == false);    // negative year is not leap
    assert(isLeapYear(1600) == true);   // divisible by 400
    assert(isLeapYear(2100) == false);  // future century not divisible by 400
    return 0;
}
// The leap year rule can be expressed as: a year `y` is a leap year if `y % 4 == 0` and (`y % 100 != 0` or `y % 400 == 0`). The provided code snippet incorrectly uses `&&` between all three conditions, which would never be true because `(year % 100 != 0)` and `(year % 400 == 0)` cannot both be true—if a year is divisible by 400, it is also divisible by 100. The correct logic is to check divisibility by 4 first, then exclude century years unless they are also divisible by 400. Edge cases include years like 1900 (divisible by 100 but not 400 → not leap), 2000 (divisible by 400 → leap), 2024 (divisible by 4 and not a century → leap), and 2023 (not divisible by 4 → not leap). Negative years and zero should return `false` because the Gregorian calendar does not define leap years for those. The algorithm uses constant-time arithmetic operations; time complexity is O(1) and space complexity is O(1).
