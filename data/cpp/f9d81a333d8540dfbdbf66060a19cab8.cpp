// Write a C++ function `int countTrailheadScore(const std::vector<std::vector<int>>& grid)` that, given a rectangular grid of digits (0–9) representing a topographic map, counts the total number of distinct full hiking trails that start at any cell with height 0 and end at a cell with height 9. A trail must increase in height by exactly 1 with each step, moving only up, down, left, or right (no diagonal moves). A trail cannot revisit a cell. For each distinct starting cell (height 0), you count every distinct path that reaches a height-9 cell. Sum these counts over all starting cells. The grid is guaranteed non-empty and rectangular. For example, a 3x3 grid `{{0,1,2},{3,4,5},{6,7,8}}` has one starting point and exactly one path to the 9? Actually it has no 9; adjust: for a grid with a single path from 0 to 9 you'd count 1. The function should return the total sum. Assume the input grid is immutable.
// The problem is essentially counting all distinct simple paths in a directed acyclic graph (DAG) where nodes are grid cells with heights, and edges connect a cell to adjacent cells of height exactly one greater. Since heights strictly increase, no cycles are possible, so a simple non-revisiting constraint is automatically satisfied. The main algorithm is a depth-first search (DFS) from each cell with height 0. At each step, if the current height is 9, return 1 (a completed trail). Otherwise, explore all four orthogonal neighbors whose height equals current height + 1, summing the results. To avoid recomputation, memoization (dynamic programming) over `(row, col)` is possible because the number of distinct trails from a given cell is fixed and independent of the path taken to reach it. However, for simplicity and given typical grid sizes, a plain DFS per starting point works; but memoization improves performance. Edge cases: all cells are zeros (then no trail), isolated 0 with no path to 9 (count 0), multiple starts and overlapping paths (each path is distinct by its own route). Time complexity: Without memoization, in the worst case (a “grid” that is a long snake), each path explored once, but could be O(number of trails) which can be exponential in grid area. With memoization using a 2D table storing `long long` counts, each cell is processed once per its possible continuation, so time is O(R*C*4) = O(R*C) per start? Actually memoization makes total time O(R*C) because each cell's result is computed once and reused across different starts, since the number of trails from a given cell depends only on that cell, not the path taken. But careful: memoization is valid because the future options from a cell are independent of how we reached it. Thus we can compute a DP table storing the count of trails from a cell to any 9 reachable by increasing steps. Then sum for all cells with height 0. Space O(R*C). Simpler: without memoization, still acceptable for small grids; but we'll implement memoization for efficiency.
#include <vector>
#include <functional>

// Count total number of distinct hiking trails from all height-0 cells to height-9 cells.
// A trail moves orthogonally and increases height by exactly 1 each step.
int countTrailheadScore(const std::vector<std::vector<int>>& grid) {
    if (grid.empty() || grid[0].empty()) return 0;
    int rows = grid.size();
    int cols = grid[0].size();
    
    // Memoization table: -1 means not computed, otherwise number of trails from this cell.
    std::vector<std::vector<long long>> memo(rows, std::vector<long long>(cols, -1));
    
    // Recursive DFS with memoization.
    std::function<long long(int,int)> dfs = [&](int r, int c) -> long long {
        if (memo[r][c] != -1) return memo[r][c];
        
        int current = grid[r][c];
        if (current == 9) {
            memo[r][c] = 1;
            return 1;
        }
        
        int target = current + 1;
        long long total = 0;
        // Check four orthogonal neighbors.
        if (r > 0 && grid[r-1][c] == target) total += dfs(r-1, c);
        if (r+1 < rows && grid[r+1][c] == target) total += dfs(r+1, c);
        if (c > 0 && grid[r][c-1] == target) total += dfs(r, c-1);
        if (c+1 < cols && grid[r][c+1] == target) total += dfs(r, c+1);
        
        memo[r][c] = total;
        return total;
    };
    
    long long total_score = 0;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] == 0) {
                total_score += dfs(r, c);
            }
        }
    }
    
    // The expected answer fits in int, but using long long for safety.
    return static_cast<int>(total_score);
}
#include <cassert>
#include <vector>

// The solution function is assumed to be declared above.

int main() {
    // Simple 1x2 grid: 0 -> 9 has one path.
    std::vector<std::vector<int>> g1 = {{0, 9}};
    assert(countTrailheadScore(g1) == 1);
    
    // 3x3 grid with a single trail from 0 to 9.
    std::vector<std::vector<int>> g2 = {
        {0, 1, 2},
        {5, 4, 3},
        {6, 7, 8}  // Actually last row needs 8 then 9? Let's build a proper snake.
    };
    // Better: a 3x3 grid where only one path exists.
    std::vector<std::vector<int>> g2_fixed = {
        {0, 1, 2},
        {5, 4, 3},
        {6, 7, 8}  // no 9, so score 0.
    };
    assert(countTrailheadScore(g2_fixed) == 0);
    
    // A grid with exactly one trail: 0-1-2-3-4-5-6-7-8-9 in a row.
    std::vector<std::vector<int>> g3 = {{0,1,2,3,4,5,6,7,8,9}};
    assert(countTrailheadScore(g3) == 1);
    
    // Two separate trails from same start to two different 9s.
    std::vector<std::vector<int>> g4 = {
        {0, 1},
        {2, 1},
        {3, 2},
        {8, 3},
        {9, 8}
    };
    // This is complex; let's do a simple Y shape.
    std::vector<std::vector<int>> g4_simple = {
        {0, 1, 9},
        {0, 1, 9}  // not correct as same cell? Let's do a plus shape.
    };
    // Use a 3x3 grid: top-left 0, path to center 1, then to right 2, down 8? Let's do a known one.
    std::vector<std::vector<int>> g4_known = {
        {0, 1, 2},
        {9, 8, 3},
        {8, 7, 4}  // no 9 at end? Actually there are two 9s at (1,0) and (2,0). Path: 0-1-8-9 and 0-1-8-9? Not distinct.
    };
    // Simpler: two branches from same 0.
    std::vector<std::vector<int>> g4_branch = {
        {0, 1, 9},
        {9, 1, 2}  // Not valid.
    };
    // Let's use a well-defined test: a grid with two distinct trails each starting from 0 and ending at different 9s.
    std::vector<std::vector<int>> g4_valid = {
        {0, 1, 2},
        {9, 8, 3},
        {9, 7, 4}  // Actually 8 then 9 at (1,0) and (2,0). Path1: 0(0,0)->1(0,1)->2(0,2)->3(1,2)->4(2,2) not to 9. So not good.
    };
    // Let's just do a 2x2 grid: 0-1, 9-8? That doesn't have a path. Use:
    // 0 at (0,0), 1 at (0,1), 2 at (1,1), then 9 at (1,0) and (1,2)? Not.
    // Simpler: a 1x4 grid: [0, 1, 8, 9] and also [0,1,2,9]? Not possible.
    // To avoid confusion, test a known simple case: a grid with two distinct starts and each has one path.
    std::vector<std::vector<int>> g5 = {
        {0, 1, 2},
        {9, 8, 3},
        {0, 7, 4}  // start at (0,0) goes to 9 at (1,0)? No: 0(0,0)->1(0,1)->2(0,2)->3(1,2)->4(2,2) no 9. So zero.
    };
    // Let's just test the given example from the snippet: a mini example we can compute.
    // mini_example.txt would be "0123\n1234\n2345\n3456" but no 9. So we'll make a simple one.
    std::vector<std::vector<int>> g6 = {
        {0, 1},
        {9, 1}  // from (0,0) to (0,1) to (1,1)? no, (0,1) height 1, (1,1) height 1 not 2. So only path: 0->1->9? But (0,1)->(1,1) not. Actually (0,1) to (1,1) is 1 to 1 not valid. So no path.
    };
    // Let's do a correct one:
    std::vector<std::vector<int>> g7 = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    assert(countTrailheadScore(g7) == 1);
    
    // Grid with two starts each having one path:
    std::vector<std::vector<int>> g8 = {
        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
        {9, 8, 7, 6, 5, 4, 3, 2, 1, 0}
    };
    // From (0,0) one path to (0,9). From (1,9) one path to (1,0)? Actually (1,9) is 0, then 1 at (1,8) ... so one path. So total 2.
    assert(countTrailheadScore(g8) == 2);
    
    // All zeros: no trails.
    std::vector<std::vector<int>> g9 = {{0,0,0},{0,0,0}};
    assert(countTrailheadScore(g9) == 0);
    
    // Single cell 0 with no 9: 0.
    std::vector<std::vector<int>> g10 = {{0}};
    assert(countTrailheadScore(g10) == 0);
    
    return 0;
}
