Write a C++ function named `drawGridPattern` that takes two positive integers `rows` and `cols` (each greater than 0) and returns a `std::string` representing a rectangular grid of `rows` lines and `cols` columns. The grid must follow the exact pattern produced by the provided code snippet: odd-numbered rows (1st, 3rd, 5th, ...) are filled entirely with `'#'` characters; rows where the row number modulo 4 equals 2 (2nd, 6th, 10th, ...) have all columns except the last filled with `'.'` and the last column filled with `'#'`; and rows where the row number modulo 4 equals 0 (4th, 8th, 12th, ...) have the first column filled with `'#'` and all remaining columns filled with `'.'`. Each line must be terminated by a newline character `'\n'`. The function must not print anything; it only returns the constructed string. You may assume the inputs are positive integers (≥ 1), so no validation is needed. The result should match exactly what the original code would print for the same `rows` and `cols`.

The solution involves iterating through each row index from 1 to `rows` inclusive. For each row, determine its pattern based on the row index:
- If the row index is odd: append `'#'` exactly `cols` times.
- Else if the row index modulo 4 equals 2: append `'.'` exactly `cols - 1` times, then append `'#'`.
- Else (row index is even and modulo 4 equals 0): append `'#'`, then append `'.'` exactly `cols - 1` times.
After each row, append a newline `'\n'`. Use a `std::string` to accumulate the output, potentially using `std::string::append(str, count)` or `assign` for efficiency. Since the loop constructs only one row at a time and appends, the time complexity is \(O(rows \cdot cols)\) because each cell character is generated exactly once. The space complexity is \(O(rows \cdot cols)\) for the returned string, with only O(1) auxiliary space during construction. Edge cases include `rows = 1` (only an odd row) and `cols = 1` (rows modulo 4 equals 2 produce no leading dots, and modulo 4 equals 0 produce only one `'#'`), both handled naturally by the logic.

#include <string>

// Build a grid pattern according to the given rules.
// Returns a string of 'rows' lines, each of length 'cols', terminated by newline.
std::string drawGridPattern(int rows, int cols) {
    std::string result;
    result.reserve(static_cast<size_t>(rows) * (cols + 1)); // reserve for efficiency

    for (int i = 1; i <= rows; ++i) {
        if (i % 2 == 1) {
            // Odd row: all '#'
            result.append(cols, '#');
        } else if (i % 4 == 2) {
            // Row number modulo 4 == 2: dots then '#'
            result.append(cols - 1, '.');
            result.push_back('#');
        } else {
            // Row number modulo 4 == 0: '#' then dots
            result.push_back('#');
            result.append(cols - 1, '.');
        }
        result.push_back('\n');
    }
    return result;
}

#include <cassert>
#include <string>

// Declaration of the function under test
std::string drawGridPattern(int rows, int cols);

int main() {
    // Single row, single column -> just '#' and newline
    assert(drawGridPattern(1, 1) == "#\n");

    // Single row, multiple columns -> all '#'
    assert(drawGridPattern(1, 5) == "#####\n");

    // Two rows -> row1 all '#', row2 dots then '#'
    assert(drawGridPattern(2, 3) == "###\n..#\n");

    // Four rows, cols=1 -> edge for modulo-4 rows (only '#' per line)
    assert(drawGridPattern(4, 1) == "#\n#\n#\n#\n");

    // Four rows, cols=4 -> exact pattern from the original code
    assert(drawGridPattern(4, 4) == "####\n...#\n####\n#...\n");

    // Six rows, cols=2 -> pattern checks
    assert(drawGridPattern(6, 2) == "##\n.#\n##\n#.\n##\n.#\n");

    // Larger test: 8 rows, 5 cols
    std::string expected8x5 =
        "#####\n"
        "....#\n"
        "#####\n"
        "#....\n"
        "#####\n"
        "....#\n"
        "#####\n"
        "#....\n";
    assert(drawGridPattern(8, 5) == expected8x5);

    // Ensure function returns a string without trailing extra spaces
    std::string s = drawGridPattern(3, 2);
    assert(s == "##\n.#\n##\n");

    // Test with rows=0? Not required, but if called with 0, returns empty string (no newlines) – we don't test that because spec says positive ints.
    // Test with cols=0? Not required.

    return 0;
}
