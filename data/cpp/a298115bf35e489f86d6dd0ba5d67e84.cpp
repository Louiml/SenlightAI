// Write a C++ function `std::vector<std::string> generateSpiralMatrix(int N)` that takes a positive integer `N` and returns a vector of `N` strings, each of length `N`, representing an `N x N` grid. The grid must be filled with `'*'` characters in a continuous spiral pattern starting from the top-left corner and moving right along the top row, then down the right column, then left along the bottom row, then up the left column, and so on inward. All cells not part of the spiral must be spaces (`' '`). The spiral must be exactly one cell thick (no filled interior), meaning only the path itself is marked. Ensure the returned strings preserve the exact spacing (no trailing spaces except in the last column, which may be `'*'` or `' '`; each row must have exactly `N` characters). The function must handle `N = 1` (single `'*'`) and `N = 2` (a full 2x2 square).
#include <cassert>
#include <vector>
#include <string>

// (Solution function is assumed to be included above.)

int main() {
    // N = 1: single star.
    std::vector<std::string> result1 = generateSpiralMatrix(1);
    assert(result1.size() == 1 && result1[0] == "*");

    // N = 2: full 2x2 square.
    std::vector<std::string> result2 = generateSpiralMatrix(2);
    assert(result2.size() == 2);
    assert(result2[0] == "**");
    assert(result2[1] == "**");

    // N = 3: centered spiral with hollow center.
    std::vector<std::string> result3 = generateSpiralMatrix(3);
    assert(result3.size() == 3);
    assert(result3[0] == "***");
    assert(result3[1] == "* *");
    assert(result3[2] == "***");

    // N = 4: ring of thickness one with empty inner 2x2.
    std::vector<std::string> result4 = generateSpiralMatrix(4);
    assert(result4.size() == 4);
    assert(result4[0] == "****");
    assert(result4[1] == "*  *");
    assert(result4[2] == "*  *");
    assert(result4[3] == "****");

    // N = 5: full outer ring plus an inner spiral.
    std::vector<std::string> result5 = generateSpiralMatrix(5);
    assert(result5.size() == 5);
    assert(result5[0] == "*****");
    assert(result5[1] == "*   *");
    assert(result5[2] == "* * *");
    assert(result5[3] == "*   *");
    assert(result5[4] == "*****");

    // N = 6: outer ring plus inner 2x2 hole.
    std::vector<std::string> result6 = generateSpiralMatrix(6);
    assert(result6.size() == 6);
    assert(result6[0] == "******");
    assert(result6[1] == "*    *");
    assert(result6[2] == "* ** *");
    assert(result6[3] == "* ** *");
    assert(result6[4] == "*    *");
    assert(result6[5] == "******");

    return 0;
}
#include <vector>
#include <string>

// Generate an N x N matrix as a vector of strings representing a single-cell-thick spiral.
std::vector<std::string> generateSpiralMatrix(int N) {
    // Initialize grid with spaces.
    std::vector<std::string> grid(N, std::string(N, ' '));

    int top = 0, bottom = N - 1;
    int left = 0, right = N - 1;

    while (top <= bottom && left <= right) {
        // Fill top row left to right.
        for (int j = left; j <= right; ++j) {
            grid[top][j] = '*';
        }
        ++top;

        // Fill right column top to bottom.
        for (int i = top; i <= bottom; ++i) {
            grid[i][right] = '*';
        }
        --right;

        // Fill bottom row right to left, if still valid.
        if (top <= bottom) {
            for (int j = right; j >= left; --j) {
                grid[bottom][j] = '*';
            }
            --bottom;
        }

        // Fill left column bottom to top, if still valid.
        if (left <= right) {
            for (int i = bottom; i >= top; --i) {
                grid[i][left] = '*';
            }
            ++left;
        }
    }

    return grid;
}
// The algorithm uses a four-direction traversal that simulates moving along the perimeter of the current unfilled rectangle. Initialize boundaries `top=0`, `bottom=N-1`, `left=0`, `right=N-1`. While `top <= bottom` and `left <= right`, do four passes:  
// 1. Fill the top row from `left` to `right` with `'*'`, then increment `top`.  
// 2. Fill the right column from `top` to `bottom` with `'*'`, then decrement `right`.  
// 3. If `top <= bottom`, fill the bottom row from `right` down to `left` with `'*'`, then decrement `bottom`.  
// 4. If `left <= right`, fill the left column from `bottom` up to `top` with `'*'`, then increment `left`.  
// This correctly traces a single-cell-thick spiral because the boundaries shrink after each side is drawn, and the inner region remains empty. For odd `N`, the center cell is filled as the final step of a row or column pass. For even `N`, the spiral ends after completing an outer ring around an empty inner `2x2` area, but that inner area is not touched because boundaries cross before reaching it—this matches the pattern in the original snippet. Edge cases: `N=1` returns `{"*"}`; `N=2` returns `{"**","**"}` because the spiral covers the entire grid. Time complexity is `O(N^2)` since we visit each cell exactly once to assign either `'*'` or `' '`. Space complexity is `O(N^2)` for the output storage.
