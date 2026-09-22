Write a C++ function that takes three integers `n`, `m`, and `k` as input, where `n` and `m` are positive integers representing the dimensions of a grid and `k` is a non-negative integer. The function should determine whether it is possible to traverse from the top-left cell `(1,1)` to the bottom-right cell `(n,m)` in an `n × m` grid using only down and right moves, such that the total cost of the path is exactly `k`. The cost of moving down from row `i` to row `i+1` at any column is `i+1` (the row index being entered), and the cost of moving right from column `j` to column `j+1` at any row is `j+1` (the column index being entered). Return `true` if such a path exists, and `false` otherwise. Note that the starting cell has zero cost, and the grid coordinates start at 1. The function should handle arbitrary positive values of `n` and `m` up to at least 30, and `k` up to the maximum possible path cost.
// The problem is equivalent to finding the minimum and maximum possible path costs from `(1,1)` to `(n,m)` and checking if `k` lies within that inclusive range. Since every path from `(1,1)` to `(n,m)` involves exactly `(n-1)` downward moves and `(m-1)` rightward moves, the set of possible costs form a contiguous range of integers. This is a classic property for such grid cost problems where each move adds a fixed positive value dependent only on the source or destination coordinate. To compute the minimum and maximum costs, we can use dynamic programming. Define `dp_min[i][j]` as the minimum cost to reach cell `(i,j)`, and similarly `dp_max[i][j]` for the maximum. Since moves are only down and right, we can fill the DP table in row-major order. For the minimum, at each cell except `(1,1)`, the cost is the minimum of `dp_min[i][j-1] + i` (if coming from left, where `i` is the row index being entered) and `dp_min[i-1][j] + j` (if coming from above, where `j` is the column index being entered). For the maximum, we take the maximum similarly. The base cases are `dp_min[1][1] = dp_max[1][1] = 0` since we start there. All other cells are initialized to a large value for minimum and zero for maximum. After filling the tables, `dp_min[n][m]` gives the smallest possible cost and `dp_max[n][m]` the largest. If `k` lies between these two values inclusive, the answer is `true`; otherwise `false`. Important edge cases include `n = 1` or `m = 1`, where there is only one path (straight right or straight down), so both min and max are equal; the DP handles this correctly because only one transition is possible. Also, for larger grids, the DP must avoid accessing out-of-bounds indices; we can either pad arrays or add explicit boundary checks. Time complexity is `O(n*m)` to fill two DP tables, and space complexity is `O(n*m)` for the two tables. Given constraints up to 30, this is efficient.
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

// Determines if a path with exactly cost k exists in an n x m grid.
// Moves: down (cost = row index being entered) and right (cost = column index being entered).
bool pathWithExactCost(int n, int m, int k) {
    // DP tables for minimum and maximum cost to reach each cell.
    std::vector<std::vector<int>> min_cost(n + 1, std::vector<int>(m + 1, std::numeric_limits<int>::max()));
    std::vector<std::vector<int>> max_cost(n + 1, std::vector<int>(m + 1, 0));

    // Starting cell has zero cost.
    min_cost[1][1] = 0;
    max_cost[1][1] = 0;

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (i == 1 && j == 1) continue;  // Skip starting cell.

            // Transition from left: cost = current row index i.
            if (j > 1) {
                min_cost[i][j] = std::min(min_cost[i][j], min_cost[i][j - 1] + i);
                max_cost[i][j] = std::max(max_cost[i][j], max_cost[i][j - 1] + i);
            }
            // Transition from above: cost = current column index j.
            if (i > 1) {
                min_cost[i][j] = std::min(min_cost[i][j], min_cost[i - 1][j] + j);
                max_cost[i][j] = std::max(max_cost[i][j], max_cost[i - 1][j] + j);
            }
        }
    }

    int overall_min = min_cost[n][m];
    int overall_max = max_cost[n][m];
    return (k >= overall_min && k <= overall_max);
}
#include <cassert>

int main() {
    // Single-cell grid: only cost 0.
    assert(pathWithExactCost(1, 1, 0) == true);
    assert(pathWithExactCost(1, 1, 1) == false);

    // Single row, multiple columns: only one path, cost = sum of column indexes entered (2..m).
    assert(pathWithExactCost(1, 3, 2 + 3) == true);
    assert(pathWithExactCost(1, 3, 2 + 3 + 1) == false);

    // Single column, multiple rows: only one path, cost = sum of row indexes entered (2..n).
    assert(pathWithExactCost(3, 1, 2 + 3) == true);
    assert(pathWithExactCost(3, 1, 2 + 3 - 1) == false);

    // 2x2 grid: possible costs are 5 (right then down) or 5 (down then right), actually both same.
    // Right cost=2, then down cost=2 → total 4. Wait: down from (1,1) to (2,1) costs 1? No, cost = row index entered = 2. Right from (1,1) to (1,2) costs column index entered = 2. So both paths cost 2+2=4.
    assert(pathWithExactCost(2, 2, 4) == true);
    assert(pathWithExactCost(2, 2, 5) == false);

    // 3x3 grid: min cost? Path right, right, down, down: costs 2+3+2+3=10. Max cost? Path down, down, right, right: costs 2+3+2+3=10 also? Actually symmetric – both paths give 10. But let's compute: any path has two rights (costs 2 and 3) and two downs (costs 2 and 3), total 10. So only 10.
    assert(pathWithExactCost(3, 3, 10) == true);
    assert(pathWithExactCost(3, 3, 9) == false);

    // 4x3 grid: min cost? Let's compute min: right,right,down,down,down: costs 2+3+2+3+4=14. Max cost: down,down,down,right,right: costs 2+3+4+2+3=14 also. Actually all paths have same multiset? No, down costs depend on which row you enter, right costs on column. For 4x3, we have 3 downs (entering rows 2,3,4) and 2 rights (entering columns 2,3). So total = (2+3+4)+(2+3)=14 always. Thus only 14.
    assert(pathWithExactCost(4, 3, 14) == true);
    assert(pathWithExactCost(4, 3, 15) == false);

    // A case where range is non-trivial? Actually for this cost model, all paths have the same total? Let's check: total cost = sum of all entered row indices (for down moves) + sum of all entered column indices (for right moves). But the set of entered row indices is always {2,3,...,n} exactly once each, because you go down n-1 times and must enter each row from 2 to n exactly once. Similarly for columns. So total is fixed! Thus min = max always. So the function should only return true for exactly that fixed sum. Let's test: for n=3,m=4, sum rows entered = 2+3=5, sum columns entered = 2+3+4=9, total=14.
    assert(pathWithExactCost(3, 4, 14) == true);
    assert(pathWithExactCost(3, 4, 13) == false);

    // Large grid to ensure no overflow: n=30, m=30, total = sum(2..30) twice = 2*( (30*31/2)-1 ) = 2*(465-1)=928.
    assert(pathWithExactCost(30, 30, 928) == true);
    assert(pathWithExactCost(30, 30, 927) == false);
}
