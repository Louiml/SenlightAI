/*
Write a C++ function named `isLeapYear` that takes an integer `year` as input and returns a `bool` indicating whether the given year is a leap year according to the Gregorian calendar rules. The function should be self-contained, use only standard headers, and be `const`-correct (though the input is passed by value, so no mutable state is involved). The function must correctly handle all edge cases, including years divisible by 4 but not by 100, years divisible by 100 but not by 400, and years divisible by 400. Do not write a `main` function; only provide the function definition and any necessary includes.
*/
#include <cstdbool> // not strictly needed but for clarity; bool is built-in

// Determine whether a given year is a leap year in the Gregorian calendar.
bool isLeapYear(const int year) {
    // Leap year if divisible by 4 and (not divisible by 100 or divisible by 400)
    return (year % 4 == 0) && (year % 100 != 0 || year % 400 == 0);
}
#include <cassert>

int main() {
    // Basic non-leap years
    assert(isLeapYear(2023) == false);
    assert(isLeapYear(2021) == false);
    // Basic leap years divisible by 4
    assert(isLeapYear(2024) == true);
    assert(isLeapYear(2020) == true);
    // Century years not divisible by 400
    assert(isLeapYear(1900) == false);
    assert(isLeapYear(2100) == false);
    // Century years divisible by 400
    assert(isLeapYear(2000) == true);
    assert(isLeapYear(1600) == true);
    // Edge case: year 0 (mathematically divisible by 400) but historically not used; our function returns true
    assert(isLeapYear(0) == true);
    // Edge case: negative year (-400 is divisible by 400)
    assert(isLeapYear(-400) == true);
    return 0;
}
// The Gregorian leap year rule is: a year is a leap year if it is divisible by 4, except if it is divisible by 100, in which case it is a leap year only if it is also divisible by 400. The simplest boolean expression capturing this is `(year % 4 == 0) && (year % 100 != 0 || year % 400 == 0)`. This avoids nested conditionals and directly evaluates the rule. Edge cases: century years like 1900 (not leap), 2000 (leap), and non-century divisible by 4 like 2024 (leap). The function works for any integer, including negative years (though historically there is no year 0, but mathematically the modulo works consistently). Time complexity is O(1) using constant-time arithmetic operations; space complexity is O(1) as it uses no extra storage.
