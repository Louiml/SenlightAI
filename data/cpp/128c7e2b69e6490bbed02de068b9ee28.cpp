Write a C++ function named `minimumFlipsToAlternate` that takes a non-empty string consisting only of characters `'0'` and `'1'`, and returns the minimum number of bit flips (changing a `'0'` to `'1'` or vice versa) required to make the entire string alternate between `'0'` and `'1'` (i.e., no two adjacent characters are the same). The string length can be up to \(10^5\), so the solution must be efficient. For example, for input `"0001010111"` the function should return `2` because flipping the 2nd bit from `0` to `1` and the 9th bit from `1` to `0` yields `"0101010101"`, which is alternating. The function must be declared with `const` correctness, use only standard library headers, and not include a `main` function.

#include <cassert>

int main() {
    assert(minimumFlipsToAlternate("0") == 0);
    assert(minimumFlipsToAlternate("1") == 0);
    assert(minimumFlipsToAlternate("00") == 1);
    assert(minimumFlipsToAlternate("11") == 1);
    assert(minimumFlipsToAlternate("0101") == 0);
    assert(minimumFlipsToAlternate("1010") == 0);
    assert(minimumFlipsToAlternate("001") == 1);
    assert(minimumFlipsToAlternate("0001010111") == 2);
    assert(minimumFlipsToAlternate("111000") == 2);
    assert(minimumFlipsToAlternate("010") == 0);
}

#include <string>
#include <algorithm>

// Helper: count flips needed to make string alternate starting with 'expected'.
int flipsForPattern(const std::string& s, char expected) {
    int flipCount = 0;
    for (char c : s) {
        if (c != expected) {
            ++flipCount;
        }
        expected = (expected == '0') ? '1' : '0';
    }
    return flipCount;
}

// Returns the minimum number of bit flips to make the string alternate.
int minimumFlipsToAlternate(const std::string& s) {
    return std::min(flipsForPattern(s, '0'), flipsForPattern(s, '1'));
}

// The key observation is that an alternating binary string has only two possible patterns: one starting with `'0'` (i.e., `"010101..."`) and one starting with `'1'` (i.e., `"101010..."`). For any given string, the minimum flips needed is the smaller of the mismatches against these two patterns.  
// Algorithm:  
// 1. Define a helper function that, given a starting expected character, iterates through the string. For each position, if the current character differs from the expected character, increment a flip counter. Then toggle the expected character to the opposite for the next position.  
// 2. Call this helper twice—once with expected `'0'` and once with expected `'1'`—and return the minimum of the two counts.  
// Edge cases: The string length is always at least 1, so both patterns are valid. If the string is already alternating, one of the counts will be zero. If the string length is 1, both counts will be 0 (since the single character matches either starting pattern).  
// Time complexity: \(O(n)\) because each character is examined exactly twice (once per pattern). Space complexity: \(O(1)\) auxiliary space, not counting the input string itself.
