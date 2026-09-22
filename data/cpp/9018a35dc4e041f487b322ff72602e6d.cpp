Write a C++ function that takes a non-negative integer `n` and returns the number of trailing zeros in its factorial representation, but using the arbitrary-precision integer type `boost::multiprecision::cpp_int` to avoid overflow. The function should handle inputs where `n` can be as large as 10^9, but note that the factorial itself is too large to compute directly for large `n`; instead, you must compute the count of trailing zeros analytically. If the input is negative, throw an `std::invalid_argument` exception. Your function should be named `factorialTrailingZeros` and accept the input as a `long long` (or `int64_t`). Do not include a `main` function in your solution.
The number of trailing zeros in `n!` is equal to the number of times 10 divides `n!`, which is the minimum of the exponents of 2 and 5 in its prime factorization. Since there are always more factors of 2 than 5 in any factorial, the count equals the exponent of 5 in `n!`. That exponent is computed by repeatedly dividing `n` by 5 and summing the integer quotients: `n/5 + n/25 + n/125 + ...` until the quotient becomes zero. This works because each multiple of 5 contributes at least one factor of 5, each multiple of 25 contributes an extra, and so on. Edge cases: `n=0` and `n=1` both return 0 because 0! and 1! have no trailing zeros. Negative input throws an exception. The algorithm runs in O(log_5 n) time, which is effectively O(log n) for practical inputs, and uses O(1) auxiliary space, since no large numbers are stored—only the input integer and the accumulator, both of which fit in standard integer types.
#include <stdexcept>
#include <cstdint>

// Compute the number of trailing zeros in n! for a non-negative integer n.
// Throws std::invalid_argument if n is negative.
long long factorialTrailingZeros(long long n) {
    if (n < 0) {
        throw std::invalid_argument("n must be non-negative");
    }
    long long count = 0;
    while (n >= 5) {
        n /= 5;
        count += n;
    }
    return count;
}
#include <cassert>
#include <stdexcept>

int main() {
    // Basic cases
    assert(factorialTrailingZeros(0) == 0);
    assert(factorialTrailingZeros(1) == 0);
    assert(factorialTrailingZeros(5) == 1);
    assert(factorialTrailingZeros(10) == 2);
    assert(factorialTrailingZeros(25) == 6);
    assert(factorialTrailingZeros(100) == 24);
    assert(factorialTrailingZeros(125) == 31);
    assert(factorialTrailingZeros(1000) == 249);

    // Large input (up to 1e9) works without overflow
    assert(factorialTrailingZeros(1000000000LL) == 249999998LL);

    // Negative input throws
    bool threw = false;
    try {
        factorialTrailingZeros(-1);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);
}
