/*
Write a C++ function named `maximumOddBinaryNumber` that takes a non-empty string `s` consisting only of the characters `'0'` and `'1'`, and returns a new string of the same length representing the maximum odd binary number that can be formed by rearranging the characters of `s`. The output must be odd, meaning its least significant bit (rightmost character) must be `'1'`. Among all possible odd binary numbers that can be formed from the same multiset of bits, the function must return the one with the greatest numerical value. The input string may contain any number of `'1'`s and `'0'`s, but it is guaranteed to contain at least one `'1'`. Your function should be efficient and handle strings of arbitrary length.
*/
#include <string>
#include <algorithm>

// Return the maximum odd binary number that can be formed by rearranging bits of s.
// s is a non-empty string of '0' and '1' and contains at least one '1'.
std::string maximumOddBinaryNumber(const std::string& s) {
    int n = static_cast<int>(s.size());
    // Count the number of '1's in the input
    int ones = static_cast<int>(std::count(s.begin(), s.end(), '1'));
    
    // Build result: (ones - 1) leading '1's, then (n - ones) zeros, then a final '1'
    std::string result;
    result.reserve(n);
    result.append(ones - 1, '1');
    result.append(n - ones, '0');
    result.push_back('1');
    return result;
}
#include <cassert>
#include <string>

// Assume maximumOddBinaryNumber is defined above

int main() {
    // Basic cases
    assert(maximumOddBinaryNumber("1") == "1");
    assert(maximumOddBinaryNumber("01") == "01");
    assert(maximumOddBinaryNumber("10") == "01");
    assert(maximumOddBinaryNumber("11") == "11");
    
    // Mixed bits
    assert(maximumOddBinaryNumber("0101") == "1001");
    assert(maximumOddBinaryNumber("1010") == "1001");
    assert(maximumOddBinaryNumber("0001") == "0001");
    assert(maximumOddBinaryNumber("1000") == "0001");
    
    // Multiple ones and zeros
    assert(maximumOddBinaryNumber("111000") == "110001");
    assert(maximumOddBinaryNumber("010101") == "110001");
    assert(maximumOddBinaryNumber("11111") == "11111");
    assert(maximumOddBinaryNumber("00000") == "00001"); // though input guaranteed at least one '1', test robustness
    assert(maximumOddBinaryNumber("1010101010") == "1111100001");
    
    return 0;
}
// The key insight is that for a binary number to be odd, its least significant bit (LSB) must be `1`. To maximize the numeric value, we want all remaining `1`s to be placed as far left (most significant) as possible, and all `0`s to be placed after them. Thus, the optimal arrangement is: all `1`s except one placed at the leftmost positions (MSB side), followed by all `0`s, and finally a single `1` at the rightmost position (LSB). The algorithm counts the total number of `'1'`s in the input. If the count is `k`, then the result string should have `k-1` leading `'1'`s, then `(n - k)` zeros, and finally a `'1'`. This is equivalently done by initializing a result string of all `'0'`s, placing a `'1'` at the LSB position, and then placing `'1'`s from the MSB position for each remaining `'1'` in the input, which is exactly the approach in the snippet. Alternatively, we can build the string directly. Edge cases: if the string is all `'1'`s (e.g., `"111"`), then the result is `"111"` (since we place `k-1` leading ones and one trailing one, with no zeros). If there is exactly one `'1'`, the result is `"0...01"` (all zeros except the last character). The time complexity is O(n) because we scan the input once and construct the result in O(n) time. The space complexity is O(n) for the result string (excluding input). The algorithm is straightforward and does not require sorting.
