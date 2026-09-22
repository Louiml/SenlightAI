// Write a C++ function `printRightAlignedTriangle` that takes a positive integer `n` and returns a `std::string` containing a right-aligned isosceles triangle (half of a pyramid) made of asterisks, with exactly `n` rows. The triangle should be right-aligned: the top row has `n-1` spaces followed by 1 asterisk, the next row has `n-2` spaces followed by 2 asterisks, and so on, with the bottom row having 0 spaces and `n` asterisks. Each row must be terminated by a newline character, and there should be no trailing spaces after the final asterisk in any row. The function must not read from standard input or produce any output; it should only build and return the string. Handle the edge case where `n == 1` by returning a single asterisk followed by a newline.
// The algorithm iterates from row `0` to `n-1`. For each row `i` (using 0-indexing, where `i=0` is the top), the number of leading spaces is `n - 1 - i`, and the number of asterisks is `i + 1`. Construct each row by appending that many spaces, then that many asterisks, then a newline character to the result string. For `n=1`, the loop produces row `0` with 0 spaces and 1 asterisk, correctly yielding `"*\n"`. No special-case handling is needed beyond the general loop. Time complexity is `O(n^2)` because the total number of characters produced is approximately `n*(n+1)/2` for asterisks plus the spaces (sum of `0` to `n-1`), which is still `O(n^2)`. Space complexity is `O(n^2)` due to the returned string size. The function should be `const`-correct: since it only reads `n`, pass by value to avoid unnecessary copies.
#include <string>

// Build a right-aligned triangle of asterisks with n rows.
std::string printRightAlignedTriangle(const int n) {
    std::string result;
    result.reserve(static_cast<size_t>(n * (n + 1) / 2 + n * (n - 1) / 2 + n)); // optional optimization

    for (int i = 0; i < n; ++i) {
        // Append leading spaces: n-1-i spaces
        for (int j = 0; j < n - 1 - i; ++j) {
            result.push_back(' ');
        }
        // Append asterisks: i+1 asterisks
        for (int j = 0; j < i + 1; ++j) {
            result.push_back('*');
        }
        // End of row
        result.push_back('\n');
    }
    return result;
}
#include <cassert>
#include <string>

// Declaration of the function being tested
std::string printRightAlignedTriangle(const int n);

int main() {
    // n = 1
    assert(printRightAlignedTriangle(1) == "*\n");

    // n = 2
    assert(printRightAlignedTriangle(2) == " *\n**\n");

    // n = 3
    assert(printRightAlignedTriangle(3) == "  *\n **\n***\n");

    // n = 4
    assert(printRightAlignedTriangle(4) == "   *\n  **\n ***\n****\n");

    // n = 5
    assert(printRightAlignedTriangle(5) == "    *\n   **\n  ***\n ****\n*****\n");

    // n = 0 (though problem says positive, verify behavior with empty string)
    assert(printRightAlignedTriangle(0) == "");

    // Ensure no trailing spaces are present in any row (check last non-newline char)
    std::string tri = printRightAlignedTriangle(6);
    // The string should end with a newline, and the character before that should be '*'
    assert(tri.size() > 1);
    assert(tri.back() == '\n');
    assert(tri[tri.size() - 2] == '*');

    return 0;
}
