// Write a C++ function that calculates a person's age in complete years given their birth date and a current date, both provided as integers in `AAAAMMDD` format (8-digit year-month-day, e.g., 19850723 for July 23, 1985). The function must compute age correctly even when the current month/day has not yet reached the birth month/day in the current year. For example, if born on 19900615 and current date is 20200501, the age should be 29 (not 30), because the birthday has not occurred yet. The input values are guaranteed to be valid dates with the current date later than the birth date, but you should still handle cases where the dates are exactly equal (age 0) or where the birth date is in the same month but later day. Do not rely on any external date libraries; perform the calculation manually using integer arithmetic.
The core algorithm involves extracting the year, month, and day components from each 8-digit integer. The year is obtained by integer division by 10000, the month by taking `(date / 100) % 100`, and the day by `date % 100`. The initial crude age is `(currentYear - birthYear)`. This is correct if the current month and day are both greater than or equal to the birth month and day; otherwise, we need to subtract 1 because the birthday has not been reached. Edge cases include: (1) when the birth month equals the current month but the current day is smaller (e.g., born on 19900115, current date 20200101), the age is `currentYear - birthYear - 1`; (2) when the birth month is greater than the current month (e.g., born on 19901215, current date 20200101), the age is `currentYear - birthYear - 1`; (3) when current month is greater than birth month, no adjustment needed; (4) when all dates are identical, the age is 0 because currentYear - birthYear = 0 and no subtraction is needed. The algorithm uses only integer arithmetic and constant space, running in O(1) time and O(1) auxiliary space.
#include <string> // Not strictly needed but included for completeness; this header is not used here.

// Calculate complete years between a birth date and a current date.
// Input format: integer in AAAAMMDD (e.g., 19850723 for July 23, 1985).
// Assumes the current date is later than or equal to the birth date.
int calculateAge(int birthDate, int currentDate) {
    const int birthYear = birthDate / 10000;
    const int birthMonth = (birthDate / 100) % 100;
    const int birthDay = birthDate % 100;
    
    const int currentYear = currentDate / 10000;
    const int currentMonth = (currentDate / 100) % 100;
    const int currentDay = currentDate % 100;
    
    int age = currentYear - birthYear;
    
    // If the birthday has not occurred yet this year, subtract one.
    if (currentMonth < birthMonth || 
        (currentMonth == birthMonth && currentDay < birthDay)) {
        --age;
    }
    
    return age;
}
#include <cassert>

// Prototype for the solution function (declared here for testing).
int calculateAge(int birthDate, int currentDate);

int main() {
    // Basic same-month, same-day (exact birthday).
    assert(calculateAge(19900615, 20200615) == 30);
    
    // Birthday later in the current month but not yet reached.
    assert(calculateAge(19900615, 20200501) == 29);
    
    // Same month, but current day less than birth day.
    assert(calculateAge(19900115, 20200101) == 29);
    
    // Birth month after current month.
    assert(calculateAge(19901215, 20200101) == 29);
    
    // Current month after birth month (birthday already passed).
    assert(calculateAge(19900310, 20200701) == 30);
    
    // Identical dates (newborn).
    assert(calculateAge(20200520, 20200520) == 0);
    
    // Exactly one year later, same date.
    assert(calculateAge(20190101, 20200101) == 1);
    
    // Different year but same month and day, should be exact difference.
    assert(calculateAge(19850723, 20250723) == 40);
    
    // Leap day born on 20000229, current date 20240228 (not reached).
    assert(calculateAge(20000229, 20240228) == 23);
    
    // Leap day born on 20000229, current date 20240301 (birthday passed).
    assert(calculateAge(20000229, 20240301) == 24);
    
    return 0;
}
