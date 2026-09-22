Write a C++ function that reads a real number `N` and an integer `T` from standard input, and returns the value of `N` multiplied by 1.000000011 raised to the power of `T`. Because repeated floating-point multiplication can be slow for very large `T`, your function must compute the result using a precomputed block of 1000 multiplications (i.e., multiply by `1.000000011` exactly 1000 times to get a block factor), then apply full blocks and a final remainder efficiently, mimicking the provided snippet’s logic. The input values can be any finite positive or negative real number for `N`, and `T` is a non-negative integer (including zero). The result must be printed with exactly 8 digits after the decimal point (using `printf("%.8f")` or equivalent). Your solution must implement a self-contained function with a descriptive name, and handle edge cases such as `T = 0` (returning `N` unchanged) and very large `T` (up to at least 10^9) without timing out. For floating-point comparisons in tests, use a tolerance of 1e-6 relative or absolute error.

The core challenge is that the brute-force approach of multiplying `T` times by `1.000000011` is linear in `T`, which fails for large `T` (e.g., 10^9). The snippet uses a clever block decomposition: precompute `ten` as the product of `1.000000011` repeated 1000 times, then repeatedly apply this block to reduce the number of multiplications. Specifically, let `factor = 1.000000011`. Precompute `block = factor^1000` by multiplying `factor` with itself 1000 times (constant cost). Then, while `T > 0`, if `T >= 1000`, multiply the current result by `block` and subtract 1000 from `T`; otherwise multiply by `factor` and decrement `T` by 1. This reduces the number of multiplications to roughly `T/1000 + 1000` (the latter from precomputation), which is O(T/1000) time. The algorithm is correct because multiplication is commutative and associative; applying `block` is exactly equivalent to multiplying by `factor^1000`. Edge cases: `T = 0` returns `N` unchanged. Negative or zero `N` works fine because multiplication is valid. For large `T` (up to 10^9) and 1000 per block, the loop runs at most 1,000,000 block applications plus up to 999 remainder multiplications, which is efficient. Time complexity is O(T/1000) ≈ O(T) but with a constant divisor of 1000, and space complexity is O(1). Floating-point accumulation errors are unavoidable, but the tests will use a tolerance to accommodate them.

#include <iostream>
#include <cstdio>

// Precompute a block factor: multiply 1.000000011 by itself 1000 times.
// Then apply the block to the input as many times as possible, then apply
// individual multiplications for the remainder.
double applyGrowth(double initial, int repetitions) {
    const double factor = 1.000000011;
    const int blockSize = 1000;

    // Precompute factor^1000 once.
    double block = 1.0;
    for (int i = 0; i < blockSize; ++i) {
        block *= factor;
    }

    double result = initial;
    int remaining = repetitions;

    while (remaining > 0) {
        if (remaining >= blockSize) {
            result *= block;
            remaining -= blockSize;
        } else {
            result *= factor;
            --remaining;
        }
    }

    return result;
}

#include <cassert>
#include <cmath>

int main() {
    // T = 0 returns N unchanged.
    assert(std::fabs(applyGrowth(5.0, 0) - 5.0) < 1e-6);

    // Simple small case: 1 * (1.000000011)^10
    double expected_small = 1.0;
    for (int i = 0; i < 10; ++i) expected_small *= 1.000000011;
    assert(std::fabs(applyGrowth(1.0, 10) - expected_small) < 1e-6);

    // Negative N works.
    double expected_neg = -3.0;
    for (int i = 0; i < 100; ++i) expected_neg *= 1.000000011;
    assert(std::fabs(applyGrowth(-3.0, 100) - expected_neg) < 1e-6);

    // Large T (100000) should match repeated multiplication but is still testable.
    double expected_large = 1.0;
    for (int i = 0; i < 100000; ++i) expected_large *= 1.000000011;
    assert(std::fabs(applyGrowth(1.0, 100000) - expected_large) < 1e-6);

    // T exactly a multiple of block size (2000) – compare to brute force.
    double expected_block = 2.0;
    for (int i = 0; i < 2000; ++i) expected_block *= 1.000000011;
    assert(std::fabs(applyGrowth(2.0, 2000) - expected_block) < 1e-6);

    // T slightly above block size (1500) – remainder handled correctly.
    double expected_mixed = 4.0;
    for (int i = 0; i < 1500; ++i) expected_mixed *= 1.000000011;
    assert(std::fabs(applyGrowth(4.0, 1500) - expected_mixed) < 1e-6);

    // Verify with a moderately large T and non-integer N.
    double expected_random = 123.456;
    for (int i = 0; i < 777; ++i) expected_random *= 1.000000011;
    assert(std::fabs(applyGrowth(123.456, 777) - expected_random) < 1e-6);

    // Ensure the function doesn't crash for very large T (e.g., 1e9) – just check it returns finite.
    double result_huge = applyGrowth(1.0, 1000000000);
    assert(std::isfinite(result_huge));

    return 0;
}
