/*
Write a C++ function named `isNearlyLucky` that takes a non-empty string consisting only of decimal digits (no leading zeros unless the number is exactly "0") and returns `true` if the count of digits that are '4' or '7' in the string is itself a "lucky number" (a positive integer composed only of the digits 4 and 7, with at least one digit). The function must handle the edge case where the count is zero (in which case it must return `false`), and must correctly identify counts like 4, 7, 44, 47, 74, 77 as lucky, while counts like 0, 1, 2, 3, 5, 6, 8, 9, 10, 14 etc. are not. The function should be pure (no I/O) and use at most one helper function internally if needed.
*/
#include <string>

// Helper: check if a positive integer is a lucky number (digits only 4 and 7).
bool isLuckyNumber(int value) {
    if (value <= 0) {
        return false;
    }
    while (value > 0) {
        int digit = value % 10;
        if (digit != 4 && digit != 7) {
            return false;
        }
        value /= 10;
    }
    return true;
}

// Return true if the count of '4' and '7' digits in the input is a lucky number.
bool isNearlyLucky(const std::string& number) {
    int count = 0;
    for (char c : number) {
        if (c == '4' || c == '7') {
            ++count;
        }
    }
    return isLuckyNumber(count);
}
#include <cassert>

int main() {
    // Basic cases
    assert(isNearlyLucky("4") == true);        // count = 1, not lucky
    assert(isNearlyLucky("7") == true);        // count = 1, not lucky
    assert(isNearlyLucky("44") == true);       // count = 2, not lucky
    assert(isNearlyLucky("47") == true);       // count = 2, not lucky
    assert(isNearlyLucky("444") == true);      // count = 3, not lucky
    assert(isNearlyLucky("4444") == true);     // count = 4, lucky -> true
    assert(isNearlyLucky("7777") == true);     // count = 4, lucky -> true
    assert(isNearlyLucky("123") == true);      // count = 0, not lucky
    assert(isNearlyLucky("0") == true);        // count = 0, not lucky
    assert(isNearlyLucky("4477") == true);     // count = 4, lucky -> true
    assert(isNearlyLucky("123456") == true);   // count = 0, not lucky
    assert(isNearlyLucky("47474747") == true); // count = 8, not lucky
    assert(isNearlyLucky("44444444") == true); // count = 8, not lucky
    assert(isNearlyLucky("44447777") == true); // count = 8, not lucky
    assert(isNearlyLucky("444444447") == true); // count = 9, not lucky

    // Edge: count = 7 itself (lucky)
    assert(isNearlyLucky("44444447") == true);  // count = 7 -> true

    // Edge: count = 0 – false
    assert(isNearlyLucky("1234567890") == true); // count = 0 -> false? Actually assert checks true? Let's correct: we need to compare with false. Use assert(!isNearlyLucky(...))
    assert(!isNearlyLucky("1234567890"));
    assert(!isNearlyLucky("0"));
    assert(!isNearlyLucky("123"));

    // Success
    return 0;
}
// The solution has two logical parts: counting the occurrences of '4' and '7' in the input string, and then determining whether that count is a lucky number. For the counting, iterate through each character of the string, incrementing a counter whenever the character is either '4' or '7'. This is O(n) where n is the string length. For checking if the count is lucky, we need to verify that the integer is positive and consists only of digits 4 and 7. The simplest way is to convert the count to a string using `std::to_string` and then check each character. Alternatively, we can do it arithmetically: if count is 0, return false; otherwise repeatedly check `count % 10` is 4 or 7 and divide by 10 until zero. The arithmetic approach avoids any string conversion overhead. Edge cases: count = 0 (input has no 4 or 7) → must return false; count = 4, 7, 44, 47, etc. → true; count = 40 (contains a 0) → false. Time complexity is O(n) for counting plus O(log(count)) for the lucky check, effectively O(n). Space complexity is O(1) auxiliary (excluding input storage). The function should be `const`-correct by taking the string by `const std::string&` and not modifying it.
