/*
Write a C++ function that, given a positive integer `n`, returns the number of trailing zeros in the decimal representation of `n!` (the factorial of `n`). Your function must handle large values of `n` efficiently (up to \(10^{18}\)) and return the result as an unsigned 64-bit integer. The input can be any unsigned 64-bit integer greater than zero. The factorial itself may be astronomically large, so you must not compute it directly; instead, determine the count of factors of 10 (i.e., pairs of factors 2 and 5) in the prime factorization of `n!`. Since factors of 2 are always more abundant than factors of 5 in any factorial, the number of trailing zeros equals the exponent of 5 in `n!`. You must also handle the edge case where `n` is 0 or 1 (both have 0 trailing zeros).
*/
#include <cstdint>

// Returns the number of trailing zeros in the decimal representation of n!.
// Input: positive integer n (0 <= n <= 10^18). Handles n = 0 and n = 1 correctly.
// Uses Legendre's formula: sum_{k>=1} floor(n / 5^k).
uint64_t factorialTrailingZeros(uint64_t n) {
    uint64_t count = 0;
    uint64_t divisor = 5;

    // Avoid overflow: while divisor <= n, but compute divisor * 5 safely.
    while (divisor <= n) {
        count += n / divisor;

        // If divisor is about to overflow when multiplied by 5, break.
        if (divisor > n / 5) {
            break;
        }
        divisor *= 5;
    }

    return count;
}
#include <cassert>
#include <cstdint>

// Declare the function (in a real project this would be in a header).
uint64_t factorialTrailingZeros(uint64_t n);

int main() {
    // Basic small factorials
    assert(factorialTrailingZeros(0) == 0);   // 1! (or 0!) = 1, no zeros
    assert(factorialTrailingZeros(1) == 0);   // 1! = 1
    assert(factorialTrailingZeros(5) == 1);   // 120
    assert(factorialTrailingZeros(10) == 2);  // 3628800
    assert(factorialTrailingZeros(20) == 4);  // 2432902008176640000
    // Multiples of 25 give extra zeros
    assert(factorialTrailingZeros(25) == 6);  // 15511210043330985984000000
    assert(factorialTrailingZeros(100) == 24);
    // Large value (10^18)
    assert(factorialTrailingZeros(1000000000000000000ULL) == 249999999999999996ULL);
    // Edge case where divisor exactly hits n
    assert(factorialTrailingZeros(125) == 31); // floor(125/5)=25 + 5 + 1 = 31
    // n just below a power of 5
    assert(factorialTrailingZeros(124) == 28); // 24 + 4 = 28
    // Two consecutive large values to ensure no overflow
    assert(factorialTrailingZeros(1000000000000000000ULL - 1) == 249999999999999995ULL);
    assert(factorialTrailingZeros(1ULL << 60) == 259614842926741381ULL); // 2^60 ≈ 1.15e18
}
// The key insight is that trailing zeros in `n!` come from factors of 10 = 2 × 5. For every factor of 5 in the prime factorization, there is a matching factor of 2 (since even numbers are far more frequent than multiples of 5), so the count of trailing zeros equals the number of times 5 divides `n!`. That count is given by Legendre’s formula: sum over k of floor(n / 5^k) for k = 1, 2, 3, …, until 5^k > n. For example, for n=25, we get floor(25/5)=5 and floor(25/25)=1, total 6. This works because each multiple of 5 contributes at least one factor of 5, each multiple of 25 contributes an extra one, etc. The algorithm iterates by multiplying a divisor variable by 5 each step, which for n up to 10^18 requires at most about log_5(10^18) ≈ 26 iterations, so time is O(log n). Space is O(1). Edge cases: n=0 or n=1 produce 0 because there are no multiples of 5. Also, since the input is a 64-bit unsigned integer, 5^k could overflow during multiplication if k is too large; to avoid overflow, we break early when the divisor exceeds n / 5 (or check before multiplying).
