Write a C++ function named `maximumOddBinaryString` that takes a string `s` consisting only of the characters `'0'` and `'1'`, and returns a new string that is a permutation of `s` such that:
1. The binary value represented by the returned string is an odd number (i.e., its least significant bit is `'1'`).
2. Among all possible odd-numbered permutations, the returned string is the largest possible binary value when interpreted as an unsigned integer (i.e., it has the most `'1'` bits at the most significant positions).
The input string `s` always contains at least one `'1'`, so an odd permutation always exists. The function should not modify the input string and should be efficient in both time and space with respect to the length of the string.

#include <cassert>
#include <string>

int main() {
    // Basic cases
    assert(maximumOddBinaryString("010") == "100");
    assert(maximumOddBinaryString("110") == "101");
    assert(maximumOddBinaryString("101") == "110");
    assert(maximumOddBinaryString("111") == "111");
    assert(maximumOddBinaryString("1") == "1");

    // Single '1' mixed with zeros
    assert(maximumOddBinaryString("0001") == "0001");
    assert(maximumOddBinaryString("0010") == "0001");
    assert(maximumOddBinaryString("1000") == "0001");

    // Multiple '1's and '0's
    assert(maximumOddBinaryString("1100") == "1001");
    assert(maximumOddBinaryString("1010") == "1001");
    assert(maximumOddBinaryString("0110") == "1001");

    // Larger random-like
    assert(maximumOddBinaryString("101010") == "110000");
    assert(maximumOddBinaryString("00001111") == "11110001");

    // All ones except one zero
    assert(maximumOddBinaryString("1110") == "1101");
    assert(maximumOddBinaryString("0111") == "1101");
}

#include <string>
#include <algorithm>

// Return the largest odd binary number permutation of the input string.
std::string maximumOddBinaryString(const std::string& s) {
    const int n = static_cast<int>(s.size());
    const int count_ones = static_cast<int>(std::count(s.begin(), s.end(), '1'));

    std::string result(n, '0');

    // Place all but one '1' at the most significant positions.
    for (int i = 0; i < count_ones - 1; ++i) {
        result[i] = '1';
    }

    // Ensure the least significant bit is '1' for oddness.
    result[n - 1] = '1';

    return result;
}

// The key insight is that to make the binary number odd, the rightmost bit (index `n-1`) must be `'1'`. To maximize the numeric value while keeping it odd, we should place all remaining `'1'` bits as far left as possible. Therefore, the optimal permutation is: all extra `'1'`s (count minus one) are placed at the beginning of the string, followed by all `'0'`s, and finally a single `'1'` at the last position. If there is exactly one `'1'`, the result is a string of all `'0'`s except the last character is `'1'`. Edge cases include strings of length 1 (then the only `'1'` is the last character, and it is already odd), strings with all `'1'`s (then the result is all `'1'`s except the last still `'1'`, which is unchanged from all ones), and strings with no zeros (the result is all ones). The algorithm counts the number of `'1'`s in the input using a single pass (or `std::count`), builds the result string of length `n` initialized to `'0'`, then fills the first `count-1` positions with `'1'` and sets the last position to `'1'`. If `count` is zero, this case is not allowed per the task. Time complexity is O(n) for counting and O(n) for building the result string, giving O(n) total. Space complexity is O(n) for the result string. The solution is simple, avoids extra memory beyond the output, and handles all valid inputs correctly.
