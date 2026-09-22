/*
Write a C++ function named `computePower32` that accepts a single floating-point value `x` and returns `x` raised to the 32nd power as a `float` — but you are required to compute this without using any loop, recursion, or the `<cmath>` `pow` function. Instead, use repeated squaring with local `const` variables only, mirroring the technique shown in the snippet (multiply `x` by itself to get `x^2`, then square that to get `x^4`, then `x^8`, then `x^16`, then `x^32`). The function must be self-contained, use appropriate `const` correctness, and handle typical floating-point edge cases gracefully (e.g., very large or very small magnitudes, zero, negative values) — note that the result is still a `float`, so precision loss is expected for extreme inputs. The function must not print anything; it should simply return the computed value. Provide a separate test program that calls this function multiple times and verifies the results using `assert` against expected values computed with `std::pow` (using a tolerance or exact comparison where safe). The test must also include a case where the input is `0`, a case where it is `1`, a case with a negative value, and a case with a fractional value.
*/

// Computes x raised to the 32nd power using repeated squaring.
// No loops, recursion, or pow() used — only 5 multiplications.
// All local intermediates are const for clarity and safety.
float computePower32(float x) {
    const float x2 = x * x;       // x^2
    const float x4 = x2 * x2;     // x^4
    const float x8 = x4 * x4;     // x^8
    const float x16 = x8 * x8;    // x^16
    const float x32 = x16 * x16;  // x^32
    return x32;
}

#include <cassert>
#include <cmath>
#include <cfloat>

int main() {
    // Exact cases: 0 and 1 (and -1) yield exact results.
    assert(std::fabs(computePower32(0.0f) - 0.0f) < FLT_EPSILON);
    assert(std::fabs(computePower32(1.0f) - 1.0f) < FLT_EPSILON);
    assert(std::fabs(computePower32(-1.0f) - 1.0f) < FLT_EPSILON);

    // Fractional value: compare with std::pow using a relative tolerance.
    float x = 0.5f;
    float expected = std::pow(x, 32.0f);
    float result = computePower32(x);
    assert(std::fabs(result - expected) <= 1e-6f * std::fabs(expected) + 1e-6f);

    // Small negative fractional value.
    x = -0.25f;
    expected = std::pow(x, 32.0f);
    result = computePower32(x);
    assert(std::fabs(result - expected) <= 1e-6f * std::fabs(expected) + 1e-6f);

    // Larger magnitude (still within float range).
    x = 2.0f;
    expected = std::pow(x, 32.0f);  // 2^32 = 4294967296.0 exactly representable in float
    result = computePower32(x);
    assert(std::fabs(result - expected) < 1.0f);  // exact should be equal

    // Small value near zero.
    x = 0.001f;
    expected = std::pow(x, 32.0f);
    result = computePower32(x);
    assert(std::fabs(result - expected) <= 1e-3f * std::fabs(expected) + 1e-8f);

    return 0;
}

// The solution is straightforward: we compute the power by repeated squaring, reducing the number of multiplications from 31 (naive approach) to just 5 multiplications. Starting from the input `x`, we create `const float x2 = x * x;` which is `x^2`, then `const float x4 = x2 * x2;` (`x^4`), then `x8 = x4 * x4` (`x^8`), `x16 = x8 * x8` (`x^16`), and finally `x32 = x16 * x16` (`x^32`). This matches the algorithm in the snippet exactly, but we wrap it in a function named `computePower32` for reusability. There are no loops or recursion, so the time complexity is O(1) — always 5 multiplications regardless of input. The space complexity is O(1) because we only store a constant number of local variables. Edge cases: if `x` is `0`, the result is `0` (since `0^32 = 0`). If `x` is `1` or `-1`, the result is `1` (because `(-1)^32 = 1`). If `x` is very large (e.g., `1e10`), `x^32` will overflow to infinity for `float`; that's acceptable because we're not asked to handle overflow specially. For fractional inputs, repeated squaring is mathematically correct, but floating-point rounding may slightly differ from `std::pow` due to different orders of operations; hence in tests we should compare with `std::pow(x, 32.0f)` using a small tolerance (e.g., `fabs(a - b) < 1e-3 * fabs(a)`) or use exact equality for cases like `0` and `1` where precision is exact. The function should be marked `const`-correct by making all local variables `const` where appropriate.
