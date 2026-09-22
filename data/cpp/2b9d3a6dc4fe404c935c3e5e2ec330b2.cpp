// Write a C++ function named `fixParityBit` that takes a non-empty string `s` representing a binary sequence followed by a parity bit, where the last character is either `'e'` (meaning even parity is desired) or `'o'` (meaning odd parity is desired). The function must count the number of `'1'` characters in all positions except the last, determine whether that count is even or odd, and replace the final character with the correct parity bit: `'0'` if the parity condition is satisfied, `'1'` otherwise. Specifically, for even parity (`'e'`), the final bit must make the total number of `'1'`s in the entire string (including the parity bit) even; for odd parity (`'o'`), it must make the total odd. The function returns the modified string with the parity bit replaced by `'0'` or `'1'`. The input is guaranteed to contain at least two characters (so the parity bit position exists) and only the characters `'0'`, `'1'`, `'e'`, or `'o'` in the last position. Edge cases include strings with all zeros, all ones, or where the parity condition is already satisfied (in which case the final character becomes `'0'` for even parity or `'1'` for odd parity if the count itself already matches—note the logic must follow the snippet exactly: for `'e'`, if count is even set `'0'`, else set `'1'`; for `'o'`, if count is even set `'1'`, else set `'0'`).
#include <cassert>
#include <string>

std::string fixParityBit(const std::string& s);

int main() {
    // Example from snippet: even parity, count of ones is 2 (even) -> 0
    assert(fixParityBit("110e") == "1100"); // prefix "110" has 2 ones -> even, so parity bit 0
    // Odd parity, count of ones is 2 (even) -> 1
    assert(fixParityBit("110o") == "1101");
    // Even parity, count of ones is 1 (odd) -> 1
    assert(fixParityBit("10e") == "101");
    // Odd parity, count of ones is 1 (odd) -> 0
    assert(fixParityBit("10o") == "100");
    // All zeros, even parity: count 0 -> 0
    assert(fixParityBit("000e") == "0000");
    // All zeros, odd parity: count 0 -> 1
    assert(fixParityBit("000o") == "0001");
    // All ones (prefix length 3), even parity: count 3 -> 1
    assert(fixParityBit("111e") == "1111");
    // All ones (prefix length 3), odd parity: count 3 -> 0
    assert(fixParityBit("111o") == "1110");
    // Single prefix bit '1', odd parity: count 1 -> 0
    assert(fixParityBit("1o") == "10");
    // Single prefix bit '0', even parity: count 0 -> 0
    assert(fixParityBit("0e") == "00");
    return 0;
}
#include <string>

// Fix the parity bit of a binary string.
// The last character is 'e' (even parity) or 'o' (odd parity).
// Count '1's in the prefix (all but last), then set the last char.
std::string fixParityBit(const std::string& s) {
    int countOnes = 0;
    // Count '1's in the first s.size()-1 characters
    for (std::size_t i = 0; i < s.size() - 1; ++i) {
        if (s[i] == '1') {
            ++countOnes;
        }
    }
    
    std::string result = s; // work on a copy
    char mode = s.back();
    if (mode == 'e') {
        // Even parity: total count including parity bit must be even.
        result.back() = (countOnes % 2 == 0) ? '0' : '1';
    } else { // mode == 'o'
        // Odd parity: total count including parity bit must be odd.
        result.back() = (countOnes % 2 == 0) ? '1' : '0';
    }
    return result;
}
// The core algorithm is a single linear scan of the string up to the second-to-last character, counting how many `'1'` characters appear there. This count is stored as an integer. Then, based on the last character (which is a parity mode indicator), we decide the replacement bit. The decision logic is: if the mode is `'e'` (even), then if the count is even, the parity bit should be `'0'` (because adding `'0'` keeps the total even), otherwise it should be `'1'` (because adding `'1'` makes an odd count into even). If the mode is `'o'` (odd), then if the count is even, the parity bit must be `'1'` (to make the total odd), otherwise it must be `'0'` (because an odd count plus `'0'` stays odd). The function directly modifies a copy of the input string (or returns a new string) by assigning the last character accordingly. Time complexity is O(n) where n is the length of the string, and space complexity is O(n) if we return a copy or O(1) if we modify in place (we'll return a copy for safety). Important edge case: the string might already have the correct parity bit, but the function overwrites it based on the count regardless, which is correct per the specification. Also, the last character is never counted as a `'1'` since we only iterate up to `s.size()-1` exclusive.
