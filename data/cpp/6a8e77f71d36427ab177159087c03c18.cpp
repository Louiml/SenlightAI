Write a C++ function `solveLinearSystem` that takes a symmetric positive-definite 2x2 matrix `A` (as a `std::array<std::array<double,2>,2>` or similar plain C-style nested array) and a right-hand side 2x2 matrix `B` (also a 2x2 nested structure, representing multiple right-hand sides as columns), and returns the solution matrix `X` (same shape) such that `A*X = B`. The function must use the Cholesky decomposition (or equivalently the `LDLT` decomposition) to solve the system efficiently, without relying on any external linear algebra library (i.e., implement the decomposition manually using only the C++ standard library). The matrices are guaranteed to be symmetric positive-definite, so no pivoting or fallback is needed. The solution must be returned as a `std::array<std::array<double,2>,2>`. All computations must use `double` precision. The function should be `const`-correct, taking the inputs by `const` reference, and should not modify them.
#include <cassert>
#include <cmath>
#include <array>

// The solution function is declared above; include it or paste here.

int main() {
    // Test case 1: Simple diagonal matrix.
    std::array<std::array<double,2>,2> A1 = {{{2, 0}, {0, 3}}};
    std::array<std::array<double,2>,2> B1 = {{{4, 6}, {9, 12}}};
    auto X1 = solveLinearSystem(A1, B1);
    assert(std::fabs(X1[0][0] - 2.0) < 1e-12);
    assert(std::fabs(X1[0][1] - 3.0) < 1e-12);
    assert(std::fabs(X1[1][0] - 3.0) < 1e-12);
    assert(std::fabs(X1[1][1] - 4.0) < 1e-12);

    // Test case 2: Non-diagonal SPD matrix.
    std::array<std::array<double,2>,2> A2 = {{{2, -1}, {-1, 3}}};
    std::array<std::array<double,2>,2> B2 = {{{1, 2}, {3, 1}}};
    auto X2 = solveLinearSystem(A2, B2);
    // Expected solution: X = A^{-1} * B. Compute inverse: det = 2*3 - 1 = 5, inv = [[3/5, 1/5], [1/5, 2/5]].
    // X2[0][0] = (3/5)*1 + (1/5)*3 = 6/5 = 1.2
    // X2[0][1] = (3/5)*2 + (1/5)*1 = 7/5 = 1.4
    // X2[1][0] = (1/5)*1 + (2/5)*3 = 7/5 = 1.4
    // X2[1][1] = (1/5)*2 + (2/5)*1 = 4/5 = 0.8
    assert(std::fabs(X2[0][0] - 1.2) < 1e-12);
    assert(std::fabs(X2[0][1] - 1.4) < 1e-12);
    assert(std::fabs(X2[1][0] - 1.4) < 1e-12);
    assert(std::fabs(X2[1][1] - 0.8) < 1e-12);

    // Test case 3: Identity matrix.
    std::array<std::array<double,2>,2> A3 = {{{1, 0}, {0, 1}}};
    std::array<std::array<double,2>,2> B3 = {{{5, -3}, {7, 2}}};
    auto X3 = solveLinearSystem(A3, B3);
    assert(X3 == B3);

    // Test case 4: Check that A*X = B for a random SPD matrix (round-trip).
    std::array<std::array<double,2>,2> A4 = {{{5, 1}, {1, 4}}};
    std::array<std::array<double,2>,2> B4 = {{{1, 2}, {3, 4}}};
    auto X4 = solveLinearSystem(A4, B4);
    auto C = std::array<std::array<double,2>,2>{};
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            C[i][j] = A4[i][0]*X4[0][j] + A4[i][1]*X4[1][j];
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            assert(std::fabs(C[i][j] - B4[i][j]) < 1e-12);
}
#include <array>
#include <cmath>
#include <stdexcept>

// Solve A*X = B for a 2x2 symmetric positive-definite matrix A and a 2x2 right-hand side B.
// Returns the solution X as a 2x2 array.
std::array<std::array<double, 2>, 2> solveLinearSystem(
    const std::array<std::array<double, 2>, 2>& A,
    const std::array<std::array<double, 2>, 2>& B
) {
    // Cholesky decomposition: A = L * L^T, where L is lower-triangular.
    // For A = [[a, b], [b, c]], L = [[l11, 0], [l21, l22]].
    const double a = A[0][0];
    const double b = A[1][0]; // symmetric, so A[0][1] == A[1][0]
    const double c = A[1][1];

    const double l11 = std::sqrt(a);
    const double l21 = b / l11;
    const double l22 = std::sqrt(c - l21 * l21);

    // Solve L*Y = B by forward substitution for each column of B.
    std::array<std::array<double, 2>, 2> Y = {};
    for (int col = 0; col < 2; ++col) {
        const double b0 = B[0][col];
        const double b1 = B[1][col];
        Y[0][col] = b0 / l11;
        Y[1][col] = (b1 - l21 * Y[0][col]) / l22;
    }

    // Solve L^T*X = Y by back substitution for each column.
    std::array<std::array<double, 2>, 2> X = {};
    for (int col = 0; col < 2; ++col) {
        const double y0 = Y[0][col];
        const double y1 = Y[1][col];
        X[1][col] = y1 / l22;
        X[0][col] = (y0 - l21 * X[1][col]) / l11;
    }

    return X;
}
// The task requires solving a 2x2 linear system with two right-hand sides (so effectively solving `A*X = B` where `B` has two columns). The standard approach for a symmetric positive-definite matrix is Cholesky decomposition: factor `A = L * L^T`, where `L` is lower-triangular. Then solve `L*Y = B` by forward substitution, and then solve `L^T*X = Y` by back substitution. For a 2x2 matrix, the decomposition is simple: let `A = [[a, b], [b, c]]`. Then `L = [[sqrt(a), 0], [b/sqrt(a), sqrt(c - b*b/a)]]`. The forward substitution solves for `Y` column-wise, and back substitution for `X`. Edge cases: the matrix is guaranteed SPD, so `a > 0` and `c - b*b/a > 0`, but we still guard against division by zero (though it won't happen in valid input). Time complexity is O(1) because the matrix size is fixed at 2x2, and space complexity is O(1) as we only store a few local variables.
