Write a C++ function `reconstructSymmetricMatrix` that takes a 3-element array of eigenvalues and a 3×3 matrix whose columns are orthonormal eigenvectors, and returns the reconstructed symmetric 3×3 matrix (often this is the original matrix from the eigendecomposition, computed as \( A = E \cdot \Lambda \cdot E^T \)). The function must compute the transpose of the eigenvector matrix, perform the matrix multiplication \( E \cdot \Lambda \cdot E^T \) using the eigenvalue-weighted outer-product form, and return the resulting matrix as a 2D `std::array` or a `double[3][3]` via `std::array<std::array<double,3>,3>`. Assume the input eigenvectors are already normalized (orthonormal), so no validation or normalization is needed. The function must be `const`-correct for its inputs and use no global or static variables. Edge cases: the matrix may contain negative eigenvalues or zero eigenvalues; multiplication must be done in double precision and the output may have tiny floating-point rounding errors, but no special handling is required.

// The core algorithm is the spectral decomposition: given eigenvalues \(\lambda_i\) and eigenvectors \(v_i\) (columns of \(E\)), the reconstructed matrix is computed as \(A = \sum_{i=1}^3 \lambda_i v_i v_i^T\). Equivalently, compute the transpose \(E^T\) first, then perform three nested loops: for each eigenvalue index \(i\), for each row \(j\), for each column \(k\), add \(\lambda_i \cdot E[j][i] \cdot E^T[i][k]\) to the result. This is essentially the block multiplication \(E \cdot \Lambda \cdot E^T\) without explicitly forming \(\Lambda\) as a 3×3 matrix. Important edge cases: if any eigenvalue is zero, that term contributes nothing; if all eigenvalues are zero, the result is the zero matrix. The time complexity is \(O(3^3) = O(1)\) since dimensions are fixed at 3, and auxiliary space is \(O(1)\) for the result matrix and a temporary transpose matrix (both 3×3). The approach is straightforward and requires no special handling for negative eigenvalues because floating-point arithmetic handles them naturally.

#include <array>
#include <cmath>

// Reconstruct symmetric matrix A = E * diag(eigenvalues) * E^T
// eigenValues: array of 3 eigenvalues
// eigenvectors: 3x3 matrix where columns are orthonormal eigenvectors
// Returns the reconstructed 3x3 symmetric matrix as std::array<std::array<double,3>,3>
std::array<std::array<double,3>,3> reconstructSymmetricMatrix(
    const std::array<double,3>& eigenValues,
    const std::array<std::array<double,3>,3>& eigenvectors)
{
    // Compute transpose of eigenvectors
    std::array<std::array<double,3>,3> eigenvectorsTranspose{};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            eigenvectorsTranspose[i][j] = eigenvectors[j][i];
        }
    }
    
    // Initialize result matrix to zero
    std::array<std::array<double,3>,3> result{};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result[i][j] = 0.0;
        }
    }
    
    // Compute A = sum_i eigenvalue_i * column_i * row_i
    for (int i = 0; i < 3; ++i) {
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) {
                result[row][col] += eigenValues[i] * eigenvectors[row][i] * eigenvectorsTranspose[i][col];
            }
        }
    }
    
    return result;
}

#include <cassert>
#include <cmath>
#include <array>

// The solution function is assumed to be defined above.
// For testing we re-declare it here to be self-contained.
// In a real project, include the header or place the function in the same file.
std::array<std::array<double,3>,3> reconstructSymmetricMatrix(
    const std::array<double,3>& eigenValues,
    const std::array<std::array<double,3>,3>& eigenvectors);

int main() {
    // Test case 1: Given example from the prompt
    std::array<double,3> ev1 = {9.0, 9.0, 18.0};
    std::array<std::array<double,3>,3> vec1 = {{
        {1.0/std::sqrt(2.0), 1.0/std::sqrt(18.0), 2.0/3.0},
        {1.0/std::sqrt(2.0), -1.0/std::sqrt(18.0), -2.0/3.0},
        {0.0, -4.0/std::sqrt(18.0), 1.0/3.0}
    }};
    auto result1 = reconstructSymmetricMatrix(ev1, vec1);
    // Expected matrix (the one from the prompt's output)
    // [9, 0, 0]  [0, 9, 0]  [0, 0, 18]? Actually the prompt computes something else.
    // The correct reconstruction should give the identity-like? Let's manually check: For ev = {9,9,18} and orthonormal columns, the result should be diag? 
    // Actually the matrix A = E*diag*E^T will be symmetric. We'll just verify that the diagonal is [9,9,18]? No. Let's compute expected: 
    // Use a known property: trace(A) = sum eigenvalues = 36. 
    // We can test symmetry: result[i][j] ≈ result[j][i]. 
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            assert(std::fabs(result1[i][j] - result1[j][i]) < 1e-12);
        }
    }
    // Check trace
    assert(std::fabs(result1[0][0] + result1[1][1] + result1[2][2] - 36.0) < 1e-9);

    // Test case 2: Identity eigenvectors and eigenvalues = {1,2,3}
    std::array<double,3> ev2 = {1.0, 2.0, 3.0};
    std::array<std::array<double,3>,3> vec2 = {{
        {1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}
    }};
    auto result2 = reconstructSymmetricMatrix(ev2, vec2);
    // Should be diag(1,2,3)
    assert(std::fabs(result2[0][0] - 1.0) < 1e-12);
    assert(std::fabs(result2[1][1] - 2.0) < 1e-12);
    assert(std::fabs(result2[2][2] - 3.0) < 1e-12);
    assert(std::fabs(result2[0][1]) < 1e-12);
    assert(std::fabs(result2[1][0]) < 1e-12);
    assert(std::fabs(result2[0][2]) < 1e-12);
    assert(std::fabs(result2[2][0]) < 1e-12);
    assert(std::fabs(result2[1][2]) < 1e-12);
    assert(std::fabs(result2[2][1]) < 1e-12);

    // Test case 3: All zero eigenvalues -> zero matrix
    std::array<double,3> ev3 = {0.0, 0.0, 0.0};
    std::array<std::array<double,3>,3> vec3 = vec1; // any orthonormal vectors
    auto result3 = reconstructSymmetricMatrix(ev3, vec3);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            assert(std::fabs(result3[i][j]) < 1e-12);
        }
    }

    // Test case 4: Negative eigenvalue
    std::array<double,3> ev4 = {-5.0, 0.0, 0.0};
    std::array<std::array<double,3>,3> vec4 = {{
        {1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}
    }};
    auto result4 = reconstructSymmetricMatrix(ev4, vec4);
    assert(std::fabs(result4[0][0] + 5.0) < 1e-12);
    assert(std::fabs(result4[1][1]) < 1e-12);
    assert(std::fabs(result4[2][2]) < 1e-12);

    return 0;
}
