// Write a C++ function that takes a starting date (represented by a struct with `Day`, `Month`, and `Year` as `short` integers) and a positive integer number of vacation days, and returns the business-day return date (the date when the employee must return to work) by skipping weekends (Friday and Saturday) and counting only business days (Sunday through Thursday). The function must not count the starting date itself as a vacation day, and if the starting date falls on a weekend, it should be moved forward to the next business day before counting begins. After adding the specified number of vacation days (each as one business day), if the resulting date is a weekend, continue advancing until a business day is found. The function should return the resulting date as a `stDate` struct.

// The solution must first normalize the start date: if it is a weekend (Friday or Saturday), advance it day-by-day until it becomes a business day. Then, we need to count exactly `VacationDays` business days. A straightforward approach loops from the normalized start date, advancing one day at a time. For each day, if it is a business day, we increment a counter; when the counter reaches `VacationDays`, that day is the last vacation day, and we must return the next business day after it (since the return date is the first working day after vacation ends). To implement this cleanly, we can perform a loop that advances the date by one day, checks if the new date is a business day, and if so increments a counter. The loop continues until the counter equals the vacation days. At that point, the current date is the last day of vacation (a business day). Then we advance one more day and, if needed, skip any weekends to get the first business day after vacation. Edge cases: if vacation days is 0, the return date is the first business day on or after the start date (if start is weekend, move to next business day, else start itself is the return date if it is a business day — but the spec says the start date itself is not a vacation day, so with 0 vacation days, the return date should be the start date if it's a business day, or the next business day if it's a weekend). Also, the year may change during increments. The algorithm is O(VacationDays) time, since each day after start is incremented at most once plus a constant number of weekend skips, and O(1) auxiliary space.

#include <cstddef>

struct stDate {
    short Day;
    short Month;
    short Year;
};

bool isLeapYear(short year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

short daysInMonth(short month, short year) {
    if (month < 1 || month > 12) return 0;
    const short days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year)) return 29;
    return days[month - 1];
}

stDate increaseByOneDay(const stDate& date) {
    stDate next = date;
    if (next.Day < daysInMonth(next.Month, next.Year)) {
        ++next.Day;
    } else {
        next.Day = 1;
        if (next.Month == 12) {
            next.Month = 1;
            ++next.Year;
        } else {
            ++next.Month;
        }
    }
    return next;
}

short dayOfWeekOrder(const stDate& date) {
    short a = (14 - date.Month) / 12;
    short y = date.Year - a;
    short m = date.Month + 12 * a - 2;
    return (date.Day + y + y/4 - y/100 + y/400 + (31 * m) / 12) % 7;
}

bool isWeekend(const stDate& date) {
    short dayIndex = dayOfWeekOrder(date);
    return (dayIndex == 5 || dayIndex == 6); // Friday (5) or Saturday (6)
}

bool isBusinessDay(const stDate& date) {
    return !isWeekend(date);
}

// Return the first business day after a vacation of VacationDays business days.
// The start date itself is not counted as a vacation day.
stDate computeReturnDate(stDate startDate, short vacationDays) {
    // Move to the first business day if start is on a weekend.
    while (isWeekend(startDate)) {
        startDate = increaseByOneDay(startDate);
    }

    // If vacation days is zero, the return date is the start date (which is business).
    if (vacationDays == 0) {
        return startDate;
    }

    // Count business days from the day after the normalized start date.
    stDate current = startDate;
    short countedDays = 0;

    while (countedDays < vacationDays) {
        current = increaseByOneDay(current);
        if (isBusinessDay(current)) {
            ++countedDays;
        }
    }

    // 'current' is the last day of vacation (a business day). Move to the next business day.
    current = increaseByOneDay(current);
    while (isWeekend(current)) {
        current = increaseByOneDay(current);
    }

    return current;
}

#include <cassert>

int main() {
    // Use helper comparisons: dates are equal if all fields match.
    auto assertDate = [](stDate actual, stDate expected) {
        assert(actual.Day == expected.Day);
        assert(actual.Month == expected.Month);
        assert(actual.Year == expected.Year);
    };

    // Case 1: Start Monday, 3 vacation days -> return Friday
    stDate d1 = {1, 1, 2024}; // Monday, Jan 1 2024
    stDate r1 = computeReturnDate(d1, 3);
    assertDate(r1, {5, 1, 2024}); // Friday Jan 5

    // Case 2: Start Friday (weekend), 1 vacation day -> return Monday
    stDate d2 = {5, 1, 2024}; // Friday
    stDate r2 = computeReturnDate(d2, 1);
    assertDate(r2, {8, 1, 2024}); // Monday Jan 8

    // Case 3: Start Saturday, 0 vacation days -> return Sunday (next business day)
    stDate d3 = {6, 1, 2024}; // Saturday
    stDate r3 = computeReturnDate(d3, 0);
    assertDate(r3, {7, 1, 2024}); // Sunday

    // Case 4: Start Sunday, 1 vacation day -> return Tuesday (since Monday is business)
    stDate d4 = {7, 1, 2024}; // Sunday
    stDate r4 = computeReturnDate(d4, 1);
    assertDate(r4, {9, 1, 2024}); // Tuesday

    // Case 5: Start Thursday, 5 vacation days -> skip weekend, return Thursday next week
    stDate d5 = {11, 1, 2024}; // Thursday
    stDate r5 = computeReturnDate(d5, 5);
    assertDate(r5, {18, 1, 2024}); // Thursday next week

    // Case 6: Start Friday, 10 vacation days -> return Monday two weeks later
    stDate d6 = {12, 1, 2024}; // Friday
    stDate r6 = computeReturnDate(d6, 10);
    assertDate(r6, {29, 1, 2024}); // Monday Jan 29

    // Case 7: Start Monday, 0 vacation days -> return same day
    stDate d7 = {2, 9, 2024}; // Monday
    stDate r7 = computeReturnDate(d7, 0);
    assertDate(r7, {2, 9, 2024});

    // Case 8: Year boundary, start Thu Dec 26 2024, 3 vacation days -> return Tue Dec 31 (business)
    stDate d8 = {26, 12, 2024}; // Thursday
    stDate r8 = computeReturnDate(d8, 3);
    assertDate(r8, {31, 12, 2024}); // Tuesday

    return 0;
}
