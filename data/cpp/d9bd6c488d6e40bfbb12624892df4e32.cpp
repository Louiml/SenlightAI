/*
Write a C++ function named `drawSymbolPattern` that accepts four parameters: the number of rows, the number of columns, a character `marker` for special positions, and a character `filler` for all other positions. The function must print a rectangular grid of size `rows` × `columns` where the `marker` character appears at positions where the row index equals 1 and column equals 2, or row equals 2 and column equals 2 (using 0‑based indexing). All other cells contain the `filler` character. Each row must end with a newline. The function must not read from or write to any global state; it only uses the provided arguments. For example, calling `drawSymbolPattern(4, 5, '@', '*')` produces exactly the pattern:
```
*****
**@**
**@**
*****
```
If `rows` is 0 or `columns` is 0, the function should print nothing. Handle edge cases where the special positions fall outside the given grid dimensions gracefully (e.g., if `rows < 3`, the special lines will not appear). The function should return `void` and must not use any standard library containers or algorithms; only `<iostream>` is needed.
*/
#include <iostream>

// Prints a rectangular pattern where the marker character appears at
// positions (1,2) and (2,2) (0-based), all other cells use the filler.
void drawSymbolPattern(int rows, int columns, char marker, char filler) {
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            if ((row == 1 && col == 2) || (row == 2 && col == 2)) {
                std::cout << marker;
            } else {
                std::cout << filler;
            }
        }
        std::cout << '\n';
    }
}
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

// The function to test is from the solution above.
void drawSymbolPattern(int rows, int columns, char marker, char filler);

// Helper to capture output from the solution function.
std::string captureOutput(int rows, int columns, char marker, char filler) {
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    drawSymbolPattern(rows, columns, marker, filler);
    std::cout.rdbuf(old);
    return oss.str();
}

int main() {
    // Test the original example.
    assert(captureOutput(4, 5, '@', '*') == "*****\n**@**\n**@**\n*****\n");

    // Test with different markers.
    assert(captureOutput(4, 5, 'X', '.') == ".....\n..X..\n..X..\n.....\n");

    // Test when rows or columns are zero -> no output.
    assert(captureOutput(0, 5, '@', '*') == "");
    assert(captureOutput(4, 0, '@', '*') == "");

    // Test a grid too small for the marker positions (e.g., 2x3).
    assert(captureOutput(2, 3, '@', '*') == "***\n***\n");

    // Test a grid where columns are smaller than needed (e.g., 3x2).
    assert(captureOutput(3, 2, '@', '*') == "**\n**\n**\n");

    // Test a single cell (no markers).
    assert(captureOutput(1, 1, '@', '#') == "#\n");

    std::cout << "All tests passed.\n";
    return 0;
}
// The solution is straightforward: use two nested loops, the outer loop iterates over row indices from 0 to `rows-1`, the inner loop iterates over column indices from 0 to `columns-1`. For each cell, decide whether to print the `marker` character if `(row == 1 && col == 2) || (row == 2 && col == 2)`, otherwise print the `filler`. After the inner loop completes for a row, output a newline. Edge cases: if `rows` or `columns` is 0, the loops simply do not execute, so nothing is printed. If the grid is smaller than 3×3, the special condition can never be true because row or column indices will not reach the required values, so the pattern will be all fillers. No special handling is needed for those cases—the condition naturally fails. Time complexity is O(rows × columns) because each cell is visited exactly once. Space complexity is O(1) since only a few local integer variables are used. The function parameters are passed by value and used `const` only in the sense that we do not modify them, though they are local copies already. For the solution, we will declare the parameters without `const` (as they are pass‑by‑value), but we will not modify them inside the function.
