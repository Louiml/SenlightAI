Write a C++ function named `daysInMonth` that takes an integer month number (1-12) and returns the number of days in that month as an integer, assuming a non-leap year (February has 28 days). The function should handle invalid inputs (outside 1-12) by returning -1. Use conditional logic or switch statements, but do not use arrays or lookup tables. The function must be efficient and readable, using appropriate `const` where possible and clear naming. Your solution should avoid the duplicate-switch error pattern from the given snippet, which checks the same condition redundantly.
The problem requires mapping month numbers to day counts: months 1,3,5,7,8,10,12 have 31 days; months 4,6,9,11 have 30 days; month 2 has 28 days; and any other input is invalid. The main algorithm is straightforward: first validate the input is between 1 and 12 inclusive; if not, return -1. Then, use a switch statement on the month value, or a series of if-else conditions, to return the correct day count. Edge cases include month 2 (28, not 29 since non-leap year), the pattern of alternating months for 31/30 days (months 1-7 odd have 31, months 8-12 even have 31), and invalid values such as 0, 13, or negative numbers. Time complexity is O(1) because it involves only constant-time comparisons and return statements. Space complexity is O(1) as no extra data structures are used. The key is to avoid logical errors like the original snippet where switch conditions are used incorrectly.
// Return the number of days in a given month (non-leap year), or -1 if invalid.
constexpr int daysInMonth(int month) {
    if (month < 1 || month > 12) {
        return -1; // Invalid month input
    }
    switch (month) {
        case 2:  return 28; // February in non-leap year
        case 4: case 6: case 9: case 11:
            return 30; // Months with 30 days
        default:
            return 31; // All remaining valid months have 31 days
    }
}
int main() {
    // Valid month checks
    assert(daysInMonth(1) == 31);
    assert(daysInMonth(2) == 28);
    assert(daysInMonth(3) == 31);
    assert(daysInMonth(4) == 30);
    assert(daysInMonth(5) == 31);
    assert(daysInMonth(6) == 30);
    assert(daysInMonth(7) == 31);
    assert(daysInMonth(8) == 31);
    assert(daysInMonth(9) == 30);
    assert(daysInMonth(10) == 31);
    assert(daysInMonth(11) == 30);
    assert(daysInMonth(12) == 31);

    // Invalid inputs
    assert(daysInMonth(0) == -1);
    assert(daysInMonth(13) == -1);
    assert(daysInMonth(-3) == -1);
}
