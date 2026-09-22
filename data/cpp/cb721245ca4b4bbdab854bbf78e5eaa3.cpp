Write a C++ function `normalizeChainAngles` that takes a vector of six joint angles (in radians) representing a robotic arm's IK solution and returns a new vector of six angles where each angle is normalized to the range \([-\pi, \pi)\). Additionally, the function must handle the special case where the input vector does not have exactly six elements by returning an empty vector. The normalization must be computed using a custom `wrapToPi` helper that replicates the behavior of the `IKfmod` and angle adjustment logic seen in the provided ikfast solver: the angle is repeatedly reduced modulo \(2\pi\) to the range \([0, 2\pi)\) and then shifted to \([-\pi, \pi)\) by subtracting \(2\pi\) if the result is greater than or equal to \(\pi\). The implementation should use only standard C++17 features, avoid external libraries, and be self-contained (i.e., no reliance on the ikfast namespace or its types).

// The solution uses a helper function `wrapToPi` that first normalizes the input angle to a non-negative remainder by adding \(2\pi\) repeatedly while the angle is negative (mirroring the `IKfmod` loop in the provided code), then applies `std::fmod` with \(2\pi\) to get a value in \([0, 2\pi)\). Finally, if the remainder is greater than or equal to \(\pi\), \(2\pi\) is subtracted to bring it into \([-\pi, \pi)\). The main function `normalizeChainAngles` checks the input vector's size; if it is not exactly 6, it returns an empty vector. Otherwise, it applies `wrapToPi` to each element and returns the new vector. The time complexity is \(O(1)\) per angle (the while loop runs at most a few iterations because angles are typically within a few multiples of \(2\pi\) even if arbitrary), so overall \(O(6)\) constant time. Space complexity is \(O(6)\) for the output vector. Edge cases include very large positive or negative angles, which are handled by the sequential addition and `fmod` combination; the approach matches the reference ikfast normalization logic for robustness.

#include <vector>
#include <cmath>

namespace {
    constexpr double kPi = 3.14159265358979323846;
    constexpr double kTwoPi = 2.0 * kPi;

    // Normalize an angle to the range [-pi, pi) following the
    // approach used in ikfast: shift negatives to positive, fmod by 2pi,
    // then wrap to [-pi, pi).
    double wrapToPi(double angle) {
        // Ensure angle is non-negative before fmod, matching IKfmod behavior.
        while (angle < 0) {
            angle += kTwoPi;
        }
        double remainder = std::fmod(angle, kTwoPi);
        if (remainder >= kPi) {
            remainder -= kTwoPi;
        }
        return remainder;
    }
}

// Normalize a vector of six joint angles to [-pi, pi) each.
// Returns an empty vector if the input size is not exactly six.
std::vector<double> normalizeChainAngles(const std::vector<double>& angles) {
    if (angles.size() != 6) {
        return {};
    }
    std::vector<double> result;
    result.reserve(6);
    for (double a : angles) {
        result.push_back(wrapToPi(a));
    }
    return result;
}

#include <cassert>
#include <cmath>
#include <vector>

double normalizeChainAngles(const std::vector<double>& angles);

int main() {
    // Basic cases in range
    {
        std::vector<double> in = {0.0, 1.0, -1.0, 3.0, -3.0, 2.5};
        std::vector<double> out = normalizeChainAngles(in);
        assert(out.size() == 6);
        for (size_t i = 0; i < 6; ++i) {
            assert(out[i] >= -3.14159265358979323846 && out[i] < 3.14159265358979323846);
        }
        // Check exact values for simple inputs
        assert(std::fabs(out[0] - 0.0) < 1e-12);
        assert(std::fabs(out[1] - 1.0) < 1e-12);
        assert(std::fabs(out[2] - (-1.0)) < 1e-12);
        assert(std::fabs(out[3] - 3.0) < 1e-12);
        assert(std::fabs(out[4] - (-3.0)) < 1e-12);
        assert(std::fabs(out[5] - 2.5) < 1e-12);
    }

    // Wrap positive values beyond pi
    {
        std::vector<double> in = {3.5, 6.283185307179586, 6.0, 10.0, 4.0, 7.0};
        std::vector<double> out = normalizeChainAngles(in);
        assert(out.size() == 6);
        // pi ~= 3.14159, 3.5 wraps to 3.5 - 2pi = -2.783185
        assert(std::fabs(out[0] - (3.5 - 2.0 * 3.14159265358979323846)) < 1e-12);
        // 2pi wraps to 0
        assert(std::fabs(out[1]) < 1e-12);
        // 6.0 wraps to 6.0 - 2pi = -0.283185
        assert(std::fabs(out[2] - (6.0 - 2.0 * 3.14159265358979323846)) < 1e-12);
        // 10.0 wraps to 10.0 - 2*2pi = -2.5663706
        assert(std::fabs(out[3] - (10.0 - 4.0 * 3.14159265358979323846)) < 1e-12);
        // 4.0 wraps to 4.0 - 2pi = -2.283185
        assert(std::fabs(out[4] - (4.0 - 2.0 * 3.14159265358979323846)) < 1e-12);
        // 7.0 wraps to 7.0 - 2pi = 0.7168147
        assert(std::fabs(out[5] - (7.0 - 2.0 * 3.14159265358979323846)) < 1e-12);
    }

    // Wrap negative values
    {
        std::vector<double> in = {-1.0, -3.5, -6.283185307179586, -10.0, -4.0, -7.0};
        std::vector<double> out = normalizeChainAngles(in);
        assert(out.size() == 6);
        // -1 stays -1
        assert(std::fabs(out[0] - (-1.0)) < 1e-12);
        // -3.5 + 2pi = 2.783185
        assert(std::fabs(out[1] - (-3.5 + 2.0 * 3.14159265358979323846)) < 1e-12);
        // -2pi + 2pi = 0
        assert(std::fabs(out[2]) < 1e-12);
        // -10 + 4pi = 2.5663706
        assert(std::fabs(out[3] - (-10.0 + 4.0 * 3.14159265358979323846)) < 1e-12);
        // -4 + 2pi = 2.283185
        assert(std::fabs(out[4] - (-4.0 + 2.0 * 3.14159265358979323846)) < 1e-12);
        // -7 + 2pi = -0.7168147 (in range, so true)
        assert(std::fabs(out[5] - (-7.0 + 2.0 * 3.14159265358979323846)) < 1e-12);
    }

    // Edge: pi itself should wrap to -pi (since we want [-pi, pi))
    {
        std::vector<double> in = {3.14159265358979323846, 0, 0, 0, 0, 0};
        std::vector<double> out = normalizeChainAngles(in);
        assert(std::fabs(out[0] + 3.14159265358979323846) < 1e-12);
    }

    // Edge: -pi should stay as -pi
    {
        std::vector<double> in = {-3.14159265358979323846, 0, 0, 0, 0, 0};
        std::vector<double> out = normalizeChainAngles(in);
        assert(std::fabs(out[0] + 3.14159265358979323846) < 1e-12);
    }

    // Edge: zero vector should stay zero
    {
        std::vector<double> in = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
        std::vector<double> out = normalizeChainAngles(in);
        for (double v : out) {
            assert(std::fabs(v) < 1e-12);
        }
    }

    // Wrong size returns empty
    {
        std::vector<double> in5 = {0.0, 0.0, 0.0, 0.0, 0.0};
        assert(normalizeChainAngles(in5).empty());
        std::vector<double> in7 = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
        assert(normalizeChainAngles(in7).empty());
        std::vector<double> inEmpty;
        assert(normalizeChainAngles(inEmpty).empty());
    }

    return 0;
}
