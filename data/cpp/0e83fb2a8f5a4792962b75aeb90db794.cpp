Write a C++ function named `printNumberRectangle` that takes a positive integer `n` and returns a `std::string` containing a right-aligned numeric rectangle pattern. The rectangle must have `(2*n - 1)` rows and `2*n` columns. For each row `i` (with `i` starting at 1), every cell in that row must contain the integer `i` (as a digit, since `i` will be between 1 and `2n-1`). Rows are separated by newline characters, and there must be no trailing newline after the last row. For example, if `n = 3`, the function should return `"111111\n222222\n333333\n444444\n555555"` (5 rows, 6 columns each). The input `n` is guaranteed to be at least 1. The function must be `const`-correct and should not read from or write to standard I/O; it should only build and return the string.

// The core algorithm is straightforward nested loops: iterate `i` from 1 to `(2*n - 1)` for rows, and for each row, append the digit `i` exactly `(2*n)` times. Because `i` can be a multi-digit number when `n` is large (e.g., `n=6` gives rows up to 11), we must not use `char` arithmetic but rather convert `i` to a string using `std::to_string` and append that string repeatedly. The total number of characters is approximately `(2n-1)*(2n)*digits(i)`, but for simplicity we can build the string using `std::string::append` with a temporary string that repeats the representation of `i`. We accumulate rows into a `std::string` result, adding `'\n'` between rows but not after the last. Edge cases: `n=1` yields a single row of two `1`s (i.e., `"11"`), which is correct. Complexity: the total output length is `O(n^2)` characters (since both row and column counts are linear in `n`), and we perform that many character appends, so time complexity is O(n^2). Auxiliary space is O(n) for building each row string plus the final result, but since the result itself is O(n^2), we consider the algorithm to use O(n^2) output space.

#include <string>

// Returns a numeric rectangle with (2*n-1) rows and 2*n columns.
// Row i (1-based) contains the integer i repeated 2*n times.
std::string printNumberRectangle(int n) {
    std::string result;
    int rows = 2 * n - 1;
    int cols = 2 * n;

    for (int i = 1; i <= rows; ++i) {
        // Build the segment for this row: digit i repeated cols times.
        std::string value = std::to_string(i);
        std::string row;
        row.reserve(cols * value.size());
        for (int j = 0; j < cols; ++j) {
            row += value;
        }

        if (i > 1) {
            result += '\n';
        }
        result += row;
    }
    return result;
}

#include <cassert>
#include <string>

// Declaration of the solution function (assumed to be included above).
std::string printNumberRectangle(int n);

int main() {
    assert(printNumberRectangle(1) == "11");
    assert(printNumberRectangle(2) == "1111\n2222\n3333");
    assert(printNumberRectangle(3) == "111111\n222222\n333333\n444444\n555555");
    assert(printNumberRectangle(4) == "11111111\n22222222\n33333333\n44444444\n55555555\n66666666\n77777777");
    // For n=6, rows go up to 11, testing multi-digit values.
    std::string pattern6;
    for (int i = 1; i <= 11; ++i) {
        if (i > 1) pattern6 += '\n';
        for (int j = 0; j < 12; ++j) {
            pattern6 += std::to_string(i);
        }
    }
    assert(printNumberRectangle(6) == pattern6);
    // Verify length for a larger n: rows=2n-1, each row has 2n * digit_count.
    std::string result7 = printNumberRectangle(7);
    int expectedLen = 0;
    for (int i = 1; i <= 13; ++i) {
        expectedLen += 14 * std::to_string(i).size();
        if (i < 13) expectedLen += 1; // newline
    }
    assert(static_cast<int>(result7.size()) == expectedLen);
    assert(result7.find('\n') != std::string::npos);
    assert(result7.back() != '\n'); // No trailing newline
    return 0;
}
