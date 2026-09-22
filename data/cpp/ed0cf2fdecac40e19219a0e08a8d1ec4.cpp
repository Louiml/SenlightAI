Write a C++ function named `isLeapYear` that takes an integer year as input and returns `true` if the year is a leap year according to the Gregorian calendar rules, and `false` otherwise. A year is a leap year if it is divisible by 4, except for years that are divisible by 100 but not by 400. For example, 2000 is a leap year, 1900 is not, and 2024 is. The function must handle all valid integer years, including negative years (treat them using the same divisibility rules; for instance, -4 is a leap year, -100 is not). The function should not read from or write to any standard input/output streams; it should only compute and return the boolean result.

The main algorithm directly implements the Gregorian leap-year rule: first check if the year is divisible by 400 (then leap), else if divisible by 100 (then not leap), else if divisible by 4 (then leap), otherwise not leap. Edge cases include years like 0, negative years, and boundary multiples of 100 and 400. Use the modulo operator `%` carefully with negative numbers in C++; since `-4 % 4 == 0`, the divisibility check works uniformly for negative years. The solution can be written as a single boolean expression using nested conditionals or logical operators. Time complexity is O(1) and space complexity is O(1).

// Return true if the given year is a leap year according to the Gregorian calendar.
bool isLeapYear(int year) {
    if (year % 400 == 0) {
        return true;
    }
    if (year % 100 == 0) {
        return false;
    }
    return year % 4 == 0;
}

int main() {
    assert(isLeapYear(2000) == true);   // divisible by 400
    assert(isLeapYear(1900) == false);  // divisible by 100 but not 400
    assert(isLeapYear(2024) == true);   // divisible by 4 and not by 100
    assert(isLeapYear(2023) == false);  // not divisible by 4
    assert(isLeapYear(0) == true);      // 0 % 400 == 0
    assert(isLeapYear(-4) == true);     // divisible by 4 and not by 100
    assert(isLeapYear(-100) == false);  // divisible by 100 not 400
    assert(isLeapYear(-400) == true);   // divisible by 400
}
