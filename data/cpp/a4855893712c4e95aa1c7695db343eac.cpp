Write a C++ function `bool isDangerous(const std::string& players)` that examines a string consisting only of characters `'0'` and `'1'` (where `'0'` represents one team and `'1'` represents the other). The function must return `true` if the string contains at least seven consecutive identical characters (i.e., seven `'0'`s in a row or seven `'1'`s in a row), and `false` otherwise. The input string will be non-empty and may have length up to 100. Your function should not read from or write to standard input/output; it should only analyze the given string.

The core algorithm is a linear scan of the string while maintaining two counters: `count0` for consecutive `'0'`s and `count1` for consecutive `'1'`s. As we iterate through each character, we increment the counter for that character and reset the other counter to zero. After each increment, we check if the current counter reaches 7; if so, we immediately return `true`. This early exit avoids unnecessary scanning once the condition is satisfied. Edge cases include: the string consisting entirely of one character (e.g., `"0000000"` → true, `"111111"` → false), a string where the dangerous run occurs at the very end (e.g., `"1010101010101111111"` → true), and strings that never reach 7 consecutive characters (e.g., `"01010101"` → false). The algorithm runs in O(n) time, where n is the string length, and uses O(1) auxiliary space because only two integer counters are needed.

#include <string>

// Returns true if the string contains at least seven consecutive identical characters.
bool isDangerous(const std::string& players) {
    int count0 = 0;  // Consecutive '0's seen so far
    int count1 = 0;  // Consecutive '1's seen so far

    for (char c : players) {
        if (c == '0') {
            ++count0;
            count1 = 0;  // Reset '1' counter on a '0'
        } else {  // c == '1'
            ++count1;
            count0 = 0;  // Reset '0' counter on a '1'
        }

        if (count0 >= 7 || count1 >= 7) {
            return true;
        }
    }
    return false;
}

#include <cassert>
#include <string>

// Source solution function (declared here to include in test, but in actual task it would be in a header)
bool isDangerous(const std::string& players);

int main() {
    // Basic true cases
    assert(isDangerous("0000000") == true);               // exactly 7 zeros
    assert(isDangerous("1111111") == true);               // exactly 7 ones
    assert(isDangerous("1010101010101111111") == true);   // 7 ones at end
    assert(isDangerous("00000001111") == true);           // 7 zeros at start

    // Basic false cases
    assert(isDangerous("000000") == false);               // only 6 zeros
    assert(isDangerous("111111") == false);               // only 6 ones
    assert(isDangerous("01010101") == false);             // alternating
    assert(isDangerous("1") == false);                    // single character

    // Edge: run of 7 appears but not at beginning
    assert(isDangerous("00110011000000011") == true);     // 7 zeros in middle

    // Long string with no danger
    assert(isDangerous("010101010101010101010101010101010") == false);

    return 0;
}
