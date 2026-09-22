// Write a C++ function `lastDigitOfFibonacciSum` that takes a non-negative integer `n` (0 ≤ n ≤ 10^18) and returns the last decimal digit of the sum of the first `n` Fibonacci numbers (F(1) + F(2) + ... + F(n)), where F(1) = 1, F(2) = 1, and F(k) = F(k-1) + F(k-2) for k ≥ 3. For n = 0, the sum is 0. The function must handle very large `n` efficiently, using matrix exponentiation modulo 10 (since only the last digit is needed). The result should be an integer between 0 and 9. Do not compute Fibonacci numbers directly.

#include <cassert>
#include <cstdint>

// Assume the function declaration is above.
int main() {
    // Small known values
    assert(lastDigitOfFibonacciSum(0) == 0); // sum of first 0 = 0
    assert(lastDigitOfFibonacciSum(1) == 1); // 1
    assert(lastDigitOfFibonacciSum(2) == 2); // 1+1=2
    assert(lastDigitOfFibonacciSum(3) == 4); // 1+1+2=4
    assert(lastDigitOfFibonacciSum(4) == 7); // 1+1+2+3=7
    assert(lastDigitOfFibonacciSum(5) == 2); // 1+1+2+3+5=12 -> last digit 2
    assert(lastDigitOfFibonacciSum(6) == 0); // 1+1+2+3+5+8=20 -> 0
    // Large n, verifying the periodic mod 10 pattern (period 60 for Fibonacci, but sum has period 60 too)
    // S(60) mod 10 = ? Known F(62) = 4052739537881, last digit 1, F(62)-1 last digit 0? Actually F(62)-1 = 4052739537880 -> last digit 0.
    assert(lastDigitOfFibonacciSum(60) == 0);
    // Very large n, ensure no overflow and works
    assert(lastDigitOfFibonacciSum(1000000000000000000LL) >= 0 && lastDigitOfFibonacciSum(1000000000000000000LL) <= 9);
    // Check consistency: For any n, lastDigitOfFibonacciSum(n) should equal (F(n+2)-1) mod 10, but we can check small n via direct loop.
    long long fib_prev = 0, fib_cur = 1, sum = 0;
    for (int i = 1; i <= 20; ++i) {
        sum += fib_cur;
        assert(lastDigitOfFibonacciSum(i) == static_cast<int>(sum % 10));
        long long next = fib_prev + fib_cur;
        fib_prev = fib_cur;
        fib_cur = next;
    }
    return 0;
}

#include <cstdint>

// Compute the last digit of the sum of the first n Fibonacci numbers.
int lastDigitOfFibonacciSum(std::int64_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 2;

    // Standard Fibonacci matrix: [[1,1],[1,0]]
    // We need F(n+2) mod 10. Compute M^(n+1) and take top-left.
    // But to align with the snippet, we use n-2 exponent on a matrix that gives F(n+1), F(n).
    // However, we'll compute M^(n+1) directly for clarity.

    // Matrix multiplication modulo 10
    auto mul = [](const long long a[2][2], const long long b[2][2], long long out[2][2]) {
        long long c[2][2] = {{0,0},{0,0}};
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                for (int k = 0; k < 2; ++k)
                    c[i][j] = (c[i][j] + a[i][k] * b[k][j]) % 10;
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                out[i][j] = c[i][j];
    };

    long long base[2][2] = {{1,1},{1,0}};
    long long result[2][2] = {{1,0},{0,1}}; // identity
    std::int64_t exp = n + 1; // because M^(n+1) gives [F(n+2), F(n+1)] with start [1,0]
    while (exp > 0) {
        if (exp & 1) {
            long long temp[2][2];
            mul(result, base, temp);
            for (int i=0;i<2;i++) for (int j=0;j<2;j++) result[i][j] = temp[i][j];
        }
        long long temp[2][2];
        mul(base, base, temp);
        for (int i=0;i<2;i++) for (int j=0;j<2;j++) base[i][j] = temp[i][j];
        exp >>= 1;
    }
    // result[0][0] is F(n+2) mod 10
    long long f_n_plus_2 = result[0][0];
    // S(n) = F(n+2) - 1
    int ans = (f_n_plus_2 - 1) % 10;
    if (ans < 0) ans += 10;
    return ans;
}

// The sum of the first n Fibonacci numbers has a known identity: S(n) = F(n+2) - 1. Therefore, the last digit of S(n) equals (F(n+2) - 1) mod 10. We can compute F(n+2) mod 10 efficiently using matrix exponentiation. The Fibonacci recurrence can be represented as a 2×2 matrix M = [[1,1],[1,0]], and M^(k) * [F(1), F(0)]^T = [F(k+1), F(k)]^T, with F(0)=0. To compute F(n+2) mod 10, we compute M^(n+1) and take the top-left element (which equals F(n+2) for our starting vector). However, to keep the function simple and directly matching the snippet's logic (which uses a base matrix for F(1)=1, F(2)=1), we can set up the matrix such that M^(n-1) gives F(n+1) and F(n). For n≥1, we compute M^(n-1) and then F(n+2) = F(n+1)+F(n). But we can also just compute M^(n+1) starting from the standard F(0), F(1) pair. Edge cases: n=0 gives 0; n=1 gives 1; n=2 gives 2 (1+1). For all n, we reduce modulo 10 at each matrix multiplication to avoid overflow and keep numbers small. Time complexity is O(log n) due to fast exponentiation, space complexity O(1). The identity S(n)=F(n+2)-1 works for n≥1; for n=0, the sum is 0, and our formula gives F(2)-1 = 1-1=0, so it still works if we define F(2)=1. Actually F(2)=1, so S(0)=0 works. So we can use the closed form for all n≥0.
