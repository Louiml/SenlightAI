// Write a C++ function `double findShiftedMean(double z, double p, double sd)` that returns the required mean (location parameter) of a normal distribution with a given standard deviation `sd` such that exactly a fraction `p` of the distribution lies at or below the threshold `z`. The function should compute the location using the inverse standard normal CDF (quantile function) via the relationship `mean = z - sd * quantile(p)`. The input probability `p` must be in the open interval (0,1); if `p` is not within this range, throw an `std::invalid_argument`. For example, if `z = -2.0`, `p = 0.001`, and `sd = 1.0`, the function should return approximately `1.09023`. The function must be self-contained and should not rely on any external libraries beyond the C++ standard library, but it may implement the quantile function using a well-known approximation or a simple numerical method (e.g., bisection on the CDF) internally. Ensure the returned value is accurate to within 1e-6 for typical inputs.
// The core idea is to invert the cumulative distribution function (CDF) of the normal distribution. For a normal distribution with mean μ and standard deviation σ, the CDF at a point z is `Φ((z − μ)/σ)`, where Φ is the standard normal CDF. We want `Φ((z − μ)/σ) = p`. Solving for μ gives `μ = z − σ * Φ^{-1}(p)`, where `Φ^{-1}` is the quantile function. The main challenges are (1) computing `Φ^{-1}` accurately without external libraries, and (2) handling edge cases for p near 0 or 1.
//
// A robust approach is to implement the standard normal CDF `Φ(x)` using the complementary error function `erfc` (available in `<cmath>` as `std::erfc`), since `Φ(x) = 0.5 * erfc(−x/√2)`. Then, to find `Φ^{-1}(p)`, we can use Newton–Raphson iteration or the well-known Beasley–Springer–Moro approximation. A simpler and reliable method is to use bisection on the CDF because we only need a few iterations and the function is monotonic. We bracket the quantile between, say, −10 and +10 (or dynamically widen) and repeatedly halve the interval until the CDF value is within a tolerance. This takes about 40 iterations for a fixed bracket width of 20, giving about 1e-12 accuracy in x. Time complexity is O(log(1/ε)), effectively constant. Space complexity is O(1). Edge cases: if p ≤ 0 or p ≥ 1, throw `std::invalid_argument`. Also, if sd is zero or negative, it makes no sense; we can handle that by throwing an `std::invalid_argument` as well, though the task does not explicitly require it—still, it's good practice. For p very close to 0 or 1, the quantile becomes large in magnitude, but bisection can still work if we widen the bracket adaptively (e.g., start at ±10 and double until the CDF at the endpoints brackets p). This ensures correctness for all p in (0,1).
#include <cmath>
#include <stdexcept>

// Returns the mean (location) of a normal distribution with standard deviation sd
// such that the fraction of the distribution at or below z equals p.
// p must be strictly between 0 and 1. If not, throws std::invalid_argument.
double findShiftedMean(double z, double p, double sd) {
    if (!(p > 0.0 && p < 1.0)) {
        throw std::invalid_argument("findShiftedMean: p must be in (0,1)");
    }
    if (!(sd > 0.0)) {
        throw std::invalid_argument("findShiftedMean: sd must be positive");
    }

    // Standard normal CDF: Phi(x) = 0.5 * erfc(-x / sqrt(2))
    auto phi = [](double x) -> double {
        return 0.5 * std::erfc(-x / std::sqrt(2.0));
    };

    // Quantile function: find x such that Phi(x) = p, via bisection.
    double lo = -10.0, hi = 10.0;
    // Ensure the interval brackets the desired p.
    while (phi(lo) > p) {
        lo *= 2.0;
    }
    while (phi(hi) < p) {
        hi *= 2.0;
    }
    for (int iter = 0; iter < 60; ++iter) {
        double mid = 0.5 * (lo + hi);
        if (phi(mid) < p) {
            lo = mid;
        } else {
            hi = mid;
        }
    }
    double quantile = 0.5 * (lo + hi);
    return z - sd * quantile;
}
#include <cassert>
#include <cmath>
#include <iostream>

// Declaration of the solution function (assume it's in the same translation unit)
double findShiftedMean(double z, double p, double sd);

int main() {
    // Standard example: z = -2, p = 0.001, sd = 1 -> mean ~ 1.09023
    double result = findShiftedMean(-2.0, 0.001, 1.0);
    assert(std::abs(result - 1.09023) < 1e-4);

    // Symmetric case: p = 0.5 => quantile = 0, so mean = z
    result = findShiftedMean(3.0, 0.5, 2.0);
    assert(std::abs(result - 3.0) < 1e-9);

    // Another known quantile: p = 0.975, z = 0, sd = 1 -> quantile ~ 1.95996, mean = -1.95996
    result = findShiftedMean(0.0, 0.975, 1.0);
    assert(std::abs(result - (-1.95996)) < 1e-4);

    // p very close to 1: p = 0.999, z = 5, sd = 2 -> mean = 5 - 2*3.09023 = -1.18046
    result = findShiftedMean(5.0, 0.999, 2.0);
    assert(std::abs(result - (-1.18046)) < 1e-3);

    // p very small: p = 0.0001, z = 0, sd = 1 -> quantile ~ -3.71902, mean = 3.71902
    result = findShiftedMean(0.0, 0.0001, 1.0);
    assert(std::abs(result - 3.71902) < 1e-3);

    // Check that invalid p throws
    bool threw = false;
    try {
        findShiftedMean(0.0, 0.0, 1.0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        findShiftedMean(0.0, 1.0, 1.0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Check invalid sd throws
    threw = false;
    try {
        findShiftedMean(0.0, 0.5, 0.0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    std::cout << "All tests passed." << std::endl;
    return 0;
}
