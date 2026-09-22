// Write a standalone C++ function named `printAlphabetDiamond` that takes a single positive integer `n` as input and returns a `std::string` containing the alphabet diamond pattern exactly as described: for `n` equal to the number of rows in the upper half, the output has a symmetric diamond shape where the first row has one 'A' centered with `(n-1)` leading spaces, each subsequent row in the upper half increases the number of characters by 2 (following the sequence A, B, C, ...) and decreases leading spaces by 1, and the lower half mirrors the upper half in reverse order without repeating the middle row. The function must handle edge cases: if `n` is 0 or negative, return an empty string; if `n` is 1, return just "A\n" (with a newline). The returned string must have no trailing spaces, and each row must end with a newline character, including the last row. Do not include any `main` function in the solution; the test harness will provide it.
// The solution constructs the diamond row by row. For the upper half (lines 1 through `n`), the number of leading spaces is `(n - line)` and the number of characters is `2*line - 1`, where characters are `'A' + index` for index from 0 to count-1. For the lower half (lines `n+1` through `2n-1`), compute a mirrored row index `mirroredLine = 2*n - line` (which ranges from `n-1` down to 1), then spaces = `(line - n)`, and characters = `2*mirroredLine - 1`. This mirrors the upper half correctly without duplicating the middle row. Edge cases: if `n <= 0`, return an empty string; if `n == 1`, the loop for the upper half produces one line "A\n" and the lower half loop does not execute (since the loop condition `line = n+1` to `2n-1` would be `2` to `1`, which doesn't run), so the function correctly returns "A\n". Time complexity is O(n^2) because the total number of printed characters across all rows is the sum of an arithmetic progression of odd numbers, which is proportional to n^2. Space complexity is O(n^2) because the returned string stores all characters. The algorithm uses simple integer arithmetic and character casting, with no floating-point or complex data structures.
#include <string>

// Returns the alphabet diamond pattern for a given positive integer n.
// For n <= 0, returns an empty string. Each row ends with a newline.
std::string printAlphabetDiamond(int n) {
    if (n <= 0) {
        return "";
    }

    std::string result;
    result.reserve(static_cast<size_t>(n * n * 2)); // rough upper bound

    // Upper half: rows 1 to n
    for (int line = 1; line <= n; ++line) {
        // Leading spaces
        for (int space = 0; space < (n - line); ++space) {
            result += ' ';
        }
        // Characters
        int charCount = 2 * line - 1;
        for (int idx = 0; idx < charCount; ++idx) {
            result += static_cast<char>('A' + idx);
        }
        result += '\n';
    }

    // Lower half: rows n+1 to 2n-1
    for (int line = n + 1; line <= 2 * n - 1; ++line) {
        // Leading spaces
        for (int space = 0; space < (line - n); ++space) {
            result += ' ';
        }
        // Characters: compute mirrored line number
        int mirroredLine = 2 * n - line;
        int charCount = 2 * mirroredLine - 1;
        for (int idx = 0; idx < charCount; ++idx) {
            result += static_cast<char>('A' + idx);
        }
        result += '\n';
    }

    return result;
}
#include <cassert>
#include <string>

// Declare the function being tested (if not included via header)
std::string printAlphabetDiamond(int n);

int main() {
    // Test n = 1
    assert(printAlphabetDiamond(1) == "A\n");

    // Test n = 2
    assert(printAlphabetDiamond(2) == " A\nABC\n A\n");

    // Test n = 3
    assert(printAlphabetDiamond(3) == "  A\n ABC\nABCDE\n ABC\n  A\n");

    // Test n = 4
    std::string expected4 = 
        "   A\n"
        "  ABC\n"
        " ABCDE\n"
        "ABCDEFG\n"
        " ABCDE\n"
        "  ABC\n"
        "   A\n";
    assert(printAlphabetDiamond(4) == expected4);

    // Test n = 0 (invalid input)
    assert(printAlphabetDiamond(0) == "");

    // Test n = -5 (invalid input)
    assert(printAlphabetDiamond(-5) == "");

    // Additional check: ensure no trailing spaces in each line (manually verified from expected strings)

    return 0;
}
