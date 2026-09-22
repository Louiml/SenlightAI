// Write a C++ function `int daysInMonth(int year, int month)` that takes a valid year (positive integer) and a month number (1–12) and returns the number of days in that month, correctly handling leap years. A leap year occurs if the year is divisible by 400, or if it is divisible by 4 but not by 100. The function must return 28 or 29 for February depending on leap‑year status; for all other months, return the standard fixed day counts (31 for Jan, Mar, May, Jul, Aug, Oct, Dec; 30 for Apr, Jun, Sep, Nov). The function must not read input or print output; it simply receives the two integers and returns the day count.

int main() {
    // Non‑leap years
    assert(daysInMonth(2021, 1) == 31);
    assert(daysInMonth(2021, 2) == 28);
    assert(daysInMonth(2021, 4) == 30);
    assert(daysInMonth(2021, 12) == 31);

    // Leap years (divisible by 4 but not 100)
    assert(daysInMonth(2020, 2) == 29);
    assert(daysInMonth(2016, 2) == 29);

    // Century years: not leap unless divisible by 400
    assert(daysInMonth(1900, 2) == 28);
    assert(daysInMonth(2000, 2) == 29);

    // Leap‑year Feb has same day counts for other months
    assert(daysInMonth(2020, 7) == 31);
    assert(daysInMonth(2020, 9) == 30);

    // Boundary: year 1 (not leap)
    assert(daysInMonth(1, 2) == 28);

    return 0;
}

#include <cassert>

// Return the number of days in the given month of the given year.
// Assumes month is between 1 and 12 inclusive; year is a positive integer.
int daysInMonth(int year, int month) {
    // Leap year rule: divisible by 400, or divisible by 4 but not by 100.
    const bool isLeap = (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);

    // Handle February specially.
    if (month == 2) {
        return isLeap ? 29 : 28;
    }

    // Standard day counts for non‑February months.
    // Use a compact array indexed by month (1‑12), giving 31,28,31,30,31,30,31,31,30,31,30,31.
    const int daysInMonthTable[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return daysInMonthTable[month];
}

// The solution uses the standard leap‑year rule: a year is a leap year if `(year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)`. To avoid repeating the ternary logic, we can compute a boolean `isLeap` once. Then we use an array or a switch statement to map each month to its typical day count, but February is special: if `month == 2`, return `isLeap ? 29 : 28`. Since the problem guarantees a valid month (1–12), no error handling for invalid months is needed, but to be robust we could return `0` for invalid months—though the specification says month is valid. The algorithm is constant time: `O(1)` time and `O(1)` space.
