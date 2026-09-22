Write a standalone C++ function `daysRemaining` that, given a date in a fictional calendar system (year `y`, month `m`, day `d`, all 1-indexed), computes how many days remain in a fixed 1000‑year era, starting from that date (exclusive of the given day) up to and including the last day of year 1000. The calendar has 10 months per year. In a normal year, months alternate: month 1 has 20 days, month 2 has 19 days, month 3 has 20 days, …, month 9 has 20 days, month 10 has 19 days. In a leap year (when year mod 3 == 2), every month has exactly 20 days. Years are numbered 1..1000. The function must return an integer equal to the total number of days from the day *after* the given date through Dec 31 of year 1000, inclusive. For example, the date (1,1,1) is day 0 of the era; the number of days from Jan 2, year 1 to the end is the total days in the era minus 1. Assume the input date is always valid, with 1 ≤ y ≤ 1000, 1 ≤ m ≤ 10, and 1 ≤ d ≤ (19,20, or 20 depending on month and leap status).
The calendar is periodic but not purely uniform because leap years occur every 3 years (years 2,5,8,...) with all 20‑day months. To compute the remaining days efficiently, precompute two prefix‑sum arrays: `monthPrefix[11]` for normal years (cumulative days from month 1 to month m), and `leapMonthPrefix[11]` for leap years. Also precompute a prefix‑sum array `yearPrefix[1001]` where `yearPrefix[i]` = total days from year 1 to year i (inclusive). Then for a given date `(y,m,d)`, compute `daysElapsedBeforeInput` = `yearPrefix[y-1] + (if year y is leap ? leapMonthPrefix[m-1] : monthPrefix[m-1]) + d`. The total days in the 1000‑year era is `yearPrefix[1000]`. The remaining days = `yearPrefix[1000] - daysElapsedBeforeInput`. We exit the loop that builds `yearPrefix` from 0 to 999, using the correct month length for year i+1. Edge cases: December of the last year (y=1000) returns 0 if date is the last day; leap‑year month lengths must be correct; use `long long` to avoid overflow (max ~ 200,000 days). Time O(1) per query after O(1000) preprocessing, space O(1000).
#include <vector>

// Return the number of days from the day after the given 1-indexed date
// (y,m,d) to the end of year 1000 inclusive.
long long daysRemaining(int y, int m, int d) {
    const int MONTHS = 10;
    const int YEARS = 1000;

    // Prefix sums for normal years: monthPrefix[k] = days in months 1..k
    std::vector<long long> monthPrefix(MONTHS + 1, 0);
    // Prefix sums for leap years (year % 3 == 2)
    std::vector<long long> leapMonthPrefix(MONTHS + 1, 0);
    for (int i = 0; i < MONTHS; ++i) {
        // Normal year month lengths: odd index (1-based) -> 20, even -> 19
        int normalDays = (i % 2 == 0) ? 20 : 19; // i is 0‑based: month 1,3,5,... -> 20
        int leapDays = 20;
        monthPrefix[i + 1] = monthPrefix[i] + normalDays;
        leapMonthPrefix[i + 1] = leapMonthPrefix[i] + leapDays;
    }

    // yearPrefix[i] = total days in years 1..i (0 for i=0)
    std::vector<long long> yearPrefix(YEARS + 1, 0);
    for (int year = 1; year <= YEARS; ++year) {
        bool isLeap = (year % 3 == 2);
        long long daysThisYear = isLeap ? leapMonthPrefix[MONTHS] : monthPrefix[MONTHS];
        yearPrefix[year] = yearPrefix[year - 1] + daysThisYear;
    }

    bool isLeapYear = (y % 3 == 2);
    long long daysInPreviousYears = yearPrefix[y - 1];
    long long daysInMonthsBefore = isLeapYear ? leapMonthPrefix[m - 1] : monthPrefix[m - 1];
    long long daysElapsedBeforeInput = daysInPreviousYears + daysInMonthsBefore + d;

    return yearPrefix[YEARS] - daysElapsedBeforeInput;
}
#include <cassert>

int main() {
    // Last day of year 1000: zero days remain.
    assert(daysRemaining(1000, 10, 19) == 0);
    // Last day of a normal month (month 10 has 19 days in normal years)
    assert(daysRemaining(999, 10, 19) == 20); // because year 1000 is leap with 200 days
    // First day of the era: total days in era minus 1.
    // Total days: number of leap years (333) with 200 days + 667 normal with 199 days
    // = 333*200 + 667*199 = 66600 + 132733 = 199333. So remaining = 199332.
    assert(daysRemaining(1, 1, 1) == 199332);
    // Day 2 of year 1: remaining = 199331
    assert(daysRemaining(1, 1, 2) == 199331);
    // Last day of year 1 (month 10, day 19): remaining = total days of years 2..1000
    // Years 2..1000 inclusive = 999 years. Among these, from year 2 to 1000,
    // leap years are 2,5,...,998 (since 1000 % 3 == 1) => (998-2)/3+1 = 333 leap years,
    // and 666 normal years. So remaining = 333*200 + 666*199 = 66600 + 132534 = 199134.
    assert(daysRemaining(1, 10, 19) == 199134);
    // Mid‑leap‑year date: year 2 is leap, month 2 day 1.
    // Days before: year 1 (199 days) + month 1 (20 days) + 1 = 220.
    // Total = 199333, remaining = 199333-220 = 199113.
    assert(daysRemaining(2, 2, 1) == 199113);
    // Date in a normal year, month 1 day 20 (last day of first month).
    // Year 3 is normal. Days before: year1+year2 (199+200=399) + 20 days = 419.
    // Remaining = 199333-419 = 198914.
    assert(daysRemaining(3, 1, 20) == 198914);
    // Last day of normal year 4 (month 10 day 19).
    // Days before = years 1..3 total (199+200+199=598) + 199 = 797.
    // Remaining = 199333-797 = 198536.
    assert(daysRemaining(4, 10, 19) == 198536);
    // First day of year 1000 (leap year): remaining = full 200 days of year 1000.
    assert(daysRemaining(1000, 1, 1) == 200);
    // Last day of month 9 in a normal year (month 9 has 20 days).
    // Year 1, month 9 day 20: days before = 20+19+20+19+20+19+20+19+20 = 176? Let's compute:
    // months 1..8: 20+19+20+19+20+19+20+19 = 156, plus month 9 day 20 = 176.
    // Remaining = 199333-176 = 199157.
    assert(daysRemaining(1, 9, 20) == 199157);
    return 0;
}
