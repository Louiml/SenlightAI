Write a C++ function named `printStarPattern` that takes a single positive integer `n` as input and returns a string containing the following star pattern: for the first `n` rows, the i-th row (1-indexed) contains exactly `i` asterisks (`*`); for the next `n-1` rows, the row contains exactly `n-1`, `n-2`, …, `1` asterisks respectively. Each row must be separated by a newline character `'\n'` with no trailing newline after the last row. If `n` is 0 or negative, return an empty string. The function must produce the exact same output as the given code snippet for valid positive inputs. Ensure the function is `const`-correct and does not use any I/O operations directly—only builds and returns the string.

// The solution builds a string row by row. For the increasing part, iterate `i` from 0 to n-1 and append `(i+1)` asterisks followed by a newline (except after the very last row, which is handled by the second loop). For the decreasing part, iterate `i` from 0 to n-2 and append `(n-1-i)` asterisks, each followed by a newline. To avoid an extra trailing newline, we can build the entire string in one loop using a counter that tracks the total rows: the total number of rows is `2*n - 1`. For row index `r` (0-indexed), the star count is `min(r+1, 2*n-1-r)`. Append that many asterisks, then append `'\n'` only if `r != 2*n-2`. For `n <= 0`, return an empty string. Time complexity is O(n^2) because we append O(n) stars per row for O(n) rows; space complexity is O(n^2) for the resulting string. Edge case: `n = 1` produces just one row with one star and no trailing newline, which is correctly handled by the formula.

#include <string>

// Return the star pattern string for given n.
// For n <= 0, return an empty string.
std::string printStarPattern(int n) {
    if (n <= 0) return "";

    std::string result;
    int totalRows = 2 * n - 1;
    for (int r = 0; r < totalRows; ++r) {
        int stars = (r < n) ? (r + 1) : (2 * n - 1 - r);
        result.append(stars, '*');
        if (r != totalRows - 1) {
            result.push_back('\n');
        }
    }
    return result;
}

#include <cassert>
#include <string>

std::string printStarPattern(int n); // Declare the function

int main() {
    // n = 1: single row, no trailing newline
    assert(printStarPattern(1) == "*");
    // n = 2: 1 star, 2 stars, 1 star
    assert(printStarPattern(2) == "*\n**\n*");
    // n = 3: 1,2,3,2,1 rows
    assert(printStarPattern(3) == "*\n**\n***\n**\n*");
    // n = 4: 1,2,3,4,3,2,1 rows
    assert(printStarPattern(4) == "*\n**\n***\n****\n***\n**\n*");
    // n = 5: 1..5..1
    assert(printStarPattern(5) == "*\n**\n***\n****\n*****\n****\n***\n**\n*");
    // Edge case: n = 0 -> empty string
    assert(printStarPattern(0) == "");
    // Edge case: n = -3 -> empty string
    assert(printStarPattern(-3) == "");
    // Edge case: large n, just check length: total chars = sum of rows stars + (rows-1) newlines
    int n = 10;
    std::string result = printStarPattern(n);
    int expectedLength = 0;
    for (int i = 1; i <= n; ++i) expectedLength += i;
    for (int i = n-1; i >= 1; --i) expectedLength += i;
    expectedLength += (2*n - 2); // newlines
    assert((int)result.length() == expectedLength);
    return 0;
}
