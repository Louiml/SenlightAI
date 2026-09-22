// Write a C++ function `sortedPlusString(const std::string& input)` that takes a string representing a sum of single-digit numbers from 1 to 3, separated by `'+'` characters (e.g., `"3+1+2+3+1"`), and returns the same sum expression but with the digits rearranged in non-decreasing order, preserving the `'+'` separators. For example, `"3+1+2"` becomes `"1+2+3"`. The input string is guaranteed to contain at least one digit, only digits 1, 2, or 3, and the digits are separated by single `'+'` characters with no spaces. The output must have the same number of `'+'` separators as the input, and no trailing or leading spaces.

The problem reduces to counting how many times each digit (1, 2, or 3) appears in the input string, then building the output by writing all the 1s first, then all the 2s, then all the 3s, each separated by `'+'`. Because the input alternates digit, `'+'`, digit, we can simply iterate over every character at even indices (0, 2, 4, ...) to extract the digits. We maintain a count array of size 4 (indices 1, 2, 3 used; index 0 ignored). Then we construct the result by, for each digit from 1 to 3, appending that digit as many times as its count, and placing a `'+'` between every pair of digits. A critical edge case is when there is only one digit in total — no `'+'` should appear in the output. Also, we must handle counts of zero for some digits gracefully, skipping them entirely. The time complexity is \(O(n)\) where \(n\) is the length of the input string (since we scan it once and then build an output of proportional size), and the space complexity is \(O(1)\) auxiliary (the count array is fixed size and the output string is part of the return value). The approach is straightforward and robust.

#include <string>
#include <vector>

// Rearranges a string of '1'..'3' separated by '+' into non-decreasing order.
std::string sortedPlusString(const std::string& input) {
    int count[4] = {0, 0, 0, 0}; // indices 1..3 used
    
    // Digits appear at even positions in the input (0, 2, 4, ...).
    for (std::size_t i = 0; i < input.size(); i += 2) {
        int digit = input[i] - '0';
        if (digit >= 1 && digit <= 3) {
            count[digit]++;
        }
    }
    
    std::string result;
    bool first = true;
    for (int d = 1; d <= 3; ++d) {
        for (int j = 0; j < count[d]; ++j) {
            if (!first) {
                result.push_back('+');
            }
            result.push_back(static_cast<char>('0' + d));
            first = false;
        }
    }
    return result;
}

#include <cassert>
#include <string>

int main() {
    assert(sortedPlusString("1") == "1");
    assert(sortedPlusString("3+2+1") == "1+2+3");
    assert(sortedPlusString("3+3+3") == "3+3+3");
    assert(sortedPlusString("1+2+3+1") == "1+1+2+3");
    assert(sortedPlusString("2+2+1+3+2") == "1+2+2+2+3");
    assert(sortedPlusString("3+1+2+3+1+2") == "1+1+2+2+3+3");
    assert(sortedPlusString("2+1") == "1+2");
    assert(sortedPlusString("3+1") == "1+3");
    assert(sortedPlusString("2+3") == "2+3");
    assert(sortedPlusString("1+1+1") == "1+1+1");
}
