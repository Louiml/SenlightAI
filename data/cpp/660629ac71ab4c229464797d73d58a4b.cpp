Write a C++ function named `isNumberPalindrome` that takes a string representing a positive integer (containing only digits, no leading zeros) and returns a boolean indicating whether the number is a palindrome (reads the same forward and backward). The function should also handle the case where the string is empty or contains non-digit characters by returning `false`. Additionally, your solution must include a helper function to extract and return the middle digit as an integer, but only if the number is a palindrome and has an odd number of digits; for even-length palindromes, return `-1` to indicate no single middle digit exists. The main function should not be part of your solution—only the described helper functions.

// The solution involves two main functions: `isNumberPalindrome` and `middleDigit`. For `isNumberPalindrome`, first validate that the input string is non-empty and consists only of digits (using `std::all_of` with `::isdigit`). Then compare characters from the start and end moving inward. If any pair mismatches, return `false`; if all pairs match, return `true`. Time complexity is O(n) where n is the string length, and space complexity is O(1) (no extra data structures beyond a few indices). For `middleDigit`, first check if the input is a palindrome (call `isNumberPalindrome`). If not, or if the length is even, return `-1`. Otherwise, compute the middle index as `length / 2` (integer division) and return `num[middleIndex] - '0'` to convert the character digit to an integer. Edge cases include empty strings, non-digit characters, negative signs (not allowed), single-digit numbers (trivially palindromic, middle digit is the digit itself), and even-length palindromes (return -1). For complexity, `middleDigit` also operates in O(n) time due to the palindrome check, and O(1) extra space.

#include <string>
#include <algorithm>
#include <cctype>

// Returns true if the given string represents a positive integer (digits only, non-empty) and reads the same forward and backward.
bool isNumberPalindrome(const std::string& num) {
    if (num.empty()) return false;
    if (!std::all_of(num.begin(), num.end(), ::isdigit)) return false;
    
    size_t left = 0;
    size_t right = num.size() - 1;
    while (left < right) {
        if (num[left] != num[right]) return false;
        ++left;
        --right;
    }
    return true;
}

// Returns the middle digit as an integer if the number is a palindrome and has an odd length; otherwise returns -1.
int middleDigit(const std::string& num) {
    if (!isNumberPalindrome(num)) return -1;
    size_t length = num.size();
    if (length % 2 == 0) return -1;  // even length => no single middle digit
    size_t mid = length / 2;
    return num[mid] - '0';
}

#include <cassert>

int main() {
    // Basic palindrome checks
    assert(isNumberPalindrome("12321") == true);
    assert(isNumberPalindrome("12345") == false);
    assert(isNumberPalindrome("1") == true);
    assert(isNumberPalindrome("11") == true);

    // Edge cases: empty and non-digit input
    assert(isNumberPalindrome("") == false);
    assert(isNumberPalindrome("123a4") == false);
    assert(isNumberPalindrome("-121") == false);

    // Middle digit for odd-length palindromes
    assert(middleDigit("12321") == 3);
    assert(middleDigit("1") == 1);
    assert(middleDigit("123454321") == 5);

    // Even-length palindrome => no middle digit
    assert(middleDigit("11") == -1);
    assert(middleDigit("1221") == -1);

    // Non-palindrome or invalid input => -1
    assert(middleDigit("123") == -1);
    assert(middleDigit("abc") == -1);
}
