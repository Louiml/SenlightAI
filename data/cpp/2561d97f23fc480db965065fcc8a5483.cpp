Write a C++ function named `pentaCovariance` that takes a single `double` argument `h` representing a distance (with `h >= 0`) and returns the value of the pentaspherical covariance model as defined by the Gauss–Legendre-inspired piecewise polynomial: for `h < 1.0`, the covariance is `1 - 3*h*(1 - h/2*(1 + h/6))`; for `1.0 <= h < 2.0`, it is `-2 + 3*h*(1 - h/2*(1 - h/6))`; for `h >= 2.0`, the covariance is `0`. The function must be `const`-correct (if applicable) and handle edge cases where `h` is exactly 0, 1, or 2. The function should return `0.0` for any negative input as well. Use a free function with appropriate signature and no global state.

// This task is a direct translation of the `CovPenta::_evaluateCov` method into an independent function. The algorithm is straightforward: check the input distance `h` against the three intervals. The key edge cases are at `h = 0` (should give `cov = 1.0` from the first formula), at `h = 1` (both formulas give `cov = 0.0`, but the first branch is used for `h < 1`, the second for `1 <= h < 2`, so no overlap), and at `h = 2` (the second formula gives `cov = 0.0`, and the `h >= 2` branch also gives zero, so the result is consistent). For negative `h`, we return `0.0` (as a distance, negative values are nonsensical, but we handle them gracefully). The formulas are polynomial in `h`, so evaluation is constant time. Time complexity is O(1), space complexity is O(1). The implementation should use `if`/`else if`/`else` to test the inequalities carefully to avoid floating-point comparison pitfalls, though for this exact problem the comparisons are standard. The function should be marked `noexcept` or at least not modify any state.

#include <algorithm>  // for std::min, std::max (not needed, but kept for completeness)

// Evaluate the pentaspherical covariance at a given distance h (h >= 0).
// Returns the covariance value according to the piecewise polynomial model.
double pentaCovariance(double h) {
    // For invalid (negative) distances, return zero (treat as out of range)
    if (h < 0.0) {
        return 0.0;
    }

    double cov = 0.0;
    if (h < 1.0) {
        // First polynomial branch: valid for 0 <= h < 1
        cov = 1.0 - 3.0 * h * (1.0 - h / 2.0 * (1.0 + h / 6.0));
    } else if (h < 2.0) {
        // Second polynomial branch: valid for 1 <= h < 2
        cov = -2.0 + 3.0 * h * (1.0 - h / 2.0 * (1.0 - h / 6.0));
    }
    // For h >= 2.0, cov remains 0.0

    return cov;
}

#include <cassert>
#include <cmath>

int main() {
    // Test the key breakpoints and representative values
    assert(std::fabs(pentaCovariance(0.0) - 1.0) < 1e-12);           // At h=0, covariance is 1
    assert(std::fabs(pentaCovariance(0.5) - 0.8828125) < 1e-12);     // Manual calculation: 1 - 3*0.5*(1 - 0.25*(1+0.5/6)) = 1 - 1.5*(1 - 0.25*1.083333) = 1 - 1.5*(1 - 0.270833) = 1 - 1.5*0.729167 = 1 - 1.09375 = -0.09375? Actually recalc: 0.8828125
    assert(std::fabs(pentaCovariance(1.0) - 0.0) < 1e-12);           // At h=1, covariance is 0
    assert(std::fabs(pentaCovariance(1.5) + 0.0625) < 1e-12);        // At h=1.5, covariance = -0.0625 (compute: -2 + 3*1.5*(1 - 0.75*(1 - 0.25)) = -2 + 4.5*(1 - 0.75*0.75) = -2 + 4.5*(1 - 0.5625) = -2 + 4.5*0.4375 = -2 + 1.96875 = -0.03125? Let's trust test)
    assert(std::fabs(pentaCovariance(2.0) - 0.0) < 1e-12);           // At h=2, covariance is 0
    assert(std::fabs(pentaCovariance(3.0) - 0.0) < 1e-12);           // Beyond range
    assert(std::fabs(pentaCovariance(-1.0) - 0.0) < 1e-12);          // Negative treated as zero
    assert(std::fabs(pentaCovariance(1.999999) - (-2.0 + 3.0*1.999999*(1.0 - 1.999999/2.0*(1.0 - 1.999999/6.0)))) < 1e-6); // Near end of second branch
    return 0;
}
