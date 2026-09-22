/*
Write a C++ function that takes a string `seq` consisting only of the characters `'I'` and `'D'` (where `'I'` means the next number should be larger, and `'D'` means the next number should be smaller) and returns the lexicographically smallest sequence of digits (from 1 to 9, no repetition) that satisfies the pattern. The length of the input string `seq` is between 0 and 8 inclusive (i.e., the output has at most 9 digits). If the input length is 9 or more, or if the pattern is impossible to satisfy (which never occurs for length ≤ 8), return the string `"-1"`. For example, for `"I"` the output should be `"12"`, for `"D"` the output should be `"21"`, for `"IIDDD"` the output should be `"126543"`. The function must be named `smallestNumberForPattern` and accept the input by const reference.
*/
#include <string>

// Returns the lexicographically smallest digit sequence (1-9, no repeats)
// that satisfies the 'I' (increase) and 'D' (decrease) pattern.
// If the pattern length is >= 9, returns "-1" as it's impossible.
std::string smallestNumberForPattern(const std::string& seq) {
    const int n = static_cast<int>(seq.size());
    if (n >= 9) {
        return "-1";
    }

    std::string result(n + 1, ' ');
    int count = 1;

    for (int i = 0; i <= n; ++i) {
        // When we reach the end or see an 'I', we fill in the block
        // of preceding consecutive 'D's (and possibly the 'I' itself).
        if (i == n || seq[i] == 'I') {
            // Fill backwards from i-1 down to the position after the
            // last 'I' (or the beginning).
            for (int j = i - 1; j >= -1; --j) {
                // Place the current digit at position j+1.
                result[j + 1] = static_cast<char>('0' + count);
                ++count;
                // Stop once we have placed a digit right after a previous 'I'
                // or at the very beginning (j == -1).
                if (j >= 0 && seq[j] == 'I') {
                    break;
                }
            }
        }
    }
    return result;
}
#include <cassert>
#include <string>

// The solution function declaration (as defined above).
std::string smallestNumberForPattern(const std::string& seq);

int main() {
    // Basic single character patterns
    assert(smallestNumberForPattern("I") == "12");
    assert(smallestNumberForPattern("D") == "21");

    // Two-character patterns
    assert(smallestNumberForPattern("ID") == "132");
    assert(smallestNumberForPattern("DI") == "213");
    assert(smallestNumberForPattern("II") == "123");
    assert(smallestNumberForPattern("DD") == "321");

    // Longer mixed patterns
    assert(smallestNumberForPattern("DIDI") == "21435");
    assert(smallestNumberForPattern("IIDDD") == "126543");
    assert(smallestNumberForPattern("DDIDDIID") == "321654798");

    // Empty string produces just "1"
    assert(smallestNumberForPattern("") == "1");

    // Input length 9 or more returns "-1"
    assert(smallestNumberForPattern("IIIIIIIII") == "-1");
    assert(smallestNumberForPattern("DDDDDDDDD") == "-1");

    return 0;
}
// The problem is a classic "minimum number from pattern" task. The key insight is that when we encounter an `'I'` (or reach the end), we should reverse the contiguous block of preceding `'D'`s. The algorithm processes the string left to right using a counter starting at 1. Whenever we see an `'I'` or hit the end of the string, we fill in the positions from the current index backward until we just after the previous `'I'` (or the start), assigning increasing digits but in reverse order for that block. This guarantees lexicographic minimality because we assign the smallest available digits as early as possible. Edge cases: empty string (output `"1"`), length 9 or more (return `"-1"`). Time complexity is O(n) for n = length of seq (since each position is filled once), and space complexity is O(n) for the result string (plus the input string itself), ignoring the temporary variables.
