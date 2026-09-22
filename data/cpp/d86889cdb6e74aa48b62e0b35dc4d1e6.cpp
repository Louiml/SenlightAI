// Write a C++ function named `countRealRoots` that takes four double-precision floating-point coefficients (`a`, `b`, `c`, `d`) representing the cubic polynomial \(a x^3 + b x^2 + c x + d = 0\) and returns the number of distinct real roots (counting multiplicity as separate roots, such that a double root contributes 2). The function must handle degenerate cases: if `|a|` is below a tolerance of \(1 \times 10^{-12}\), it should reduce to a quadratic (or linear or constant) root-counting problem accordingly. The function should use a robust algebraic method (not numerical root-finding) and account for floating-point comparisons using a tolerance of \(1 \times 10^{-9}\) when determining whether roots are distinct. It must never produce a negative count, and it must correctly handle cases where all coefficients are effectively zero (returning 0 since infinitely many roots are not countable). The solution should not depend on external libraries beyond standard headers.
#include <cassert>
#include <cmath>

int main() {
    // Basic cubic with three distinct real roots: x^3 - 6x^2 + 11x - 6 = (x-1)(x-2)(x-3)
    assert(countRealRoots(1.0, -6.0, 11.0, -6.0) == 3);

    // Cubic with one real root: x^3 + x + 1 = 0
    assert(countRealRoots(1.0, 0.0, 1.0, 1.0) == 1);

    // Cubic with a double root: (x-1)^2 (x+2) = x^3 - 3x + 2
    assert(countRealRoots(1.0, 0.0, -3.0, 2.0) == 3);

    // Cubic with a triple root: (x-2)^3 = x^3 - 6x^2 + 12x - 8
    assert(countRealRoots(1.0, -6.0, 12.0, -8.0) == 3);

    // Degenerate to quadratic: 2x^2 - 4x + 2 = 2(x-1)^2
    assert(countRealRoots(0.0, 2.0, -4.0, 2.0) == 2);

    // Degenerate to linear: 3x - 6 = 0
    assert(countRealRoots(0.0, 0.0, 3.0, -6.0) == 1);

    // Degenerate to constant non-zero: no roots
    assert(countRealRoots(0.0, 0.0, 0.0, 5.0) == 0);

    // Degenerate to constant zero: infinite roots, return 0 to be safe
    assert(countRealRoots(0.0, 0.0, 0.0, 0.0) == 0);

    // Quadratic with two distinct roots: x^2 - 1 = 0
    assert(countRealRoots(0.0, 1.0, 0.0, -1.0) == 2);

    // Quadratic with no real roots: x^2 + 1 = 0
    assert(countRealRoots(0.0, 1.0, 0.0, 1.0) == 0);

    // Near-degenerate with tiny leading coefficient should still be treated as cubic
    assert(countRealRoots(1e-13, 1.0, 0.0, -1.0) == 1); // essentially x^2 - 1, but small a

    return 0;
}
#include <cmath>
#include <cstddef>

// Count the number of real roots of a cubic polynomial a*x^3 + b*x^2 + c*x + d = 0,
// counting multiplicities (a double root contributes 2, a triple root contributes 3).
// The function handles degenerate cases: if |a| is negligible, it reduces to quadratic;
// if both |a| and |b| are negligible, it reduces to linear; if all are negligible, returns 0.
int countRealRoots(double a, double b, double c, double d) {
    constexpr double COEFF_EPS = 1e-12;
    constexpr double ROOT_EPS = 1e-9;

    // Handle degenerate cases
    if (std::abs(a) < COEFF_EPS) {
        // Quadratic: b*x^2 + c*x + d = 0
        if (std::abs(b) < COEFF_EPS) {
            // Linear: c*x + d = 0
            if (std::abs(c) < COEFF_EPS) {
                // Constant: d = 0 (infinitely many roots) or d != 0 (none)
                return 0;
            }
            return 1; // Single root
        }
        // Quadratic discriminant (within tolerance)
        double disc = c * c - 4.0 * b * d;
        if (disc > ROOT_EPS) {
            return 2; // Two distinct real roots
        } else if (disc < -ROOT_EPS) {
            return 0; // No real roots
        } else {
            return 2; // One double root (multiplicity 2)
        }
    }

    // Normalize coefficients
    double a_inv = 1.0 / a;
    double p = b * a_inv;
    double q = c * a_inv;
    double r = d * a_inv;

    // Depressed cubic: t^3 + P*t + Q = 0, where x = t - p/3
    double p2 = p * p;
    double P = q - p2 / 3.0;
    double Q = r + (2.0 * p * p2 - 9.0 * p * q) / 27.0;

    // Discriminant of the depressed cubic: (Q/2)^2 + (P/3)^3
    double halfQ = Q / 2.0;
    double thirdP = P / 3.0;
    double delta = halfQ * halfQ + thirdP * thirdP * thirdP;

    if (delta > ROOT_EPS) {
        return 1; // One real root, two complex
    } else if (delta < -ROOT_EPS) {
        return 3; // Three distinct real roots
    } else {
        // delta == 0 (within tolerance): all roots real, at least two equal
        // Total multiplicity is always 3
        return 3;
    }
}
// The task requires implementing an exact algebraic count of real roots for a cubic polynomial. The approach uses the standard discriminant-based classification. First, normalize the cubic if `|a|` is above the tolerance; otherwise, reduce to a lower-degree polynomial. For the cubic, compute the depressed form by substituting \(x = t - b/(3a)\). The discriminant \(\Delta = 18abcd - 4b^3d + b^2c^2 - 4ac^3 - 27a^2d^2\) determines the nature:
// - If \(\Delta > 0\): three distinct real roots → 3.
// - If \(\Delta = 0\): multiple roots. Need to count multiplicity: either a double root and a single distinct root → 3 (with multiplicity), or one triple root → 3 (with multiplicity). However, the task asks for “count of distinct real roots (counting multiplicity as separate roots)”, meaning multiplicities are summed, i.e., a double root contributes 2 and a triple contributes 3. So for \(\Delta = 0\) the count is always 3 (since total multiplicity of a cubic is 3, and all roots are real when \(\Delta \ge 0\)).
// - If \(\Delta < 0\): exactly one real root → 1.
//
// For degenerate cases:
// - If `|a|` is negligible, solve quadratic \(b x^2 + c x + d = 0\). The discriminant \(D = c^2 - 4bd\):
//   - \(D > 0\): two distinct real roots → 2.
//   - \(D = 0\): one double root → 2 (multiplicity).
//   - \(D < 0\): no real roots → 0.
// - If both `a` and `b` are negligible, solve linear \(c x + d = 0\): one real root → 1 if `|c|` is not negligible, else 0 if all coefficients are negligible.
//
// Edge cases include floating-point near-zero values; use tolerance-based comparisons. The time complexity is \(O(1)\), space complexity \(O(1)\). The main challenge is robustly deciding when a value is zero within tolerance to avoid division by zero or misclassifying roots. The solution carefully uses `std::abs` and a constexpr tolerance.
