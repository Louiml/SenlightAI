/*
Write a C++ function that takes a binary string `s` containing at least one `'1'` and returns a new string representing the maximum odd binary number that can be formed by rearranging the characters of `s`. The resulting binary number must be odd (i.e., its last character must be `'1'`). The string may contain leading zeros in the result. The function should be const-correct and take the input by `const std::string&`. For example, given `"0101"`, the maximum odd binary number is `"1001"` (which is 9 in decimal). The function should handle strings of any length, but you may assume the input contains at least one `'1'`.
*/

#include <string>
#include <algorithm>

// Returns the maximum odd binary number that can be formed by rearranging
// the characters of the given binary string (which contains at least one '1').
std::string maximumOddBinaryNumber(const std::string& s) {
    int ones = std::count(s.begin(), s.end(), '1');
    int zeros = s.size() - ones;
    // Place all '1's except one at the front, then all '0's, then the last '1' to make it odd.
    return std::string(ones - 1, '1') + std::string(zeros, '0') + std::string(1, '1');
}

#include <cassert>
#include <string>

// Function prototype for testing
std::string maximumOddBinaryNumber(const std::string& s);

int main() {
    assert(maximumOddBinaryNumber("1") == "1");
    assert(maximumOddBinaryNumber("0001") == "0001");
    assert(maximumOddBinaryNumber("0101") == "1001");
    assert(maximumOddBinaryNumber("111") == "111");
    assert(maximumOddBinaryNumber("1010") == "1100");
    assert(maximumOddBinaryNumber("000") == "001"); // Note: at least one '1' is guaranteed, but just in case.
    assert(maximumOddBinaryNumber("1100") == "1001");
    assert(maximumOddBinaryNumber("10101") == "11100");
    assert(maximumOddBinaryNumber("00001111") == "11110000");
    assert(maximumOddBinaryNumber("1") == "1");
    return 0;
}

// The key observation is that for a binary number to be odd, the least significant bit (the rightmost character) must be `'1'`. To maximize the value of the binary number, we want to place as many `'1'`s as possible at the most significant positions (left side), because each `'1'` contributes a higher power of two when placed further left. The optimal arrangement is: place all but one `'1'` at the front (leftmost positions), followed by all `'0'`s, and finally a single `'1'` at the end to ensure the number is odd. This gives the maximum possible binary value because the higher-order bits are maximized. Edge cases: if the string contains only one `'1'` (e.g., `"1"` or `"0001"`), then the result is all zeros followed by that single `'1'` (e.g., `"0001"`). If there are no zeros, the result is all `'1'`s (which is automatically odd). The algorithm simply counts the number of `'1'`s and `'0'`s, then constructs the result string. Time complexity is O(n) where n is the length of the input string, due to counting and string construction. Space complexity is O(n) for the output string (excluding input).
