/*
Write a C++ function `fibonacciRangeSum(long long n, long long m)` that takes two non-negative integers \(n \le m\) and returns the sum of Fibonacci numbers \(F_n + F_{n+1} + \dots + F_m\) modulo \(10^9+7\). Here \(F_0 = 0\), \(F_1 = 1\), and \(F_k = F_{k-1} + F_{k-2}\) for \(k \ge 2\). The function must handle very large \(m\) (up to \(10^{18}\)) efficiently using matrix exponentiation. The result must be a non‑negative integer in the range \([0, 10^9+6]\). The task is to implement the function so that it computes the prefix sum \(S(k) = \sum_{i=0}^k F_i\) efficiently, and then returns \((S(m) - S(n-1)) \mod (10^9+7)\). The function should be self-contained, use `const` appropriately, and be tested against known small values.
*/
#include <cstdint>
#include <cstring>

const int64_t MOD = 1000000007LL;

struct Mat2 {
    int64_t a[2][2];
    Mat2() { memset(a, 0, sizeof(a)); }
    void identity() {
        a[0][0] = a[1][1] = 1;
        a[0][1] = a[1][0] = 0;
    }
    Mat2 operator*(const Mat2& other) const {
        Mat2 res;
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                for (int k = 0; k < 2; ++k) {
                    res.a[i][j] = (res.a[i][j] + a[i][k] * other.a[k][j]) % MOD;
                }
            }
        }
        return res;
    }
};

// Compute the n-th Fibonacci number modulo MOD, where F_0 = 0, F_1 = 1.
int64_t fibMod(int64_t n) {
    if (n < 0) return 0;
    if (n == 0) return 0;
    if (n == 1) return 1;

    Mat2 base, result;
    base.a[0][0] = 1; base.a[0][1] = 1;
    base.a[1][0] = 1; base.a[1][1] = 0;
    result.identity();

    int64_t exp = n - 1; // because base^(n-1) * [F_1, F_0]^T gives [F_n, F_{n-1}]^T
    while (exp > 0) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    // result * [F_1, F_0]^T = [F_n, F_{n-1}]^T, so F_n = result.a[0][0] * F_1 + result.a[0][1] * F_0 = result.a[0][0]
    return result.a[0][0] % MOD;
}

// Prefix sum S(k) = F_0 + ... + F_k,  using identity S(k) = F_{k+2} - 1.
int64_t prefixFibSum(int64_t k) {
    if (k < 0) return 0;
    int64_t f_k2 = fibMod(k + 2);
    return (f_k2 - 1 + MOD) % MOD;
}

// Main task function: sum of Fibonacci numbers from F_n to F_m inclusive, modulo MOD.
int64_t fibonacciRangeSum(int64_t n, int64_t m) {
    if (n > m) return 0;
    // Sum from n to m = S(m) - S(n-1)
    int64_t sum_m = prefixFibSum(m);
    int64_t sum_before_n = prefixFibSum(n - 1);
    return (sum_m - sum_before_n + MOD) % MOD;
}
#include <cassert>
#include <cstdint>

int64_t fibonacciRangeSum(int64_t n, int64_t m); // assume defined above

int main() {
    // Test small values: F_0=0, F_1=1, F_2=1, F_3=2, F_4=3, F_5=5
    assert(fibonacciRangeSum(0, 0) == 0);        // F_0 = 0
    assert(fibonacciRangeSum(0, 1) == 1);        // 0 + 1
    assert(fibonacciRangeSum(1, 3) == 4);        // 1+1+2
    assert(fibonacciRangeSum(2, 5) == 11);       // 1+2+3+5
    assert(fibonacciRangeSum(0, 5) == 12);       // 0+1+1+2+3+5
    assert(fibonacciRangeSum(5, 5) == 5);        // just F_5
    assert(fibonacciRangeSum(0, 10) == 143);     // sum F_0..F_10 = 143
    // Large value check with known modulo result: F_50 = 12586269025, mod 1e9+7 = 260835651
    // Sum F_0..F_50 = F_52 - 1. F_52 = F_51 + F_50. Compute F_51 mod MOD = 365010934? Let's just check consistency relation:
    // S(50) = F_52 - 1 mod MOD.
    int64_t F_50 = 12586269025LL % MOD; // compute using function maybe
    // We'll rely on the known identity, so just check that prefixFibSum(0)==0 and prefixFibSum(1)==1.
    // Use a larger test: sum from 1 to 10 = 142? Let's verify: F_1..F_10 = 1+1+2+3+5+8+13+21+34+55 = 143? Actually F_10=55, sum from F_1 to F_10 = 142? Let's compute: 1+1+2=4, +3=7, +5=12, +8=20, +13=33, +21=54, +34=88, +55=143. So sum 1..10 = 143? Wait F_0..F_10 = 143, so F_1..F_10 = 143 - 0 = 143? No, F_0=0 so sum 0..10 = 143, and sum 1..10 = 143 as well? Actually 0 doesn't affect. So sum 1..10 = 143. Check: 1+1+2+3+5+8+13+21+34+55 = 143. Yes.
    assert(fibonacciRangeSum(1, 10) == 143);
    // Edge case: n=0 and m large, use identity S(m)=F_{m+2}-1.
    // Test m=20: F_22 = 17711, so sum 0..20 = 17710.
    assert(fibonacciRangeSum(0, 20) == 17710);
    // Test n=7, m=7: F_7 = 13
    assert(fibonacciRangeSum(7, 7) == 13);
    // Test n=5, m=6: F_5+F_6 = 5+8=13
    assert(fibonacciRangeSum(5, 6) == 13);
    return 0;
}
// The core idea is to express the Fibonacci numbers via a linear recurrence and use matrix exponentiation to compute the required sums in \(O(\log m)\) time.  
// Define a state vector \(V_k = [F_k, F_{k-1}, S_{k-1}]^T\) where \(S_{k-1} = \sum_{i=0}^{k-1} F_i\). The recurrence is:  
// \(F_{k+1} = F_k + F_{k-1}\), and \(S_k = S_{k-1} + F_k\).  
// Thus, the transition matrix \(M\) (size \(3 \times 3\)) that maps \(V_k\) to \(V_{k+1}\) is:  
// Row0: \(M[0][0]=1, M[0][1]=1, M[0][2]=0\) (because \(F_{k+1} = F_k + F_{k-1}\))  
// Row1: \(M[1][0]=1, M[1][1]=0, M[1][2]=0\) (because \(F_k = F_k\))  
// Row2: \(M[2][0]=1, M[2][1]=0, M[2][2]=1\) (because \(S_k = F_k + S_{k-1}\))  
// We start with \(V_1 = [F_1, F_0, S_0]^T = [1, 0, 0]^T\). Then \(V_{k} = M^{k-1} \cdot V_1\). The sum \(S(k) = F_{k+1}?\) Wait, careful: Because \(V_{k+1}\) contains \(S_k\) in its third component. Alternatively we can directly compute \(S(k)\) by using the known identity: \(S(k) = F_{k+2} - 1\). Indeed, \(\sum_{i=0}^k F_i = F_{k+2} - 1\). That simplifies the problem: compute \(F_{k+2}\) using fast doubling or matrix exponentiation on a \(2 \times 2\) Fibonacci matrix. For each prefix sum, we need \(F_{k+2} \mod mod\), then subtract 1.  
// Base cases:  
// - If \(n = 0\), then \(S(n-1) = S(-1) = 0\).  
// - If \(m = 0\), then \(F_0 = 0\), so the sum is \(F_0 = 0\).  
// - For \(k \ge 0\), \(S(k) = (F_{k+2} - 1 + mod) \% mod\).  
// Then answer = \((S(m) - S(n-1) + mod) \% mod\).  
// We can compute the \(n\)-th Fibonacci number modulo \(10^9+7\) using iterative matrix exponentiation on a \(2 \times 2\) matrix \([[1,1],[1,0]]\). The complexity is \(O(\log k)\) per Fibonacci call. Since we only need two calls (for \(m+2\) and \(n+1\)), total time is \(O(\log m)\) and space is \(O(1)\).  
// Edge cases:  
// - \(n = 0\): `fib(n-1)` is not defined, so handle separately by returning 0 for \(S(n-1)\).  
// - \(m = 0\): the sum is just \(F_0 = 0\).  
// - Large values: use `long long` and modulo after every multiplication to avoid overflow.
