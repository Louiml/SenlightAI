// Write a C++ function named `printTrianglePattern` that takes a single positive integer `n` and returns a `std::string` containing a right-aligned triangular pattern with `n` rows. For each row `i` (0-indexed), the pattern should contain exactly `i+1` characters: the first and last characters of the row are asterisks `*`, the last row (row `n-1`) consists entirely of asterisks, and all other interior positions (between the first and last) are periods `.`. Each row must be terminated by a newline character `\n`. The function must handle the edge case `n = 1` by returning a single asterisk followed by a newline. The output string should have no extra leading or trailing whitespace other than the row-terminating newlines.
#include <cassert>
#include <string>

// Declaration of the function under test
std::string printTrianglePattern(int n);

int main() {
    // n = 1: single asterisk row
    assert(printTrianglePattern(1) == "*\n");

    // n = 2: two rows; last row is all stars
    assert(printTrianglePattern(2) == "*\n**\n");

    // n = 3: classic small triangle
    assert(printTrianglePattern(3) == "*\n**\n***\n");

    // n = 4: first interior dot appears
    assert(printTrianglePattern(4) == "*\n**\n*.*\n****\n");

    // n = 5: multiple dots in interior rows
    assert(printTrianglePattern(5) == "*\n**\n*.*\n*..*\n*****\n");

    // n = 6: last two interior rows have dots
    assert(printTrianglePattern(6) == "*\n**\n*.*\n*..*\n*...*\n******\n");

    // n = 7: verify pattern scales, last row length matches n
    std::string result = printTrianglePattern(7);
    assert(result.length() == 7 * 8 / 2 + 7); // total chars = 28 pattern + 7 newlines
    assert(result[result.size() - 1] == '\n');
    // Sanity check for a middle row: row 4 (0-indexed) should be "*...*"
    assert(result.substr(0, 5) == "*\n**\n*.*\n*..*\n");

    // n = 0 is not in spec; but if called, loop does nothing and returns empty
    assert(printTrianglePattern(0) == "");

    return 0;
}
#include <string>

// Build a triangular pattern: row i has i+1 chars; first and last are '*',
// interior chars are '.', and the final row is all '*'s.
// Each row ends with a newline. Returns the complete pattern as a string.
std::string printTrianglePattern(int n) {
    std::string result;
    result.reserve(static_cast<size_t>(n * (n + 1) / 2 + n)); // preallocate for efficiency

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j <= i; ++j) {
            if (j == 0 || i == n - 1 || j == i) {
                result += '*';
            } else {
                result += '.';
            }
        }
        result += '\n';
    }
    return result;
}
// The algorithm directly simulates the nested loop logic from the code snippet but builds a string instead of printing to the console. For each row index `i` from 0 to `n-1`, iterate over column index `j` from 0 to `i` inclusive. A character is `*` if `j == 0` (first column), `j == i` (last column for that row), or `i == n-1` (last row). Otherwise, the character is `.`. After finishing all columns of a row, append a newline. The key edge case is when `n == 1`: the condition `j == 0` and `j == i` both hold for `j=0`, so it correctly produces a single `*`. The condition `i == n-1` also holds for row 0 when `n=1`, but since `j==0` and `j==i` already trigger, it doesn't cause extra characters. No special handling for `n` beyond ensuring it is positive is required, but we can assume the input is valid. Time complexity is O(n^2) because the total number of characters is the sum of the arithmetic series `1 + 2 + ... + n = n(n+1)/2`. Space complexity is O(n^2) in total for the returned string, but the auxiliary space used during computation is only O(1) beyond the string itself.
