Implement a C++ function that performs batched matrix multiplication for 2D matrices with an optional transpose of the second operand, following the core logic of the provided snippet. Specifically, write a free function `std::vector<std::vector<float>> batchedMatMul(const std::vector<std::vector<float>>& A, const std::vector<std::vector<float>>& B, bool transposeB)` that: (1) accepts two 2D matrices represented as vectors of rows, (2) if `transposeB` is false, transposes `B` internally before multiplication, (3) computes the product `A * B` where the inner dimension of `A` must equal the inner dimension of `B` (or the number of columns of `B` when `transposeB` is true, since `B` is treated as already transposed), and (4) returns the resulting matrix as a vector of rows. The function must handle edge cases like empty matrices, single-row/single-column inputs, and rectangular dimensions, and must not modify its inputs. Use `std::vector<float>` for rows, and check dimensions with `assert` or a clear error (e.g., throw `std::invalid_argument`). Provide full implementation with `const` correctness.

// The solution mirrors the provided `matmul_transb` and `transpose` helpers. First, extract dimensions: `M = A.size()` (rows of A), `K = A[0].size()` (columns of A / rows of B if transposed), and `N = transposeB ? B[0].size() : B.size()` (columns of the result). If `transposeB` is false, create a transposed copy `BT` of `B` so that each row of `BT` corresponds to a column of the original `B`. Then for each row `i` of `A` and each column `j` of `BT`, compute the dot product of that row with column `j` (which is row `j` of `BT`). The naive triple loop runs in `O(M*N*K)` time, uses `O(K)` extra space for the transposed copy (or `O(1)` if `transposeB` is true), and returns a new matrix with `M` rows and `N` columns. Edge cases: if either matrix is empty or dimensions mismatch, throw `std::invalid_argument`; a matrix with zero rows or columns is invalid because the inner dimension must be positive; handle the case where `A` is a row vector (`M=1`) or `B` is a column vector (`N=1`) naturally without special branching. The implementation avoids modifying inputs by using `const` references and only reading from them.

#include <vector>
#include <stdexcept>
#include <cassert>

// Compute the matrix product of A and B (optionally transposing B first).
// Returns a new matrix as a vector of rows.
std::vector<std::vector<float>> batchedMatMul(
    const std::vector<std::vector<float>>& A,
    const std::vector<std::vector<float>>& B,
    bool transposeB) {
    
    // Validate inputs.
    if (A.empty() || B.empty() || A[0].empty() || B[0].empty()) {
        throw std::invalid_argument("Matrices must be non-empty and have at least one row and column.");
    }
    const size_t M = A.size();
    const size_t K_A = A[0].size();
    const size_t rowsB = B.size();
    const size_t colsB = B[0].size();
    
    // Inner dimension comes from A's columns and B's rows (if not transposed) or B's columns (if transposed).
    const size_t K = transposeB ? colsB : rowsB;
    if (K_A != K) {
        throw std::invalid_argument("Inner dimensions of matrices do not match.");
    }
    
    // Result dimensions.
    const size_t N = transposeB ? rowsB : colsB;
    
    // Build transposed B if needed.
    std::vector<std::vector<float>> BT;
    if (!transposeB) {
        BT.resize(N, std::vector<float>(K));
        for (size_t j = 0; j < N; ++j) {
            for (size_t k = 0; k < K; ++k) {
                BT[j][k] = B[k][j];
            }
        }
    }
    
    // Compute product.
    std::vector<std::vector<float>> result(M, std::vector<float>(N, 0.0f));
    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            float sum = 0.0f;
            if (transposeB) {
                // B is already treated as transposed: each row of B corresponds to a column of the result.
                for (size_t k = 0; k < K; ++k) {
                    sum += A[i][k] * B[j][k];
                }
            } else {
                // Use transposed copy BT where row j = column j of original B.
                for (size_t k = 0; k < K; ++k) {
                    sum += A[i][k] * BT[j][k];
                }
            }
            result[i][j] = sum;
        }
    }
    
    return result;
}

#include <cassert>
#include <vector>
#include <cmath>

int main() {
    // Test 1: Basic 2x3 * 3x2 (no transpose)
    std::vector<std::vector<float>> A1 = {{1,2,3}, {4,5,6}};
    std::vector<std::vector<float>> B1 = {{7,8}, {9,10}, {11,12}};
    auto R1 = batchedMatMul(A1, B1, false);
    assert(R1.size() == 2 && R1[0].size() == 2);
    assert(std::fabs(R1[0][0] - 58) < 1e-5);
    assert(std::fabs(R1[0][1] - 64) < 1e-5);
    assert(std::fabs(R1[1][0] - 139) < 1e-5);
    assert(std::fabs(R1[1][1] - 154) < 1e-5);

    // Test 2: With transposeB = true (effectively A * B^T)
    std::vector<std::vector<float>> B2 = {{7,9,11}, {8,10,12}}; // this is B^T from B1
    auto R2 = batchedMatMul(A1, B2, true);
    assert(R2.size() == 2 && R2[0].size() == 2);
    assert(R2 == R1); // should be equal to previous result

    // Test 3: Single row A (row vector) times matrix
    std::vector<std::vector<float>> Arow = {{1,2}};
    std::vector<std::vector<float>> B3 = {{3,4,5}, {6,7,8}};
    auto R3 = batchedMatMul(Arow, B3, false);
    assert(R3.size() == 1 && R3[0].size() == 3);
    assert(std::fabs(R3[0][0] - 15) < 1e-5);
    assert(std::fabs(R3[0][1] - 18) < 1e-5);
    assert(std::fabs(R3[0][2] - 21) < 1e-5);

    // Test 4: Single column B (column vector) via transpose
    std::vector<std::vector<float>> A4 = {{1,2}, {3,4}};
    std::vector<std::vector<float>> B4 = {{5,6}}; // transposeB = false -> B is 1x2, but we want 2x1 column
    // Actually B4 as is has 1 row, 2 cols; to multiply as A (2x2) * (2x1) we need B4 to be 2x1, so use transposeB=true with B4 being 2x1? 
    // Let's define B_col as 2x1: {{5}, {6}} and use transposeB=false.
    std::vector<std::vector<float>> B_col = {{5}, {6}};
    auto R4 = batchedMatMul(A4, B_col, false);
    assert(R4.size() == 2 && R4[0].size() == 1);
    assert(std::fabs(R4[0][0] - 17) < 1e-5);
    assert(std::fabs(R4[1][0] - 39) < 1e-5);

    // Test 5: 1x1 matrices
    std::vector<std::vector<float>> A5 = {{3}};
    std::vector<std::vector<float>> B5 = {{4}};
    auto R5 = batchedMatMul(A5, B5, false);
    assert(R5.size() == 1 && R5[0].size() == 1);
    assert(std::fabs(R5[0][0] - 12) < 1e-5);

    // Test 6: transposeB with 1x1 still works
    auto R6 = batchedMatMul(A5, B5, true);
    assert(R6[0][0] == 12);

    // Test 7: Dimension mismatch should throw
    bool threw = false;
    try {
        std::vector<std::vector<float>> A7 = {{1,2}};
        std::vector<std::vector<float>> B7 = {{1,2,3}};
        batchedMatMul(A7, B7, false);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test 8: Empty matrix should throw
    threw = false;
    try {
        std::vector<std::vector<float>> A8 = {};
        std::vector<std::vector<float>> B8 = {{1}};
        batchedMatMul(A8, B8, false);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test 9: Larger rectangular multiplication 3x2 * 2x4
    std::vector<std::vector<float>> A9 = {{1,2}, {3,4}, {5,6}};
    std::vector<std::vector<float>> B9 = {{1,0,2,1}, {0,3,1,0}};
    auto R9 = batchedMatMul(A9, B9, false);
    assert(R9.size() == 3 && R9[0].size() == 4);
    assert(std::fabs(R9[0][0] - 1) < 1e-5);
    assert(std::fabs(R9[0][1] - 6) < 1e-5);
    assert(std::fabs(R9[0][2] - 4) < 1e-5);
    assert(std::fabs(R9[0][3] - 1) < 1e-5);
    assert(std::fabs(R9[2][3] - 5) < 1e-5);

    return 0;
}
