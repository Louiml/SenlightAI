// Given a vector of discrete sample values representing a uniformly sampled signal, write a C++ function that returns a vector of interpolated values at a finer resolution using linear interpolation. The function should accept the original samples, the start index, the end index, and a step size for the interpolation grid. It must handle the case where the end index is greater than or equal to the start index, and ensure that the interpolation step is positive. The output vector should contain interpolated values at positions starting from `start_index` and incrementing by `step` until reaching `end_index` (inclusive), with each output value computed by linear interpolation between the two nearest sample points. If the requested position exactly matches a sample index, that sample value is returned. If the requested position falls outside the sample range (due to the step not aligning with the last index), the function should stop before exceeding `end_index`.
#include <cassert>
#include <cmath>
#include <vector>

// Include the solution function here (or link to it)

int main() {
    // Basic linear interpolation between 0 and 10 at step 5
    std::vector<double> s1 = {0.0, 10.0};
    auto r1 = linearInterpolate(s1, 0.0, 1.0, 0.5);
    assert(r1.size() == 3);
    assert(std::fabs(r1[0] - 0.0) < 1e-9);
    assert(std::fabs(r1[1] - 5.0) < 1e-9);
    assert(std::fabs(r1[2] - 10.0) < 1e-9);

    // Step larger than range: only start point
    auto r2 = linearInterpolate(s1, 0.0, 1.0, 2.0);
    assert(r2.size() == 1);
    assert(std::fabs(r2[0] - 0.0) < 1e-9);

    // start == end
    auto r3 = linearInterpolate(s1, 0.5, 0.5, 0.1);
    assert(r3.size() == 1);
    assert(std::fabs(r3[0] - 5.0) < 1e-9);

    // Non-integer start index
    std::vector<double> s2 = {0.0, 10.0, 20.0};
    auto r4 = linearInterpolate(s2, 0.5, 1.5, 0.5);
    assert(r4.size() == 3);
    assert(std::fabs(r4[0] - 5.0) < 1e-9);
    assert(std::fabs(r4[1] - 10.0) < 1e-9);
    assert(std::fabs(r4[2] - 15.0) < 1e-9);

    // Step not dividing range evenly, ends before end_index
    auto r5 = linearInterpolate(s2, 0.0, 1.0, 0.4);
    // x values: 0.0, 0.4, 0.8 → size 3
    assert(r5.size() == 3);
    assert(std::fabs(r5[0] - 0.0) < 1e-9);
    assert(std::fabs(r5[1] - 4.0) < 1e-9);
    assert(std::fabs(r5[2] - 8.0) < 1e-9);

    // Single sample
    std::vector<double> s3 = {42.0};
    auto r6 = linearInterpolate(s3, 0.0, 0.0, 0.1);
    assert(r6.size() == 1);
    assert(std::fabs(r6[0] - 42.0) < 1e-9);

    // Invalid step returns empty
    auto r7 = linearInterpolate(s1, 0.0, 1.0, 0.0);
    assert(r7.empty());

    // End index beyond samples but step stops earlier
    auto r8 = linearInterpolate(s1, 0.0, 5.0, 2.0);
    // x values: 0.0, 2.0, 4.0 → all clamp to last sample (10.0)
    assert(r8.size() == 3);
    for (double val : r8) {
        assert(std::fabs(val - 10.0) < 1e-9);
    }

    return 0;
}
#include <vector>
#include <cmath>

// Interpolate samples at positions from start_index to end_index with given step.
std::vector<double> linearInterpolate(const std::vector<double>& samples,
                                      double start_index,
                                      double end_index,
                                      double step) {
    std::vector<double> result;
    if (step <= 0.0 || start_index > end_index || samples.empty()) {
        return result;
    }

    double x = start_index;
    while (x <= end_index + 1e-12) { // small epsilon to handle floating point
        int lower = static_cast<int>(std::floor(x));
        // Clamp lower to valid range
        if (lower < 0) lower = 0;
        if (lower >= static_cast<int>(samples.size()) - 1) lower = static_cast<int>(samples.size()) - 1;
        
        double t = x - lower;
        if (lower >= static_cast<int>(samples.size()) - 1) {
            // At the last sample index, just push the last value
            result.push_back(samples.back());
        } else {
            double y = samples[lower] * (1.0 - t) + samples[lower + 1] * t;
            result.push_back(y);
        }
        x += step;
    }
    return result;
}
// The solution iterates over a virtual grid of positions `x` starting from `start_index`, incrementing by `step`, until `x` exceeds `end_index`. For each position, we find the lower sample index `i = floor(x)` and the fractional part `t = x - i`. If `i` equals the last sample index, we directly use the last sample. Otherwise, we linearly interpolate between `samples[i]` and `samples[i+1]` using the formula `y = samples[i] * (1 - t) + samples[i+1] * t`. Edge cases include when `start_index == end_index` (returns a single value), when `step` is large so that only one or few values are produced, and when `x` exactly equals an integer index (fractional part zero, interpolation reduces to the sample). The algorithm runs in O(m) time where m is the number of interpolated points, and uses O(m) auxiliary space for the output vector. No additional data structures are needed; we only store the result.
