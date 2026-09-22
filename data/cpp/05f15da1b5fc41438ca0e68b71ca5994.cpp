// Write a C++ function `std::string rightAlignedTriangle(int n)` that takes a positive integer `n` and returns a multi-line string representing a right-aligned triangle of asterisks with `n` rows, where each row `i` (1-indexed) contains `n - i` leading spaces followed by `i` asterisks, and rows are separated by newline characters (`'\n'`). The function must handle any `n >= 1`. Do not print to the console; instead build and return the full string. If `n` is 0 or negative, return an empty string. Each row must have exactly `n` characters total (spaces + asterisks), and there should be no trailing spaces at the end of a row (only the asterisks after the spaces). The output should not have an extra newline after the last row.
#include <cassert>

int main() {
    // Test n=1: single asterisk, no spaces, no newline
    assert(rightAlignedTriangle(1) == "*");
    // Test n=2: two rows
    assert(rightAlignedTriangle(2) == " *\n**");
    // Test n=3: three rows
    assert(rightAlignedTriangle(3) == "  *\n **\n***");
    // Test n=4: four rows
    assert(rightAlignedTriangle(4) == "   *\n  **\n ***\n****");
    // Test n=5: five rows
    assert(rightAlignedTriangle(5) == "    *\n   **\n  ***\n ****\n*****");
    // Test n=0: empty string
    assert(rightAlignedTriangle(0) == "");
    // Test n=-3: empty string
    assert(rightAlignedTriangle(-3) == "");
    // Test n=1 again for consistency
    assert(rightAlignedTriangle(1) == "*");
    // Test n=2 again (duplicate checks)
    assert(rightAlignedTriangle(2) == " *\n**");
    // Test n=3 again (duplicate checks)
    assert(rightAlignedTriangle(3) == "  *\n **\n***");
}
#include <string>

// Return a right-aligned triangle of asterisks with n rows.
// Row i (1-based) has (n-i) spaces then i asterisks.
std::string rightAlignedTriangle(int n) {
    if (n <= 0) {
        return std::string();
    }

    std::string result;
    for (int i = 1; i <= n; ++i) {
        // Append n - i spaces
        for (int space = 0; space < n - i; ++space) {
            result += ' ';
        }
        // Append i asterisks
        for (int star = 0; star < i; ++star) {
            result += '*';
        }
        // Add newline after all rows except the last
        if (i != n) {
            result += '\n';
        }
    }
    return result;
}
// The algorithm constructs the triangle row by row using a loop from `1` to `n`. For each row index `i`, we generate `n - i` space characters, then `i` asterisk characters, and append the resulting line to a result string, adding a newline after each line except the last. To avoid extra trailing newline, we can append `'\n'` after every row except when `i == n`, or we can always append and then remove the last newline with `pop_back()`. Edge cases: if `n <= 0`, return an empty string. The time complexity is O(n^2) because the total number of characters output is about n(n+1)/2 + (n-1) newlines, i.e., O(n²). Space complexity is O(n²) for the returned string. No extra data structures are used aside from the result string and a few loop counters.
