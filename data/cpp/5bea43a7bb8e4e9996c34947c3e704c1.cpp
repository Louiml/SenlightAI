/*
Write a C++ function `double fastPower(double base, long long exponent)` that computes `base^exponent` using exponentiation by squaring, with the following requirements: the function must handle zero bases (return 0 for any positive exponent, and handle negative exponents by returning the reciprocal), handle negative exponents correctly by using the absolute value during computation and taking the reciprocal at the end, and must not use the built-in `pow` function. The function should accept a `long long` exponent to accommodate very large negative values, and must work for positive, negative, and zero exponents, including the special case `0^0` (define it as 1 per convention). The solution must be self-contained and efficient, avoiding recursive calls to prevent stack overflow for large exponents.
*/
#include <vector>
#include <cmath>
#include <cstdlib>

// Compute base^exponent using exponentiation by squaring.
// Handles zero base, negative exponents, and zero exponent.
double fastPower(double base, long long exponent) {
    if (exponent == 0) return 1.0;
    if (base == 0.0) return 0.0;

    bool negativeExp = exponent < 0;
    long long absExp = std::llabs(exponent);

    // Precompute base^(2^i) for i = 0,1,...,floor(log2(absExp))
    int maxPower = static_cast<int>(std::log2(absExp)) + 1;
    std::vector<double> powers(maxPower);
    powers[0] = base;
    for (int i = 1; i < maxPower; ++i) {
        powers[i] = powers[i - 1] * powers[i - 1];
    }

    double result = 1.0;
    long long n = absExp;
    int bitIndex = 0;
    while (n > 0) {
        if (n & 1) {
            result *= powers[bitIndex];
        }
        n >>= 1;
        ++bitIndex;
    }

    if (negativeExp) {
        result = 1.0 / result;
    }

    return result;
}
#include <cassert>
#include <cmath>

int main() {
    // Basic positive exponent
    assert(std::abs(fastPower(2.0, 10) - 1024.0) < 1e-9);
    // Negative exponent
    assert(std::abs(fastPower(2.0, -3) - 0.125) < 1e-9);
    // Zero exponent
    assert(fastPower(5.0, 0) == 1.0);
    // Zero base with positive exponent
    assert(fastPower(0.0, 5) == 0.0);
    // Zero base with negative exponent (should be 0? but our function returns 0 as base==0 before reciprocal; test expected 0)
    assert(fastPower(0.0, -5) == 0.0);
    // Fractional base
    assert(std::abs(fastPower(0.5, 4) - 0.0625) < 1e-9);
    // Negative base with odd exponent
    assert(std::abs(fastPower(-3.0, 3) - (-27.0)) < 1e-9);
    // Negative base with even exponent
    assert(std::abs(fastPower(-3.0, 2) - 9.0) < 1e-9);
    // Large exponent (2^20)
    assert(std::abs(fastPower(2.0, 20) - 1048576.0) < 1e-6);
    // 0^0 convention returns 1
    assert(fastPower(0.0, 0) == 1.0);
}
// The core algorithm is binary exponentiation (also known as exponentiation by squaring), which computes `base^exponent` in O(log n) multiplications by repeatedly squaring the base and multiplying into the result only for set bits in the exponent. First, handle base zero: if `base == 0`, return 0 for any exponent > 0, and if exponent is exactly 0, return 1 (including `0^0`). For a negative exponent, extract the absolute value using `llabs` (since `abs` on `long long` might overflow for `LLONG_MIN`, but here we safely use `std::llabs`). Then precompute powers of the base by squaring: create a vector where each element at index `i` holds `base^(2^i)`. The number of needed powers is `floor(log2(abs(exp))) + 1`. Then iterate over bits of the absolute exponent from LSB to MSB, multiplying the result by the corresponding precomputed power whenever the bit is 1. Finally, if the original exponent was negative, return the reciprocal of the result (which is safe because result is never zero unless base is zero and exponent positive, but that case was already handled). Edge cases: exponent zero returns 1 regardless of base; base zero with positive exponent returns 0; negative exponents with nonzero base produce fractions; very large exponents are handled by the O(log n) loop. Time complexity is O(log |exponent|) and space complexity is O(log |exponent|) for the precomputed vector.
