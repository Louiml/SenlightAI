/*
Write a standalone C++ function that, given a 2D integer matrix represented as a `std::vector<std::vector<int>>` and a target integer value, returns a `std::pair<int,int>` representing the row and column indices of the **first** occurrence of that value when scanning the matrix in row-major order (row by row from top to bottom, and within each row from left to right). If the value is not found, return `{-1,-1}`. The function must accept the matrix by `const` reference and handle empty matrices or rows. You may assume all rows have the same length, but the matrix itself may be empty (0 rows) or contain empty rows (0 columns).
*/
#include <vector>
#include <utility>

// Return the row and column of the first occurrence of target in row-major order.
// If target is not found, return {-1, -1}.
std::pair<int,int> findFirstOccurrence(const std::vector<std::vector<int>>& matrix, int target) {
    for (int i = 0; i < static_cast<int>(matrix.size()); ++i) {
        for (int j = 0; j < static_cast<int>(matrix[i].size()); ++j) {
            if (matrix[i][j] == target) {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}
#include <cassert>
#include <vector>
#include <utility>

// (Solution function is assumed to be declared above)

int main() {
    // Basic case with first occurrence in the middle.
    std::vector<std::vector<int>> m1 = {{1, 2}, {3, 4}};
    assert(findFirstOccurrence(m1, 3) == std::make_pair(1, 0));

    // Value appears multiple times; first in row-major order is (0,1).
    std::vector<std::vector<int>> m2 = {{5, 7}, {7, 8}};
    assert(findFirstOccurrence(m2, 7) == std::make_pair(0, 1));

    // Value not present.
    assert(findFirstOccurrence(m2, 9) == std::make_pair(-1, -1));

    // Single element matrix.
    std::vector<std::vector<int>> m3 = {{42}};
    assert(findFirstOccurrence(m3, 42) == std::make_pair(0, 0));

    // Empty matrix (no rows).
    std::vector<std::vector<int>> m4;
    assert(findFirstOccurrence(m4, 1) == std::make_pair(-1, -1));

    // Matrix with empty rows.
    std::vector<std::vector<int>> m5 = {{}, {}};
    assert(findFirstOccurrence(m5, 1) == std::make_pair(-1, -1));

    // Larger matrix with target at last position.
    std::vector<std::vector<int>> m6 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    assert(findFirstOccurrence(m6, 9) == std::make_pair(2, 2));

    // Negative and zero values.
    std::vector<std::vector<int>> m7 = {{-1, -2}, {0, -3}};
    assert(findFirstOccurrence(m7, -3) == std::make_pair(1, 1));
    assert(findFirstOccurrence(m7, 0) == std::make_pair(1, 0));

    return 0;
}
// The solution iterates through the matrix row by row (outer loop over rows, inner loop over columns). For each element, it checks if it equals the target. The first match found is immediately returned as a `std::pair<int,int>` with the current row and column indices, because the nested loop order naturally respects row-major scanning. Edge cases: if the matrix is empty (`m == 0` or `n == 0`), the loops do not execute, so the function returns `{-1,-1}`. If the target appears multiple times, the first match in scan order is returned, as intended. The algorithm runs in \(O(m \times n)\) time in the worst case (when the value is absent or appears at the last position) and uses \(O(1)\) auxiliary space, because only constant storage is used for the indices. No modifications are made to the input, hence `const` correctness is applied.
