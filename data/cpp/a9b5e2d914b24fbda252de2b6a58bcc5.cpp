// Given a rectangular grid of characters (each cell either `*` or `.`), write a C++ function `minimumOperations` that takes the number of rows `n`, number of columns `m`, and a `std::vector<std::string>&` representing the grid, and returns the minimum number of cells that must be changed (either adding or removing a `*`) so that exactly one row and one column contain a `*` at their intersection, and every other cell in that row and column is `.`. In other words, after the changes, the grid must have exactly one `*` and it must be the only `*` in its row and its column. The function should consider making any cell the unique star, and count how many changes are needed if that cell is chosen; return the minimum over all cells. If the grid already satisfies the condition for some cell, the answer is 0.

// For each cell `(i, j)`, we can count the number of stars in its row (`row[i]`) and column (`col[j]`) from the input. If we decide that cell `(i,j)` will be the unique star after modification, then:
// - The cell itself must be `*`. If it already is, no change there; if it's `.`, we need to add a star (1 change).
// - All other cells in row `i` must be `.`. Currently, there are `row[i]` stars in that row. If the cell itself is `*`, then the number of other stars in the row is `row[i]-1`; if the cell is `.`, then the number of other stars is `row[i]`. These other stars must be removed, so that many changes.
// - Similarly, for column `j`, the number of other stars to remove is `col[j] - (arr[i][j]=='*')`.
//
// Thus, total changes for choosing cell `(i,j)` = (number of other stars in row) + (number of other stars in column) + (1 if cell is `.`, else 0). This simplifies to: `row[i] + col[j] - (arr[i][j]=='*') + (arr[i][j]=='.' ? 1 : 0)`. But note `(arr[i][j]=='.' ? 1 : 0)` equals `1 - (arr[i][j]=='*')`. So formula becomes `row[i] + col[j] - (arr[i][j]=='*') + (1 - (arr[i][j]=='*')) = row[i] + col[j] + 1 - 2*(arr[i][j]=='*')`. However, the reference code uses `n + m - 1 - (row[i]+col[j] - (arr[i][j]=='*'))`, which gives `n+m-1 - row[i] - col[j] + (arr[i][j]=='*')`. That appears to be an alternative formulation, but the correct minimal changes are directly `row[i] + col[j] - (arr[i][j]=='*') + (arr[i][j] != '*' ? 1 : 0)`. Let’s verify with an example: grid 2x2, all dots. For (0,0): row[0]=0, col[0]=0, cell is '.', so changes = 0+0+1 = 1. We need to make (0,0) a star, and ensure no other stars in row/col (none). So 1 change is correct. For a grid with a star at (0,0) only: row[0]=1, col[0]=1, cell is '*', changes = 1+1-1 =1? But actually we need zero changes because (0,0) is already unique. Wait, formula gives 1+1-1 =1, which is wrong. Let's recount: Other stars in row = row[i] - (cell is '*') = 1-1=0. Other stars in column = 0. Cell already '*', so 0 changes. So correct formula is `(row[i] - (arr[i][j]=='*')) + (col[j] - (arr[i][j]=='*')) + (arr[i][j] != '*' ? 1 : 0)`. That equals `row[i]+col[j] - 2*(arr[i][j]=='*') + (1 - (arr[i][j]=='*')) = row[i]+col[j]+1 - 3*(arr[i][j]=='*')`. For (0,0) star: 1+1+1-3 =0. For (0,0) dot: 0+0+1-0=1. Good. So we can compute minimum over all cells. The original snippet's computation seems off (they compute something like n+m-1 - ...), maybe they meant something else; we ignore that. The task is to write a function that returns the minimal changes. Edge cases: n or m could be 0? Usually positive. The grid is non-empty. If grid already has exactly one star and its row and column each have exactly one star, then answer 0. Complexity: O(n*m) time to compute row/col counts and iterate over all cells, O(n+m) space for counts.

#include <vector>
#include <string>
#include <algorithm>

// Returns the minimum number of cell modifications so that exactly one row and one column
// contain a single '*' at their intersection, and all other cells in that row/column are '.'.
int minimumOperations(int n, int m, const std::vector<std::string>& grid) {
    std::vector<int> rowStars(n, 0), colStars(m, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == '*') {
                ++rowStars[i];
                ++colStars[j];
            }
        }
    }

    int best = n * m; // upper bound; we can always change all cells
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            bool isStar = (grid[i][j] == '*');
            // Stars in other rows/columns that must be removed
            int changes = (rowStars[i] - (isStar ? 1 : 0))
                        + (colStars[j] - (isStar ? 1 : 0))
                        + (isStar ? 0 : 1); // add a star if this cell is '.'
            best = std::min(best, changes);
        }
    }
    return best;
}

#include <cassert>
#include <vector>
#include <string>

// Assume the function above is defined here.

int main() {
    // Single cell grid, already has star
    std::vector<std::string> g1 = {"*"};
    assert(minimumOperations(1, 1, g1) == 0);

    // Single cell grid, no star
    std::vector<std::string> g2 = {"."};
    assert(minimumOperations(1, 1, g2) == 1);

    // 2x2 all dots, choose any cell -> 1 change
    std::vector<std::string> g3 = {"..", ".."};
    assert(minimumOperations(2, 2, g3) == 1);

    // 2x2 with star at (0,0) only -> already satisfies condition
    std::vector<std::string> g4 = {"*.", ".."};
    assert(minimumOperations(2, 2, g4) == 0);

    // 2x2 with stars at (0,0) and (1,1) -> need to remove one star, then the other is unique
    std::vector<std::string> g5 = {"*.", ".*"};
    assert(minimumOperations(2, 2, g5) == 1);

    // 3x3 with a full row of stars, choose (0,0) as unique: remove 2 in row, 0 in col, keep star -> 2 changes
    std::vector<std::string> g6 = {"***", "...", "..."};
    assert(minimumOperations(3, 3, g6) == 2);

    // 3x3 with star at center only -> already unique
    std::vector<std::string> g7 = {"...", ".*.", "..."};
    assert(minimumOperations(3, 3, g7) == 0);

    // 3x3 with two stars in same column and one elsewhere -> pick the lone one? Let's check: grid ["*.", "..", ".*"] (3 rows,2 cols) but adjust to 3x3
    std::vector<std::string> g8 = {"*..", "...", "..*"};
    // For (0,0): row[0]=1, col[0]=1 => changes=0 (already unique). For (2,2): similarly 0. So answer 0.
    assert(minimumOperations(3, 3, g8) == 0);

    // 3x3 with two stars in same row and one in same column? Example: ["*.", "*..", ".."]? Actually 3 rows, 3 cols.
    std::vector<std::string> g9 = {"*..", "*..", "..."}; // stars at (0,0) and (1,0)
    // Choose (0,0): other star in col[0]=2 -> remove one (1 change), row[0]=1 already ok, keep star -> 1 change. Choose (1,0) gives 1 too. Choose any other cell like (0,2): need to add star, remove both stars in col0? Actually col[2]=0, row[0]=1 -> remove 1 star from row, add 1 -> 2 changes. So answer 1.
    assert(minimumOperations(3, 3, g9) == 1);

    // 4x4 with many stars, ensure we find minimal
    std::vector<std::string> g10 = {"....", ".*..", "..*.", "...."};
    // For (1,1): row[1]=1, col[1]=1, already star, other stars in row/col =0, changes=0. So answer 0.
    assert(minimumOperations(4, 4, g10) == 0);
}
