// Write a C++ function `std::vector<std::vector<int>> buildDistinctNeighborGrid(int n, int m)` that, given positive integers `n` and `m`, returns an `n`×`m` grid containing the numbers from 1 to `n*m` exactly once, such that no two vertically or horizontally adjacent cells contain numbers that differ by exactly 1. If such a grid is impossible, return an empty vector. The function must handle either orientation: for instance, if `n > m`, you may internally transpose the problem and then transpose the final grid back to respect the original dimensions. The numbers must be placed in a deterministic pattern that guarantees the adjacency property. For efficiency, the solution should run in O(n*m) time and O(n*m) space (for the output). Input constraints: 1 ≤ n,m ≤ 100. The function must be const-correct and use only standard library facilities.
The algorithm directly implements the described pattern. We use a 1-indexed linear array `lin` of size `n*m+1` to mimic the original snippet's macro `X(i,j) = (i-1)*m+j`. The filling step iterates through all even numbers from 2 to `total` and assigns them to the first `total/2` cell indices (in ascending order), then assigns all odd numbers to the remaining indices. After this, we check every adjacent pair (right and down) for an absolute difference of 1; if any such pair exists, we return an empty vector. The check requires visiting each of the `n*m` cells and examining its right and down neighbors, which is O(n*m) time. The linear array stores O(n*m) integers, and the returned 2D vector also uses O(n*m) space. For the transposed case (`n > m`), we recursively construct the `m`×`n` version; if it fails, we propagate the empty result; otherwise we transpose each element to produce the `n`×`m` output, which also takes O(n*m) time and space. Edge cases include tiny grids like 1×2 or 2×2, where the pattern inevitably places consecutive numbers next to each other, resulting in an empty return.
#include <vector>
#include <cstdlib>

// Helper: construct an oriented rows x cols grid using the even-first pattern.
// Returns an empty vector if the pattern violates the adjacency rule.
static std::vector<std::vector<int>> constructOriented(int rows, int cols) {
    int total = rows * cols;
    std::vector<int> lin(total + 1, 0); // 1-indexed linear storage

    int next = 1;
    // Place even numbers first.
    for (int v = 2; v <= total; v += 2) {
        lin[v] = next++;
    }
    // Place odd numbers next.
    for (int v = 1; v <= total; v += 2) {
        lin[v] = next++;
    }

    // Lambda to access value at row r, col c (0-based) from 1-indexed lin.
    auto at = [&](int r, int c) -> int {
        return lin[r * cols + c + 1];
    };

    // Check all right and down neighbours.
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int cur = at(r, c);
            if (r + 1 < rows) {
                int down = at(r + 1, c);
                if (std::abs(cur - down) == 1) return {};
            }
            if (c + 1 < cols) {
                int right = at(r, c + 1);
                if (std::abs(cur - right) == 1) return {};
            }
        }
    }

    // Build the 2D grid.
    std::vector<std::vector<int>> grid(rows, std::vector<int>(cols));
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            grid[r][c] = at(r, c);
        }
    }
    return grid;
}

// Public function: returns the constructed n x m grid, or empty if invalid.
std::vector<std::vector<int>> buildGrid(int n, int m) {
    if (n <= m) {
        return constructOriented(n, m);
    } else {
        auto base = constructOriented(m, n);
        if (base.empty()) return {};
        // Transpose m x n to n x m.
        std::vector<std::vector<int>> result(n, std::vector<int>(m));
        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < m; ++c) {
                result[r][c] = base[c][r];
            }
        }
        return result;
    }
}
#include <cassert>
#include <vector>

// buildGrid is assumed to be defined as above.

int main() {
    // 1x1 – single cell
    auto g1 = buildGrid(1, 1);
    assert(g1.size() == 1 && g1[0].size() == 1 && g1[0][0] == 1);

    // 1x2 – impossible pattern
    assert(buildGrid(1, 2).empty());

    // 1x3 – impossible pattern
    assert(buildGrid(1, 3).empty());

    // 1x4 – valid pattern
    auto g4 = buildGrid(1, 4);
    assert(!g4.empty());
    assert(g4[0] == std::vector<int>({2, 4, 1, 3}));

    // 2x2 – impossible pattern
    assert(buildGrid(2, 2).empty());

    // 2x3 – pattern yields a violation (vertical pair 2 and 1)
    assert(buildGrid(2, 3).empty());

    // 3x2 – swap and pattern also invalid
    assert(buildGrid(3, 2).empty());

    // 1x5 – valid pattern
    auto g5 = buildGrid(1, 5);
    assert(!g5.empty());
    assert(g5[0] == std::vector<int>({3, 1, 4, 2, 5}));

    // 2x4 – valid pattern
    auto g24 = buildGrid(2, 4);
    assert(!g24.empty());
    assert(g24[0] == std::vector<int>({5, 1, 6, 2}));
    assert(g24[1] == std::vector<int>({7, 3, 8, 4}));
}
