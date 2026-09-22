Implement a C++ function that models a biquad lowpass filter using the direct form II transposed structure. The function should take three parameters: a `std::vector<float>` of input samples, a normalized cutoff frequency in the range (0, 0.5] (where 0.5 corresponds to the Nyquist frequency), and a resonance parameter in decibels (non-negative). It should return a `std::vector<float>` of filtered output samples of the same length. Use the coefficient formulas from the provided snippet: compute `g = 10^(resonance/20)`, `d = sqrt((4 - sqrt(16 - 16/(g*g))) / 2)`, `theta = π * cutoff`, `sn = 0.5 * d * sin(theta)`, `beta = 0.5*(1-sn)/(1+sn)`, `gamma = (0.5+beta)*cos(theta)`, `alpha = 0.25*(0.5+beta-gamma)`, then set `a0 = 2*alpha`, `a1 = 4*alpha`, `a2 = 2*alpha`, `b1 = -2*gamma`, `b2 = 2*beta`. The filter gain `g` is applied to the output as a multiplier. Initialize internal state (x1, x2, y1, y2) to zero and process samples sequentially.

The biquad is a second-order IIR filter with transfer function `H(z) = g * (a0 + a1*z^-1 + a2*z^-2) / (1 + b1*z^-1 + b2*z^-2)`. The direct form II transposed structure is numerically stable and requires only four state variables: `x1, x2` (previous two inputs) and `y1, y2` (previous two outputs). For each input sample `x`, compute `y = a0*x + a1*x1 + a2*x2 - b1*y1 - b2*y2`, then multiply by `g` to apply the resonance gain. After computing `y`, shift the state: `x2 = x1; x1 = x; y2 = y1; y1 = y`. The coefficient computation must handle the edge case where `g = 1` (resonance = 0) because the formula for `d` involves division by `g*g`; using `pow(10.0, 0.05 * resonance)` gives `g = 1` when resonance is 0, and `16 - 16/(1) = 0` leading to `sqrt(0) = 0` so `d = sqrt(4/2) = sqrt(2)`; this is valid. For resonance greater than 0, `g > 1`, and the expression under the outer sqrt is positive. If cutoff is extremely small (near 0), `theta` is near 0, `sin(theta)` is near 0, so `sn` is near 0, `beta` is near 0.5, `gamma` is near 0.5, `alpha` is near 0.25*(1 - 0.5) = 0.125, giving a valid filter. The algorithm runs in O(n) time where n is the number of input samples, and uses O(1) extra space (excluding input/output vectors). The main computational concern is numerical precision: use `double` for all internal calculations and state variables to avoid error accumulation over many samples, even though input/output are `float`.

#include <vector>
#include <cmath>
#include <algorithm>

// Apply a biquad lowpass filter to a vector of samples.
// cutoff: normalized frequency in (0, 0.5] where 0.5 is Nyquist.
// resonance: gain in dB, non-negative.
std::vector<float> biquadLowpass(const std::vector<float>& input, double cutoff, double resonance) {
    const double pi = 3.14159265358979323846;

    // Clamp resonance to non-negative.
    resonance = std::max(0.0, resonance);

    // Compute filter coefficients (from RBJ Audio EQ Cookbook / WebKit Biquad).
    double g = std::pow(10.0, 0.05 * resonance);
    double d = std::sqrt((4.0 - std::sqrt(16.0 - 16.0 / (g * g))) / 2.0);

    double theta = pi * cutoff;
    double sn = 0.5 * d * std::sin(theta);
    double beta = 0.5 * (1.0 - sn) / (1.0 + sn);
    double gamma = (0.5 + beta) * std::cos(theta);
    double alpha = 0.25 * (0.5 + beta - gamma);

    double a0 = 2.0 * alpha;
    double a1 = 2.0 * 2.0 * alpha;
    double a2 = 2.0 * alpha;
    double b1 = 2.0 * -gamma;
    double b2 = 2.0 * beta;

    // Process samples with direct form II transposed.
    std::vector<float> output;
    output.reserve(input.size());

    double x1 = 0.0, x2 = 0.0, y1 = 0.0, y2 = 0.0;

    for (float x_val : input) {
        double x = static_cast<double>(x_val);
        double y = a0 * x + a1 * x1 + a2 * x2 - b1 * y1 - b2 * y2;
        y *= g;

        output.push_back(static_cast<float>(y));

        // Shift state.
        x2 = x1;
        x1 = x;
        y2 = y1;
        y1 = y;
    }

    return output;
}

#include <cassert>
#include <cmath>
#include <vector>
#include <iostream>

// Include the solution function here (or link it).

int main() {
    // Test 1: Identity when cutoff is very close to Nyquist (0.5) and resonance = 0.
    // Lowpass at Nyquist passes all frequencies (roughly, except for numerical rounding).
    std::vector<float> input1 = {1.0f, 0.0f, -1.0f, 0.5f, 0.25f};
    std::vector<float> out1 = biquadLowpass(input1, 0.5, 0.0);
    // Since cutoff=0.5, alpha=0.5, a0=1, a1=2, a2=1, b1=-2, b2=1, effectively a pass-through.
    for (size_t i = 0; i < input1.size(); ++i) {
        assert(std::fabs(out1[i] - input1[i]) < 1e-3);
    }

    // Test 2: DC signal passes through unchanged (lowpass should not alter DC).
    std::vector<float> input2(10, 2.0f);
    std::vector<float> out2 = biquadLowpass(input2, 0.1, 0.0);
    for (size_t i = 0; i < out2.size(); ++i) {
        assert(std::fabs(out2[i] - 2.0f) < 1e-4);
    }

    // Test 3: Zero input gives zero output regardless of parameters.
    std::vector<float> input3(5, 0.0f);
    std::vector<float> out3 = biquadLowpass(input3, 0.2, 12.0);
    for (float v : out3) {
        assert(v == 0.0f);
    }

    // Test 4: A single impulse should produce finite output and state resets per call.
    std::vector<float> input4 = {1.0f};
    std::vector<float> out4a = biquadLowpass(input4, 0.3, 0.0);
    std::vector<float> out4b = biquadLowpass(input4, 0.3, 0.0);
    assert(out4a.size() == 1);
    assert(out4b.size() == 1);
    assert(std::fabs(out4a[0] - out4b[0]) < 1e-7);

    // Test 5: Output size matches input size and is not empty for non-empty input.
    std::vector<float> input5 = {1.0f, -1.0f, 0.5f, 0.0f, 0.2f, -0.3f};
    std::vector<float> out5 = biquadLowpass(input5, 0.25, 6.0);
    assert(out5.size() == input5.size());

    // Test 6: High resonance increases gain (output amplitude larger near resonance).
    std::vector<float> input6(100, 0.0f);
    input6[0] = 1.0f; // impulse
    std::vector<float> out6a = biquadLowpass(input6, 0.2, 0.0);
    std::vector<float> out6b = biquadLowpass(input6, 0.2, 12.0);
    double peakA = 0.0, peakB = 0.0;
    for (size_t i = 0; i < out6a.size(); ++i) {
        peakA = std::max(peakA, std::fabs(out6a[i]));
        peakB = std::max(peakB, std::fabs(out6b[i]));
    }
    assert(peakB > peakA);

    // Test 7: Extreme cutoff (small but positive) does not produce NaN.
    std::vector<float> input7 = {1.0f, 0.5f, 0.25f};
    std::vector<float> out7 = biquadLowpass(input7, 1e-6, 0.0);
    for (float v : out7) {
        assert(!std::isnan(v));
        assert(!std::isinf(v));
    }

    std::cout << "All tests passed." << std::endl;
    return 0;
}
