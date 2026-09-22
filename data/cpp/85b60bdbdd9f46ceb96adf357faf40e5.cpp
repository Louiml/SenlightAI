// Write a C++ function named `strassenMatrixMultiply` that takes two square integer matrices `A` and `B` of the same size `n` (where `n` is a power of 2 and at most 8, to keep stack usage safe) and outputs their product into a third matrix `C`. The function must implement Strassen's algorithm for matrix multiplication, which reduces the complexity compared to the naive triple‑loop method. The matrices are passed as fixed‑size 2D arrays (`int[8][8]`), and only the top‑left `n × n` submatrix is used. The function should have signature `void strassenMatrixMultiply(const int A[8][8], const int B[8][8], int C[8][8], int n)`, with `const` correctness applied to the input matrices. The algorithm must handle the base case `n == 1` directly, recursively split the matrices into four quadrants, compute the seven Strassen intermediate products (M1 through M7), and combine them to form the result. You may assume the input `n` is always a power of 2 and does not exceed 8; no error checking is required. The final implementation must not include a `main` function, but must be self‑contained with all necessary headers and helper functions.
#include <cassert>
#include <cstddef>

int main() {
    // Test case 1: 1x1 multiplication.
    int A1[8][8] = {{0}};
    int B1[8][8] = {{0}};
    int C1[8][8] = {{0}};
    A1[0][0] = 3;
    B1[0][0] = 7;
    strassenMatrixMultiply(A1, B1, C1, 1);
    assert(C1[0][0] == 21);

    // Test case 2: 2x2 multiplication.
    int A2[8][8] = {{1,2,0,0},{3,4,0,0},{0,0,0,0},{0,0,0,0}};
    int B2[8][8] = {{5,6,0,0},{7,8,0,0},{0,0,0,0},{0,0,0,0}};
    int C2[8][8] = {{0}};
    // Expected product: [[19,22],[43,50]]
    strassenMatrixMultiply(A2, B2, C2, 2);
    assert(C2[0][0] == 19);
    assert(C2[0][1] == 22);
    assert(C2[1][0] == 43);
    assert(C2[1][1] == 50);

    // Test case 3: 4x4 multiplication with identity matrices.
    int I[8][8] = {{0}};
    for (int i = 0; i < 4; ++i) I[i][i] = 1;
    int X[8][8] = {{0}};
    X[0][1] = 2; X[2][3] = -1; X[3][0] = 5;
    int C3[8][8] = {{0}};
    strassenMatrixMultiply(I, X, C3, 4);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            assert(C3[i][j] == X[i][j]);
        }
    }

    // Test case 4: 4x4 multiplication with zero matrix.
    int Z[8][8] = {{0}};
    int C4[8][8] = {{0}};
    strassenMatrixMultiply(X, Z, C4, 4);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            assert(C4[i][j] == 0);
        }
    }

    // Test case 5: 8x8 multiplication with all ones.
    int O[8][8] = {{1}}; // actually all ones below
    int A8[8][8] = {{0}}, B8[8][8] = {{0}}, C8[8][8] = {{0}};
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            A8[i][j] = 1;
            B8[i][j] = 1;
        }
    }
    strassenMatrixMultiply(A8, B8, C8, 8);
    // Each entry should be 8*1*1 = 8.
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            assert(C8[i][j] == 8);
        }
    }

    return 0;
}
#include <cstddef>

// Helper: compute C = A + B or C = A - B, for n x n matrices.
// If subtract is true, C = A - B, otherwise C = A + B.
void matrixAddSub(const int A[8][8], const int B[8][8], int C[8][8], int n, bool subtract) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (subtract) {
                C[i][j] = A[i][j] - B[i][j];
            } else {
                C[i][j] = A[i][j] + B[i][j];
            }
        }
    }
}

// Strassen's matrix multiplication for n x n matrices (n is a power of 2, n <= 8).
void strassenMatrixMultiply(const int A[8][8], const int B[8][8], int C[8][8], int n) {
    // Base case: 1x1 matrix.
    if (n == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return;
    }

    int half = n / 2;

    // Temporary matrices for quadrants of A and B.
    int A11[8][8], A12[8][8], A21[8][8], A22[8][8];
    int B11[8][8], B12[8][8], B21[8][8], B22[8][8];

    // Temporary matrices for Strassen products and intermediate sums.
    int M1[8][8], M2[8][8], M3[8][8], M4[8][8], M5[8][8], M6[8][8], M7[8][8];
    int T1[8][8], T2[8][8];

    // Split A and B into quadrants.
    for (int i = 0; i < half; ++i) {
        for (int j = 0; j < half; ++j) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][half + j];
            A21[i][j] = A[half + i][j];
            A22[i][j] = A[half + i][half + j];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][half + j];
            B21[i][j] = B[half + i][j];
            B22[i][j] = B[half + i][half + j];
        }
    }

    // M1 = A11 * (B12 - B22)
    matrixAddSub(B12, B22, T1, half, true);
    strassenMatrixMultiply(A11, T1, M1, half);

    // M2 = (A11 + A12) * B22
    matrixAddSub(A11, A12, T1, half, false);
    strassenMatrixMultiply(T1, B22, M2, half);

    // M3 = (A21 + A22) * B11
    matrixAddSub(A21, A22, T1, half, false);
    strassenMatrixMultiply(T1, B11, M3, half);

    // M4 = A22 * (B21 - B11)
    matrixAddSub(B21, B11, T1, half, true);
    strassenMatrixMultiply(A22, T1, M4, half);

    // M5 = (A11 + A22) * (B11 + B22)
    matrixAddSub(A11, A22, T1, half, false);
    matrixAddSub(B11, B22, T2, half, false);
    strassenMatrixMultiply(T1, T2, M5, half);

    // M6 = (A12 - A22) * (B21 + B22)
    matrixAddSub(A12, A22, T1, half, true);
    matrixAddSub(B21, B22, T2, half, false);
    strassenMatrixMultiply(T1, T2, M6, half);

    // M7 = (A11 - A21) * (B11 + B12)
    matrixAddSub(A11, A21, T1, half, true);
    matrixAddSub(B11, B12, T2, half, false);
    strassenMatrixMultiply(T1, T2, M7, half);

    // Combine results into C.
    // C11 = M5 + M4 - M2 + M6
    matrixAddSub(M5, M4, T1, half, false);
    matrixAddSub(T1, M2, T2, half, true);
    matrixAddSub(T2, M6, T1, half, false);
    for (int i = 0; i < half; ++i) {
        for (int j = 0; j < half; ++j) {
            C[i][j] = T1[i][j];
        }
    }

    // C12 = M1 + M2
    matrixAddSub(M1, M2, T1, half, false);
    for (int i = 0; i < half; ++i) {
        for (int j = 0; j < half; ++j) {
            C[i][half + j] = T1[i][j];
        }
    }

    // C21 = M3 + M4
    matrixAddSub(M3, M4, T1, half, false);
    for (int i = 0; i < half; ++i) {
        for (int j = 0; j < half; ++j) {
            C[half + i][j] = T1[i][j];
        }
    }

    // C22 = M5 + M1 - M3 - M7
    matrixAddSub(M5, M1, T1, half, false);
    matrixAddSub(T1, M3, T2, half, true);
    matrixAddSub(T2, M7, T1, half, true);
    for (int i = 0; i < half; ++i) {
        for (int j = 0; j < half; ++j) {
            C[half + i][half + j] = T1[i][j];
        }
    }
}
// The solution follows the classic Strassen algorithm. For matrices of size `n`, if `n == 1` the product is simply `C[0][0] = A[0][0] * B[0][0]`. Otherwise, split `A` and `B` into four `(n/2)×(n/2)` submatrices: `A11`, `A12`, `A21`, `A22` and `B11`, `B12`, `B21`, `B22`. Then compute seven recursive products:  
// M1 = A11 * (B12 - B22),  
// M2 = (A11 + A12) * B22,  
// M3 = (A21 + A22) * B11,  
// M4 = A22 * (B21 - B11),  
// M5 = (A11 + A22) * (B11 + B22),  
// M6 = (A12 - A22) * (B21 + B22),  
// M7 = (A11 - A21) * (B11 + B12).  
// Because subtraction and addition on submatrices are element‑wise, we can compute the intermediate M matrices using temporary arrays, or we can directly embed the arithmetic in the recursive calls by creating helper arrays. However, to keep the code clean and avoid extra matrix addition functions, the reference solution uses a small helper `addSub` that adds or subtracts two submatrices into a result. The M matrices are then combined using element‑wise additions/subtractions to form the four quadrants of `C`:  
// C11 = M5 + M4 - M2 + M6,  
// C12 = M1 + M2,  
// C21 = M3 + M4,  
// C22 = M5 + M1 - M3 - M7.  
// The recursion depth is `log2(n)`, and at each level we allocate 7 temporary submatrices of size `(n/2)`. Since fixed‑size arrays are used, we must ensure `n` is small enough (≤8) to avoid stack overflow. Time complexity is `O(n^log2(7)) ≈ O(n^2.81)`, which is better than `O(n^3)`. Space complexity is `O(n^2)` due to the temporary matrices at each recursion level (though in practice the fixed‑size arrays stack up).
