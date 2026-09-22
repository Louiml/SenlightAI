// Write a C++ function named `chessElephantPaths` that takes two non-negative integers `n` and `m`, representing the coordinates of a destination cell on a grid, where the elephant starts at cell `(0, 0)` and can move any positive number of steps either horizontally left-to-right (increasing the row coordinate) or vertically bottom-to-top (increasing the column coordinate) in a single move. The function should return the total number of distinct paths from `(0,0)` to `(n,m)` using only these legal moves. For example, `(1,1)` can be reached by moving horizontally to `(1,0)` then vertically to `(1,1)`, or vertically to `(0,1)` then horizontally to `(1,1)`, giving 2 paths. The result may be large, so the function should return a `long long`. The function must handle the base cases where `(0,0)` has exactly 1 path (the empty path), and any negative coordinate should return 0 paths (though inputs are guaranteed non-negative, the recursive calls may produce negative intermediates in some implementations). Provide a recursive solution with memoization for efficiency.

// The problem is a variation of counting paths on a grid where from any cell `(i,j)` you can move to any cell `(i+k, j)` (horizontal moves, increasing row) or `(i, j+k)` (vertical moves, increasing column) for any `k>0`. The recurrence is: `path(i,j) = sum_{k=0}^{i-1} path(k,j) + sum_{k=0}^{j-1} path(i,k)`, with base cases `path(0,0)=1` and `path(i<0 or j<0)=0`. This recurrence directly mirrors the given snippet but without memoization would be exponential. By using a 2D memoization table (or a 1D DP with prefix sums), we can compute the answer in O(n*m) time and O(n*m) space. Edge cases: when `n=0` and `m>0`, only vertical moves are possible, and the number of paths is 1 (since any positive vertical move from `(0,0)` to `(0,m)` is a single move, but also there are no intermediate steps; actually the recurrence gives sum over `k=0` to `m-1` of `path(0,k)`, which with memoization yields 1 for any `m>0` because `path(0,0)=1` and the rest are 1). Similarly for `m=0`. The result grows quickly; for `n=m=10` it is 184756 (central binomial coefficient). Time complexity is O(n*m) due to each cell being computed once, and each cell's computation involves summing over previous rows/columns, which can be optimized with prefix sums to O(n*m) overall. Space complexity is O(n*m) for the memo table.

#include <vector>

// Count distinct paths for an elephant moving only right/up any positive distance.
long long chessElephantPaths(int n, int m) {
    // Memoization table: dp[i][j] = number of paths from (0,0) to (i,j)
    // Initialize with -1 to indicate uncomputed.
    std::vector<std::vector<long long>> dp(n + 1, std::vector<long long>(m + 1, -1));
    
    // Helper lambda for recursion with memoization.
    // Using a lambda with 'auto' and capturing dp by reference.
    auto solve = [&](auto&& self, int i, int j) -> long long {
        // Base cases
        if (i == 0 && j == 0) return 1;
        if (i < 0 || j < 0) return 0;
        if (dp[i][j] != -1) return dp[i][j];
        
        long long ans = 0;
        // Horizontal moves: from any (k, j) where 0 <= k < i
        for (int k = 0; k < i; ++k) {
            ans += self(self, k, j);
        }
        // Vertical moves: from any (i, k) where 0 <= k < j
        for (int k = 0; k < j; ++k) {
            ans += self(self, i, k);
        }
        dp[i][j] = ans;
        return ans;
    };
    
    return solve(solve, n, m);
}

int main() {
    assert(chessElephantPaths(0, 0) == 1);
    assert(chessElephantPaths(1, 0) == 1);
    assert(chessElephantPaths(0, 1) == 1);
    assert(chessElephantPaths(1, 1) == 2);
    assert(chessElephantPaths(2, 2) == 6);
    assert(chessElephantPaths(3, 3) == 20);
    assert(chessElephantPaths(4, 4) == 70);
    assert(chessElephantPaths(10, 10) == 184756);
    // Large grid but still safe for long long
    assert(chessElephantPaths(12, 12) == 2704156);
    assert(chessElephantPaths(5, 2) == 21);
    return 0;
}
