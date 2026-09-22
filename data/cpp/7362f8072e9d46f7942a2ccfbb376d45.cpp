// Write a C++ function named `starPattern` that takes a single positive integer `n` as input and returns a `std::string` containing the complete symmetric hourglass-like pattern of asterisks and spaces, exactly as produced by the given (buggy) code's intended output. The pattern consists of two triangles: the top half grows from 1 to `n` asterisks on each side, and the bottom half shrinks from `n` down to 1 asterisk on each side. In each row, the sides are separated by a number of spaces that decreases linearly from `2n-2` in the first row to `0` in the middle row, then increases symmetrically back to `2n-2` in the last row. Each row ends with a newline character `\n`. The function should handle the case `n = 1` correctly, producing a single row `"*\n"`. Ensure the returned string exactly matches the intended output (not the buggy code) with no extra leading or trailing whitespace other than the newlines at the end of each row.

// The intended algorithm builds the top half row by row from `i = 1` to `n`. For each row, it appends `i` asterisks, then appends `2*n - 2*i` spaces, then appends `i` asterisks, and finally a newline. The bottom half mirrors this by iterating `i` from `n` down to `1`, appending `i` asterisks, `2*n - 2*i` spaces, `i` asterisks, and a newline. Edge cases: when `n = 1`, both halves produce the same single row `"*\n"` (since `2*1-2=0` spaces), so care must be taken to avoid duplicating it—the algorithm naturally produces `"*\n"` twice, but the intended pattern for `n=1` is just one row. Therefore, the bottom loop should run from `n-1` down to `1` (or equivalently only append the middle row once). Time complexity is \(O(n^2)\) because the total number of characters is approximately \(2n^2\) (sum of left+right asterisks and spaces). Space complexity is \(O(n^2)\) for the returned string, with \(O(1)\) extra auxiliary space.

#include <string>

// Returns the symmetric hourglass-like pattern of asterisks and spaces for given n.
// Complexity: O(n^2) time and space.
std::string starPattern(int n) {
    std::string result;
    result.reserve(2 * n * n); // approximate capacity to reduce reallocations

    // Top half: rows from 1 to n
    for (int i = 1; i <= n; ++i) {
        result.append(i, '*');
        result.append(2 * n - 2 * i, ' ');
        result.append(i, '*');
        result.push_back('\n');
    }

    // Bottom half: rows from n-1 down to 1 (avoid duplicating middle row)
    for (int i = n - 1; i >= 1; --i) {
        result.append(i, '*');
        result.append(2 * n - 2 * i, ' ');
        result.append(i, '*');
        result.push_back('\n');
    }

    return result;
}

#include <cassert>
#include <string>

// Declaration of the function being tested
std::string starPattern(int n);

int main() {
    // n = 1: single row with no spaces
    assert(starPattern(1) == "*\n");

    // n = 2: top row "**  **", bottom row "*    *"
    assert(starPattern(2) == "**  **\n*    *\n");

    // n = 3: full symmetric pattern
    assert(starPattern(3) == "*    *\n**  **\n******\n**  **\n*    *\n");

    // n = 4: check row lengths and symmetry manually
    std::string p4 = starPattern(4);
    assert(p4 == "*      *\n**    **\n***  ***\n********\n***  ***\n**    **\n*      *\n");

    // Verify all rows have equal length (2n characters per row plus newline)
    std::string p5 = starPattern(5);
    size_t expected_len = 2 * 5 + 1; // 2n asterisks+spaces + newline
    size_t pos = 0;
    int rows = 0;
    while (pos < p5.size()) {
        size_t newline = p5.find('\n', pos);
        assert(newline != std::string::npos);
        assert(newline - pos == expected_len - 1);
        pos = newline + 1;
        ++rows;
    }
    assert(rows == 2 * 5 - 1); // 9 rows for n=5

    // Ensure first and last rows are symmetric for n=5
    assert(p5.substr(0, 9) == "*        *\n");
    assert(p5.substr(p5.size() - 10) == "*        *\n");

    // Test a larger n to ensure no crashes or empty output
    std::string p10 = starPattern(10);
    assert(p10.size() > 0);
    assert(p10.front() == '*');
    assert(p10.back() == '\n');

    return 0;
}
