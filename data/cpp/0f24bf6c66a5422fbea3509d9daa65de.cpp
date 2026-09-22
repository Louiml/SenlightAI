Write a C++ function `std::string findSnukeCell(int H, int W, const std::vector<std::string>& grid)` that accepts the number of rows `H`, columns `W`, and a 2D grid of strings where each cell contains either `"snuke"` or some other word. The function must locate the unique cell containing exactly the string `"snuke"` and return a string in the format `{columnLetter}{rowNumber}` where `columnLetter` is a capital English letter corresponding to the column index (A for column 0, B for column 1, ..., Z only if W ≤ 26), and `rowNumber` is 1-based (i.e., add 1 to the zero-based row index). It is guaranteed that exactly one such cell exists. The grid is guaranteed to be non-empty with 1 ≤ H, W ≤ 26, and each string is non-empty, but cells may contain strings that are not `"snuke"`.
The solution is straightforward: iterate through every cell in row-major order (outer loop over rows, inner loop over columns). For each cell, compare its string to `"snuke"` using direct equality. When found, compute the column letter as `char('A' + col)` and the row number as `row + 1`. Return the concatenated string. Because the problem guarantees exactly one occurrence, early return is safe and efficient. Edge cases: W can be up to 26, so the letter range A–Z is sufficient; row numbering is 1-based, so add 1 to the zero-based index. The main algorithm runs in O(H·W) time and uses O(1) extra space (excluding the input grid storage). No special handling for empty strings or missing cells is needed due to constraints.
#include <string>
#include <vector>

// Find the unique cell containing "snuke" and return "{columnLetter}{rowNumber}"
std::string findSnukeCell(int H, int W, const std::vector<std::string>& grid) {
    for (int row = 0; row < H; ++row) {
        for (int col = 0; col < W; ++col) {
            if (grid[row * W + col] == "snuke") {
                return std::string(1, static_cast<char>('A' + col)) + std::to_string(row + 1);
            }
        }
    }
    return ""; // Should never happen given guarantees
}
#include <cassert>
#include <string>
#include <vector>

// Function declaration (as in solution)
std::string findSnukeCell(int H, int W, const std::vector<std::string>& grid);

int main() {
    // Basic 1x1
    assert(findSnukeCell(1, 1, {"snuke"}) == "A1");
    // Row 2, column 3 (C) in a 3x4 grid
    assert(findSnukeCell(3, 4, {
        "foo", "bar", "baz", "qux",
        "aaa", "bbb", "ccc", "ddd",
        "eee", "fff", "snuke", "ggg"
    }) == "C3");
    // Last cell of a 2x2 grid: row 2, column 2 -> B2
    assert(findSnukeCell(2, 2, {
        "x", "y",
        "z", "snuke"
    }) == "B2");
    // First cell of a 5x5 grid
    assert(findSnukeCell(5, 5, {
        "snuke", "a", "b", "c", "d",
        "e", "f", "g", "h", "i",
        "j", "k", "l", "m", "n",
        "o", "p", "q", "r", "s",
        "t", "u", "v", "w", "x"
    }) == "A1");
    // Single column with 26 rows, row 26 -> Z? Actually test row 1 -> A1, row 26 -> A26
    {
        std::vector<std::string> grid;
        for (int i = 0; i < 26; ++i) grid.push_back("other");
        grid[25] = "snuke";
        assert(findSnukeCell(26, 1, grid) == "A26");
    }
    // Single row with 26 columns, column 26 -> Z1
    {
        std::vector<std::string> grid;
        for (int i = 0; i < 26; ++i) grid.push_back("other");
        grid[25] = "snuke";
        assert(findSnukeCell(1, 26, grid) == "Z1");
    }
    return 0;
}
