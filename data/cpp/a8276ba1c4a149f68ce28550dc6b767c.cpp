Write a C++ function `bool canTransformByRotation(const std::vector<std::vector<int>>& matrix, const std::vector<std::vector<int>>& target)` that determines whether the square matrix `matrix` can be transformed into `target` by rotating `matrix` in the plane by 0, 90, 180, or 270 degrees clockwise. The input matrices are guaranteed to be square (same number of rows and columns) and non-empty. The function should return `true` if any rotation matches `target`, and `false` otherwise. Do not modify the original input matrices; the function must treat them as read-only. You may assume all elements are integers.

The core idea is to generate each of the three nontrivial rotations (90°, 180°, 270°) by repeatedly applying a 90° clockwise rotation to a local copy of the matrix, and checking equality with the target after each step. A 90° clockwise rotation can be achieved in-place by first transposing the matrix (swap elements symmetric across the main diagonal, `matrix[i][j]` with `matrix[j][i]` for `i > j`) and then reversing each row. Applying this operation once gives 90°, twice gives 180°, three times gives 270°. The original orientation is also checked first. Since the matrices are square, the operation is valid. Edge cases include a 1×1 matrix (any rotation yields the same single element) and matrices with duplicate values (comparison is element‑wise, so duplicates are fine). Time complexity: Each rotation is O(n²) for an n×n matrix, and we do at most 4 rotations, so total O(n²). Space complexity: We make a copy of the input `matrix` to avoid modifying the original, so O(n²) auxiliary space.

#include <vector>
#include <algorithm>

// Checks if target can be obtained by rotating matrix by 0, 90, 180, or 270 degrees clockwise.
bool canTransformByRotation(const std::vector<std::vector<int>>& matrix,
                            const std::vector<std::vector<int>>& target) {
    if (matrix.size() != target.size() || matrix[0].size() != target[0].size()) {
        return false;
    }

    // Work on a copy to keep the original unchanged
    std::vector<std::vector<int>> rotated = matrix;

    // Check 0 degrees
    if (rotated == target) {
        return true;
    }

    // Rotate 90, 180, and 270 degrees
    for (int rotation = 0; rotation < 3; ++rotation) {
        int n = rotated.size();
        // Transpose
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                std::swap(rotated[i][j], rotated[j][i]);
            }
        }
        // Reverse each row
        for (int i = 0; i < n; ++i) {
            std::reverse(rotated[i].begin(), rotated[i].end());
        }

        if (rotated == target) {
            return true;
        }
    }

    return false;
}

#include <cassert>
#include <vector>

int main() {
    using Matrix = std::vector<std::vector<int>>;

    // Example 1: Original matches
    Matrix m1 = {{1, 2}, {3, 4}};
    Matrix t1 = {{1, 2}, {3, 4}};
    assert(canTransformByRotation(m1, t1) == true);

    // Example 2: 90° clockwise rotation
    Matrix m2 = {{1, 2}, {3, 4}};
    Matrix t2 = {{3, 1}, {4, 2}};
    assert(canTransformByRotation(m2, t2) == true);

    // Example 3: 180° rotation
    Matrix m3 = {{1, 2}, {3, 4}};
    Matrix t3 = {{4, 3}, {2, 1}};
    assert(canTransformByRotation(m3, t3) == true);

    // Example 4: 270° rotation
    Matrix m4 = {{1, 2}, {3, 4}};
    Matrix t4 = {{2, 4}, {1, 3}};
    assert(canTransformByRotation(m4, t4) == true);

    // Example 5: Not a rotation
    Matrix m5 = {{1, 2}, {3, 4}};
    Matrix t5 = {{1, 3}, {2, 4}};
    assert(canTransformByRotation(m5, t5) == false);

    // Example 6: 1×1 matrix
    Matrix m6 = {{7}};
    Matrix t6 = {{7}};
    assert(canTransformByRotation(m6, t6) == true);

    // Example 7: All same elements, any rotation matches
    Matrix m7 = {{5, 5}, {5, 5}};
    Matrix t7 = {{5, 5}, {5, 5}};
    assert(canTransformByRotation(m7, t7) == true);

    // Example 8: Larger matrix, 90° rotation
    Matrix m8 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    Matrix t8 = {{7, 4, 1}, {8, 5, 2}, {9, 6, 3}};
    assert(canTransformByRotation(m8, t8) == true);

    // Example 9: Larger matrix, 180° rotation
    Matrix m9 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    Matrix t9 = {{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};
    assert(canTransformByRotation(m9, t9) == true);

    // Example 10: Empty internally? Should not happen per spec, but test for robustness
    Matrix m10 = {{}};
    Matrix t10 = {{}};
    assert(canTransformByRotation(m10, t10) == true);
}
