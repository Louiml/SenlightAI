/*
Write a standalone C++ function named `scaleToSafeNorm` that takes a flat array of complex numbers (`std::complex<double>`) and its dimensions (rows and columns) and conditionally scales all elements by a power-of-two factor to bring the matrix norm into a safe range, mimicking the scaling logic shown in the snippet. Specifically, the function must: (1) compute the maximum absolute value (complex modulus) over all elements; if this maximum is zero, NaN, or infinite, do nothing; (2) if the maximum is between 1e-140 and 1e138, do nothing; (3) otherwise, scale the entire matrix by a factor that moves the maximum to approximately 1e-140 or 1e138 using a binary-exponent style adjustment, repeatedly multiplying or dividing by 2.0041683600089728e-292 or 4.9896007738368e+291 (as in the snippet) until the new maximum falls within the safe range. The function must modify the input array in place, must handle empty arrays (0 rows or columns) gracefully, and must preserve the exact ratios between all original elements. The function signature must be: `void scaleToSafeNorm(std::complex<double>* data, int rows, int cols);`
*/

#include <complex>
#include <cmath>
#include <cstddef>

// Scale a matrix (stored in row-major order as a flat array) so that its
// maximum complex modulus falls within [1e-140, 1e138].
// If the input is empty, contains NaN/Inf, or is already in safe range,
// the function does nothing. The scaling is applied in place.
void scaleToSafeNorm(std::complex<double>* data, int rows, int cols) {
    if (data == nullptr || rows <= 0 || cols <= 0) {
        return;
    }

    const std::size_t total = static_cast<std::size_t>(rows) * static_cast<std::size_t>(cols);

    // Compute the maximum absolute value (complex modulus) over all elements.
    double anrm = 0.0;
    for (std::size_t i = 0; i < total; ++i) {
        double abs_val = std::abs(data[i]);
        if (std::isnan(abs_val) || std::isinf(abs_val)) {
            // If any element has NaN/Inf modulus, we cannot scale safely.
            return;
        }
        if (abs_val > anrm) {
            anrm = abs_val;
        }
    }

    // If the norm is zero or already within [1e-140, 1e138], nothing to do.
    const double lower = 1e-140;
    const double upper = 1e138;
    if (anrm == 0.0 || (anrm >= lower && anrm <= upper)) {
        return;
    }

    // Determine target norm and scaling direction.
    double target;
    bool scaleDown; // true if we need to multiply by a factor < 1
    if (anrm < lower) {
        target = lower;
        scaleDown = false; // multiply by factor > 1 to increase
    } else {
        target = upper;
        scaleDown = true;  // multiply by factor < 1 to decrease
    }

    // Apply scaling in small steps to avoid intermediate overflow/underflow.
    // Use factors similar to the original snippet.
    const double small_down = 2.0041683600089728e-292; // ~ 2^-970, multiply when increasing
    const double small_up   = 4.9896007738368e+291;    // ~ 2^970, multiply when decreasing

    double current = anrm;
    bool done = false;

    while (!done) {
        double factor;
        if (!scaleDown) {
            // Need to increase norm. Try multiplying by small_down first.
            double test = current * small_down;
            if (test > target || current == 0.0) {
                // We would exceed target; use direct ratio to land exactly.
                factor = target / current;
                done = true;
            } else {
                factor = small_down;
                current = test;
            }
        } else {
            // Need to decrease norm. Try multiplying by small_up first.
            double test = current / (1.0 / small_up); // same as current * small_up
            test = current * (1.0 / small_up); // avoid division
            test = current * (1.0 / small_up);
            if (test < target || current == 0.0) {
                factor = target / current;
                done = true;
            } else {
                factor = 1.0 / small_up; // ~ 2^-970
                current = test;
            }
        }

        // Apply the factor to every element.
        for (std::size_t i = 0; i < total; ++i) {
            data[i] *= factor;
        }
    }

    // After the loop, the norm should be approximately equal to target.
    // (For extreme cases, tiny rounding errors might remain, but the ratio
    //  between elements is preserved exactly via the same factor per element.)
}

#include <cassert>
#include <complex>
#include <cmath>

// The function under test.
void scaleToSafeNorm(std::complex<double>* data, int rows, int cols);

int main() {
    // Test 1: Empty array should not crash.
    std::complex<double> empty[1];
    scaleToSafeNorm(empty, 0, 0);

    // Test 2: Already safe range, no scaling.
    std::complex<double> a[4] = {{1.0, 0.0}, {2.0, 0.0}, {0.0, 3.0}, {0.0, 0.0}};
    scaleToSafeNorm(a, 2, 2);
    assert(a[0] == std::complex<double>(1.0, 0.0));
    assert(a[1] == std::complex<double>(2.0, 0.0));
    assert(a[2] == std::complex<double>(0.0, 3.0));
    assert(a[3] == std::complex<double>(0.0, 0.0));

    // Test 3: Very small norm, should scale up so that max modulus ≈ 1e-140.
    std::complex<double> b[2] = {{1e-300, 0.0}, {0.0, 0.0}};
    scaleToSafeNorm(b, 1, 2);
    double max_mod = std::max(std::abs(b[0]), std::abs(b[1]));
    assert(max_mod >= 1e-141 && max_mod <= 1e-139);
    // Ratios preserved: second element still zero.
    assert(b[1] == std::complex<double>(0.0, 0.0));

    // Test 4: Very large norm, should scale down so that max modulus ≈ 1e138.
    std::complex<double> c[3] = {{1e300, 0.0}, {5e299, 0.0}, {0.0, 1e300}};
    scaleToSafeNorm(c, 1, 3);
    double max_c = std::max({std::abs(c[0]), std::abs(c[1]), std::abs(c[2])});
    assert(max_c >= 1e137 && max_c <= 1e139);
    // Ratio between c[0] and c[1] preserved.
    double ratio_expected = 1e300 / 5e299; // = 2.0
    double ratio_actual = std::abs(c[0]) / std::abs(c[1]);
    assert(std::fabs(ratio_actual - ratio_expected) < 1e-12);

    // Test 5: Zero matrix unchanged.
    std::complex<double> d[4] = {{0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}};
    scaleToSafeNorm(d, 2, 2);
    for (int i = 0; i < 4; ++i) {
        assert(d[i] == std::complex<double>(0.0, 0.0));
    }

    // Test 6: NaN should not change anything.
    std::complex<double> e[1] = {{std::nan(""), 0.0}};
    scaleToSafeNorm(e, 1, 1);
    assert(std::isnan(e[0].real()) && e[0].imag() == 0.0);

    // Test 7: Inf should not change anything.
    std::complex<double> f[1] = {{std::numeric_limits<double>::infinity(), 0.0}};
    scaleToSafeNorm(f, 1, 1);
    assert(std::isinf(f[0].real()));

    // Test 8: Single element exactly at boundary lower – unchanged.
    std::complex<double> g[1] = {{1e-140, 0.0}};
    scaleToSafeNorm(g, 1, 1);
    assert(g[0] == std::complex<double>(1e-140, 0.0));

    // Test 9: Single element just below lower boundary – scaled up.
    std::complex<double> h[1] = {{1e-141, 0.0}};
    scaleToSafeNorm(h, 1, 1);
    assert(std::fabs(std::abs(h[0]) - 1e-140) / 1e-140 < 1e-10);

    // Test 10: Non-trivial complex values with imaginary parts.
    std::complex<double> k[2] = {{1e200, 2e200}, {-3e200, 4e200}};
    scaleToSafeNorm(k, 2, 1);
    double max_k = std::max(std::abs(k[0]), std::abs(k[1]));
    assert(max_k >= 1e137 && max_k <= 1e139);
    // Ratio of moduli preserved.
    double ratio_before = std::sqrt(1e200*1e200 + 2e200*2e200) / std::sqrt(9e200 + 16e200);
    double ratio_after = std::abs(k[0]) / std::abs(k[1]);
    assert(std::fabs(ratio_after - ratio_before) < 1e-12);

    return 0;
}

// The solution approach mirrors the scaling logic from the snippet. First, compute the Frobenius-style max norm by iterating over all elements, using `std::abs` for complex modulus, and handle NaN/Inf by returning early (do nothing). If the norm is zero or already within the safe range [1e-140, 1e138], no scaling is needed. Otherwise, we need to determine a multiplier `a` such that multiplying all elements by `a` brings the norm to the nearer safe boundary (either 1e-140 or 1e138, whichever is closer in logarithmic sense). The snippet uses an iterative binary-exponent approach: choose an initial target `anrmto` (either 1e-140 or 1e138), then repeatedly multiply the matrix by 2.0041683600089728e-292 (if scaling down) or by 4.9896007738368e+291 (if scaling up) until the norm would cross the target, then finally multiply by the remaining ratio `target / current_norm`. This avoids underflow/overflow by applying small factors repeatedly. For our standalone version, we can simplify: compute the current max norm `anrm`; if `anrm < 1e-140`, set target = 1e-140 and scale factor = target / anrm; if `anrm > 1e138`, set target = 1e138 and scale factor = target / anrm. To avoid extreme values, apply the scaling in a loop that multiplies by small factors (like 2.0041683600089728e-292 or 4.9896007738368e+291) and then by the final residual. However, for correctness with typical finite doubles, a direct multiplication by `scaleFactor` is acceptable unless the factor itself overflows/underflows (which it won't for our range, since the factor is between about 1e-292 and 1e292, both representable). We'll still implement the iterative approach for educational fidelity, but ensure the final result is correct. Edge cases: rows*cols == 0 → return; any NaN or Inf → return; all zeros → return. Time complexity O(rows*cols) for norm computation plus O(rows*cols) for scaling, so O(n) where n = rows*cols. Space complexity O(1) auxiliary.
