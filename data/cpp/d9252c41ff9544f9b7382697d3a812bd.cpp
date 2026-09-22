// Write a C++ function `std::string normalizeBinary(const std::string& s)` that takes a binary string `s` consisting only of characters `'0'` and `'1'`, and returns its normalized form as a string. The normalization rule is: if the input string is exactly `"0"`, return `"0"`. Otherwise, remove all leading zeros and compress the remaining number: since any non-zero binary number starts with `'1'`, the normalized representation is a `'1'` followed by exactly as many `'0'` characters as there are `'0'` digits in the original input string (including zeros that may have been after the leading ones). In other words, the output is `"1"` concatenated with a number of `'0'` equal to the count of `'0'` characters in the input. For example, `"100"` → `"100"` (one zero), `"1010"` → `"100"` (two zeros), `"0000"` → `"0"` (because it's all zeros). The input string length `n` is between 1 and 10^5. The function should handle large inputs efficiently.

// The problem is a simple counting task disguised as a string transformation. Since we only care about the presence of at least one `'1'` and the total count of `'0'` characters, we can avoid any actual binary arithmetic. The main algorithm: first check if the string is exactly `"0"` (which also covers the case of a single zero). If yes, return `"0"`. Otherwise, iterate through the string and count every character that equals `'0'`. The result is a string built as `"1"` followed by that many `'0'` characters. Edge cases: input like `"0"` returns `"0"`; input like `"000"` (all zeros but not exactly `"0"`) also returns `"0"` because there is no `'1'` to anchor the output — but wait, the problem statement says if the input is exactly `"0"` return `"0"`. For `"000"`, the rule "since any non-zero binary number starts with '1'" implies that if there is no `'1'` at all, the number is zero, so we should also return `"0"`. So a more robust check is: if the string contains no `'1'`, return `"0"`. If it contains at least one `'1'`, then count zeros and return `"1"` + zeros. This handles all cases. The time complexity is O(n) for the single pass to count zeros, and the space complexity is O(n) for the output string (which has length 1 + zero count, at most n). The approach is purely linear.

#include <string>

// Normalize a binary string: if it contains no '1', return "0";
// otherwise return "1" followed by as many '0's as there are in the input.
std::string normalizeBinary(const std::string& s) {
    int zeroCount = 0;
    bool hasOne = false;

    for (char c : s) {
        if (c == '1') {
            hasOne = true;
        } else if (c == '0') {
            ++zeroCount;
        }
    }

    if (!hasOne) {
        return "0";
    }

    return "1" + std::string(zeroCount, '0');
}

#include <cassert>
#include <string>
#include <iostream>

// The solution function declaration (for testing)
std::string normalizeBinary(const std::string& s);

int main() {
    assert(normalizeBinary("0") == "0");
    assert(normalizeBinary("1") == "1");
    assert(normalizeBinary("10") == "10");
    assert(normalizeBinary("11") == "1");
    assert(normalizeBinary("1010") == "100");
    assert(normalizeBinary("000") == "0");
    assert(normalizeBinary("1000001") == "100000");
    assert(normalizeBinary("0001") == "10");
    assert(normalizeBinary("01010") == "100");
    assert(normalizeBinary("111111") == "1");
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
