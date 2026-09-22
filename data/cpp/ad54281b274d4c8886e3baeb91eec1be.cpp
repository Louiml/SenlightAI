// Write a C++ function `std::string leapYearClassification(int year)` that returns the string `"a leap year"` if the given year is a leap year, otherwise returns `"a normal year"`. A year is a leap year if it is divisible by 400, or if it is divisible by 4 but not by 100. The function must handle any integer year (including negative years, which should be treated as normal years) and must not read from standard input or print to standard output—only return the classification string. The output must exactly match the given phrases.

// The solution follows the standard Gregorian calendar leap year rule: a year is leap if `(year % 4 == 0 && year % 100 != 0) || year % 400 == 0`. Since the problem states that negative years should be treated as normal years, we first check if `year <= 0` and return `"a normal year"` early. For positive years, the modulo operations correctly handle all cases. Edge cases include years like 1900 (divisible by 100 but not 400 → normal), 2000 (divisible by 400 → leap), and 4 (leap). Since the function only performs constant-time arithmetic, time complexity is O(1) and space complexity is O(1).

#include <string>

// Returns "a leap year" if the given positive year is a leap year,
// otherwise returns "a normal year". Negative years are considered normal.
std::string leapYearClassification(int year) {
    if (year <= 0) {
        return "a normal year";
    }
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
        return "a leap year";
    }
    return "a normal year";
}

#include <cassert>
#include <string>

// Declaration of the function under test (usually in a header, but
// included here for a self-contained test).
std::string leapYearClassification(int year);

int main() {
    // Standard leap years
    assert(leapYearClassification(2000) == "a leap year");
    assert(leapYearClassification(2024) == "a leap year");
    assert(leapYearClassification(2020) == "a leap year");
    
    // Non-leap years
    assert(leapYearClassification(1900) == "a normal year");
    assert(leapYearClassification(2023) == "a normal year");
    assert(leapYearClassification(2025) == "a normal year");
    
    // Edge cases
    assert(leapYearClassification(0) == "a normal year");
    assert(leapYearClassification(-4) == "a normal year");
    assert(leapYearClassification(1) == "a normal year");
    assert(leapYearClassification(4) == "a leap year");
}
