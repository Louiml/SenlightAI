Write a C++ function that, given a square matrix represented as a `std::vector<std::vector<double>>` (with equal row and column sizes, and guaranteed non-empty), computes the operator norm (the largest singular value, also known as the spectral norm) of the matrix. The input matrix is symmetric (the function must assume and exploit this property), and you should compute the norm efficiently using only the lower triangular part of the matrix. Return the result as a `double`. Your solution must not rely on external linear algebra libraries; implement the computation using standard C++ only.

The operator norm of a symmetric matrix equals the largest absolute eigenvalue (since for a real symmetric matrix, singular values coincide with absolute eigenvalues). To find the largest absolute eigenvalue, we can use the power iteration method: start with a random non-zero vector, repeatedly multiply it by the matrix and normalize, and the scaling factor (the Rayleigh quotient) converges to the dominant eigenvalue. Since the matrix is symmetric, the power iteration converges to the largest (in magnitude) eigenvalue, and taking the absolute value gives the operator norm.  
We exploit symmetry by only reading the lower triangular part: for row `i` and column `j`, if `i >= j`, use `matrix[i][j]`, else use `matrix[j][i]`. Edge cases: if the matrix is all zeros, the norm is 0; power iteration would produce a zero vector, so we must handle that directly (return 0.0). Also, to avoid division by zero during normalization, always check the norm of the current vector. The number of iterations is fixed (e.g., 100) to guarantee convergence for typical cases; the initial vector is set to all ones for determinism. Time complexity is `O(iterations * n^2)`, where `n` is the matrix dimension; for fixed `iterations`, it is `O(n^2)`. Space complexity is `O(n)` for the vector.  
For the reference solution, we implement a free function `double operatorNorm(const std::vector<std::vector<double>>& matrix)` that uses power iteration and returns the norm.

#include <vector>
#include <cmath>
#include <cstddef>

// Compute the operator norm (largest singular value) of a symmetric matrix.
// Assumes the matrix is symmetric and uses only the lower triangular part.
// Uses power iteration to find the largest absolute eigenvalue.
double operatorNorm(const std::vector<std::vector<double>>& matrix) {
    const std::size_t n = matrix.size();
    if (n == 0) return 0.0;

    // Check for all-zero matrix to handle edge case
    bool allZero = true;
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j <= i; ++j) {
            if (matrix[i][j] != 0.0) {
                allZero = false;
                break;
            }
        }
    }
    if (allZero) return 0.0;

    // Initialize vector with all ones (deterministic)
    std::vector<double> vec(n, 1.0);

    const int iterations = 100;
    for (int iter = 0; iter < iterations; ++iter) {
        // Multiply vector by matrix (using symmetry)
        std::vector<double> result(n, 0.0);
        for (std::size_t i = 0; i < n; ++i) {
            double sum = 0.0;
            for (std::size_t j = 0; j < n; ++j) {
                // Access symmetric element via lower triangle
                double val = (i >= j) ? matrix[i][j] : matrix[j][i];
                sum += val * vec[j];
            }
            result[i] = sum;
        }

        // Compute norm of result
        double norm = 0.0;
        for (std::size_t i = 0; i < n; ++i) {
            norm += result[i] * result[i];
        }
        norm = std::sqrt(norm);

        // If norm is zero, matrix is singular with zero dominant eigenvalue
        if (norm < 1e-12) return 0.0;

        // Normalize the vector for next iteration
        for (std::size_t i = 0; i < n; ++i) {
            vec[i] = result[i] / norm;
        }
    }

    // Compute Rayleigh quotient to get eigenvalue estimate
    std::vector<double> result(n, 0.0);
    for (std::size_t i = 0; i < n; ++i) {
        double sum = 0.0;
        for (std::size_t j = 0; j < n; ++j) {
            double val = (i >= j) ? matrix[i][j] : matrix[j][i];
            sum += val * vec[j];
        }
        result[i] = sum;
    }

    double rayleigh = 0.0;
    for (std::size_t i = 0; i < n; ++i) {
        rayleigh += vec[i] * result[i];
    }

    // Return absolute value as the operator norm
    return std::fabs(rayleigh);
}

#include <cassert>
#include <cmath>
#include <vector>

// Include the solution function here (or declare it)

int main() {
    // 1x1 identity
    std::vector<std::vector<double>> m1 = {{1.0}};
    assert(std::fabs(operatorNorm(m1) - 1.0) < 1e-6);

    // 3x3 matrix of ones (given in prompt) - norm is 3
    std::vector<std::vector<double>> m2 = {
        {1.0, 1.0, 1.0},
        {1.0, 1.0, 1.0},
        {1.0, 1.0, 1.0}
    };
    assert(std::fabs(operatorNorm(m2) - 3.0) < 1e-4);

    // 2x2 diagonal matrix diag(2, -1) - largest abs eigenvalue = 2
    std::vector<std::vector<double>> m3 = {
        {2.0, 0.0},
        {0.0, -1.0}
    };
    assert(std::fabs(operatorNorm(m3) - 2.0) < 1e-6);

    // 2x2 symmetric matrix [[1,2],[2,1]] - eigenvalues 3 and -1, norm = 3
    std::vector<std::vector<double>> m4 = {
        {1.0, 2.0},
        {2.0, 1.0}
    };
    assert(std::fabs(operatorNorm(m4) - 3.0) < 1e-6);

    // All-zero matrix
    std::vector<std::vector<double>> m5 = {
        {0.0, 0.0},
        {0.0, 0.0}
    };
    assert(std::fabs(operatorNorm(m5) - 0.0) < 1e-12);

    // 3x3 with negative dominant eigenvalue: diag(-5, 2, 1) - norm = 5
    std::vector<std::vector<double>> m6 = {
        {-5.0, 0.0, 0.0},
        {0.0, 2.0, 0.0},
        {0.0, 0.0, 1.0}
    };
    assert(std::fabs(operatorNorm(m6) - 5.0) < 1e-6);

    // 4x4 with larger off-diagonal elements: matrix of ones scaled by 2 -> norm = 8
    std::vector<std::vector<double>> m7 = {
        {2.0, 2.0, 2.0, 2.0},
        {2.0, 2.0, 2.0, 2.0},
        {2.0, 2.0, 2.0, 2.0},
        {2.0, 2.0, 2.0, 2.0}
    };
    assert(std::fabs(operatorNorm(m7) - 8.0) < 1e-4);

    return 0;
}
