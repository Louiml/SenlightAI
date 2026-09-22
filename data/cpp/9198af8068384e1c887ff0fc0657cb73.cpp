/*
Given an \( n \times n \) grid of distinct positive integers representing altitudes (with \( 2 \le n \le 50 \)), write a C++ function that returns a `std::vector<std::pair<int,int>>` containing the coordinates (using 1-based indexing, row then column) of a longest strictly decreasing path, where movement is allowed only to adjacent cells (up, down, left, right). If multiple longest paths exist, return one that starts at the lexicographically smallest starting cell (by row, then column) and, when moving step by step, always chooses the smallest row index (then smallest column index) among valid next cells that lie on some longest path. The returned path must be ordered from the starting cell to the ending cell (the end being the cell with minimum altitude on the path). The function signature is: `std::vector<std::pair<int,int>> longestDecreasingPath(const std::vector<std::vector<int>>& grid)`. The grid is guaranteed to have no cycles of decreasing moves, and every cell has a finite longest path length.
*/

#include <vector>
#include <algorithm>
#include <cstddef>

// Compute longest strictly decreasing path on a grid. Returns coordinates (1-based row, col) from start to end.
std::vector<std::pair<int,int>> longestDecreasingPath(const std::vector<std::vector<int>>& grid) {
    const int rows = static_cast<int>(grid.size());
    if (rows == 0) return {};
    const int cols = static_cast<int>(grid[0].size());
    
    // Memoization table: alt[i][j] = longest path length starting at (i,j)
    std::vector<std::vector<int>> alt(rows, std::vector<int>(cols, 0));
    
    // Direction vectors for up, right, down, left
    const int dx[4] = {-1, 0, 1, 0};
    const int dy[4] = {0, 1, 0, -1};
    
    // Depth-first search with memoization
    std::function<int(int,int)> dfs = [&](int x, int y) -> int {
        if (alt[x][y] != 0) return alt[x][y];
        int best = 1;
        for (int k = 0; k < 4; ++k) {
            int nx = x + dx[k];
            int ny = y + dy[k];
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && grid[nx][ny] < grid[x][y]) {
                best = std::max(best, 1 + dfs(nx, ny));
            }
        }
        alt[x][y] = best;
        return best;
    };
    
    // Find maximum length and smallest starting cell (row then column)
    int maxLen = 0;
    int startRow = rows, startCol = cols;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int len = dfs(i, j);
            if (len > maxLen || (len == maxLen && (i < startRow || (i == startRow && j < startCol)))) {
                maxLen = len;
                startRow = i;
                startCol = j;
            }
        }
    }
    
    // Reconstruct path from start to end
    std::vector<std::pair<int,int>> path;
    int cx = startRow, cy = startCol;
    while (true) {
        path.emplace_back(cx + 1, cy + 1); // convert to 1-based
        if (alt[cx][cy] == 1) break;
        // Choose next cell: neighbor with alt[neighbor] + 1 == alt[current], smallest row then col
        int bestNx = -1, bestNy = -1;
        for (int k = 0; k < 4; ++k) {
            int nx = cx + dx[k];
            int ny = cy + dy[k];
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && grid[nx][ny] < grid[cx][cy] && alt[nx][ny] + 1 == alt[cx][cy]) {
                if (bestNx == -1 || nx < bestNx || (nx == bestNx && ny < bestNy)) {
                    bestNx = nx;
                    bestNy = ny;
                }
            }
        }
        cx = bestNx;
        cy = bestNy;
    }
    return path;
}

#include <cassert>
#include <vector>
#include <utility>

// Assume the solution function is provided above

int main() {
    // Test 1: 3x3 grid with a simple longest path
    std::vector<std::vector<int>> grid1 = {
        {9, 8, 7},
        {6, 5, 4},
        {3, 2, 1}
    };
    auto path1 = longestDecreasingPath(grid1);
    // Longest path length is 9, starting at (1,1) ending at (3,3)
    assert(path1.size() == 9);
    assert(path1.front() == std::make_pair(1, 1));
    assert(path1.back() == std::make_pair(3, 3));
    
    // Test 2: Grid with multiple paths, lexicographically smallest start
    std::vector<std::vector<int>> grid2 = {
        {1, 2, 3},
        {6, 5, 4},
        {7, 8, 9}
    };
    auto path2 = longestDecreasingPath(grid2);
    // Longest decreasing path length is 5 (from 9 to 1 or 6 to 1 etc.)
    assert(path2.size() == 5);
    // The start is (2,1) with value 6? Actually check: 6->5->4->3->2->1 length 6? No, 6->5->4->3->2->1 = 6? Wait 6,5,4,3,2,1 = 6 cells? Let's see: 6(2,1),5(2,2),4(2,3),3(1,3),2(1,2),1(1,1) length 6.
    // But also 9(3,3)->8(3,2)->7(3,1)->6... Actually 7->6 is not decreasing since 7>6? No, 7 to 6 is decreasing, but 7(3,1)->6(2,1)->5->4->3->2->1 length 7? That's longest.
    // Let's compute: 7,6,5,4,3,2,1 = 7. Start at (3,1). So assert size 7 and first is (3,1).
    assert(path2.size() == 7);
    assert(path2.front() == std::make_pair(3, 1));
    assert(path2.back() == std::make_pair(1, 1));
    
    // Test 3: Single cell grid (though constraints say n>=2, handle gracefully)
    std::vector<std::vector<int>> grid3 = {{42}};
    auto path3 = longestDecreasingPath(grid3);
    assert(path3.size() == 1);
    assert(path3.front() == std::make_pair(1, 1));
    
    // Test 4: Grid with multiple longest paths, tie-breaking on start and steps
    std::vector<std::vector<int>> grid4 = {
        {2, 1},
        {3, 4}
    };
    auto path4 = longestDecreasingPath(grid4);
    // Longest decreasing path: 4(2,2)->3(2,1)->2(1,1)->1(1,2) length 4, or 4->2? No, 4->3->1? 4->3->2->1 length 4, and 4->2->1? 4>2 but 2 is not adjacent? Actually (2,2) neighbors: (2,1)=3 and (1,2)=1. So 4->3->2->1 length 4, and 4->1 length 2. So start at (2,2), length 4. Tie with? 3->2->1 length 3. So only one start. Check path.
    assert(path4.size() == 4);
    assert(path4.front() == std::make_pair(2, 2));
    assert(path4.back() == std::make_pair(1, 2)); // 1 is at (1,2)
    
    // Test 5: A case where tie-breaking on next step matters
    std::vector<std::vector<int>> grid5 = {
        {5, 4, 3},
        {2, 1, 0}
    };
    auto path5 = longestDecreasingPath(grid5);
    // Longest: 5(1,1)->4(1,2)->3(1,3)->0? No, 3->0 not adjacent. Actually 5->4->3->0? (1,3) to (2,3)=0, yes. Length 4. Also 5->4->1->0? (1,2)->(2,2)=1->(2,3)=0 length 4. And 5->2->1->0? (1,1)->(2,1)=2->(2,2)=1->(2,3)=0 length 4. All start at (1,1). For next step, among (1,2)=4 and (2,1)=2, both valid, pick smallest row then col: (1,2). Then from (1,2), next choices: (1,3)=3 and (2,2)=1, pick (1,3). Then from (1,3) only (2,3)=0. So path is (1,1),(1,2),(1,3),(2,3).
    assert(path5.size() == 4);
    assert(path5[0] == std::make_pair(1, 1));
    assert(path5[1] == std::make_pair(1, 2));
    assert(path5[2] == std::make_pair(1, 3));
    assert(path5[3] == std::make_pair(2, 3));
    
    // Test 6: Large grid with values increasing and decreasing, check length equals count of distinct values in a path
    std::vector<std::vector<int>> grid6 = {
        {7, 8, 9},
        {6, 5, 4},
        {1, 2, 3}
    };
    auto path6 = longestDecreasingPath(grid6);
    // Longest path: 9(1,3)->8(1,2)->7(1,1)->6(2,1)->5(2,2)->4(2,3)->3(3,3)->2(3,2)->1(3,1) length 9? Check adjacency: all adjacent? 9->8 yes, 8->7 yes, 7->6 yes, 6->5 yes, 5->4 yes, 4->3? (2,3) to (3,3)=3 yes, 3->2 yes, 2->1 yes. So length 9. Start at (1,3), end at (3,1).
    assert(path6.size() == 9);
    assert(path6.front() == std::make_pair(1, 3));
    assert(path6.back() == std::make_pair(3, 1));
    
    // Test 7: All cells distinct but decreasing only in one direction
    std::vector<std::vector<int>> grid7 = {
        {10, 9, 8},
        {7, 6, 5},
        {4, 3, 2}
    };
    auto path7 = longestDecreasingPath(grid7);
    // Longest path is all 9 cells from (1,1) to (3,3)
    assert(path7.size() == 9);
    assert(path7.front() == std::make_pair(1, 1));
    assert(path7.back() == std::make_pair(3, 3));
    
    // Test 8: Check path is strictly decreasing by values
    auto path8 = longestDecreasingPath(grid1);
    for (size_t i = 1; i < path8.size(); ++i) {
        int prevR = path8[i-1].first - 1;
        int prevC = path8[i-1].second - 1;
        int curR = path8[i].first - 1;
        int curC = path8[i].second - 1;
        assert(grid1[prevR][prevC] > grid1[curR][curC]);
        // Check adjacency
        assert(abs(prevR - curR) + abs(prevC - curC) == 1);
    }
    
    // Test 9: Ensure no duplicate coordinates in path
    auto path9 = longestDecreasingPath(grid1);
    std::vector<std::pair<int,int>> unique = path9;
    std::sort(unique.begin(), unique.end());
    assert(std::unique(unique.begin(), unique.end()) == unique.end());
    
    // Test 10: Path ends at a cell with the smallest value in the path (length 1)
    auto path10 = longestDecreasingPath(grid1);
    int lastR = path10.back().first - 1;
    int lastC = path10.back().second - 1;
    assert(grid1[lastR][lastC] == 1);
    
    return 0;
}

// The problem is a classic longest decreasing path on a grid, solvable via dynamic programming with memoization. For each cell, compute the length of the longest decreasing path starting from that cell by recursively exploring its four neighbors that have strictly smaller altitude, taking the maximum over those results plus one. The base case is a cell with no smaller neighbors, giving length 1. Process all cells and record the maximum length and the starting cell with the smallest row then column among those achieving the maximum. To reconstruct the path, start from that chosen cell, and at each step among the neighbors that are smaller and satisfy `alt[neighbor] + 1 == alt[current]` (i.e., lie on some longest path from the current), choose the neighbor with the smallest row then column. Continue until reaching a cell whose `alt` is 1. Edge cases include a single-cell grid (n=1 is not allowed per constraints but handle anyway), grids with all equal values (but constraints say distinct), and ensuring coordinates are 1-based in the output. Time complexity is \( O(n^2) \) because each cell is processed once via memoization with constant neighbor checks. Space complexity is \( O(n^2) \) for the memo table and recursion stack in the worst case.
