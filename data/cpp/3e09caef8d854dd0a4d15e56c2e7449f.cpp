// Write a C++ function named `printNumberStarPattern` that takes a positive integer `n` and returns a `std::string` containing the pattern shown in the snippet, where for each row `i` (1-indexed), you print numbers from 1 to `n-i+1`, then `2*(i-1)` asterisks, then numbers from `n-i+1` down to 1, all separated by single spaces, and each row terminated by a newline. The function must handle edge cases: if `n` is 1, the single row is `1 1`. Assume `n >= 1`. The output string should have no trailing spaces at the end of any line, and no extra blank lines. Return the complete pattern as a single string.

// The pattern consists of three segments per row:
// 1. An increasing sequence from 1 to `limit = n - row + 1`.
// 2. A block of `2 * (row - 1)` asterisks.
// 3. A decreasing sequence from `limit` down to 1.
// Each segment is joined with a single space. The entire row is built by concatenating numbers (converted to strings) and asterisks with spaces. After each row, a newline is added, except after the last row to avoid a trailing newline (though a trailing newline is acceptable in some tests, it's cleaner to omit). 
//
// Edge case: `n = 1` → limit = 1, stars = 0, so the row is `"1 1"`. 
//
// Time complexity: `O(n^2)` because for each of `n` rows, the total number of characters printed is proportional to `2*limit + 2*(row-1) = 2*n` per row, so total characters ~2*n^2. Space complexity: the returned string is `O(n^2)`.

#include <string>

// Returns the number-star pattern for a given positive integer n.
// Each row: increasing numbers 1..(n-row+1), then 2*(row-1) asterisks,
// then decreasing numbers (n-row+1)..1, all space-separated.
std::string printNumberStarPattern(int n) {
    std::string result;
    for (int row = 1; row <= n; ++row) {
        int limit = n - row + 1;

        // Increasing numbers from 1 to limit
        for (int num = 1; num <= limit; ++num) {
            if (num > 1) result += ' ';
            result += std::to_string(num);
        }

        // Asterisks: 2*(row-1) of them
        int starCount = 2 * (row - 1);
        for (int s = 0; s < starCount; ++s) {
            result += " *"; // leading space before each asterisk
        }

        // Decreasing numbers from limit down to 1
        for (int num = limit; num >= 1; --num) {
            result += ' ';
            result += std::to_string(num);
        }

        // Newline except after last row
        if (row < n) result += '\n';
    }
    return result;
}

#include <cassert>
#include <string>

// Function declaration (for linking)
std::string printNumberStarPattern(int n);

int main() {
    // n = 1
    assert(printNumberStarPattern(1) == "1 1");

    // n = 2
    assert(printNumberStarPattern(2) == "1 2 2 1\n1 * * 1");

    // n = 3
    assert(printNumberStarPattern(3) == "1 2 3 3 2 1\n1 2 * * 2 1\n1 * * * * 1");

    // n = 4 (matching the given snippet output)
    assert(printNumberStarPattern(4) == "1 2 3 4 4 3 2 1\n1 2 3 * * 3 2 1\n1 2 * * * * 2 1\n1 * * * * * * 1");

    // n = 5 (extended from the snippet, the snippet shows n=5)
    assert(printNumberStarPattern(5) == "1 2 3 4 5 5 4 3 2 1\n1 2 3 4 * * 4 3 2 1\n1 2 3 * * * * 3 2 1\n1 2 * * * * * * 2 1\n1 * * * * * * * * 1");

    // Ensure no trailing newline in the full string
    assert(printNumberStarPattern(3).back() == '1');
    assert(printNumberStarPattern(2).back() == '1');

    // Check line count for n=4
    int newlines = 0;
    std::string pat = printNumberStarPattern(4);
    for (char c : pat) if (c == '\n') ++newlines;
    assert(newlines == 3); // 4 rows -> 3 newlines

    return 0;
}
