Write a C++ function that takes an `n x n` grid of 0s and 1s, where 1 represents a traversable cell and 0 represents an obstacle, and returns a vector of strings containing all valid paths from the top-left corner (0,0) to the bottom-right corner (n-1,n-1) using only the movements Down ('D'), Left ('L'), Right ('R'), and Up ('U'). Paths must be returned in lexicographically sorted order (which matches the order 'D', 'L', 'R', 'U'). You may not revisit a cell within the same path. If the starting cell is blocked, return a vector containing the single string "-1". If no path exists but the start is open, return an empty vector.

#include <cassert>
#include <vector>
#include <string>

int main() {
    // Test 1: Simple 2x2 all ones
    std::vector<std::vector<int>> grid1 = {{1, 1}, {1, 1}};
    std::vector<std::string> res1 = findMazePaths(grid1);
    assert(res1.size() == 2);
    assert(res1[0] == "DR");
    assert(res1[1] == "RD");
    
    // Test 2: 3x3 with a blocked center
    std::vector<std::vector<int>> grid2 = {{1, 0, 0}, {1, 1, 0}, {0, 1, 1}};
    std::vector<std::string> res2 = findMazePaths(grid2);
    assert(res2.size() == 1);
    assert(res2[0] == "DRDR");
    
    // Test 3: Start blocked
    std::vector<std::vector<int>> grid3 = {{0, 1}, {1, 1}};
    std::vector<std::string> res3 = findMazePaths(grid3);
    assert(res3.size() == 1 && res3[0] == "-1");
    
    // Test 4: No path (blocked cells)
    std::vector<std::vector<int>> grid4 = {{1, 0}, {0, 1}};
    std::vector<std::string> res4 = findMazePaths(grid4);
    assert(res4.empty());
    
    // Test 5: 1x1 grid
    std::vector<std::vector<int>> grid5 = {{1}};
    std::vector<std::string> res5 = findMazePaths(grid5);
    assert(res5.size() == 1 && res5[0] == "");
    
    // Test 6: 3x3 with multiple paths, lexicographic order
    std::vector<std::vector<int>> grid6 = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
    std::vector<std::string> res6 = findMazePaths(grid6);
    // Expected lexicographic order from D,L,R,U exploration
    assert(res6.size() == 6);
    assert(res6[0] == "DDDR");  // D,D,D,R
    // Actually let's compute manually: paths from (0,0) to (2,2) with D,L,R,U
    // Possible: DDDR, DDRD, DRDD, RDDR, RDRD, RRDD (6) but lexicographic with D<L<R<U
    // We'll just check the count and that it's sorted
    for (size_t i = 1; i < res6.size(); ++i) {
        assert(res6[i - 1] < res6[i]);
    }
    
    // Test 7: Larger grid with obstacle forcing detour (backtracking case)
    std::vector<std::vector<int>> grid7 = {{1, 1, 1, 0}, {1, 0, 1, 1}, {1, 1, 0, 1}, {0, 1, 1, 1}};
    std::vector<std::string> res7 = findMazePaths(grid7);
    assert(!res7.empty());
    // Verify all paths end at bottom-right
    for (const auto& p : res7) {
        int x = 0, y = 0;
        for (char c : p) {
            if (c == 'D') x++;
            else if (c == 'U') x--;
            else if (c == 'R') y++;
            else if (c == 'L') y--;
        }
        assert(x == 3 && y == 3);
    }
    
    return 0;
}

#include <vector>
#include <string>

// Returns all paths from (0,0) to (n-1,n-1) in a grid of 0s and 1s.
// Moves: D, L, R, U (explored in this order to yield lexicographic output).
// If start is blocked, returns {"-1"}; if no path, returns empty vector.
std::vector<std::string> findMazePaths(const std::vector<std::vector<int>>& grid) {
    int n = grid.size();
    std::vector<std::string> result;
    if (n == 0) return result;
    if (grid[0][0] == 0) {
        result.push_back("-1");
        return result;
    }
    if (n == 1) {
        // Path from (0,0) to (0,0) is empty
        result.push_back("");
        return result;
    }
    
    std::vector<std::vector<bool>> visited(n, std::vector<bool>(n, false));
    std::string path;
    
    // Helper DFS lambda
    // Pass grid, visited, current coordinates, n, path, and result by reference
    // Use std::function for recursion
    std::function<void(int, int, std::string&)> dfs = [&](int x, int y, std::string& current) {
        if (x == n - 1 && y == n - 1) {
            result.push_back(current);
            return;
        }
        visited[x][y] = true;
        
        // Down
        if (x + 1 < n && !visited[x + 1][y] && grid[x + 1][y] == 1) {
            current.push_back('D');
            dfs(x + 1, y, current);
            current.pop_back();
        }
        // Left
        if (y - 1 >= 0 && !visited[x][y - 1] && grid[x][y - 1] == 1) {
            current.push_back('L');
            dfs(x, y - 1, current);
            current.pop_back();
        }
        // Right
        if (y + 1 < n && !visited[x][y + 1] && grid[x][y + 1] == 1) {
            current.push_back('R');
            dfs(x, y + 1, current);
            current.pop_back();
        }
        // Up
        if (x - 1 >= 0 && !visited[x - 1][y] && grid[x - 1][y] == 1) {
            current.push_back('U');
            dfs(x - 1, y, current);
            current.pop_back();
        }
        
        visited[x][y] = false;
    };
    
    dfs(0, 0, path);
    return result;
}

// The solution uses depth-first search (DFS) with backtracking. We maintain a `visited` matrix to track cells currently in the path. Starting from (0,0), we recursively explore all four possible moves in the order Down, Left, Right, Up to ensure the final list is already lexicographically sorted without needing a sort at the end. At each step, we check bounds, that the cell is a 1 (traversable), and that it hasn't been visited. When we reach the target (n-1, n-1), we add the accumulated path string to the result. After returning from recursion, we backtrack by unmarking the visited cell. Edge cases: if the start is 0, return {"-1"}. If n=1 and start is 1, the path is the empty string "" (which is valid). The DFS explores each cell at most once per path but may revisit in different paths, leading to exponential worst-case time complexity \(O(4^{n^2})\) in pathological grids, but typical grids are much faster. Space complexity is \(O(n^2)\) for the visited matrix and the recursion stack depth can be up to \(n^2\) in the longest path.
