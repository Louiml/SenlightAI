// Write a C++ function `solveQuadratic` that takes three `float` parameters representing the coefficients `a`, `b`, and `c` of a quadratic equation \( ax^2 + bx + c = 0 \), and returns a `std::pair<float, float>` containing the two real roots \( x_1 \) and \( x_2 \) using the standard quadratic formula: \( x = \frac{-b \pm \sqrt{b^2 - 4ac}}{2a} \). Assume that the discriminant is non-negative and `a` is non-zero so that real roots always exist (no need to handle complex roots, division by zero, or invalid inputs). But you must handle the edge case where the discriminant is exactly zero (both roots are equal) by returning the same value for both elements of the pair. The function must be `const`-correct and self-contained (including all necessary headers). Do not write a `main` function; only the free function definition is required.
The solution directly applies the quadratic formula. First, compute the discriminant \( D = b^2 - 4ac \). Since the task guarantees `D >= 0` and `a != 0`, we can safely take the square root using `std::sqrt`. Then compute `x1 = (-b + sqrt(D)) / (2*a)` and `x2 = (-b - sqrt(D)) / (2*a)`. If `D == 0`, both roots are identical; to handle floating-point precision issues, we can optionally use an epsilon comparison, but for simplicity and given typical float inputs, checking `D == 0` is acceptable (since the discriminant is computed from given floats, exact equality is reasonable for exact zero). However, a more robust approach is to compute `x1` and `x2` as usual and then, when `D` is close to zero (e.g., within a small tolerance like `1e-6`), set `x2 = x1` to avoid tiny differences. For the task, exact equality is fine because inputs are given as exact floats. The algorithm uses constant time \( O(1) \) and constant space \( O(1) \). Edge cases: `D == 0` gives equal roots; `a` negative or positive does not affect the formula since `2*a` is used; negative `b` is handled by the `-b` term.
#include <cmath>
#include <utility>

// Solve ax^2 + bx + c = 0 for real roots, given a != 0 and discriminant >= 0.
// Returns the two roots as a pair (x1, x2), where x1 >= x2 (or equal when discriminant is zero).
std::pair<float, float> solveQuadratic(float a, float b, float c) {
    const float discriminant = b * b - 4.0f * a * c;
    const float sqrtDisc = std::sqrt(discriminant);
    const float denom = 2.0f * a;
    
    float x1 = (-b + sqrtDisc) / denom;
    float x2 = (-b - sqrtDisc) / denom;
    
    // Ensure x1 is the larger root, and for zero discriminant they are equal.
    if (x1 < x2) {
        std::swap(x1, x2);
    }
    return std::make_pair(x1, x2);
}
int main() {
    // x^2 - 5x + 6 = 0 -> roots 3 and 2
    auto r1 = solveQuadratic(1.0f, -5.0f, 6.0f);
    assert(r1.first == 3.0f && r1.second == 2.0f);

    // x^2 - 2x + 1 = 0 -> double root 1
    auto r2 = solveQuadratic(1.0f, -2.0f, 1.0f);
    assert(r2.first == 1.0f && r2.second == 1.0f);

    // 2x^2 + 4x + 2 = 0 -> double root -1
    auto r3 = solveQuadratic(2.0f, 4.0f, 2.0f);
    assert(r3.first == -1.0f && r3.second == -1.0f);

    // x^2 - 4 = 0 -> roots 2 and -2
    auto r4 = solveQuadratic(1.0f, 0.0f, -4.0f);
    assert(r4.first == 2.0f && r4.second == -2.0f);

    // -x^2 + 4x - 3 = 0 -> roots 3 and 1 (multiply by -1 gives x^2 -4x +3)
    auto r5 = solveQuadratic(-1.0f, 4.0f, -3.0f);
    assert(r5.first == 3.0f && r5.second == 1.0f);

    // 1e-8 precision test: x^2 - 3x + 2.25 = 0 -> roots 1.5 and 1.5
    auto r6 = solveQuadratic(1.0f, -3.0f, 2.25f);
    assert(std::abs(r6.first - 1.5f) < 1e-6 && std::abs(r6.second - 1.5f) < 1e-6);

    // Large coefficients: 1000x^2 - 1000x + 0 = 0 -> roots 1 and 0
    auto r7 = solveQuadratic(1000.0f, -1000.0f, 0.0f);
    assert(r7.first == 1.0f && r7.second == 0.0f);
}
