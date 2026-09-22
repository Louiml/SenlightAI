// Write a C++ function that takes a positive integer `n` and returns an `n x n` matrix (as a `std::vector<std::vector<int>>`) where the numbers `1` through `n*(n-1)/2 + n` are placed according to the following scheme: First, for every pair `(i, j)` with `0 ≤ i < j < n`, assign the next consecutive integer to both `matrix[i][someAvailableColumn]` and `matrix[j][someAvailableColumn]`, where each row fills its available columns from left to right (skipping the last column). After all pairs are processed, fill the last column of every row with the next consecutive integers, one per row from top to bottom. The function must handle all positive `n`, including `n = 1` where the matrix is just `{{1}}`. You are not allowed to use global variables; everything must be computed inside the function and returned.
// The problem is a direct simulation of the given code. The key idea is to maintain for each row a pointer (or index) indicating the next column to write into, initially 0 for all rows. We iterate over all pairs `i < j` in a nested loop. For each pair, we take the current value `x` (starting at 1), assign it to `matrix[i][pos[i]]` and `matrix[j][pos[j]]`, then increment both `pos[i]` and `pos[j]` and also increment `x`. This ensures each row receives exactly `n-1` numbers from the pair phase (since each row participates in `n-1` pairs), leaving the last column (index `n-1`) empty. After the pairs are done, we fill the last column of each row from top to bottom with the next `n` consecutive numbers. The total number of values is `n*(n-1)/2 + n`, which matches the size of the matrix. Edge case: when `n = 1`, the pair loop does nothing, and the last-column loop assigns `x = 1` to `matrix[0][0]`, producing `{{1}}`. Time complexity is `O(n^2)` because there are `n*(n-1)/2` pairs and each row fill is `O(1)`, plus `O(n)` for the last column. Space complexity is `O(n^2)` for the returned matrix (plus `O(n)` for the position array), which is necessary for the output.
#include <vector>

// Builds the n x n matrix with the described number placement scheme.
std::vector<std::vector<int>> buildNumberMatrix(int n) {
    // Initialize an n x n matrix filled with zeros.
    std::vector<std::vector<int>> matrix(n, std::vector<int>(n, 0));
    // Tracks the next available column index for each row.
    std::vector<int> nextCol(n, 0);
    int current = 1;

    // Place numbers for every unordered pair (i, j) with i < j.
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            matrix[i][nextCol[i]++] = current;
            matrix[j][nextCol[j]++] = current;
            ++current;
        }
    }

    // Fill the last column of each row.
    for (int i = 0; i < n; ++i) {
        matrix[i][n - 1] = current++;
    }

    return matrix;
}
#include <cassert>
#include <vector>

// Include the solution function definition here or link it.

int main() {
    // Basic test for n = 1.
    {
        auto m = buildNumberMatrix(1);
        assert(m.size() == 1 && m[0].size() == 1);
        assert(m[0][0] == 1);
    }
    // Test for n = 2.
    {
        auto m = buildNumberMatrix(2);
        // Pairs: (0,1) gets 1. Then last column: row0 gets 2, row1 gets 3.
        assert(m[0][0] == 1 && m[1][0] == 1);
        assert(m[0][1] == 2 && m[1][1] == 3);
    }
    // Test for n = 3.
    {
        auto m = buildNumberMatrix(3);
        // Pairs: (0,1)->1, (0,2)->2, (1,2)->3.
        // Row0: col0=1, col1=2; Row1: col0=1, col1=3; Row2: col0=2, col1=3.
        // Last column: row0=4, row1=5, row2=6.
        assert(m[0][0] == 1 && m[0][1] == 2 && m[0][2] == 4);
        assert(m[1][0] == 1 && m[1][1] == 3 && m[1][2] == 5);
        assert(m[2][0] == 2 && m[2][1] == 3 && m[2][2] == 6);
    }
    // Verify that all numbers from 1 to n*(n-1)/2 + n appear exactly once for n=4.
    {
        int n = 4;
        auto m = buildNumberMatrix(n);
        std::vector<bool> seen(n * (n - 1) / 2 + n + 1, false);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                assert(!seen[m[i][j]]);
                seen[m[i][j]] = true;
            }
        }
        for (int v = 1; v <= n * (n - 1) / 2 + n; ++v) {
            assert(seen[v]);
        }
    }
    // Diagonal sanity: for any i, the value matrix[i][i] is not set by the pair phase (only upper/lower), but last column fills all rows. Check row length consistency.
    {
        int n = 5;
        auto m = buildNumberMatrix(5);
        for (int i = 0; i < n; ++i) {
            assert(m[i].size() == static_cast<size_t>(n));
        }
    }
}
