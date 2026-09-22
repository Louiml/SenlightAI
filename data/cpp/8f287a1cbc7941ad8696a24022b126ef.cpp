/*
Write a standalone C++ function named `generateSpiralSequence` that takes a positive integer `numRows` and returns a `std::string` containing the same output as the given code snippet: starting with 1 and incrementing by 1 on each printed character, but resetting to 0 when the count reaches 10 (i.e., values cycle from 0 to 9). The function should print `numRows` lines; the first line contains `numRows` digits, the second line contains `numRows-1` digits, and so on, until the last line contains exactly 1 digit. Each line is followed by a newline character `\n`. The function returns the complete multi-line string. For example, with `numRows = 4`, the returned string must be `"1234\n567\n89\n0\n"`. If `numRows` is 0 or negative, return an empty string.
*/

#include <string>

// Returns a string with numRows lines. Line i (starting from 1) has (numRows - i + 1) digits.
// Digits cycle from 1 to 9, then 0, then 1 again. Each line ends with '\n'.
// Returns empty string if numRows <= 0.
std::string generateSpiralSequence(const int numRows) {
    if (numRows <= 0) {
        return "";
    }

    std::string result;
    result.reserve(static_cast<size_t>(numRows) * (numRows + 1) / 2 + numRows);

    int currentDigit = 1;
    for (int row = numRows; row >= 1; --row) {
        for (int col = 1; col <= row; ++col) {
            if (currentDigit == 10) {
                currentDigit = 0;
            }
            result.push_back(static_cast<char>('0' + currentDigit));
            ++currentDigit;
        }
        result.push_back('\n');
    }
    return result;
}

#include <cassert>
#include <string>

// Declaration from solution
std::string generateSpiralSequence(const int numRows);

int main() {
    assert(generateSpiralSequence(1) == "1\n");
    assert(generateSpiralSequence(2) == "12\n3\n");
    assert(generateSpiralSequence(3) == "123\n45\n6\n");
    assert(generateSpiralSequence(4) == "1234\n567\n89\n0\n");
    assert(generateSpiralSequence(5) == "12345\n6789\n012\n34\n5\n");
    assert(generateSpiralSequence(10) == "1234567890\n123456789\n01234567\n89012345\n67890123\n45678901\n23456789\n01234567\n890123\n456\n");
    assert(generateSpiralSequence(0) == "");
    assert(generateSpiralSequence(-3) == "");
    // Extra check: ensure no leading/trailing spaces, only expected newlines.
    assert(generateSpiralSequence(4).size() == 10); // 4+3+2+1 digits + 4 newlines = 10
    return 0;
}

// The problem is a straightforward pattern-generation task. We maintain a counter `currentDigit` that starts at 1 and represents the next digit to output. For each row from `numRows` down to 1, we output exactly `row` digits. For each digit, we output `currentDigit` but if `currentDigit` equals 10, we set it to 0 before outputting (so the cycle is 1,2,...,9,0,1,...). After outputting a digit, we increment `currentDigit` (so after 9 we get 10, which becomes 0 next time, then 1, etc.). After finishing a row, we append a newline `\n`. Edge cases: if `numRows <= 0`, return an empty string (no output). The algorithm runs in O(numRows^2) time because the total number of digits printed is the sum of 1 to numRows, which is numRows*(numRows+1)/2. Auxiliary space is O(numRows^2) because we build a string of that length; if we were printing directly, we could use O(1) auxiliary space, but here we need to return a string.
