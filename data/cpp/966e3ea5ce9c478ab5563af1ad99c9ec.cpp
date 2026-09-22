Write a standalone C++ function that, given a vector of complex numbers (std::vector<std::complex<double>>) and a CKKS scale factor (double), returns a vector of double-precision real values representing the rounded magnitudes of the complex numbers (i.e., sqrt(real^2 + imag^2)). The function should be designed as a pure computation function that does not depend on any external library like Microsoft SEAL – it is a simple numerical utility that can be used for testing CKKS encoding/decoding approaches. The input vector must not be empty, and the scale factor must be positive; the function should handle edge cases like very small or large values gracefully. The result should be computed element-wise, and the output vector must have the same size as the input.
#include <cassert>
#include <vector>
#include <complex>
#include <cmath>

// Function declaration (should match the solution)
std::vector<double> computeComplexMagnitudes(const std::vector<std::complex<double>>& input, double scale);

int main() {
    // Basic case
    std::vector<std::complex<double>> v1 = {{3.0, 4.0}, {0.0, 0.0}, {1.0, 1.0}};
    auto r1 = computeComplexMagnitudes(v1, 1024.0);
    assert(r1.size() == 3);
    assert(std::abs(r1[0] - 5.0) < 1e-9);
    assert(std::abs(r1[1] - 0.0) < 1e-9);
    assert(std::abs(r1[2] - std::sqrt(2.0)) < 1e-9);

    // Edge case: empty input
    std::vector<std::complex<double>> v2;
    auto r2 = computeComplexMagnitudes(v2, 1.0);
    assert(r2.empty());

    // Edge case: negative real and imaginary parts
    std::vector<std::complex<double>> v3 = {{-3.0, -4.0}, {-1.0, 0.0}};
    auto r3 = computeComplexMagnitudes(v3, 1.0);
    assert(std::abs(r3[0] - 5.0) < 1e-9);
    assert(std::abs(r3[1] - 1.0) < 1e-9);

    // Edge case: large values (no overflow with hypot)
    std::vector<std::complex<double>> v4 = {{1e200, 1e200}, {1e-200, 1e-200}};
    auto r4 = computeComplexMagnitudes(v4, 1.0);
    assert(std::isfinite(r4[0]));
    assert(std::abs(r4[0] - std::hypot(1e200, 1e200)) < 1e-9 * r4[0]);
    assert(std::abs(r4[1] - std::hypot(1e-200, 1e-200)) < 1e-9 * r4[1]);

    // Edge case: scale validation
    bool threw = false;
    try {
        computeComplexMagnitudes(v1, 0.0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Single element
    std::vector<std::complex<double>> v5 = {{6.0, 8.0}};
    auto r5 = computeComplexMagnitudes(v5, 1.0);
    assert(r5.size() == 1);
    assert(std::abs(r5[0] - 10.0) < 1e-9);
}
#include <vector>
#include <complex>
#include <cmath>
#include <stdexcept>

// Computes the magnitudes of a vector of complex numbers.
// The scale parameter is accepted for interface completeness but not used in the pure computation.
std::vector<double> computeComplexMagnitudes(const std::vector<std::complex<double>>& input, double scale) {
    if (scale <= 0.0) {
        throw std::invalid_argument("Scale must be positive");
    }
    std::vector<double> result;
    result.reserve(input.size());
    for (const auto& c : input) {
        // Use std::hypot for numerical stability (avoids overflow/underflow)
        result.push_back(std::hypot(c.real(), c.imag()));
    }
    return result;
}
// The main algorithm is straightforward: iterate through each complex number in the input vector. For each complex number z = a + bi, compute the magnitude as sqrt(a*a + b*b). The result is a double. No special handling is needed for negative scale factors (we can assert or throw if scale <= 0, but for robustness we can just use the absolute value or return empty for invalid input). Edge cases: if the input vector is empty, return an empty vector. If the magnitude overflows or underflows, we can use std::hypot (which is more numerically stable than sqrt(a*a + b*b)) to avoid overflow/underflow issues. Time complexity is O(n) where n is the number of elements. Space complexity is O(n) for the output vector, plus O(1) auxiliary space. The scale factor is not used in the computation – it is provided for interface completeness, but we can ignore it or validate it as positive. For test simplicity, we will ignore it.
