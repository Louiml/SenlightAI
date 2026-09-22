You are given a rectangular grid of characters, where each cell contains either a dot `.` or a hash `#`. The input consists of multiple test cases: the first line of input contains an integer `t` (number of test cases). For each test case, the first line contains two integers `n` and `m` (1 ≤ n, m ≤ 100), representing the number of rows and columns, followed by `n` lines each containing exactly `m` characters (no spaces). In each grid, there is exactly one contiguous block of `#` characters that forms a solid rectangle (filled without holes). Write a C++ function `pair<int,int> findCenter(int n, int m, const vector<string>& grid)` that returns the coordinates `(row, column)` (1-indexed) of the center of that rectangle. The center is defined as the intersection of the middle row and middle column of the rectangle (if the rectangle has even dimensions, choose the upper-left of the two central cells; for a 1×1 rectangle, that is the only cell). The function should take the grid as a `vector<string>` where each string is a row. The output for each test case should be `row column`. You may assume the rectangle always exists and is aligned with the grid axes (no rotation). The function must handle multiple test cases by being called once per case; the test harness will handle input/output. Ensure the solution is efficient for the given limits.

The key observation is that we don't need to scan the entire grid multiple times or explicitly find all rectangle boundaries. Instead, we can compute the bounding box of all `#` cells. Since the rectangle is solid and axis-aligned, the minimum row index and maximum row index of any `#` cell give the top and bottom edges; similarly, min and max column indices give left and right edges. The center row index (1-indexed) is `(minRow + maxRow) / 2` when using integer division (this floors the average, which corresponds to choosing the upper-left cell for even height). Similarly, center column is `(minCol + maxCol) / 2`. We can initialize `minRow = n`, `maxRow = -1`, `minCol = m`, `maxCol = -1` and update these while scanning all cells. If no `#` is found (which shouldn't happen per constraints), we can return `{-1, -1}` as a sentinel. Time complexity is O(n·m) per grid, and space complexity is O(1) extra (besides the input storage). Edge cases include a 1×1 rectangle, a rectangle with even dimensions (e.g., 2×2) where the center should be the top-left cell (row = (min+max)/2 with integer division yields the upper of the two middle rows), and grids where the rectangle touches the grid boundaries.

#include <vector>
#include <string>
#include <utility>

// Find the 1-indexed center of a solid rectangle of '#' in the grid.
// Returns {row, column} (both 1-indexed). If no '#' found, returns {-1, -1}.
std::pair<int,int> findRectangleCenter(int n, int m, const std::vector<std::string>& grid) {
    int minRow = n;      // first index where '#' appears (0-indexed)
    int maxRow = -1;     // last index where '#' appears
    int minCol = m;
    int maxCol = -1;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == '#') {
                if (i < minRow) minRow = i;
                if (i > maxRow) maxRow = i;
                if (j < minCol) minCol = j;
                if (j > maxCol) maxCol = j;
            }
        }
    }

    if (maxRow == -1) {  // no rectangle found (should not happen per problem)
        return {-1, -1};
    }

    // Center row (1-indexed): floor((minRow + maxRow) / 2) + 1
    int centerRow = (minRow + maxRow) / 2 + 1;
    // Center column (1-indexed)
    int centerCol = (minCol + maxCol) / 2 + 1;

    return {centerRow, centerCol};
}

#include <cassert>
#include <vector>
#include <string>
#include <utility>

// The solution function is declared above (or included from the solution file).
// For completeness, it is repeated here in the test file context.
std::pair<int,int> findRectangleCenter(int n, int m, const std::vector<std::string>& grid) {
    int minRow = n, maxRow = -1, minCol = m, maxCol = -1;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == '#') {
                if (i < minRow) minRow = i;
                if (i > maxRow) maxRow = i;
                if (j < minCol) minCol = j;
                if (j > maxCol) maxCol = j;
            }
        }
    }
    if (maxRow == -1) return {-1, -1};
    int centerRow = (minRow + maxRow) / 2 + 1;
    int centerCol = (minCol + maxCol) / 2 + 1;
    return {centerRow, centerCol};
}

int main() {
    // Test 1: Single cell
    std::vector<std::string> g1 = {"#"};
    assert(findRectangleCenter(1, 1, g1) == std::make_pair(1, 1));

    // Test 2: 2x2 rectangle (even dimensions) -> center is top-left (1,1)
    std::vector<std::string> g2 = {"##",
                                    "##"};
    assert(findRectangleCenter(2, 2, g2) == std::make_pair(1, 1));

    // Test 3: 3x3 rectangle in 5x5 grid
    std::vector<std::string> g3 = {".....",
                                    ".....",
                                    ".###.",
                                    ".###.",
                                    ".###."};
    assert(findRectangleCenter(5, 5, g3) == std::make_pair(4, 3));

    // Test 4: Rectangle touching edges, even width and height
    std::vector<std::string> g4 = {"####",
                                    "####"};
    assert(findRectangleCenter(2, 4, g4) == std::make_pair(1, 2));

    // Test 5: Rectangle with odd dimensions in corner
    std::vector<std::string> g5 = {"#..",
                                    "###",
                                    "#.."};
    // The rectangle is 3x3 with top-left at (0,0) and bottom-right at (2,2)
    assert(findRectangleCenter(3, 3, g5) == std::make_pair(2, 2));

    // Test 6: 1x3 horizontal rectangle
    std::vector<std::string> g6 = {"###"};
    assert(findRectangleCenter(1, 3, g6) == std::make_pair(1, 2));

    // Test 7: 3x1 vertical rectangle
    std::vector<std::string> g7 = {"#",
                                    "#",
                                    "#"};
    assert(findRectangleCenter(3, 1, g7) == std::make_pair(2, 1));

    // Test 8: Even height and odd width
    std::vector<std::string> g8 = {"..#..",
                                    "..#..",
                                    "..#..",
                                    "..#.."};
    assert(findRectangleCenter(4, 5, g8) == std::make_pair(2, 3));

    // Test 9: Odd height and even width
    std::vector<std::string> g9 = {"...",
                                    "###",
                                    "###",
                                    "###",
                                    "..."};
    // Rectangle rows 1-3 (0-indexed), cols 1-2 => center row (1+3)/2=2 -> 3, center col (1+2)/2=1 -> 2
    assert(findRectangleCenter(5, 3, g9) == std::make_pair(3, 2));

    // Test 10: Large rectangle covering entire grid
    std::vector<std::string> g10(3, std::string(3, '#'));
    assert(findRectangleCenter(3, 3, g10) == std::make_pair(2, 2));

    return 0;
}
