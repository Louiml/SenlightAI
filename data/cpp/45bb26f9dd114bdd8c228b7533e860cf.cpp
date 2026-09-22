// Write a C++ function that, given a positive integer `n`, returns the sum of the squares of the first `n` natural numbers (i.e., \(1^2 + 2^2 + \dots + n^2\)). The function must use the closed-form formula \(\frac{n(n+1)(2n+1)}{6}\) to compute the result efficiently. The input `n` can be as large as \(10^6\), and the result fits within a 64-bit unsigned integer. The function should handle `n = 0` gracefully by returning `0`, and must not overflow during intermediate multiplication (ensure you do the multiplication in a safe order or use a 128-bit intermediate if necessary, but since \(n \le 10^6\), \(n(n+1)(2n+1)\) is about \(2\times 10^{18}\), which is safely within 64-bit range). The function should be `const`-correct and take the input by value. Provide a separate test harness that verifies the function against known values for several inputs.

The problem is a classic summation of squares. The closed-form formula \(\text{sum} = \frac{n(n+1)(2n+1)}{6}\) directly gives the answer in \(O(1)\) time and \(O(1)\) auxiliary space. The main edge cases are: (1) when `n = 0`, the result should be `0` — the formula also gives `0` because the numerator becomes zero, but we should return explicitly for clarity; (2) when `n` is large, intermediate multiplication must not overflow. For `n = 10^6`, `n(n+1)(2n+1) = 10^6 * 1,000,001 * 2,000,001 ≈ 2.000003e18`, which is less than the max of `unsigned long long` (~1.84e19). Division by 6 is performed after multiplication, and since the product is always divisible by 6, integer division is exact. Time complexity is \(O(1)\), space complexity is \(O(1)\). No loops are needed, making it optimal.

#include <cstdint>

// Compute the sum of squares from 1 to n using the closed-form formula.
// n must be non-negative. Returns 0 for n == 0.
unsigned long long sumOfSquares(unsigned long long n) {
    if (n == 0) {
        return 0;
    }
    // Compute n*(n+1)*(2n+1) first, then divide by 6.
    // All terms fit in 64-bit for n up to 10^6.
    unsigned long long term1 = n;
    unsigned long long term2 = n + 1;
    unsigned long long term3 = 2 * n + 1;
    return term1 * term2 * term3 / 6;
}

#include <cassert>

int main() {
    // Test small known values
    assert(sumOfSquares(0) == 0ULL);
    assert(sumOfSquares(1) == 1ULL);
    assert(sumOfSquares(2) == 5ULL);
    assert(sumOfSquares(3) == 14ULL);
    assert(sumOfSquares(4) == 30ULL);
    assert(sumOfSquares(5) == 55ULL);
    assert(sumOfSquares(10) == 385ULL);
    // Test a larger value: sum of squares up to 1000
    // 1000*1001*2001/6 = 333833500
    assert(sumOfSquares(1000) == 333833500ULL);
    // Test a very large value within limits
    assert(sumOfSquares(1000000) == 333333833333500000ULL);
    return 0;
}
