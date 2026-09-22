Write a C++ function that takes three integers representing a date in the format day, month, year (all positive integers, with the year as a four-digit number) and a separate set of three integers representing another date in the same format. The function should determine whether the first date is at least 18 years later than the second date, considering leap years (years divisible by 4 are leap years, including century years divisible by 400, though for simplicity assume a simple rule: a year is a leap year if it is divisible by 4; note that this is what the original code uses). The function must return `true` if the first date is exactly 18 years, or more, after the second date, otherwise `false`. The input dates are always valid calendar dates (e.g., day is between 1 and 31, month between 1 and 12, and the day is valid for the given month and year). Edge cases include boundary dates such as leap day February 29 in a leap year, and comparing dates on the same day but different years. The function signature should be `bool isAdult(const int birthDay, const int birthMonth, const int birthYear, const int currentDay, const int currentMonth, const int currentYear)`, where the first three parameters are the birth date (day, month, year) and the last three are the current date (day, month, year). Return `true` if the current date is at least 18 years after the birth date.

#include <cassert>

bool isAdult(int, int, int, int, int, int); // Forward declaration

int main() {
    // Basic cases: exactly 18 years later
    assert(isAdult(1, 1, 2000, 1, 1, 2018) == true);
    // One day before 18th birthday
    assert(isAdult(10, 5, 2000, 9, 5, 2018) == false);
    // One day after 18th birthday
    assert(isAdult(10, 5, 2000, 11, 5, 2018) == true);
    // Exactly 18 years with same month but earlier day
    assert(isAdult(15, 6, 2000, 14, 6, 2018) == false);
    // Same month and day
    assert(isAdult(15, 6, 2000, 15, 6, 2018) == true);
    // Older than 18
    assert(isAdult(1, 1, 1990, 1, 1, 2020) == true);
    // Younger than 18
    assert(isAdult(1, 1, 2005, 1, 1, 2020) == false);
    // Leap day birth (Feb 29, 2000) turning 18 in 2018 (non-leap year, but we compare numeric dates)
    assert(isAdult(29, 2, 2000, 28, 2, 2018) == false);
    assert(isAdult(29, 2, 2000, 1, 3, 2018) == true);
    // Month greater than birth month
    assert(isAdult(1, 5, 2000, 1, 6, 2018) == true);
    return 0;
}

#include <vector>
#include <algorithm>

// Determine if the current date is at least 18 years after the birth date.
bool isAdult(const int birthDay, const int birthMonth, const int birthYear,
             const int currentDay, const int currentMonth, const int currentYear) {
    int yearDiff = currentYear - birthYear;
    if (yearDiff > 18) {
        return true;
    }
    if (yearDiff < 18) {
        return false;
    }
    // Exactly 18 years difference; compare month and day.
    if (currentMonth > birthMonth) {
        return true;
    }
    if (currentMonth < birthMonth) {
        return false;
    }
    // Same month; compare day.
    return currentDay >= birthDay;
}

// The solution compares the two dates by first checking the year difference. If the current year minus the birth year is greater than 18, the person is definitely an adult. If it is exactly 18, we need to compare the month and day to see if the 18th birthday has already passed or occurs today. If the year difference is exactly 18, the person is an adult if the current month is greater than the birth month, or if the month is equal and the current day is greater than or equal to the birth day. If the year difference is less than 18, the person is not an adult. The algorithm must handle the case where the birth date is February 29 (in a leap year) and the current date is not a leap year; since we are told input dates are valid, but if comparing a birth date that falls on February 29, the 18th birthday would fall on February 28 in non-leap years. However, for simplicity and aligning with the snippet's approach, we can ignore that subtlety and just compare numeric values. The main complexity is constant: \(O(1)\) time and \(O(1)\) space. Edge cases include: same date (18 years later exactly), one day before the 18th birthday, and dates in different leap years.
