Write a C++ function that, given a positive integer \(n\) (\(1 \le n \le 10^{18}\)), returns the \(n\)-th Fibonacci number modulo \(10^8+7\), where Fibonacci numbers are defined by \(F_0 = 0\), \(F_1 = 1\), and \(F_k = F_{k-1} + F_{k-2}\) for \(k \ge 2\). The function must be efficient enough for very large \(n\), and the result must be a non-negative integer less than \(10^8+7\). The input is a single integer per test case, and the function should handle multiple calls independently.
// The solution uses fast matrix exponentiation to compute Fibonacci numbers in \(O(\log n)\) time. The Fibonacci recurrence can be represented by the matrix \(M = \begin{pmatrix} 1 & 1 \\ 1 & 0 \end{pmatrix}\), such that \(M^n = \begin{pmatrix} F_{n+1} & F_n \\ F_n & F_{n-1} \end{pmatrix}\). Therefore, the \(n\)-th Fibonacci number is the top-left (or top-right) entry of \(M^n\) (since \(F_0=0\), \(F_1=1\), \(M^1\) gives \(F_2=1\) in top-left, so careful: the correct mapping is that \(M^n = \begin{pmatrix} F_{n+1} & F_n \\ F_n & F_{n-1} \end{pmatrix}\)). To get exactly \(F_n\), we compute \(M^n\) and return the element at row 0, column 1 (or equivalently row 1, column 0). The exponentiation uses binary exponentiation (exponentiation by squaring) with modulo \(10^8+7\). Edge cases: for \(n=0\), return 0; for \(n=1\), return 1; for \(n=2\), the matrix \(M^2\) gives \(F_2=1\) correctly. Since the matrix multiplication is \(O(1)\) (2x2 matrices), the overall time complexity is \(O(\log n)\) per call, and space complexity is \(O(\log n)\) due to recursion depth (or \(O(1)\) if implemented iteratively). The function must handle large \(n\) up to \(10^{18}\), so recursion depth is at most 60, which is safe.
#include <bits/stdc++.h>

// Compute n-th Fibonacci number modulo MOD using fast matrix exponentiation.
long long nthFibonacci(long long n) {
    const long long MOD = 100000007LL;
    if (n == 0) return 0;
    if (n == 1) return 1;

    using Matrix = std::array<std::array<long long, 2>, 2>;

    // Matrix multiplication modulo MOD.
    auto multiply = [&](const Matrix& a, const Matrix& b) -> Matrix {
        Matrix c = {{{0, 0}, {0, 0}}};
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                for (int k = 0; k < 2; ++k) {
                    c[i][j] = (c[i][j] + a[i][k] * b[k][j]) % MOD;
                }
            }
        }
        return c;
    };

    // Binary exponentiation of matrix.
    std::function<Matrix(const Matrix&, long long)> power = [&](const Matrix& base, long long exp) -> Matrix {
        if (exp == 1) return base;
        Matrix half = power(base, exp / 2);
        Matrix halfSquared = multiply(half, half);
        if (exp % 2 == 0) {
            return halfSquared;
        } else {
            return multiply(halfSquared, base);
        }
    };

    Matrix base = {{{1, 1}, {1, 0}}};
    Matrix result = power(base, n);
    // M^n = [[F_{n+1}, F_n], [F_n, F_{n-1}]]
    // Return F_n which is at [0][1] (or [1][0]).
    return result[0][1] % MOD;
}
#include <bits/stdc++.h>

// Declaration of the solution function
long long nthFibonacci(long long n);

int main() {
    assert(nthFibonacci(0) == 0);
    assert(nthFibonacci(1) == 1);
    assert(nthFibonacci(2) == 1);
    assert(nthFibonacci(3) == 2);
    assert(nthFibonacci(10) == 55);
    assert(nthFibonacci(20) == 6765);
    // Verify that modulo is applied: F_50 = 12586269025, modulo 100000007 = 12586269025 % 100000007 = 12586258? compute: 100000007 * 125 = 12500000875, remainder 86269025-12500000875? Wait: 12586269025 / 100000007 ≈ 125.862, so 125 * 100000007 = 12500000875, remainder 86268150. Let's just compute safely with a known small value: use F_47 = 2971215073, modulo 100000007 = 2971215073 - 29*100000007 = 2971215073 - 2900000203 = 71214870.
    assert(nthFibonacci(47) == 71214870);
    // Large n test: ensure it does not overflow and returns a value in [0, MOD-1].
    long long large = nthFibonacci(1000000000000000000LL);
    assert(large >= 0 && large < 100000007LL);
    assert(nthFibonacci(1000000000000000000LL) == large); // deterministic
}
