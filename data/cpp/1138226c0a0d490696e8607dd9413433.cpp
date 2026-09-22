/*
Write a standalone C++ function named `printRightTriangle` that takes an integer `n` and returns a string containing a right-aligned triangle of asterisks with `n` rows. Row `i` (1-indexed) must contain exactly `i` asterisks separated by a single space, and each row must end with a newline character (`\n`). If `n` is less than or equal to zero, the function must return an empty string. The function should not print anything to the console; it must only build and return the string.
*/
#include <string>

// Build a right-aligned triangle of asterisks with n rows.
// Row i contains i asterisks separated by spaces, followed by '\n'.
// Returns an empty string if n <= 0.
std::string printRightTriangle(int n) {
    if (n <= 0) {
        return "";
    }

    std::string result;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= i; ++j) {
            if (j > 1) {
                result += ' ';
            }
            result += '*';
        }
        result += '\n';
    }
    return result;
}
#include <cassert>
#include <string>

// Declaration of the function under test
std::string printRightTriangle(int n);

int main() {
    // Basic triangle for n=3
    assert(printRightTriangle(3) == "*\n* *\n* * *\n");

    // Triangle for n=1
    assert(printRightTriangle(1) == "*\n");

    // Edge case: n=0 returns empty
    assert(printRightTriangle(0) == "");

    // Edge case: negative n returns empty
    assert(printRightTriangle(-5) == "");

    // Triangle for n=5 (verify correct number of rows and content)
    std::string expected = "*\n"
                           "* *\n"
                           "* * *\n"
                           "* * * *\n"
                           "* * * * *\n";
    assert(printRightTriangle(5) == expected);

    // Check that no extra whitespace or missing newlines
    assert(printRightTriangle(2) == "*\n* *\n");

    return 0;
}
// The solution uses two nested loops to construct the triangle. The outer loop iterates `i` from 1 to `n` rows. The inner loop iterates `j` from 1 to `i` to append asterisks to a `std::string` result, adding a space after each asterisk except the last in the row (or simply adding a space before each subsequent asterisk). After completing each row, a newline character is appended. Edge cases: if `n` is negative or zero, the loops do not execute and an empty string is returned. The number of characters in the output is roughly sum of (2*i - 1) for i=1..n plus n newlines, which is O(n²) space. Time complexity is O(n²) because the inner loop runs `i` times for each `i`, and `i` goes up to `n`. The space complexity is O(n²) for the returned string. No extra data structures are needed.
