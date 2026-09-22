/*
Write a C++ function named `drawInvertedRightTriangle` that takes a single positive integer `n` as input and returns a `std::string` containing an inverted right-aligned triangle pattern of asterisks. The triangle should have `n` rows. On the first row, print `n` asterisks separated by a single space. On each subsequent row, the number of asterisks decreases by 1, so the last row contains exactly 1 asterisk. Each row must end with a newline character (`\n`). The function should handle invalid input (`n <= 0`) by returning an empty string. The function must not read from standard input or output to the console; it only builds and returns the string.
*/
#include <string>

// Builds an inverted right triangle pattern of asterisks with n rows.
// Returns an empty string if n <= 0.
std::string drawInvertedRightTriangle(int n) {
    if (n <= 0) {
        return "";
    }
    std::string result;
    // For each row from n down to 1
    for (int row = n; row >= 1; --row) {
        // Print 'row' asterisks in the current line
        for (int col = 0; col < row; ++col) {
            result += "* ";
        }
        // Remove the trailing space after the last asterisk and add newline
        if (row > 0) {
            result.pop_back(); // remove the last space
            result += '\n';
        }
    }
    return result;
}
#include <cassert>
#include <string>

// Function declaration (from solution)
std::string drawInvertedRightTriangle(int n);

int main() {
    assert(drawInvertedRightTriangle(0) == "");
    assert(drawInvertedRightTriangle(-3) == "");
    assert(drawInvertedRightTriangle(1) == "*\n");
    assert(drawInvertedRightTriangle(2) == "* *\n*\n");
    assert(drawInvertedRightTriangle(3) == "* * *\n* *\n*\n");
    assert(drawInvertedRightTriangle(4) == "* * * *\n* * *\n* *\n*\n");
    assert(drawInvertedRightTriangle(5) == "* * * * *\n* * * *\n* * *\n* *\n*\n");
    return 0;
}
// The solution uses a nested loop structure: an outer loop iterates `row` from `0` to `n-1` (or equivalently from `n` down to `1`), and an inner loop prints exactly `(n - row)` asterisks on that row. Each asterisk is followed by a space except the last one on the row, which is followed by a newline. This ensures proper formatting with no trailing spaces. Edge cases: if `n <= 0`, return an empty string immediately. If `n == 1`, the output is just `"* \n"` (one asterisk and a newline). The time complexity is O(n²) because the total number of asterisks printed is the sum of the first `n` integers, which is O(n²). The space complexity is O(n²) as well because the entire output string is built in memory before being returned. The implementation uses `std::string` and `std::to_string` (though not needed here) and applies `const` only where appropriate; the parameter is passed by value because it is a simple integer.
