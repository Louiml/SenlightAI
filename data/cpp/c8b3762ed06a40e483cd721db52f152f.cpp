// Write a C++ function named `printRectangle` that takes two integer parameters, `rows` and `cols`, both guaranteed to be non-negative, and returns a string containing a rectangular grid of asterisks (`*`) with exactly `rows` lines. Each line must contain `cols` asterisks separated by a single space, and lines must be terminated by a newline character (`\n`). If either `rows` or `cols` is zero, the function must return an empty string. The function must not print anything to the console; it must only build and return the string, making it suitable for testing and reuse.
#include <cassert>
#include <string>

// Assume printRectangle is declared above in the same translation unit.
std::string printRectangle(int rows, int cols);

int main() {
    // 1x1
    assert(printRectangle(1, 1) == "*\n");
    // 2x3
    assert(printRectangle(2, 3) == "* * *\n* * *\n");
    // 3x2
    assert(printRectangle(3, 2) == "* *\n* *\n* *\n");
    // Single row, many columns
    assert(printRectangle(1, 4) == "* * * *\n");
    // Single column, many rows
    assert(printRectangle(4, 1) == "*\n*\n*\n*\n");
    // Zero rows
    assert(printRectangle(0, 5) == "");
    // Zero columns
    assert(printRectangle(5, 0) == "");
    // Both zero
    assert(printRectangle(0, 0) == "");
    // Negative values (not expected, but safe)
    assert(printRectangle(-1, 3) == "");
    assert(printRectangle(3, -2) == "");
    return 0;
}
#include <string>

// Return a string containing a rows x cols grid of asterisks separated by spaces.
// Each row ends with '\n'. If rows or cols is 0, returns an empty string.
std::string printRectangle(int rows, int cols) {
    if (rows <= 0 || cols <= 0) {
        return "";
    }
    std::string result;
    result.reserve(rows * (cols * 2)); // rough reserve: each '*' plus space, plus newline
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (j > 0) {
                result.push_back(' ');
            }
            result.push_back('*');
        }
        result.push_back('\n');
    }
    return result;
}
// The solution builds a string progressively using `std::string` and `std::to_string`-like logic but for characters. The main algorithm iterates over each row (from 0 to `rows-1`). For each row, it appends `cols` asterisks separated by spaces; after each row, it appends a newline. Edge cases: if `rows` or `cols` is 0, return an empty string immediately to avoid building unnecessary output. Also, careful handling of trailing spaces is needed: each line should have exactly `cols` asterisks, with spaces only between them, so we append an asterisk for `j=0`, then for each subsequent `j` append a space and an asterisk. After finishing a row, append `\n`. Complexity: time is \(O(rows \times cols)\) because we append each character once, and space is \(O(rows \times cols)\) for the resulting string. There is no additional auxiliary space beyond the output string itself.
