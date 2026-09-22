// You are given an integer `n` and an `n x n` grid where each cell contains either a `*` (tree) or a `.` (empty). You must answer `q` rectangle queries. Each query provides coordinates `(r1, c1, r2, c2)` for the top-left and bottom-right corners of a rectangle, with both corners inclusive, using 1-based indexing. Write a standalone C++ function `vector<long long> countTreesInRectangles(int n, const vector<string>& grid, const vector<array<int,4>>& queries)` that, for each query, returns the total number of `*` symbols inside the specified rectangle. The grid is given as `n` strings of length `n`, each containing only `.` or `*`. The function must be efficient enough for `n` up to 1000 and `q` up to 100000 (with a grid that fits in memory). Return results in the same order as the queries. The function must not read from or write to standard input/output; all input comes through parameters and output is the returned vector.

The problem is a classic 2D prefix sum (or integrated sum) query. We build a prefix sum table `pre` of size `(n+1)x(n+1)` where `pre[i][j]` stores the number of `*` in the sub-rectangle from rows 1..i and columns 1..j using 1-based indexing. The recurrence is:  
`pre[i][j] = pre[i-1][j] + pre[i][j-1] - pre[i-1][j-1] + (grid[i-1][j-1] == '*')`.  
For a query with corners `(r1,c1)` and `(r2,c2)` (where `r1 <= r2`, `c1 <= c2`), the answer is:  
`pre[r2][c2] - pre[r1-1][c2] - pre[r2][c1-1] + pre[r1-1][c1-1]`.  
This uses inclusion-exclusion. Edge cases: when `r1=1` or `c1=1`, the subtraction terms become index 0 which is initialized to 0. complexity: building the prefix sum takes `O(n^2)` time and `O(n^2)` space (the prefix sum table). Each query is `O(1)`. Total time `O(n^2 + q)`, space `O(n^2)`.

#include <vector>
#include <string>
#include <array>

// Count trees in each query rectangle using 2D prefix sums.
// grid: n strings, each of length n, containing '.' or '*'
// queries: each is {r1, c1, r2, c2} with 1 <= r1 <= r2 <= n, 1 <= c1 <= c2 <= n
// returns vector of tree counts in the same order as queries.
std::vector<long long> countTreesInRectangles(
    int n,
    const std::vector<std::string>& grid,
    const std::vector<std::array<int,4>>& queries
) {
    // Build prefix sum table with dimensions (n+1) x (n+1)
    std::vector<std::vector<long long>> pre(n + 1, std::vector<long long>(n + 1, 0));
    
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            long long cell = (grid[i - 1][j - 1] == '*') ? 1 : 0;
            pre[i][j] = pre[i - 1][j] + pre[i][j - 1] - pre[i - 1][j - 1] + cell;
        }
    }
    
    std::vector<long long> results;
    results.reserve(queries.size());
    
    for (const auto& q : queries) {
        int r1 = q[0], c1 = q[1], r2 = q[2], c2 = q[3];
        long long sum = pre[r2][c2] 
                      - (r1 > 1 ? pre[r1 - 1][c2] : 0)
                      - (c1 > 1 ? pre[r2][c1 - 1] : 0)
                      + (r1 > 1 && c1 > 1 ? pre[r1 - 1][c1 - 1] : 0);
        results.push_back(sum);
    }
    
    return results;
}

#include <cassert>
#include <vector>
#include <string>
#include <array>

// include the solution function here (omitted for brevity in this test snippet, but assume it's above)

int main() {
    // Simple 1x1 grid
    std::vector<std::string> grid1 = {"*"};
    std::vector<std::array<int,4>> q1 = {{1,1,1,1}};
    assert(countTreesInRectangles(1, grid1, q1) == std::vector<long long>{1});

    // 2x2 grid with mixed cells
    std::vector<std::string> grid2 = {".*", "*."};
    std::vector<std::array<int,4>> q2 = {
        {1,1,2,2}, // all -> 2
        {1,2,2,2}, // right column -> 1
        {2,1,2,2}, // second row -> 1
        {1,1,1,1}, // top-left -> 0
        {2,2,2,2}  // bottom-right -> 0
    };
    auto res2 = countTreesInRectangles(2, grid2, q2);
    std::vector<long long> expected2 = {2,1,1,0,0};
    assert(res2 == expected2);

    // 3x3 grid with a diagonal of four stars
    std::vector<std::string> grid3 = {"*..", ".*.", "..*"};
    std::vector<std::array<int,4>> q3 = {
        {1,1,3,3}, // full -> 3
        {1,2,3,3}, // columns 2-3 rows 1-3 -> 2
        {2,2,2,2}, // center -> 1
        {1,1,2,2}, // top-left 2x2 -> 2
        {3,3,3,3}  // bottom-right -> 1
    };
    auto res3 = countTreesInRectangles(3, grid3, q3);
    std::vector<long long> expected3 = {3,2,1,2,1};
    assert(res3 == expected3);

    // Edge: entire grid empty
    std::vector<std::string> grid4 = {"...", "...", "..."};
    std::vector<std::array<int,4>> q4 = {{1,1,3,3}, {2,2,2,2}, {1,3,3,3}};
    auto res4 = countTreesInRectangles(3, grid4, q4);
    std::vector<long long> expected4 = {0, 0, 0};
    assert(res4 == expected4);

    // Edge: entire grid full of stars
    std::vector<std::string> grid5 = {"***", "***", "***"};
    std::vector<std::array<int,4>> q5 = {{1,1,3,3}, {2,2,2,2}, {1,2,3,3}, {2,1,3,2}};
    auto res5 = countTreesInRectangles(3, grid5, q5);
    std::vector<long long> expected5 = {9, 1, 6, 4};
    assert(res5 == expected5);

    // Larger test: 5x5 with a single star at (3,3)
    std::vector<std::string> grid6(5, ".....");
    grid6[2][2] = '*';
    std::vector<std::array<int,4>> q6 = {
        {1,1,5,5}, // full -> 1
        {1,1,3,3}, // includes star -> 1
        {4,4,5,5}, // bottom-right quadrant -> 0
        {3,3,3,3}, // exactly star -> 1
        {2,2,4,4}  // around star -> 1
    };
    auto res6 = countTreesInRectangles(5, grid6, q6);
    std::vector<long long> expected6 = {1,1,0,1,1};
    assert(res6 == expected6);

    return 0;
}
