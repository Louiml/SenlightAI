/*
Write a C++ function named `buildHermitianMatrix` that takes an integer `n` (1 ≤ n ≤ 100) and an `upper` boolean flag, and returns a square `std::vector<std::vector<std::complex<double>>>` representing an `n × n` Hermitian matrix. The matrix must be constructed as follows: for every diagonal element `(i,i)`, set the value to `std::complex<double>(4.0 * i, 0.0)`. For every off-diagonal element where `i < j`, set the value to `std::complex<double>(3.0 * i + j, 3.0 * i + j)`. If the `upper` flag is true, the matrix is stored in upper-triangular form, meaning the lower-triangular part (i > j) is automatically the complex conjugate of the upper part: `M(i,j) = conj(M(j,i))`. If the `upper` flag is false, the matrix is stored in lower-triangular form, meaning the upper-triangular part (i < j) is automatically the complex conjugate of the lower part: `M(i,j) = conj(M(j,i))`. In either case, the returned matrix must satisfy the Hermitian property: `M(i,j) = conj(M(j,i))` for all indices. Ensure that the diagonal elements are purely real with zero imaginary part.
*/

#include <vector>
#include <complex>
#include <cstddef>

// Build an n x n Hermitian matrix with given diagonal and upper/lower off-diagonal values.
// If upper is true, fill the upper triangular part directly; else fill the lower part.
// The opposite triangle is filled as the complex conjugate of the corresponding element.
std::vector<std::vector<std::complex<double>>> buildHermitianMatrix(std::size_t n, bool upper) {
    std::vector<std::vector<std::complex<double>>> matrix(n, std::vector<std::complex<double>>(n, {0.0, 0.0}));

    for (std::size_t i = 0; i < n; ++i) {
        matrix[i][i] = std::complex<double>(4.0 * static_cast<double>(i), 0.0);
    }

    if (upper) {
        // Fill upper triangular part (i < j)
        for (std::size_t i = 0; i < n; ++i) {
            for (std::size_t j = i + 1; j < n; ++j) {
                matrix[i][j] = std::complex<double>(3.0 * static_cast<double>(i) + static_cast<double>(j),
                                                    3.0 * static_cast<double>(i) + static_cast<double>(j));
            }
        }
        // Fill lower triangular part as conjugate of upper
        for (std::size_t i = 0; i < n; ++i) {
            for (std::size_t j = 0; j < i; ++j) {
                matrix[i][j] = std::conj(matrix[j][i]);
            }
        }
    } else {
        // Fill lower triangular part (i > j)
        for (std::size_t i = 0; i < n; ++i) {
            for (std::size_t j = 0; j < i; ++j) {
                matrix[i][j] = std::complex<double>(3.0 * static_cast<double>(i) + static_cast<double>(j),
                                                    3.0 * static_cast<double>(i) + static_cast<double>(j));
            }
        }
        // Fill upper triangular part as conjugate of lower
        for (std::size_t i = 0; i < n; ++i) {
            for (std::size_t j = i + 1; j < n; ++j) {
                matrix[i][j] = std::conj(matrix[j][i]);
            }
        }
    }

    return matrix;
}

#include <cassert>
#include <complex>
#include <vector>
#include <cmath>

// The solution function is declared above. This is the test harness.
// Checks Hermitian property and specific values for n=3 with both flags.

bool isHermitian(const std::vector<std::vector<std::complex<double>>>& m) {
    std::size_t n = m.size();
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            if (std::abs(m[i][j] - std::conj(m[j][i])) > 1e-12) return false;
        }
    }
    return true;
}

int main() {
    // Test n=3, upper=true
    auto m1 = buildHermitianMatrix(3, true);
    assert(isHermitian(m1));
    // Diagonal checks
    assert(m1[0][0] == std::complex<double>(0.0, 0.0));
    assert(m1[1][1] == std::complex<double>(4.0, 0.0));
    assert(m1[2][2] == std::complex<double>(8.0, 0.0));
    // Off-diagonal upper (i<j)
    assert(m1[0][1] == std::complex<double>(1.0, 1.0));
    assert(m1[0][2] == std::complex<double>(2.0, 2.0));
    assert(m1[1][2] == std::complex<double>(5.0, 5.0));
    // Check conjugate symmetry for lower part
    assert(m1[1][0] == std::conj(m1[0][1]));
    assert(m1[2][0] == std::conj(m1[0][2]));
    assert(m1[2][1] == std::conj(m1[1][2]));

    // Test n=3, upper=false
    auto m2 = buildHermitianMatrix(3, false);
    assert(isHermitian(m2));
    assert(m2[0][0] == std::complex<double>(0.0, 0.0));
    assert(m2[1][1] == std::complex<double>(4.0, 0.0));
    assert(m2[2][2] == std::complex<double>(8.0, 0.0));
    // Lower part matches direct formula
    assert(m2[1][0] == std::complex<double>(3.0, 3.0));  // i=1, j=0 => 3*1+0=3
    assert(m2[2][0] == std::complex<double>(6.0, 6.0));  // i=2, j=0 => 3*2+0=6
    assert(m2[2][1] == std::complex<double>(7.0, 7.0));  // i=2, j=1 => 3*2+1=7
    // Upper part is conjugate
    assert(m2[0][1] == std::conj(m2[1][0]));
    assert(m2[0][2] == std::conj(m2[2][0]));
    assert(m2[1][2] == std::conj(m2[2][1]));

    // Test n=1
    auto m3 = buildHermitianMatrix(1, true);
    assert(m3.size() == 1 && m3[0].size() == 1);
    assert(m3[0][0] == std::complex<double>(0.0, 0.0));
    assert(isHermitian(m3));

    // Test n=4, spot-check Hermitian property for both flags
    auto m4 = buildHermitianMatrix(4, true);
    auto m5 = buildHermitianMatrix(4, false);
    assert(isHermitian(m4));
    assert(isHermitian(m5));
    // Verify that both matrices are identical regardless of flag (since Hermitian property enforces uniqueness)
    for (std::size_t i = 0; i < 4; ++i)
        for (std::size_t j = 0; j < 4; ++j)
            assert(m4[i][j] == m5[i][j]);

    return 0;
}

// The task requires constructing a Hermitian matrix, a special complex square matrix equal to its own conjugate transpose. The approach is to first allocate an `n × n` matrix initialized to all zeros (or directly fill only the necessary triangular part). If `upper` is true, we fill the diagonal and the strictly upper-triangular part using the given formulas. Then, for the lower-triangular part, we set `M(i,j) = std::conj(M(j,i))` for all `i > j`. If `upper` is false, we fill the diagonal and strictly lower-triangular part, then set the upper-triangular part as the conjugate of the lower part. Edge cases include `n=1` (only diagonal, which is real) and handling of complex conjugation correctly when imaginary parts are non-zero. Time complexity is `O(n²)` because we iterate over all `n²` elements once. Space complexity is also `O(n²)` as we store the full matrix. The solution uses modern C++ (`std::vector`, `std::complex`, `std::conj`) and avoids external libraries like Boost, making it self-contained.
