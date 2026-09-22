// Write a C++ function that computes the n-th Fibonacci number modulo 1,000,000,007 using fast matrix exponentiation. The function must accept a single non-negative integer `n` (which may be as large as 10^18) and return a `long long` representing the Fibonacci number `F(n)` modulo the given modulus, where the Fibonacci sequence is defined as `F(0)=0`, `F(1)=1`, and `F(n)=F(n-1)+F(n-2)` for `n≥2`. The solution must not use recursion for Fibonacci computation and must handle `n=0` and `n=1` correctly.

#include <cassert>

int main() {
    // Base cases
    assert(fibonacciMod(0) == 0);
    assert(fibonacciMod(1) == 1);
    // Small values
    assert(fibonacciMod(2) == 1);
    assert(fibonacciMod(3) == 2);
    assert(fibonacciMod(4) == 3);
    assert(fibonacciMod(5) == 5);
    assert(fibonacciMod(10) == 55);
    // Larger value modulo
    assert(fibonacciMod(50) == 12586269025LL % MOD);
    // Extremely large exponent (beyond 64-bit range partially, but within 10^18)
    assert(fibonacciMod(1000000000000000000LL) == 380469528LL);
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1000000007LL;

struct Matrix {
    long long a[2][2];
};

// Multiply two 2x2 matrices modulo MOD.
Matrix matMul(const Matrix& x, const Matrix& y) {
    Matrix res;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            res.a[i][j] = 0;
            for (int k = 0; k < 2; ++k) {
                res.a[i][j] = (res.a[i][j] + x.a[i][k] * y.a[k][j]) % MOD;
            }
        }
    }
    return res;
}

// Compute base^exp for a 2x2 matrix modulo MOD.
Matrix matPow(Matrix base, long long exp) {
    Matrix result;
    result.a[0][0] = 1; result.a[0][1] = 0;
    result.a[1][0] = 0; result.a[1][1] = 1;
    while (exp > 0) {
        if (exp & 1) result = matMul(result, base);
        base = matMul(base, base);
        exp >>= 1;
    }
    return result;
}

// Return the n-th Fibonacci number modulo MOD (n >= 0).
long long fibonacciMod(long long n) {
    if (n == 0) return 0;
    Matrix base;
    base.a[0][0] = 1; base.a[0][1] = 1;
    base.a[1][0] = 1; base.a[1][1] = 0;
    Matrix powered = matPow(base, n);
    return powered.a[0][1] % MOD;
}

// The core idea is to represent the Fibonacci recurrence using a 2×2 matrix `M = [[1,1],[1,0]]`. Then the n-th Fibonacci number is the top-right element of `M^n` (or equivalently, the bottom-left element). Fast exponentiation on this matrix allows computing `M^n` in O(log n) time by repeatedly squaring the matrix and multiplying when the current bit of `n` is set. This works because matrix multiplication is associative, and we apply the modulo at each multiplication to keep values within `long long` range. Edge cases: For `n=0`, the matrix exponent yields the identity matrix (since any matrix to the power 0 is the identity), and its top-right element is 0, which is `F(0)`. For `n=1`, `M^1` is `M` itself, whose top-right element is 1, which is `F(1)`. Time complexity is O(log n) due to the binary exponentiation loop (up to ~60 iterations for `n≤10^18`), and space complexity is O(1) since only a fixed 2×2 matrix is used.
