Write a standalone C++ function that solves a quadratic equation of the form `a*x^2 + b*x + c = 0` for real coefficients `a`, `b`, and `c`, with `a` not equal to zero. The function must take four float parameters: the three coefficients and two output references for the roots. It should throw a `std::logic_error` with an appropriate message when the discriminant is negative (no real roots), and it should return the two real roots in the output references. If the discriminant is zero, both roots should be set to the same value. The function must handle edge cases such as very small or very large coefficients safely using floating-point arithmetic, and it should not read from standard input or write to standard output. The function should be declared with `const` correctness where applicable and include necessary headers.

#include <cassert>
#include <cmath>
#include <stdexcept>

// Declaration of the solution function (copy from above solution).
void solveQuadratic(const float a, const float b, const float c, float& x1, float& x2);

int main() {
    // Test 1: Standard distinct roots
    float x1, x2;
    solveQuadratic(1.0f, -3.0f, 2.0f, x1, x2); // x^2 -3x +2 => roots 1 and 2
    assert(std::fabs(x1 - 1.0f) < 1e-4 && std::fabs(x2 - 2.0f) < 1e-4);

    // Test 2: Double root
    solveQuadratic(1.0f, -2.0f, 1.0f, x1, x2); // (x-1)^2
    assert(std::fabs(x1 - 1.0f) < 1e-4 && std::fabs(x2 - 1.0f) < 1e-4);

    // Test 3: Negative coefficients and roots
    solveQuadratic(1.0f, 1.0f, -2.0f, x1, x2); // x^2 + x -2 => roots 1 and -2
    assert(std::fabs(x1 - 1.0f) < 1e-4 && std::fabs(x2 + 2.0f) < 1e-4);

    // Test 4: b == 0 with distinct roots
    solveQuadratic(1.0f, 0.0f, -4.0f, x1, x2); // x^2 -4 => roots 2 and -2
    assert(std::fabs(x1 - 2.0f) < 1e-4 && std::fabs(x2 + 2.0f) < 1e-4);

    // Test 5: a negative
    solveQuadratic(-1.0f, 2.0f, 3.0f, x1, x2); // -x^2 +2x +3 => roots 3 and -1
    assert(std::fabs(x1 - 3.0f) < 1e-4 || std::fabs(x1 + 1.0f) < 1e-4);
    assert(std::fabs(x2 - (-1.0f)) < 1e-4 || std::fabs(x2 - 3.0f) < 1e-4);

    // Test 6: Large coefficients (stability check)
    solveQuadratic(1e7f, 1e7f, 1.0f, x1, x2);
    // Roots are approximately -1 and -1e-7 (but with float may lose precision)
    // Just check that no exception and both finite
    assert(std::isfinite(x1) && std::isfinite(x2));

    // Test 7: Discriminant near zero (almost double root)
    solveQuadratic(1.0f, 2.0f, 0.999999f, x1, x2);
    assert(std::fabs(x1 - (-1.0f)) < 1e-3 && std::fabs(x2 - (-1.0f)) < 1e-3);

    // Test 8: a=0 should throw
    bool threw = false;
    try {
        solveQuadratic(0.0f, 1.0f, 1.0f, x1, x2);
    } catch (const std::logic_error&) {
        threw = true;
    }
    assert(threw);

    // Test 9: Negative discriminant should throw
    threw = false;
    try {
        solveQuadratic(1.0f, 0.0f, 1.0f, x1, x2); // x^2 +1
    } catch (const std::logic_error&) {
        threw = true;
    }
    assert(threw);

    // Test 10: Very small discriminant but positive
    solveQuadratic(1.0f, 2.0f, 0.9999999f, x1, x2); // discriminant ~ 4 - 4*0.9999999 ≈ 4e-7
    assert(std::fabs(x1 + 1.0f) < 1e-2 && std::fabs(x2 + 1.0f) < 1e-2);

    return 0;
}

#include <cmath>
#include <stdexcept>

// Solves a*x^2 + b*x + c = 0 for real roots.
// Throws std::logic_error if a == 0 or if discriminant < 0.
void solveQuadratic(const float a, const float b, const float c, float& x1, float& x2) {
    if (a == 0.0f) {
        throw std::logic_error("Coefficient a cannot be zero for a quadratic equation.");
    }

    const float discriminant = b * b - 4.0f * a * c;

    if (discriminant < 0.0f) {
        throw std::logic_error("No real roots: discriminant is negative.");
    }

    const float sqrtDiscriminant = std::sqrt(discriminant);
    const float twoA = 2.0f * a;

    // Stable form avoids cancellation when b is large.
    // Compute q = -0.5 * (b + sign(b)*sqrtD) or if b==0, use sqrtD.
    const float bSign = (b >= 0.0f) ? 1.0f : -1.0f;
    const float q = -0.5f * (b + bSign * sqrtDiscriminant);

    if (q != 0.0f) {
        x1 = q / a;
        x2 = c / q;
    } else {
        // When b == 0, discriminant = -4ac >=0 => c and a have opposite signs.
        x1 = sqrtDiscriminant / twoA;
        x2 = -sqrtDiscriminant / twoA;
    }

    // If discriminant is exactly zero, both roots are the same.
    if (discriminant == 0.0f) {
        x1 = -b / twoA;
        x2 = x1;
    }
}

// The solution computes the discriminant `D = b*b - 4*a*c`. If `D < 0`, throw a `std::logic_error` with message like "no real roots". Otherwise, compute `sqrtD = sqrt(D)`. Standard formula: `x1 = (-b + sqrtD) / (2*a)`, `x2 = (-b - sqrtD) / (2*a)`. However, to avoid cancellation when `b` is large relative to `a` and `c`, a more stable approach computes `q = -0.5 * (b + copysign(sqrtD, b))`, then `x1 = q / a` and `x2 = c / q` (if `q` is not zero). For this task, the simpler formula is acceptable, but we can mention the stable variant. Edge cases: if `a` is zero, it's not a quadratic; the function should throw a `std::logic_error`. If `D == 0`, both roots equal `-b/(2*a)`. Time complexity is O(1), space O(1). Use `float` as specified, but be aware of precision; for robustness we can use `double` internally, but the signature uses `float`. The function must be `const`? Actually it modifies output references, so it cannot be `const` member, but as a free function, we can make parameters `const float` for inputs. Output references are non-const. We'll declare inputs as `const float` and outputs as `float&`.
