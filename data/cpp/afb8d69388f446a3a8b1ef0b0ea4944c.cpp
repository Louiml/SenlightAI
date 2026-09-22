// Write a C++ function `longestZeroRun` that takes a non-empty string `s` consisting only of the characters `'0'` and `'1'`, and returns the length of the longest contiguous (consecutive) block of `'0'` characters. If there are no zeros in the string, the function should return `0`. The function must not modify the input string and should handle strings of any length, including those with only ones or only zeros.
// The algorithm performs a single left-to-right scan of the input string, tracking the current consecutive zero count. When a `'0'` is encountered, the current count is incremented; when a `'1'` is encountered (or at the end of the scan), the count resets to zero. After each update, the current count is compared against a running maximum and the maximum is updated if needed. Edge cases include: an empty string (not allowed per task, but the function can return 0 defensively), a string with no zeros (returns 0), a string with zeros only (returns the entire length), and a transition between runs (the reset logic handles it naturally). Time complexity is \(O(n)\) where \(n\) is the string length, and space complexity is \(O(1)\) (only constant extra storage for counters).
#include <string>
#include <algorithm>

// Returns the length of the longest contiguous run of '0' characters in s.
// If s contains no '0', returns 0.
int longestZeroRun(const std::string& s) {
    int currentRun = 0;
    int maxRun = 0;

    for (char c : s) {
        if (c == '0') {
            ++currentRun;
            maxRun = std::max(maxRun, currentRun);
        } else {
            currentRun = 0;
        }
    }

    return maxRun;
}
#include <cassert>

int main() {
    // Basic runs
    assert(longestZeroRun("101") == 1);
    assert(longestZeroRun("10001") == 3);
    assert(longestZeroRun("000") == 3);
    assert(longestZeroRun("111") == 0);
    assert(longestZeroRun("0") == 1);
    assert(longestZeroRun("1") == 0);

    // Mixed longer strings
    assert(longestZeroRun("1001000") == 3);
    assert(longestZeroRun("010010001") == 3);
    assert(longestZeroRun("000111000") == 3);
    assert(longestZeroRun("001100") == 2);

    return 0;
}
