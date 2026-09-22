/*
Given a string `s` that contains exactly 9 distinct digits from `'0'` to `'9'` (i.e., it is a permutation of 9 distinct digits, missing exactly one digit), write a C++ function `findMissingDigit(const std::string& s)` that returns the single digit (as an `int`) that does **not** appear anywhere in the string. The input string length is exactly 9, contains only digit characters, and contains no duplicates. You must not assume any particular order of the digits; the missing digit could be `0` through `9`. The function must be deterministic, simple, and avoid using any extra data structures beyond a fixed-size boolean array or a set. For example, if `s = "123456789"`, the function must return `0`; if `s = "012345678"`, the function must return `9`.
*/

#include <string>

// Returns the digit (0-9) that is not present in the given string.
// The string must contain exactly 9 distinct digits from '0' to '9'.
int findMissingDigit(const std::string& s) {
    int total_sum = 45;  // 0+1+2+...+9
    int current_sum = 0;

    // Accumulate the integer value of each digit character.
    for (char c : s) {
        current_sum += c - '0';
    }

    // The missing digit is the difference.
    return total_sum - current_sum;
}

#include <cassert>

// Forward declaration of the function to test.
int findMissingDigit(const std::string& s);

int main() {
    // All digits present except 0
    assert(findMissingDigit("123456789") == 0);
    // All digits present except 9
    assert(findMissingDigit("012345678") == 9);
    // All digits present except 5
    assert(findMissingDigit("012346789") == 5);
    // Missing 1, digits in arbitrary order
    assert(findMissingDigit("987654320") == 1);
    // Missing 8, digits in shuffled order
    assert(findMissingDigit("012345679") == 8);
    // Missing 3, only 0-9 except 3
    assert(findMissingDigit("012456789") == 3);
    // Missing 7, mixed order
    assert(findMissingDigit("012345689") == 7);
    // Missing 2
    assert(findMissingDigit("013456789") == 2);
    // Missing 4
    assert(findMissingDigit("012356789") == 4);
    // Missing 6
    assert(findMissingDigit("012345789") == 6);
    return 0;
}

// The solution is based on the fact that the digits `0` through `9` sum to a fixed total: `0+1+...+9 = 45`. Since the input string contains exactly 9 distinct digits, exactly one digit from `0..9` is missing. We can iterate through each character in the string, convert it to its integer value by subtracting `'0'`, and accumulate a running sum. The missing digit is then simply `45 - sum`. This works because the sum of the present digits equals the total sum minus the missing digit. Edge cases include the missing digit being `0` (then the sum of the 9 digits is `45`), and the missing digit being `9` (then the sum is `36`). The algorithm runs in O(n) time where n is the length of the string (here n=9, but generalize to any fixed length), and O(1) auxiliary space because only a single integer variable is needed. No additional storage is required, making it both time and space efficient.
