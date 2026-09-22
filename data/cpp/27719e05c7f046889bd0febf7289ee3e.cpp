Write a C++ function `int reachableCells(int m, int n, int k)` that simulates a robot starting at grid position `(0,0)` in an `m`-by-`n` grid (rows and columns indexed from 0). The robot can move one step up, down, left, or right, but cannot leave the grid and cannot enter any cell whose row coordinate's digit sum plus column coordinate's digit sum exceeds `k`. The function must return the total number of distinct cells the robot can reach. The grid sizes satisfy `1 <= m, n <= 100` and `k` is between `0` and `20`. The solution must handle the case where `k=0` (only cell `(0,0)` is reachable) and must not count any cell more than once even if reached via multiple paths.
#include <cassert>

int main() {
    // Basic cases from the problem statement.
    assert(reachableCells(1, 1, 0) == 1);
    assert(reachableCells(1, 1, 20) == 1);

    // k=0: only origin reachable even if grid is larger.
    assert(reachableCells(3, 3, 0) == 1);

    // Small grid: 2x2, k=1. All four cells have digit sums 0 or 1.
    // (0,0):0, (0,1):1, (1,0):1, (1,1):2 -> not allowed.
    assert(reachableCells(2, 2, 1) == 3);

    // 3x3 grid, k=1: reachable cells: (0,0), (0,1), (1,0) — (0,2) sum=2 not, 
    // (2,0) sum=2 not, (1,1) sum=2 not.
    assert(reachableCells(3, 3, 1) == 3);

    // 10x10 grid with k large enough to cover all cells.
    // Max digit sum for row/col up to 9 is 9+9=18 for (9,9). k=18 covers all.
    assert(reachableCells(10, 10, 18) == 100);

    // Larger dimension with moderate k. Manually compute for 3x3 k=2:
    // All cells allowed except (2,2)? digit sum 2+2=4 >2. Actually (0,0)=0, 
    // (0,1)=1, (0,2)=2, (1,0)=1, (1,1)=2, (1,2)=3>2, (2,0)=2, (2,1)=3>2, (2,2)=4>2.
    // Reachable: all except (1,2),(2,1),(2,2) => 6 cells.
    assert(reachableCells(3, 3, 2) == 6);

    // Asymmetric grid (1 row, many columns): k controls reachable columns.
    // For row=0, digit sum 0. So columns with digit sum <=k are reachable.
    // 10 columns, k=1: reachable columns: 0,1,10? but n=10 so columns 0..9. 
    // digit sums: 0,1,2,...9. Only sums 0 and 1 allowed: columns 0,1.
    assert(reachableCells(1, 10, 1) == 2);

    // Column that has multiple digits. e.g., n=20, k=2: columns with digit sum <=2:
    // 0,1,2,10,11,20? but max col=19. So: 0,1,2,10,11. That's 5 columns.
    assert(reachableCells(1, 20, 2) == 5);

    // Ensure no double counting in full reachable grid.
    assert(reachableCells(5, 5, 100) == 25);
}
#include <vector>

// Returns the number of cells reachable by a robot starting at (0,0) in an m x n grid,
// moving up/down/left/right, staying inside the grid, and only entering cells where
// the sum of digit sums of its row and column indices is <= k.
int reachableCells(int m, int n, int k) {
    if (m < 1 || n < 1 || k < 0) {
        return 0;
    }

    std::vector<std::vector<bool>> visited(m, std::vector<bool>(n, false));
    int count = 0;

    // Helper lambda for DFS, captures by reference.
    auto dfs = [&](int x, int y) -> void {
        if (x < 0 || x >= m || y < 0 || y >= n) {
            return;
        }
        if (visited[x][y]) {
            return;
        }

        // Compute digit sum for x and y.
        int sumX = 0;
        int tempX = x;
        while (tempX > 0) {
            sumX += tempX % 10;
            tempX /= 10;
        }
        int sumY = 0;
        int tempY = y;
        while (tempY > 0) {
            sumY += tempY % 10;
            tempY /= 10;
        }
        if (sumX + sumY > k) {
            return;
        }

        visited[x][y] = true;
        count++;
        dfs(x + 1, y);
        dfs(x - 1, y);
        dfs(x, y + 1);
        dfs(x, y - 1);
    };

    dfs(0, 0);
    return count;
}
// The problem is a classic flood-fill / reachability count on a grid with a constraint based on digit sums. The most straightforward approach is depth-first search (DFS) starting from `(0,0)`. Use a 2D boolean visited matrix to prevent revisiting cells, because the robot cannot benefit from revisiting a cell, and counting each cell once is required. At each current cell `(x,y)`, check bounds, visited status, and the digit-sum condition. If any check fails, stop exploring that branch. Otherwise, increment the count, mark visited, and recursively explore all four neighbors. Time complexity is `O(m * n)` because each cell is visited at most once, and each visit does constant work (digit sum computation for two numbers, which itself is `O(log10(max(m,n)))` but bounded by small constants since m,n ≤ 100). Space complexity is `O(m * n)` for the visited matrix plus `O(m * n)` worst-case recursion stack depth if the grid is fully traversable, but since m,n ≤ 100, recursion depth is at most 10000, which is acceptable. Edge cases include `k=0` (only origin reachable), grids where the reachable area is small (e.g., one row or column), and grids with large dimensions where digit sum constraint blocks off large regions, but the algorithm naturally handles all.
