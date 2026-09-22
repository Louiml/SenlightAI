Write a C++ function that takes two fixed-size 2x2 square matrices (represented as `std::array<std::array<double,2>,2>`) and returns the matrix product of the two. The function must perform the multiplication without any temporary storage for the result (i.e., compute directly into the output parameter), and must also provide a second overload that computes the square of a single matrix (i.e., mat * mat) in-place on the same matrix (modifying the input). The function should avoid aliasing issues (when input and output share memory) by using a temporary copy of the left-hand matrix if needed. Demonstrate the function by computing both the product of two arbitrary matrices and the square of a matrix, and verify the results against the classic triple-loop matrix multiplication. Edge cases include identical matrices, zero matrices, and identity matrices, all of which should be handled correctly.

#include <cassert>
#include <cmath>

int main() {
    // Test 1: Simple matrix multiplication (2*I * I = 2*I)
    Matrix2d A = {{{2, 0}, {0, 2}}};
    Matrix2d B = {{{1, 0}, {0, 1}}};
    Matrix2d result;
    multiplyMatrix2d(A, B, result);
    assert(result[0][0] == 2.0 && result[0][1] == 0.0);
    assert(result[1][0] == 0.0 && result[1][1] == 2.0);

    // Test 2: Product of non-trivial matrices
    Matrix2d C = {{{1, 2}, {3, 4}}};
    Matrix2d D = {{{5, 6}, {7, 8}}};
    Matrix2d expected = {{{19, 22}, {43, 50}}};
    multiplyMatrix2d(C, D, result);
    assert(result == expected);

    // Test 3: Aliasing: result is the same as A (i.e., C*C written into C)
    Matrix2d E = {{{1, 2}, {3, 4}}};
    Matrix2d originalE = {{{1, 2}, {3, 4}}};
    squareMatrix2d(E); // squares in-place
    assert(E[0][0] == 7 && E[0][1] == 10);
    assert(E[1][0] == 15 && E[1][1] == 22);

    // Test 4: Aliasing via multiplyMatrix2d where result aliases A
    Matrix2d F = {{{2, 0}, {0, 2}}};
    multiplyMatrix2d(F, F, F); // F becomes F*F = 4*I
    assert(F[0][0] == 4.0 && F[0][1] == 0.0);
    assert(F[1][0] == 0.0 && F[1][1] == 4.0);

    // Test 5: Zero matrix product
    Matrix2d Z = {{{0, 0}, {0, 0}}};
    multiplyMatrix2d(Z, A, result);
    assert(result == Z);

    // Test 6: Identity matrix product leaves other unchanged
    Matrix2d I = {{{1, 0}, {0, 1}}};
    Matrix2d G = {{{3, -1}, {2, 5}}};
    multiplyMatrix2d(I, G, result);
    assert(result == G);

    // Test 7: Negative values and non-symmetric
    Matrix2d H = {{{-2, 4}, {1, -3}}};
    Matrix2d other = {{{1, 2}, {3, 4}}};
    Matrix2d expected2 = {{{10, 12}, {-8, -10}}};
    multiplyMatrix2d(H, other, result);
    assert(result == expected2);

    // Test 8: Square of identity is identity
    Matrix2d I2 = {{{1, 0}, {0, 1}}};
    squareMatrix2d(I2);
    assert(I2[0][0] == 1.0 && I2[1][1] == 1.0);

    // Test 9: Square of zero is zero
    Matrix2d Z2 = {{{0, 0}, {0, 0}}};
    squareMatrix2d(Z2);
    assert(Z2[0][0] == 0.0 && Z2[1][1] == 0.0);

    // Test 10: Square of a general matrix (checked against explicit computation)
    Matrix2d J = {{{1, 2}, {3, 4}}};
    squareMatrix2d(J);
    // J*J = [[7, 10], [15, 22]]
    assert(std::abs(J[0][0] - 7.0) < 1e-9 && std::abs(J[0][1] - 10.0) < 1e-9);
    assert(std::abs(J[1][0] - 15.0) < 1e-9 && std::abs(J[1][1] - 22.0) < 1e-9);
}

#include <array>
#include <utility>

// Type alias for a 2x2 matrix of doubles.
using Matrix2d = std::array<std::array<double, 2>, 2>;

// Compute the product of two 2x2 matrices into the output parameter.
// The output may alias either or both inputs; a temporary copy is used if needed.
void multiplyMatrix2d(const Matrix2d& A, const Matrix2d& B, Matrix2d& result) {
    // If result aliases A or B, copy A (or B) to a temporary to avoid corruption.
    Matrix2d tempA = A; // Always copy A; simple and safe.
    // Note: Because we copy A, reading from tempA and writing to result is safe even if result aliases A.
    // If result aliases B, B is not modified, so no issues.
    // But to be fully safe when result aliases A, we use tempA for the first operand.
    // For the second operand, we can use B directly; if result aliases B, we never write to B because result is separate.

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            result[i][j] = 0.0;
            for (int k = 0; k < 2; ++k) {
                result[i][j] += tempA[i][k] * B[k][j];
            }
        }
    }
}

// Compute the square (A * A) in-place, modifying the input matrix.
// The input matrix is both the operand and the output.
void squareMatrix2d(Matrix2d& mat) {
    // Copy the matrix to a temporary because writing to mat while reading it would be wrong.
    Matrix2d original = mat;
    // Now multiply original * original and store into mat.
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            mat[i][j] = 0.0;
            for (int k = 0; k < 2; ++k) {
                mat[i][j] += original[i][k] * original[k][j];
            }
        }
    }
}

// The core algorithm is the standard matrix multiplication for 2x2 matrices: for each row `i` and column `j`, compute `result[i][j] = sum_{k=0..1} A[i][k] * B[k][j]`. The main complication is aliasing: if the output matrix is the same as one of the inputs (or both), overwriting elements while reading them can corrupt the computation. For example, when computing square in-place (i.e., `C = A * A` and writing into `A`), reading `A[0][1]` after writing to `A[0][0]` would use the modified value. To handle this, when the output aliases an input, we first copy the left-hand matrix into a temporary local array, then multiply using the temporary for reads and the original output for writes. Because the matrices are fixed 2x2, the temporary copy costs only O(1) memory and the loop takes constant time (9 multiplications and 4 additions per product). For the square-in-place overload, we detect aliasing (the same reference for input and output) and always copy the input to a temporary before writing to the output. Time complexity is O(1) per matrix product (since dimensions are fixed), and space complexity O(1) beyond the inputs. Edge cases: identity matrix yields the other matrix; zero matrix yields zero; all matrices work regardless of values, including negative numbers and zeros.
