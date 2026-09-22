// Write a C++ function `long long uncoveredCells(long long rows, long long cols)` that, given the dimensions of a rectangular grid, returns the number of cells that are not on the outer border of the grid. For a grid with at least 3 rows and at least 3 columns, this is the interior region `(rows - 2) * (cols - 2)`. For degenerate cases (1 row or 1 column), the function must handle them specially: if both dimensions are 1, only that single cell exists and is considered on the border, so return 0; if exactly one dimension is 1, the grid is a line, and all cells are on the border except when the other dimension is at least 3, in which case the cells strictly between the two ends are interior—so return `max(0, max(rows, cols) - 2)`. The function must use `const` references or values only, no `main`, and must handle very large inputs up to 10^18.
// The main observation is that the border cells form the perimeter of the rectangle. The interior consists of all cells that are not in the first row, last row, first column, or last column. For a rectangle with `rows` ≥ 2 and `cols` ≥ 2, the number of such interior cells is `(rows - 2) * (cols - 2)`, but this can be negative if either dimension is 1 or 2. The special cases:
// - If `rows == 1 && cols == 1`: the single cell is on the border, so interior count is 0.
// - If `rows == 1 && cols > 1`: the grid is a 1×cols line. All cells are on the border except the ones strictly between the two ends, which are `cols - 2` if `cols ≥ 3`, else 0. The same logic applies for `cols == 1 && rows > 1`.
// - Otherwise (both dimensions ≥ 2): if either dimension is 2, then `(rows - 2) * (cols - 2)` becomes 0 (since one factor is 0), which is correct because every cell touches the border. If both dimensions ≥ 3, the product is positive. So the formula `(rows - 2) * (cols - 2)` works for all cases except when exactly one dimension is 1, where it would incorrectly produce a negative or zero value that does not match the actual interior line cells. Therefore, handle the single-dimension case explicitly. Time complexity is O(1) and space is O(1), as only arithmetic operations are used.
#include <algorithm>

// Count cells not on the outer border of a rows x cols grid.
// Handles degenerate 1-row/1-col cases and large 64-bit values.
long long uncoveredCells(long long rows, long long cols) {
    if (rows == 1 && cols == 1) {
        return 0;
    }
    if (rows == 1) {
        return std::max(0LL, cols - 2);
    }
    if (cols == 1) {
        return std::max(0LL, rows - 2);
    }
    return (rows - 2) * (cols - 2);
}
#include <cassert>

int main() {
    assert(uncoveredCells(1, 1) == 0);
    assert(uncoveredCells(1, 5) == 3);
    assert(uncoveredCells(7, 1) == 5);
    assert(uncoveredCells(2, 10) == 0);
    assert(uncoveredCells(3, 3) == 1);
    assert(uncoveredCells(4, 5) == 6);
    assert(uncoveredCells(2, 2) == 0);
    assert(uncoveredCells(3, 2) == 0);
    assert(uncoveredCells(1000000000000000000LL, 1000000000000000000LL) == 999999999999999996LL * 999999999999999996LL);
    assert(uncoveredCells(1, 2) == 0);
}
