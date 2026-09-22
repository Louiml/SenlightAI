// Write a C++ function that takes a 2D vector of integers representing a matrix, along with its number of rows and columns, and returns the length of the longest strictly increasing path. From any cell, you can move in four directions (up, down, left, right), but you cannot move diagonally. A valid path must consist of strictly increasing values in consecutive cells, meaning each next cell’s value must be greater than the current cell’s value. The function should handle matrices of any size (including 1×1 and rectangular matrices) and must efficiently compute the result using memoization. The matrix may contain negative numbers and duplicates, but duplicates cannot be part of the same path because the requirement is strictly increasing.

// The problem asks for the maximum length of a strictly increasing path in a grid. A brute-force DFS from every cell would explore all possible paths, leading to exponential time. Instead, we use dynamic programming with memoization: for each cell, we compute the longest increasing path starting from that cell and store it in a `dp` table. The recursive function `find` takes the current cell coordinates, the previous cell’s value (or -1 for the starting point), the matrix, and the DP table. It returns 0 if the current cell is out of bounds or if the current cell's value is less than or equal to the previous value (since strictly increasing is required). Otherwise, it checks the four neighbors and recursively computes the maximum path length among them, then adds 1 for the current cell. Because the path is strictly increasing, there are no cycles, so we do not need a visited array. The DP table stores the result for each cell to avoid recomputation, reducing the time complexity to O(n*m), where n is rows and m is columns, since each cell is processed once. Space complexity is O(n*m) for the DP table plus the recursion stack in the worst case (which could also be O(n*m) for a long path, but typically it’s bounded by the maximum recursion depth which is at most n*m). Edge cases include empty matrix (return 0), a single cell (return 1), multiple disconnected increasing paths, and matrices with decreasing values where the answer is 1 (each cell alone).

#include <vector>
#include <algorithm>

// Returns the length of the longest strictly increasing path in the matrix.
int longestIncreasingPath(const std::vector<std::vector<int>>& matrix, int n, int m) {
    if (n == 0 || m == 0) return 0;
    std::vector<std::vector<int>> dp(n, std::vector<int>(m, -1));
    
    // Recursive helper with memoization.
    // Returns the longest increasing path starting at (i, j) given that the previous value is 'prev'.
    auto find = [&](int i, int j, int prev, auto&& find_ref) -> int {
        if (i < 0 || i >= n || j < 0 || j >= m || prev >= matrix[i][j]) {
            return 0;
        }
        if (dp[i][j] != -1) {
            return dp[i][j];
        }
        int up = find_ref(i - 1, j, matrix[i][j], find_ref);
        int down = find_ref(i + 1, j, matrix[i][j], find_ref);
        int left = find_ref(i, j - 1, matrix[i][j], find_ref);
        int right = find_ref(i, j + 1, matrix[i][j], find_ref);
        dp[i][j] = std::max({up, down, left, right}) + 1;
        return dp[i][j];
    };
    
    int ans = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            ans = std::max(ans, find(i, j, -1, find));
        }
    }
    return ans;
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: Simple increasing path in a 3x3 grid
    std::vector<std::vector<int>> mat1 = {{1, 2, 3}, {6, 5, 4}, {7, 8, 9}};
    assert(longestIncreasingPath(mat1, 3, 3) == 9); // Path: 1->2->3->4->5->6->7->8->9? Actually check: 1->2->3->4->5->6->7->8->9 is not a path, but there is a Hamiltonian path: 1-2-3-4-5-6-7-8-9? Let's think: 1->2->3->4 (down from 3 to 4? 3 is at (0,2), 4 is at (1,2) yes) then 5, then 6, then 7, then 8, then 9: length 9. Good.

    // Test 2: Single cell
    std::vector<std::vector<int>> mat2 = {{5}};
    assert(longestIncreasingPath(mat2, 1, 1) == 1);

    // Test 3: All decreasing - no increasing neighbor
    std::vector<std::vector<int>> mat3 = {{9, 8}, {7, 6}};
    assert(longestIncreasingPath(mat3, 2, 2) == 1);

    // Test 4: Empty matrix (n=0)
    std::vector<std::vector<int>> mat4 = {};
    assert(longestIncreasingPath(mat4, 0, 0) == 0);

    // Test 5: 2x2 with clear path
    std::vector<std::vector<int>> mat5 = {{1, 4}, {2, 3}};
    assert(longestIncreasingPath(mat5, 2, 2) == 4); // 1->2->3->4

    // Test 6: Negative numbers
    std::vector<std::vector<int>> mat6 = {{-1, -2}, {-3, -4}};
    assert(longestIncreasingPath(mat6, 2, 2) == 1); // No increasing path due to strictly increasing, all decreasing

    // Test 7: Rectangular matrix
    std::vector<std::vector<int>> mat7 = {{3, 4, 5}, {6, 2, 1}};
    assert(longestIncreasingPath(mat7, 2, 3) == 4); // 3->4->5 and also 2->? Actually 3->4->5 is length 3, 3->6? 3<6 yes then 6? can't go to 2 because 6>2. So best is 3-4-5 length 3? But also 2->? no. Let's re-evaluate: 1->2? 1<2? matrix[1][2]=1, matrix[1][1]=2, yes 1->2, then 2->3? 2<3 yes, then 3->4? 3<4, then 4->5? 4<5, that's length 5? Wait can you move from (1,2)=1 to (1,1)=2, then to (0,1)=4? 2<4 yes, then to (0,2)=5? 4<5, so path: 1->2->4->5 length 4? Also 3->4->5 length 3, and 3->6? 3<6, then 6? can't go anywhere else because 6 is max. So best is 4. assert 4.

    // Test 8: Large matrix with duplicates
    std::vector<std::vector<int>> mat8 = {{2, 2, 2}, {2, 2, 2}};
    assert(longestIncreasingPath(mat8, 2, 3) == 1); // Strictly increasing, so no two equal numbers can be consecutive

    return 0;
}
