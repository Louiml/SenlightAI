Write a C++ function that accepts a positive integer `n` and returns the sum of powers of 2 from \(2^1\) through \(2^n\) inclusive (i.e., \(2^1 + 2^2 + \dots + 2^n\)). The function must handle large values of `n` (up to 63) without overflow by returning an `unsigned long long`. The function must not use any built-in power functions and must implement its own fast exponentiation (exponentiation by squaring) for each term. Edge cases: if `n` is 0, the sum should be 0. If `n` is 1, the sum is 2. The function should be named `sumOfPowersOfTwo` and take the exponent as a parameter.
The main algorithm iterates from exponent 1 to `n` and for each exponent computes \(2^k\) using a custom fast power function that repeatedly squares the base and multiplies when the exponent is odd. This method runs in \(O(\log k)\) time per term, leading to \(O(n \log n)\) total time. However, since `n` is at most 63, this is trivial. The space complexity is \(O(1)\) as we only use a few variables. Edge cases: `n=0` returns 0 (empty sum), `n=1` returns 2. For `n=63`, the maximum term \(2^{63}\) fits in `unsigned long long` (max value \(2^{64}-1\)), and the sum of all powers from 1 to 63 is \(2^{64}-2\), which fits. If `n` were 64, the sum would overflow, so the function should document that valid input is `n ≤ 63`.
#include <cstdint>

// Returns the sum of 2^1 + 2^2 + ... + 2^n for n >= 0, n <= 63.
unsigned long long sumOfPowersOfTwo(unsigned long long n) {
    unsigned long long sum = 0;
    for (unsigned long long exp = 1; exp <= n; ++exp) {
        // Fast exponentiation: compute base^exp
        unsigned long long base = 2;
        unsigned long long power = exp;
        unsigned long long result = 1;
        while (power > 0) {
            if (power % 2 == 1) {
                result *= base;
                --power;
            } else {
                base *= base;
                power /= 2;
            }
        }
        sum += result;
    }
    return sum;
}
#include <cassert>

// The solution function is declared above in the solution section.
// This main function tests the function directly.
int main() {
    assert(sumOfPowersOfTwo(0) == 0);
    assert(sumOfPowersOfTwo(1) == 2);
    assert(sumOfPowersOfTwo(2) == 2 + 4);          // 6
    assert(sumOfPowersOfTwo(3) == 2 + 4 + 8);      // 14
    assert(sumOfPowersOfTwo(4) == 2 + 4 + 8 + 16); // 30
    assert(sumOfPowersOfTwo(5) == 62);
    assert(sumOfPowersOfTwo(10) == 2046);
    assert(sumOfPowersOfTwo(63) == 18446744073709551614ULL); // 2^64 - 2
    return 0;
}
