// Write a C++ function `bool isInvertible(const std::vector<std::vector<float>>& matrix)` that determines whether a given square matrix (2x2, 3x3, or 4x4) is invertible. A matrix is invertible if and only if its determinant is non-zero. The function should handle any square matrix dimension up to 4x4, but you may assume the input is always a non-empty square matrix (rows == columns, and dimensions between 2 and 4 inclusive). The determinant should be computed using a general recursive approach using cofactor expansion along the first row, with an epsilon tolerance of `1e-5` for floating-point comparisons. The function must not modify the input matrix and must use `const` correctly. Edge cases: matrices that are nearly singular (determinant close to zero but not exactly zero) should be treated as non-invertible if the absolute value of the determinant is less than or equal to the tolerance; the function should return `false` in that case.

The solution uses a recursive cofactor expansion to compute the determinant of any NxN matrix. The base case is a 1x1 matrix, where the determinant is the single element. For larger matrices, we iterate over the first row, compute the cofactor for each element by creating a submatrix that excludes the current row and column, recursively compute that submatrix's determinant, and sum `element * sign * subdet`, where the sign alternates +, -, +, ... The recursion depth is at most 4 (for a 4x4 input), so time complexity is O(N!) because each cofactor expansion for an NxN matrix generates N subproblems of size (N-1)x(N-1). For N=4 this is 4! = 24 operations, which is trivial. Space complexity is O(N^2) due to creating submatrices at each recursion level, but again the maximum is tiny (4x4). The main edge case is the floating-point tolerance: after computing the determinant, we compare its absolute value to `1e-5`. This handles near-zero determinants that might occur due to rounding, ensuring the matrix is only considered invertible when the determinant is clearly non-zero. The function does not mutate the input; submatrices are created as new vectors.

#include <vector>
#include <cmath>

// Recursively compute the determinant of a square matrix.
// matrix: non-empty square matrix (size >= 1).
// Returns the determinant as a float.
float determinant(const std::vector<std::vector<float>>& matrix) {
    int n = matrix.size();
    if (n == 1) {
        return matrix[0][0];
    }
    if (n == 2) {
        // Direct formula for 2x2 to avoid extra recursion depth.
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }

    float det = 0.0f;
    for (int col = 0; col < n; ++col) {
        // Build submatrix excluding row 0 and current column.
        std::vector<std::vector<float>> sub(n - 1, std::vector<float>(n - 1));
        for (int i = 1; i < n; ++i) {
            int subCol = 0;
            for (int j = 0; j < n; ++j) {
                if (j == col) continue;
                sub[i - 1][subCol++] = matrix[i][j];
            }
        }
        double sign = (col % 2 == 0) ? 1.0 : -1.0;
        det += sign * matrix[0][col] * determinant(sub);
    }
    return det;
}

// Check if a square matrix (2x2, 3x3, or 4x4) is invertible.
// Returns true if the absolute value of the determinant exceeds 1e-5.
bool isInvertible(const std::vector<std::vector<float>>& matrix) {
    int n = matrix.size();
    if (n < 2 || n > 4) {
        // The task only requires supports for 2x2 to 4x4.
        // Return false for unsupported dimensions.
        return false;
    }
    for (const auto& row : matrix) {
        if (row.size() != static_cast<size_t>(n)) {
            return false; // Not a square matrix.
        }
    }
    float det = determinant(matrix);
    return std::fabs(det) > 1e-5f;
}

#include <cassert>
#include <vector>
#include <cmath>

// The solution function is declared above. Include its definition or link appropriately.

int main() {
    // 2x2: determinant = 1*4 - 2*3 = -2, invertible
    std::vector<std::vector<float>> m2 = {{1, 2}, {3, 4}};
    assert(isInvertible(m2) == true);

    // 2x2: determinant = 2*4 - 4*2 = 0, singular
    std::vector<std::vector<float>> m2_sing = {{2, 4}, {4, 8}};
    assert(isInvertible(m2_sing) == false);

    // 3x3: determinant = 1*(5*9-6*8) - 2*(4*9-6*7) + 3*(4*8-5*7) = 1*(-3) - 2*(-6) + 3*(-3) = 0, singular
    std::vector<std::vector<float>> m3_sing = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    assert(isInvertible(m3_sing) == false);

    // 3x3: determinant = 1*(1*1 - 0*0) - 0 + 0 = 1, invertible
    std::vector<std::vector<float>> m3 = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    assert(isInvertible(m3) == true);

    // 4x4 identity: determinant = 1, invertible
    std::vector<std::vector<float>> m4_id = {{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1}};
    assert(isInvertible(m4_id) == true);

    // 4x4 with zero determinant (all rows proportional)
    std::vector<std::vector<float>> m4_sing = {{1,2,3,4}, {2,4,6,8}, {3,6,9,12}, {4,8,12,16}};
    assert(isInvertible(m4_sing) == false);

    // 4x4 nearly singular determinant = 1e-6, below tolerance, should be false
    std::vector<std::vector<float>> m4_near = {{1e-6f, 0,0,0}, {0,1e-6f,0,0}, {0,0,1e-6f,0}, {0,0,0,1e-6f}};
    // determinant = (1e-6)^4 = 1e-24, much less than 1e-5
    assert(isInvertible(m4_near) == false);

    // A non-square input (e.g., 3x2) should return false
    std::vector<std::vector<float>> m_rect = {{1,2}, {3,4}, {5,6}};
    assert(isInvertible(m_rect) == false);
    
    return 0;
}
