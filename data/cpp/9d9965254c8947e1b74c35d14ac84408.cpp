/*
Write a C++ function `std::vector<int> diagonalTraversal(const std::vector<std::vector<int>>& matrix)` that returns the elements of a non-empty 2D matrix (where each row has the same length) in a zigzag diagonal order: starting from the top‑left element, traverse diagonals moving from bottom‑left to top‑right for diagonals with even indices (0‑based), and top‑right to bottom‑left for diagonals with odd indices. The first diagonal is just the top‑left element, the second diagonal goes down‑left from top‑right, and so on until the bottom‑right element. If the matrix is empty (0 rows), return an empty vector.
*/

#include <vector>
#include <algorithm>

std::vector<int> diagonalTraversal(const std::vector<std::vector<int>>& matrix) {
    if (matrix.empty()) return {};
    const int rows = matrix.size();
    const int cols = matrix[0].size();
    std::vector<int> result;
    result.reserve(rows * cols);

    for (int d = 0; d < rows + cols - 1; ++d) {
        std::vector<int> intermediate;
        int r = (d < cols) ? 0 : d - cols + 1;
        int c = (d < cols) ? d : cols - 1;

        while (r < rows && c >= 0) {
            intermediate.push_back(matrix[r][c]);
            ++r;
            --c;
        }

        if (d % 2 == 0) {
            std::reverse(intermediate.begin(), intermediate.end());
        }
        result.insert(result.end(), intermediate.begin(), intermediate.end());
    }

    return result;
}

#include <cassert>
#include <vector>

int main() {
    // 2x2 matrix
    std::vector<std::vector<int>> m1 = {{1, 2}, {3, 4}};
    assert(diagonalTraversal(m1) == std::vector<int>({1, 2, 3, 4}));

    // 3x3 matrix
    std::vector<std::vector<int>> m2 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    assert(diagonalTraversal(m2) == std::vector<int>({1, 2, 4, 7, 5, 3, 6, 8, 9}));

    // Single element
    std::vector<std::vector<int>> m3 = {{42}};
    assert(diagonalTraversal(m3) == std::vector<int>({42}));

    // Single row
    std::vector<std::vector<int>> m4 = {{1, 2, 3}};
    assert(diagonalTraversal(m4) == std::vector<int>({1, 2, 3}));

    // Single column
    std::vector<std::vector<int>> m5 = {{1}, {2}, {3}};
    assert(diagonalTraversal(m5) == std::vector<int>({1, 2, 3}));

    // Empty matrix
    std::vector<std::vector<int>> m6;
    assert(diagonalTraversal(m6) == std::vector<int>());

    // Rectangular 2x3
    std::vector<std::vector<int>> m7 = {{1, 2, 3}, {4, 5, 6}};
    assert(diagonalTraversal(m7) == std::vector<int>({1, 2, 4, 5, 3, 6}));

    // Rectangular 3x2
    std::vector<std::vector<int>> m8 = {{1, 2}, {3, 4}, {5, 6}};
    assert(diagonalTraversal(m8) == std::vector<int>({1, 2, 3, 5, 4, 6}));

    // 4x4 with distinct numbers
    std::vector<std::vector<int>> m9 = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
    assert(diagonalTraversal(m9) == std::vector<int>({1, 2, 5, 9, 6, 3, 4, 7, 10, 13, 14, 11, 8, 12, 15, 16}));
}

// The key is to process each anti‑diagonal (index d from 0 to rows+cols-2) in a consistent direction and then reverse every other one. For a given anti‑diagonal d, the starting row is `max(0, d - cols + 1)` and the starting column is `min(d, cols - 1)`. From there, moving down‑right (r++, c--) traverses all cells on that anti‑diagonal. Collect these cells into a temporary vector. If d is even, reverse that vector so the traversal goes upward (top‑right to bottom‑left); if d is odd, keep it as is (bottom‑left to top‑right). Append the temporary vector to the result. Edge cases: empty matrix returns empty; single‑row or single‑column matrices work because the loop bounds handle them; the start calculation ensures valid indices for all d. Time complexity is O(rows*cols) because each element is visited once. Space complexity is O(rows*cols) for the result, plus O(min(rows,cols)) for the temporary vector, which is the maximum diagonal length.
