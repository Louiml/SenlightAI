// Write a C++ function that takes a square vector of strings `grid`, where each string is of length equal to the number of rows and contains only the characters `' '` space, `'/'`, and `'\'`, representing a grid divided into unit squares. Each unit square can be split into two triangular regions by a diagonal slash (`/` or `\`) or remain unsplit if the cell contains a space. The slashes connect the corners of the unit square as follows: `/` connects the top-right corner to the bottom-left corner, and `\` connects the top-left corner to the bottom-right corner. The task is to return the number of connected regions formed by these line segments across the entire grid. The grid size `n` satisfies `1 <= n <= 30`. The function should be named `regionsBySlashes` and take `const std::vector<std::string>& grid` as input, returning an `int`. Note that the grid is guaranteed to have equal number of rows and columns, and each row has exactly `n` characters.
#include <cassert>
#include <vector>
#include <string>

// The solution function declaration (assumed from above)
int regionsBySlashes(const std::vector<std::string>& grid);

int main() {
    // Single empty cell -> one region
    assert(regionsBySlashes({" "}) == 1);
    // Single slash '/' splits into two regions
    assert(regionsBySlashes({"/"}) == 2);
    // Single backslash splits into two regions
    assert(regionsBySlashes({"\\"}) == 2);
    // 2x2 all spaces -> one region
    assert(regionsBySlashes({"  ", "  "}) == 1);
    // 2x2 forming a closed loop with slashes
    assert(regionsBySlashes({"/\\", "\\/"}) == 5);
    // 2x2 with one slash and rest spaces
    assert(regionsBySlashes({" /", "  "}) == 2);
    // 3x3 all slashes forming multiple regions
    assert(regionsBySlashes({"/\\/", "\\/\\", "/\\/"}) > 2);
    // Edge case: 2x2 with only backslashes
    assert(regionsBySlashes({"\\\\", "\\\\"}) == 2); // note: string literal escapes
    // Mixed: spaces and slashes for a specific known case
    assert(regionsBySlashes({"/ ", " \\"}) == 4);
    return 0;
}
#include <vector>
#include <string>
#include <numeric>

// Count the number of connected regions formed by slashes (/, \) and spaces in a square grid.
int regionsBySlashes(const std::vector<std::string>& grid) {
    int n = static_cast<int>(grid.size());
    int total = n * n * 4;
    std::vector<int> parent(total);
    std::iota(parent.begin(), parent.end(), 0);
    int regions = total;

    // Recursive find with path compression
    std::function<int(int)> find = [&](int x) -> int {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    };

    // Union two sets; if they were separate, merge and decrement region count
    auto unite = [&](int a, int b) {
        int ra = find(a);
        int rb = find(b);
        if (ra != rb) {
            parent[ra] = rb;
            --regions;
        }
    };

    // Map cell (i,j) and triangle k (0=top,1=right,2=bottom,3=left) to a global index
    auto idx = [&](int i, int j, int k) -> int {
        return (i * n + j) * 4 + k;
    };

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            // Connect with cell above (if exists): bottom of above with top of current
            if (i > 0) {
                unite(idx(i - 1, j, 2), idx(i, j, 0));
            }
            // Connect with cell to the left (if exists): right of left with left of current
            if (j > 0) {
                unite(idx(i, j - 1, 1), idx(i, j, 3));
            }

            char c = grid[i][j];
            if (c != '/') {
                // No forward slash: top connects to right, bottom connects to left
                unite(idx(i, j, 0), idx(i, j, 1));
                unite(idx(i, j, 2), idx(i, j, 3));
            }
            if (c != '\\') {
                // No backslash: top connects to left, bottom connects to right
                unite(idx(i, j, 0), idx(i, j, 3));
                unite(idx(i, j, 2), idx(i, j, 1));
            }
        }
    }

    return regions;
}
// The problem is solved using a union-find (disjoint set) data structure. Each unit square in the `n x n` grid is divided into four smaller triangles (or regions) identified by indices 0, 1, 2, 3 representing the top, right, bottom, and left sub-triangles, respectively, when the square is subdivided by both diagonals conceptually. The total number of initial regions is `4 * n * n`, one per sub-triangle. We then connect (union) sub-triangles that share a boundary that is **not** crossed by a slash. Specifically, within a cell:
// - If the cell contains a space (`' '`), both diagonals are absent, so we union all four sub-triangles together (top with right, bottom with left, and also top with left, bottom with right) to make the entire cell one region.
// - If the cell contains `/`, we union top with right (since the `/` separates top-left from bottom-right) and union bottom with left.
// - If the cell contains `\`, we union top with left and bottom with right.
// Additionally, between adjacent cells, the sub-triangles that share a common grid edge are always connected because there is no slash on the grid line itself. For cell `(i,j)`:
// - Connect with the cell above: bottom sub-triangle of `(i-1,j)` (index 2) with top sub-triangle of `(i,j)` (index 0).
// - Connect with the cell to the left: right sub-triangle of `(i,j-1)` (index 1) with left sub-triangle of `(i,j)` (index 3).
// Each successful union reduces the region count by 1. At the end, the remaining count of disjoint sets equals the number of connected regions. Edge cases include grids of size 1, cells with spaces, and grids entirely filled with slashes. The algorithm runs in `O(n^2 * α(n^2))` time where `α` is the inverse Ackermann function (nearly constant), and uses `O(n^2)` space for the union-find array.
