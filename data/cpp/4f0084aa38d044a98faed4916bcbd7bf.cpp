Write a C++ function named `sumOfDigitsFromCharArray` that takes a null-terminated character array (C-string) representing only decimal digits (no spaces, letters, or symbols) and returns the sum of all its digit values. The input array is guaranteed to be non-empty and contain only characters from '0' to '9'. Your function must not use any standard library numeric conversion functions like `atoi` or `stoi`; instead, convert each character to its numeric value by subtracting the ASCII value of '0'. Handle the case where the array may be very large (up to 100 characters) efficiently. The function should be `const`-correct, taking the array by pointer-to-const.
The solution iterates through the character array from index 0 until it hits the null terminator `'\0'`. For each character, we verify it is a digit using `std::isdigit` (converted to `unsigned char` to avoid undefined behavior for negative `char` values). If it is a digit, we add `c - '0'` to a running sum. The null terminator is ignored. Edge cases: an empty array is not allowed per specification, but if it were, the loop would simply not execute and return 0. The input contains only digits, so no negative numbers or other characters are expected; however, the `isdigit` check is included for safety. Time complexity is O(n) where n is the length of the array (up to 100), and space complexity is O(1) since we only use a single integer accumulator. The function does not modify the input, so the parameter is `const char*`.
#include <cctype>  // for std::isdigit

// Returns the sum of all digit values in a C-string containing only digits.
int sumOfDigitsFromCharArray(const char* arr) {
    int sum = 0;
    for (int i = 0; arr[i] != '\0'; ++i) {
        unsigned char ch = static_cast<unsigned char>(arr[i]);
        if (std::isdigit(ch)) {
            sum += arr[i] - '0';
        }
    }
    return sum;
}
#include <cassert>

int main() {
    // Basic test with consecutive digits
    assert(sumOfDigitsFromCharArray("12345") == 15);
    // Single digit
    assert(sumOfDigitsFromCharArray("7") == 7);
    // All zeros
    assert(sumOfDigitsFromCharArray("0000") == 0);
    // Digits in arbitrary order
    assert(sumOfDigitsFromCharArray("9081726354") == 45); // sum of 0-9
    // Maximum length (100 digits, all '9') -> 900
    char longArr[101];
    for (int i = 0; i < 100; ++i) longArr[i] = '9';
    longArr[100] = '\0';
    assert(sumOfDigitsFromCharArray(longArr) == 900);
    // Non-digit characters are ignored (though spec says only digits, check safety)
    assert(sumOfDigitsFromCharArray("a1b2") == 3);
    // Empty string (should return 0 for safety)
    assert(sumOfDigitsFromCharArray("") == 0);

    return 0;
}
