// Write a C++ function `std::string printStarPattern(int N)` that takes a positive integer `N` and returns a string containing a centered right-aligned triangle of asterisks, where each line `i` (1-indexed) contains `N - i` leading spaces followed by `2*i - 1` asterisks, and lines are separated by a newline character `\n`. The pattern should have no trailing spaces on any line. If `N` is less than or equal to 0, return an empty string. The function should be implemented without using `std::cout`; instead, it must build the result using a `std::string` and return it.
// The algorithm constructs the output line by line. For a given `N`, loop from `row = 1` to `N`. For each row, the number of leading spaces is `N - row`, and the number of asterisks is `2 * row - 1`. Append the appropriate number of spaces and asterisks to a `std::string` result, then append a newline character after each row (except possibly the last, but the problem allows a trailing newline; to keep it simple we can append after every row, including the last). Edge cases: If `N <= 0`, return an empty string immediately. For `N = 1`, the pattern is a single `*` without leading spaces. Time complexity is `O(N^2)` because the total number of characters output is the sum of `(N - row) + (2*row - 1) = N + row - 1` for `row = 1..N`, which sums to `O(N^2)`. Space complexity is `O(N^2)` to store the result string.
#include <string>

// Return a centered triangle of asterisks with N rows.
// Each row i (1-based) has (N - i) leading spaces and (2*i - 1) asterisks.
// Returns an empty string if N <= 0.
std::string printStarPattern(int N) {
    if (N <= 0) {
        return "";
    }
    std::string result;
    result.reserve(N * N); // rough capacity to avoid reallocations
    for (int row = 1; row <= N; ++row) {
        // Append leading spaces
        result.append(N - row, ' ');
        // Append asterisks
        result.append(2 * row - 1, '*');
        // Append newline (including after the last row)
        result.push_back('\n');
    }
    return result;
}
#include <cassert>
#include <string>

// Assume printStarPattern is defined above.

int main() {
    // N = 1
    assert(printStarPattern(1) == "*\n");
    // N = 2
    assert(printStarPattern(2) == " *\n***\n");
    // N = 3
    assert(printStarPattern(3) == "  *\n ***\n*****\n");
    // N = 0 returns empty
    assert(printStarPattern(0) == "");
    // N = -5 returns empty
    assert(printStarPattern(-5) == "");
    // N = 5, check first and last lines manually
    std::string p = printStarPattern(5);
    assert(p.substr(0, 5) == "    *"); // leading spaces then one star
    assert(p.substr(p.size() - 9) == "*********\n"); // last line: 9 stars
    // Check total length: sum of (N - row) + (2*row - 1) + 1 newline each row
    // For N=5: rows lengths = 5, 7, 9, 11, 13; plus 5 newlines = 45+5=50
    assert(p.size() == 50);
    return 0;
}
