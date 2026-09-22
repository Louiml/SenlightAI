Write a C++ function `std::string drawButterflyPattern(int width)` that takes an odd integer `width` (representing the total number of columns in the pattern) and returns a string containing a symmetric butterfly pattern made of asterisks (`*`) and spaces, with each line terminated by a newline character `\n`. The pattern must consist of two vertical halves: the left half and the right half are mirror images of each other, separated by a central gap of spaces. For a given `width`, the pattern has `width` rows (if `width` is odd) and exactly `width` columns per row (including spaces). The top and bottom halves are vertically symmetric. If `width` is even or less than 1, the function should return an empty string. The pattern must be generated using the same algorithmic structure as the provided snippet: the top half uses a loop from `i = 0` to `m-1` (where `m = width / 2`), and the bottom half mirrors the top but skips the middle row. Ensure the output string is built efficiently (e.g., using `std::ostringstream` or repeated `+=`). The function must be `const`-correct and not use global variables.
The core idea is to divide the pattern into two halves: a top half (rows `0` to `m-1`) and a bottom half (rows `m-1` down to `1`), where `m = width / 2` (integer division). For each row, the pattern consists of four parts: a left block of asterisks, a left gap, a right gap, and a right block of asterisks—all of which are symmetric.  
- In the top half, for row index `i` (starting at 0), the number of asterisks in each block is `m - i` (because the outer loop runs from `i` to `m-1`). The number of spaces in each gap is `i` (since the inner loops decrease from `m` down to `m - i`, producing `i` spaces).  
- The bottom half is the exact reverse of the top half, excluding the middle row (which is already printed as the last row of the top half). For row index `i` (starting at `m-1` down to 1), the number of asterisks per block is `i` (because the loop goes from `i` to `m-1`), and the number of spaces per gap is `m - i`.  
- Edge cases: If `width` is even or less than 1, return empty string. If `width == 1`, then `m = 0`, and both loops will have zero iterations, so the result is an empty string—but that contradicts the expected pattern for `width == 1` (which would be a single asterisk with no spaces). To handle this, treat `width == 1` specially: return `"*\n"`. For odd `width >= 3`, the algorithm works as described. Otherwise, the pattern is invalid.  
- Time complexity: Each row has `width` characters, and there are `width` rows, so we construct a string of length `O(width^2)`. The loops run `O(width^2)` total iterations, so time complexity is `O(width^2)`. Auxiliary space is `O(width^2)` for the output string, plus constant space for loop counters.
#include <string>
#include <sstream>

// Returns a string containing a symmetric butterfly pattern of given width.
// Width must be an odd positive integer; otherwise returns an empty string.
std::string drawButterflyPattern(int width) {
    if (width < 1 || width % 2 == 0) {
        return "";
    }
    if (width == 1) {
        return "*\n";
    }

    const int m = width / 2;  // number of rows in each half (excluding center)
    std::ostringstream out;

    // Top half (including the middle row when i == m-1)
    for (int i = 0; i < m; ++i) {
        // Left asterisks: count = m - i
        for (int j = i; j < m; ++j) {
            out << '*';
        }
        // Left gap: count = i
        for (int j = m; j > m - i; --j) {
            out << ' ';
        }
        // Right gap: same as left gap
        for (int j = m; j > m - i; --j) {
            out << ' ';
        }
        // Right asterisks: same as left asterisks
        for (int j = i; j < m; ++j) {
            out << '*';
        }
        out << '\n';
    }

    // Bottom half (mirror of top half, skipping the middle row)
    for (int i = m - 1; i > 0; --i) {
        // Left asterisks: count = i
        for (int j = i; j < m; ++j) {
            out << '*';
        }
        // Left gap: count = m - i
        for (int j = m; j > m - i; --j) {
            out << ' ';
        }
        // Right gap: same
        for (int j = m; j > m - i; --j) {
            out << ' ';
        }
        // Right asterisks: same
        for (int j = i; j < m; ++j) {
            out << '*';
        }
        out << '\n';
    }

    return out.str();
}
#include <string>
#include <cassert>

// The solution function is declared above; here we test it via assert.
int main() {
    // Invalid widths produce empty strings
    assert(drawButterflyPattern(0) == "");
    assert(drawButterflyPattern(-3) == "");
    assert(drawButterflyPattern(4) == "");

    // Width = 1: single asterisk
    assert(drawButterflyPattern(1) == "*\n");

    // Width = 3: expected pattern
    assert(drawButterflyPattern(3) == "** **\n*   *\n** **\n");

    // Width = 5: manually constructed expected output
    std::string expected5 = "***   ***\n**     **\n*       *\n**     **\n***   ***\n";
    assert(drawButterflyPattern(5) == expected5);

    // Width = 7: check total length and a few substrings
    std::string result7 = drawButterflyPattern(7);
    assert(result7.length() == 7 * 8); // 7 rows * (7 chars + newline)
    assert(result7.substr(0, 7) == "****   ****"); // first row
    assert(result7.substr(49, 7) == "****   ****"); // last row (mirror)

    // Ensure the middle row (row index 3) has a single asterisk on each side
    // Row 3 (0-indexed) starts at byte position 3*(8) = 24
    assert(result7.substr(24, 7) == "*       *");

    return 0;
}
