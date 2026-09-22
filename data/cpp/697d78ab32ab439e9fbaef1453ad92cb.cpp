// Implement a C++ function `long double compute_determinant(const std::vector<std::vector<long double>>& matrix, bool& is_invertible)` that takes a square matrix (represented as a vector of vectors) and computes its determinant using Gaussian elimination with partial pivoting. The function should return the determinant value, and set `is_invertible` to `true` if the matrix is invertible (determinant is non-zero within a tolerance of `1e-9`), or `false` otherwise. The function must handle non-square input by throwing an `std::invalid_argument` exception. The result should match the expected mathematical determinant, including sign changes due to row swaps. Edge cases include zero matrices, identity matrices, matrices with repeated rows, and large matrices where floating-point precision matters.
// The solution uses Gaussian elimination with partial pivoting to avoid numerical instability. The algorithm iterates over each column (from 0 to n-1), selecting the row with the largest absolute value in the current column from the remaining rows (from i to n-1) as the pivot. If the pivot value is smaller than `1e-9` in absolute value, the determinant is zero and the matrix is not invertible (set `is_invertible` to false and return 0). If the pivot row differs from the current row i, swap them and negate the determinant (since swapping rows changes the sign). Multiply the determinant by the pivot value, then eliminate all entries below the pivot by subtracting a multiple of the pivot row from each lower row. The final determinant is the product of all pivots (with sign adjustments). For a non-square matrix, throw an exception. Time complexity is O(n³) for an n×n matrix, and space complexity is O(n²) due to the copy of the input matrix (necessary to avoid modifying the original). Edge cases: zero matrix → determinant 0, not invertible; identity matrix → determinant 1, invertible; singular matrices (e.g., repeated rows) → determinant 0, not invertible; tolerance of `1e-9` handles floating-point rounding.
#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>

/**
 * Computes the determinant of a square matrix using Gaussian elimination with partial pivoting.
 * @param matrix The input square matrix.
 * @param is_invertible Output parameter: set to true if the matrix is invertible (det != 0), false otherwise.
 * @return The determinant value.
 * @throws std::invalid_argument if the matrix is not square.
 */
long double compute_determinant(const std::vector<std::vector<long double>>& matrix, bool& is_invertible) {
    const size_t n = matrix.size();
    if (n == 0) {
        // Empty matrix: determinant is 1 by convention (empty product), but treat as invertible? 
        // For consistency, handle as special case: 1x1 with value 0? Let's just return 0 for empty? 
        // Better: if n==0, return 1 (empty product) and invertible=true? But typical matrices are non-empty.
        // For a 0x0 matrix, determinant is 1 (identity). We'll handle by treating as invertible.
        is_invertible = true;
        return 1.0;
    }
    
    // Check squareness
    for (const auto& row : matrix) {
        if (row.size() != n) {
            throw std::invalid_argument("Matrix must be square");
        }
    }
    
    // Copy matrix to avoid modifying the original
    std::vector<std::vector<long double>> temp = matrix;
    
    long double det = 1.0;
    const long double EPS = 1e-9;
    
    for (size_t i = 0; i < n; ++i) {
        // Partial pivoting: find row with max absolute value in this column
        size_t pivot_row = i;
        long double max_val = std::fabs(temp[i][i]);
        for (size_t j = i + 1; j < n; ++j) {
            if (std::fabs(temp[j][i]) > max_val) {
                max_val = std::fabs(temp[j][i]);
                pivot_row = j;
            }
        }
        
        // Check if pivot is zero (within tolerance)
        if (max_val < EPS) {
            is_invertible = false;
            return 0.0;
        }
        
        // Swap rows if needed
        if (pivot_row != i) {
            std::swap(temp[i], temp[pivot_row]);
            det = -det;
        }
        
        // Multiply determinant by pivot value
        det *= temp[i][i];
        
        // Eliminate entries below the pivot
        for (size_t j = i + 1; j < n; ++j) {
            long double factor = temp[j][i] / temp[i][i];
            for (size_t k = i; k < n; ++k) {
                temp[j][k] -= factor * temp[i][k];
            }
        }
    }
    
    is_invertible = (std::fabs(det) > EPS);
    return det;
}
#include <cassert>
#include <cmath>
#include <vector>
#include <stdexcept>

// Include the solution function here (copy from above)

int main() {
    // Test 1: 2x2 matrix with known determinant
    std::vector<std::vector<long double>> m1 = {{4.0, 7.0}, {2.0, 6.0}};
    bool invertible1 = false;
    long double det1 = compute_determinant(m1, invertible1);
    assert(std::fabs(det1 - 10.0) < 1e-9);
    assert(invertible1 == true);

    // Test 2: Singular matrix (rows proportional)
    std::vector<std::vector<long double>> m2 = {{1.0, 2.0}, {2.0, 4.0}};
    bool invertible2 = false;
    long double det2 = compute_determinant(m2, invertible2);
    assert(std::fabs(det2) < 1e-9);
    assert(invertible2 == false);

    // Test 3: Identity matrix
    std::vector<std::vector<long double>> m3 = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
    bool invertible3 = false;
    long double det3 = compute_determinant(m3, invertible3);
    assert(std::fabs(det3 - 1.0) < 1e-9);
    assert(invertible3 == true);

    // Test 4: 3x3 matrix with row swap needed (det -1)
    std::vector<std::vector<long double>> m4 = {{0.0, 1.0, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}};
    bool invertible4 = false;
    long double det4 = compute_determinant(m4, invertible4);
    assert(std::fabs(det4 - (-1.0)) < 1e-9);
    assert(invertible4 == true);

    // Test 5: Non-square throws exception
    std::vector<std::vector<long double>> m5 = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};
    bool invertible5 = false;
    bool threw = false;
    try {
        compute_determinant(m5, invertible5);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test 6: 1x1 matrix
    std::vector<std::vector<long double>> m6 = {{7.5}};
    bool invertible6 = false;
    long double det6 = compute_determinant(m6, invertible6);
    assert(std::fabs(det6 - 7.5) < 1e-9);
    assert(invertible6 == true);

    // Test 7: 1x1 singular
    std::vector<std::vector<long double>> m7 = {{0.0}};
    bool invertible7 = false;
    long double det7 = compute_determinant(m7, invertible7);
    assert(std::fabs(det7) < 1e-9);
    assert(invertible7 == false);

    // Test 8: Zero matrix 3x3
    std::vector<std::vector<long double>> m8 = {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};
    bool invertible8 = false;
    long double det8 = compute_determinant(m8, invertible8);
    assert(std::fabs(det8) < 1e-9);
    assert(invertible8 == false);

    // Test 9: Matrix with negative determinant
    std::vector<std::vector<long double>> m9 = {{1.0, 2.0}, {3.0, 4.0}};
    bool invertible9 = false;
    long double det9 = compute_determinant(m9, invertible9);
    assert(std::fabs(det9 - (-2.0)) < 1e-9);
    assert(invertible9 == true);

    // Test 10: Larger 4x4 matrix (det = 1) known
    std::vector<std::vector<long double>> m10 = {
        {1.0, 0.0, 0.0, 0.0},
        {2.0, 1.0, 0.0, 0.0},
        {3.0, 4.0, 1.0, 0.0},
        {5.0, 6.0, 7.0, 1.0}
    };
    bool invertible10 = false;
    long double det10 = compute_determinant(m10, invertible10);
    assert(std::fabs(det10 - 1.0) < 1e-9);
    assert(invertible10 == true);

    return 0;
}
