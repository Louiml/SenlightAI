Write a standalone C++ function named `computeMovingAverageCoefficients` that takes as input: an integer `N` (number of taps, where `N >= 2`), a double `d` (relative deviation between 0 and 1, exclusive), and a reference to an output parameter `a1` (the feedback coefficient) and `bN` (the leakage coefficient). The function must compute the coefficients of a leaky moving average filter where the impulse response is given by `h[0] = g`, `h[n] = g * a1^n` for `0 < n < N`, and `h[N] = g * bN` (where `bN = a1^N`). The filter is normalized such that the sum of all impulse response terms equals 1, and the ratio `(h[N-1] - h[0]) / h[0]` approximates the given deviation `d` (defined exactly as `d = 1 - bN`). Use Newton's method (or a direct formula) to solve for `a1` such that `bN = 1 - d`, then compute `a1 = (1 - d)^(1/N)`. After that, compute the gain `g` as the reciprocal of the sum of the impulse-response terms up to index `N-1` (excluding the final leakage term `h[N]`), but ensure that the sum includes the term at `n=0` as 1.0. The function should return `true` if computation succeeds, `false` if `d` is out of range (<=0 or >=1) or `N < 2`. The function must be `const`-correct and use only standard C++ libraries.

The core algorithm: Given the desire to design a leaky moving average filter, we want the impulse response to decay geometrically with a ratio `a1` between successive samples, but with a finite length `N` such that the final non-zero tap is at index `N-1` and the leakage is captured by the coefficient `bN`. The deviation `d` is defined as `d = 1 - bN`, meaning `bN = 1 - d`. Since `bN = a1^N`, we directly solve `a1 = (1 - d)^(1/N)`. This avoids iterative Newton's method entirely. Edge cases: `d` must be strictly between 0 and 1; if `d=0`, then `bN=1` which would make `a1=1` and the sum diverges, causing a division by zero when computing the gain; if `d>=1`, then `a1` becomes zero or negative, which is not valid for a leaky average (would produce oscillatory impulse response). For gain: we sum the impulse response terms from `n=0` to `n=N-1`: `sum = 1 + a1 + a1^2 + ... + a1^(N-1)`. This is a geometric series with `N` terms. The gain `g` is the reciprocal of this sum. The final coefficient `bN` is not included in the sum because that term represents the leakage that goes into the next block (in a real-time filter, it's used differently). But in the provided code snippet, the sum is over `n=0..N-1` indeed, and then `g = 1/sn`. The impulse response values would be `h[n] = g * a1^n` for `0 <= n < N`, and `h[N] = g * bN` (which is not included in the normalization sum). The function must handle numerical stability: when `d` is very close to 1, `a1` is close to 0, and the sum approaches 1, which is fine. When `d` is very close to 0, `a1` is close to 1, and the sum approaches N, which is fine. The time complexity is O(N) for computing the sum (or O(log N) with a closed-form formula, but a loop is simpler). Space complexity is O(1). The function returns a boolean to indicate success and writes `a1`, `bN`, and also the gain `g` to output parameters (we'll include `g` as an additional output parameter for completeness, though the snippet focuses on `a1` and `bN`; we'll include `g` as well to match the normalization requirement). The function must be const-correct, so inputs are not modified, and output parameters are non-const references.

#include <cmath>
#include <limits>

/**
 * Computes coefficients for a leaky moving average filter.
 *
 * Given the number of taps N and a deviation d (0 < d < 1), the filter has
 * impulse response: h[0] = g, h[n] = g * a1^n for 0 < n < N, and h[N] = g * bN,
 * where bN = a1^N = 1 - d, and g is chosen so that the sum of h[0]..h[N-1] equals 1.
 *
 * Parameters:
 *   N  - number of taps, must be >= 2.
 *   d  - relative deviation, must be strictly between 0 and 1.
 *   a1 - output: feedback coefficient.
 *   bN - output: leakage coefficient.
 *   g  - output: gain factor.
 *
 * Returns:
 *   true on success, false if N < 2 or d is not in (0, 1).
 */
bool computeMovingAverageCoefficients(int N, double d, double& a1, double& bN, double& g)
{
    // Validate inputs.
    if (N < 2 || d <= 0.0 || d >= 1.0) {
        return false;
    }

    // Compute bN = 1 - d, then a1 = bN^(1/N).
    bN = 1.0 - d;
    a1 = std::pow(bN, 1.0 / static_cast<double>(N));

    // Compute sum of impulse response up to index N-1: 1 + a1 + a1^2 + ... + a1^(N-1).
    // Use a simple loop for clarity and stability.
    double sum = 1.0;
    double term = 1.0; // a1^0
    for (int n = 1; n < N; ++n) {
        term *= a1;
        sum += term;
    }

    // Gain is reciprocal of the sum. If sum is very small (shouldn't happen for valid d),
    // but guard against division by zero.
    if (sum < std::numeric_limits<double>::epsilon()) {
        return false;
    }
    g = 1.0 / sum;

    return true;
}

#include <cassert>
#include <cmath>

int main() {
    double a1, bN, g;

    // Test 1: N=10, d=0.1. bN should be 0.9, a1 = 0.9^(0.1).
    assert(computeMovingAverageCoefficients(10, 0.1, a1, bN, g));
    assert(std::fabs(bN - 0.9) < 1e-12);
    assert(std::fabs(a1 - std::pow(0.9, 0.1)) < 1e-12);
    // Gain should normalize the sum of first 10 terms to 1.
    double sum = 0.0;
    double term = 1.0;
    for (int n = 0; n < 10; ++n) {
        sum += term;
        term *= a1;
    }
    assert(std::fabs(g * sum - 1.0) < 1e-12);

    // Test 2: N=2, d=0.5. a1 = sqrt(0.5) ≈ 0.7071, sum = 1 + a1 ≈ 1.7071.
    assert(computeMovingAverageCoefficients(2, 0.5, a1, bN, g));
    assert(std::fabs(a1 - std::sqrt(0.5)) < 1e-12);
    assert(std::fabs(g - 1.0 / (1.0 + a1)) < 1e-12);

    // Test 3: Edge case d very close to 0, e.g., 1e-9, N=100.
    assert(computeMovingAverageCoefficients(100, 1e-9, a1, bN, g));
    assert(bN > 0.999999999 && bN < 1.0);
    assert(a1 > 0.9999 && a1 < 1.0);

    // Test 4: Invalid d values must return false.
    assert(!computeMovingAverageCoefficients(5, 0.0, a1, bN, g));
    assert(!computeMovingAverageCoefficients(5, 1.0, a1, bN, g));
    assert(!computeMovingAverageCoefficients(5, -0.1, a1, bN, g));
    assert(!computeMovingAverageCoefficients(5, 1.1, a1, bN, g));

    // Test 5: Invalid N.
    assert(!computeMovingAverageCoefficients(1, 0.5, a1, bN, g));
    assert(!computeMovingAverageCoefficients(0, 0.5, a1, bN, g));

    // Test 6: Large N, small d. Verify that a1^N = bN.
    assert(computeMovingAverageCoefficients(1000, 0.01, a1, bN, g));
    assert(std::fabs(std::pow(a1, 1000) - bN) < 1e-12);

    return 0;
}
