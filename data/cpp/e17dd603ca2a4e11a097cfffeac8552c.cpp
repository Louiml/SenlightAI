// Write a C++ function named `power15` that takes a single floating-point argument `x` (of type `float`) and returns `x` raised to the 15th power as a `float`. The function must not use any built-in power functions (like `pow`) or loops; it must compute the result using only multiplication and division, following an efficient exponentiation-by-squaring style similar to the given snippet. Use `const` correctness where appropriate, and assume the input is any non-zero finite `float` (division by `x` is required in the algorithm, so if `x == 0` the behavior is undefined, you may simply return the result of the computation). The function should be standalone and not rely on any external libraries beyond the standard headers needed for the algorithm.
// The core algorithm is exponentiation by squaring to compute `x^15` with minimal multiplications. Since 15 in binary is `1111` (i.e., 15 = 8 + 4 + 2 + 1), we can compute successive squares: `x2 = x*x`, `x4 = x2*x2`, `x8 = x4*x4`. Then `x^15 = x8 * x4 * x2 * x`. However, the original snippet uses a clever trick: compute `x16 = x8*x8` (which is `x^16`) and then divide by `x` to get `x^15`. This avoids an extra multiplication and uses only 5 multiplications (for x2, x4, x8, x16) plus one division. Edge case: if `x == 0`, the division `x16 / x` would cause division by zero; but because `x^15` for zero is zero, we could handle it by returning 0 explicitly if `x` is zero. However, the task says "any non-zero finite float", so we can simply perform the division. The time complexity is O(1) because it uses a constant number of arithmetic operations (4 multiplications and 1 division, plus the initial square for x2). Space complexity is O(1) as well, using only a few local variables.
// Compute x^15 using exponentiation by squaring.
// Assumes x is non-zero to avoid division by zero.
float power15(float x) {
    const float x2 = x * x;       // x^2
    const float x4 = x2 * x2;     // x^4
    const float x8 = x4 * x4;     // x^8
    const float x16 = x8 * x8;    // x^16
    return x16 / x;               // x^16 / x = x^15
}
#include <cassert>
#include <cmath>

int main() {
    // Test with known values
    assert(std::fabs(power15(2.0f) - 32768.0f) < 0.0001f);
    assert(std::fabs(power15(1.0f) - 1.0f) < 0.0001f);
    assert(std::fabs(power15(-1.0f) - (-1.0f)) < 0.0001f);
    assert(std::fabs(power15(0.5f) - std::pow(0.5f, 15)) < 0.0001f);
    assert(std::fabs(power15(-2.0f) - (-32768.0f)) < 0.0001f);
    // Random value comparison with standard pow
    assert(std::fabs(power15(3.7f) - std::pow(3.7f, 15)) < 0.001f);
    // Zero is undefined per spec, but test small positive
    assert(std::fabs(power15(0.1f) - std::pow(0.1f, 15)) < 1e-6f);
    // Negative small value
    assert(std::fabs(power15(-0.2f) - std::pow(-0.2f, 15)) < 1e-6f);
    // Large value
    assert(std::fabs(power15(10.0f) / 1e15f - 1.0f) < 0.0001f);
    return 0;
}
