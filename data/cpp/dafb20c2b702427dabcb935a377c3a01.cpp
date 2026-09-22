// Write a C++ function named `computeFilteredDerivative` that simulates a discrete-time derivative filter with a first-order low-pass smoothing stage (equivalent to a band-limited differentiator). The function should accept an initial y value (the first sample), a sample time `ts` (in seconds), a smoothing bandwidth parameter `sigma` (in seconds, representing the reciprocal of the cutoff frequency), and a sequence of subsequent y samples stored in a `std::vector<float>` (the first element of the vector is the second sample). The function must return a `std::vector<float>` containing the filtered derivative estimates for each sample after the first initial value. The filter state must be reset for each call: the initial derivative estimate is 0, and the "previous sample" is the supplied initial y value. The filter coefficient is `beta = (2*sigma - ts) / (2*sigma + ts)`, and each derivative update for a new sample `y` is: `y_dot = beta * y_dot + ((1 - beta) / ts) * (y - y_d1)`, where `y_d1` is the previous sample. The function must be pure (no member state), and you must ensure it works for positive `ts` and `sigma` values (if either is non-positive, return an empty vector). The solution must be standalone, include necessary headers, and use `const` correctness where applicable.

#include <cassert>
#include <cmath>
#include <vector>

// The solution function is defined above; for the test we include it here.
// (Assume computeFilteredDerivative is available.)

int main() {
    // Empty input -> empty output
    assert(computeFilteredDerivative(0.0f, 0.1f, 0.2f, {}).empty());

    // Invalid parameters -> empty output
    assert(computeFilteredDerivative(0.0f, 0.0f, 0.2f, {1.0f}).empty());
    assert(computeFilteredDerivative(0.0f, 0.1f, 0.0f, {1.0f}).empty());
    assert(computeFilteredDerivative(0.0f, -1.0f, 0.2f, {1.0f}).empty());

    // Single sample after initial: for constant value, derivative is 0
    {
        auto out = computeFilteredDerivative(5.0f, 0.1f, 0.2f, {5.0f});
        assert(out.size() == 1);
        assert(std::fabs(out[0] - 0.0f) < 1e-6f);
    }

    // Two samples with constant slope: derivative should approach slope * (1-beta)/ts??
    // Let's compute manually: beta = (2*0.05 - 0.1)/(2*0.05+0.1) = (0.1-0.1)/(0.1+0.1)=0
    // So y_dot = 0*prev + (1/0.1)*(5-0)=50. So first derivative = 50.
    {
        auto out = computeFilteredDerivative(0.0f, 0.1f, 0.05f, {5.0f});
        assert(out.size() == 1);
        assert(std::fabs(out[0] - 50.0f) < 1e-4f);
    }

    // Three samples: verify recurrence for beta=0 (derivative becomes pure difference quotient)
    {
        float ts = 0.1f;
        float sigma = 0.05f; // beta = 0
        auto out = computeFilteredDerivative(0.0f, ts, sigma, {5.0f, 10.0f});
        assert(out.size() == 2);
        assert(std::fabs(out[0] - 50.0f) < 1e-4f);
        assert(std::fabs(out[1] - 50.0f) < 1e-4f);
    }

    // Test with beta != 0: choose ts=0.1, sigma=0.1 -> beta = (0.2-0.1)/(0.2+0.1)=0.3333
    // scale = (1-beta)/ts = (0.6667)/0.1 = 6.667
    // Sequence: initial 0, then 1, then 2
    {
        float ts = 0.1f;
        float sigma = 0.1f;
        auto out = computeFilteredDerivative(0.0f, ts, sigma, {1.0f, 2.0f});
        assert(out.size() == 2);
        // first: y_dot = 0 + 6.667*(1-0)=6.667
        float expected1 = 6.6666667f;
        assert(std::fabs(out[0] - expected1) < 1e-3f);
        // second: y_dot = 0.3333*6.667 + 6.667*(2-1) = 2.222 + 6.667 = 8.889
        float expected2 = 0.3333333f * expected1 + 6.6666667f;
        assert(std::fabs(out[1] - expected2) < 1e-3f);
    }

    return 0;
}

#include <vector>

// Compute a filtered derivative for a sequence of samples.
// Parameters:
//   initialY: the first sample (used as the "previous sample" for the first differentiation)
//   ts: sample time in seconds (must be positive)
//   sigma: bandwidth parameter in seconds (must be positive)
//   samples: subsequent samples (the first element is the second sample)
// Returns:
//   vector of filtered derivative estimates, one per element in `samples`
//   empty vector if ts <= 0 or sigma <= 0
std::vector<float> computeFilteredDerivative(float initialY, float ts, float sigma, const std::vector<float>& samples) {
    if (ts <= 0.0f || sigma <= 0.0f) {
        return {};
    }

    const float beta = (2.0f * sigma - ts) / (2.0f * sigma + ts);
    const float scale = (1.0f - beta) / ts;

    float y_dot = 0.0f;       // previous derivative estimate
    float y_d1 = initialY;    // previous sample

    std::vector<float> derivatives;
    derivatives.reserve(samples.size());

    for (float y : samples) {
        y_dot = beta * y_dot + scale * (y - y_d1);
        y_d1 = y;
        derivatives.push_back(y_dot);
    }

    return derivatives;
}

// The core algorithm implements a first-order infinite impulse response (IIR) filter for the derivative, which computes a low-pass filtered difference quotient. The state consists of `y_dot` (previous derivative estimate, initialized to 0) and `y_d1` (previous sample, initialized to the supplied initial value). For each sample in the input vector (in order), the new derivative is computed using the recurrence: first compute the difference between the current sample and `y_d1`, scale it by `(1-beta)/ts`, then add `beta` times the previous `y_dot`. After computing, update `y_d1` to the current sample. The output vector is built by pushing each computed derivative. Edge cases: if `ts <= 0` or `sigma <= 0`, the filter is invalid and an empty vector is returned. If the input vector is empty, there are no subsequent samples, so the output is also empty. The function is stateless, so each call starts fresh with `y_dot = 0`. Time complexity is O(n) where n is the number of input subsequent samples, and space complexity is O(n) for the output vector (plus O(1) auxiliary state).
