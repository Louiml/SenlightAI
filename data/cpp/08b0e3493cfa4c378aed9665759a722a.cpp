// Write a C++ function `matrixPower` that takes a square matrix `A` (represented as `vector<vector<int>>`) and a non-negative integer exponent `k` (as `long long`), and returns `A^k` where all arithmetic is performed modulo 1000. The matrix size `n` (where `1 ≤ n ≤ 50`) is determined by the input matrix dimensions. The function must handle the case `k = 0` by returning the identity matrix of size `n`, and must compute the result efficiently using fast exponentiation (divide-and-conquer), ensuring that intermediate products do not overflow a 32-bit integer (each multiplication of two values in `[0,999]` produces at most `998001`, and summing up to `n` such products stays below `2^31` for `n ≤ 50`). The input matrix may contain any non-negative integers, and the result must have every element reduced modulo 1000.

#include <cassert>
#include <vector>

// Include the solution code above here (not shown in test snippet for brevity)

int main() {
    // Test 1: power 0 returns identity
    Matrix A1 = {{1, 2}, {3, 4}};
    Matrix I = matrixPower(A1, 0);
    assert(I.size() == 2);
    assert(I[0][0] == 1 && I[0][1] == 0);
    assert(I[1][0] == 0 && I[1][1] == 1);

    // Test 2: power 1 returns same matrix (mod 1000)
    Matrix A2 = {{999, 1001}, {1000, 5000}};
    Matrix P1 = matrixPower(A2, 1);
    assert(P1[0][0] == 999 && P1[0][1] == 1); // 1001 % 1000 = 1
    assert(P1[1][0] == 0 && P1[1][1] == 0);   // 1000%1000=0, 5000%1000=0

    // Test 3: power 2 for a known small matrix
    Matrix A3 = {{1, 1}, {1, 0}}; // Fibonacci-like
    Matrix P2 = matrixPower(A3, 2);
    // A^2 = {{2,1},{1,1}}
    assert(P2[0][0] == 2 && P2[0][1] == 1);
    assert(P2[1][0] == 1 && P2[1][1] == 1);

    // Test 4: power 5 for the same matrix, mod 1000
    Matrix P5 = matrixPower(A3, 5);
    // Fibonacci: F(5)=5, F(4)=3, F(3)=2, F(2)=1
    // A^5 = {{F(6),F(5)},{F(5),F(4)}} = {{8,5},{5,3}}
    assert(P5[0][0] == 8 && P5[0][1] == 5);
    assert(P5[1][0] == 5 && P5[1][1] == 3);

    // Test 5: large exponent, ensure modulo 1000
    Matrix A4 = {{2, 0}, {0, 3}};
    Matrix P10 = matrixPower(A4, 10);
    // 2^10 mod 1000 = 24, 3^10 mod 1000 = 49 (since 59049 % 1000 = 49)
    assert(P10[0][0] == 24 && P10[0][1] == 0);
    assert(P10[1][0] == 0 && P10[1][1] == 49);

    // Test 6: single element matrix
    Matrix A5 = {{7}};
    Matrix P7 = matrixPower(A5, 7);
    // 7^7 = 823543, mod 1000 = 543
    assert(P7[0][0] == 543);

    return 0;
}

#include <vector>

using Matrix = std::vector<std::vector<int>>;

// Multiply two square matrices modulo 1000.
Matrix matMul(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix C(n, std::vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int sum = 0;
            for (int k = 0; k < n; ++k) {
                sum = (sum + A[i][k] * B[k][j]) % 1000;
            }
            C[i][j] = sum;
        }
    }
    return C;
}

// Compute A^k modulo 1000 using fast exponentiation.
Matrix matrixPower(const Matrix& A, long long k) {
    int n = A.size();
    if (n == 0) return {};
    // Identity matrix for k == 0
    Matrix result(n, std::vector<int>(n, 0));
    for (int i = 0; i < n; ++i) result[i][i] = 1;

    Matrix base = A;
    // Reduce base modulo 1000 initially (in case input has large values)
    for (auto& row : base) {
        for (auto& val : row) {
            val %= 1000;
        }
    }

    while (k > 0) {
        if (k % 2 == 1) {
            result = matMul(result, base);
        }
        base = matMul(base, base);
        k /= 2;
    }
    return result;
}

// The solution uses binary exponentiation (fast power). For a square matrix `A` and exponent `k`, we recursively or iteratively compute:
// - If `k == 0`, return the identity matrix (diagonal 1, off-diagonal 0) of the same size.
// - If `k == 1`, return `A` (already reduced modulo 1000 if needed, but careful to copy it).
// - If `k` is even, compute `A^(k/2)` and square it.
// - If `k` is odd, compute `A^(k-1)` and multiply by `A`.
//
// Matrix multiplication is standard triple-nested loop: for each `i`, `j`, we sum `A[i][k] * B[k][j]` over `k`, taking modulo 1000 after each addition to keep numbers small. Edge cases: empty matrix (n=0) is not in constraints, but we can handle gracefully by returning empty. Identity matrix must be freshly generated for each call, not global, to avoid state pollution. Time complexity is O(n^3 log k) because each matrix multiplication is O(n^3) and we perform O(log k) multiplications. Space complexity is O(n^2) for temporary matrices (each matrix is of size n^2). The function should be `const`-correct: take `A` by `const&` and return a new matrix.
