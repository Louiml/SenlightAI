Write a standalone C++ function that analyzes a time series of interval bounds stored in parallel vectors for multiple state variables and computes the minimum width ratio between inner and outer approximations at each time step, returning a vector of these ratios. The function should take as input: a vector of time points, a 3D vector `outer` where `outer[t][i]` is a pair `{lower, upper}` representing the outer approximation for variable `i` at time `t`, and a similarly structured 3D vector `inner` for the inner approximations. For each time step, compute the width of the inner interval divided by the width of the outer interval for each variable, take the minimum of these ratios across all variables, and store the result. If the width of the outer interval is zero (degenerate interval), the ratio should be set to 1.0 to avoid division by zero. The returned vector of ratios should have the same length as the number of time steps. Assume all input vectors are well-formed: for each time step, the number of variables in `outer` and `inner` is the same and at least 1, and each interval has `lower <= upper`.

The solution iterates over each time step `t` from 0 to `T-1`, where `T` is the number of time points. For each time step, it initializes a `minRatio` variable to a large value (e.g., `std::numeric_limits<double>::max()`). Then for each variable `i` (from 0 to `sysdim-1`), it extracts the widths: `outerWidth = outer[t][i].second - outer[t][i].first` and `innerWidth = inner[t][i].second - inner[t][i].first`. If `outerWidth` is exactly 0.0 (or very close due to floating-point), set the ratio for that variable to 1.0; otherwise compute `innerWidth / outerWidth`. Update `minRatio` as the minimum of its current value and the computed ratio. After processing all variables, push `minRatio` into the result vector. Edge cases: empty time series (if `timePoints` is empty, return an empty vector); if `outerWidth` is negative due to ill-formed input, handle gracefully by using absolute value or clamping to zero; but per specification, assume valid intervals. The algorithm runs in O(T * N) time where T is number of time steps and N is number of variables, and uses O(T) auxiliary space for the output. If the caller wants to avoid extra space, they could process step-by-step, but the task requires returning a vector.

#include <vector>
#include <utility>
#include <limits>
#include <algorithm>

// Compute the minimum width ratio (inner width / outer width) across all variables
// at each time step. Returns a vector of ratios, one per time step.
std::vector<double> minWidthRatioPerStep(
    const std::vector<double>& timePoints,
    const std::vector<std::vector<std::pair<double, double>>>& outer,
    const std::vector<std::vector<std::pair<double, double>>>& inner)
{
    const std::size_t T = timePoints.size();
    std::vector<double> ratios;
    ratios.reserve(T);

    for (std::size_t t = 0; t < T; ++t) {
        double minRatio = std::numeric_limits<double>::max();
        const std::vector<std::pair<double, double>>& outerVars = outer[t];
        const std::vector<std::pair<double, double>>& innerVars = inner[t];
        const std::size_t N = outerVars.size(); // same as innerVars.size()

        for (std::size_t i = 0; i < N; ++i) {
            double outerWidth = outerVars[i].second - outerVars[i].first;
            double innerWidth = innerVars[i].second - innerVars[i].first;

            double ratio;
            if (outerWidth == 0.0) {
                ratio = 1.0; // degenerate outer interval; avoid division by zero
            } else {
                ratio = innerWidth / outerWidth;
            }
            minRatio = std::min(minRatio, ratio);
        }
        ratios.push_back(minRatio);
    }

    return ratios;
}

#include <cassert>
#include <vector>
#include <utility>

// Declare the function (already defined above)
std::vector<double> minWidthRatioPerStep(
    const std::vector<double>& timePoints,
    const std::vector<std::vector<std::pair<double, double>>>& outer,
    const std::vector<std::vector<std::pair<double, double>>>& inner);

int main() {
    // Test 1: typical case with two variables and two time steps
    std::vector<double> timePoints = {0.0, 1.0};
    std::vector<std::vector<std::pair<double, double>>> outer = {
        {{0.0, 2.0}, {1.0, 3.0}},      // t=0: variables 0 and 1
        {{0.0, 4.0}, {2.0, 6.0}}       // t=1
    };
    std::vector<std::vector<std::pair<double, double>>> inner = {
        {{0.5, 1.5}, {1.5, 2.5}},      // t=0: widths 1.0, 1.0; outer widths 2.0, 2.0 => ratios 0.5, 0.5 => min 0.5
        {{1.0, 3.0}, {3.0, 5.0}}       // t=1: widths 2.0, 2.0; outer widths 4.0, 4.0 => ratios 0.5, 0.5 => min 0.5
    };
    std::vector<double> result = minWidthRatioPerStep(timePoints, outer, inner);
    assert(result.size() == 2);
    assert(result[0] == 0.5);
    assert(result[1] == 0.5);

    // Test 2: one variable, degenerate outer interval at a step
    timePoints = {0.0, 2.0};
    outer = {{{1.0, 1.0}}, {{0.0, 2.0}}};
    inner = {{{1.0, 1.0}}, {{0.5, 1.5}}};
    result = minWidthRatioPerStep(timePoints, outer, inner);
    assert(result.size() == 2);
    assert(result[0] == 1.0); // degenerate outer -> ratio 1.0
    assert(result[1] == 0.5); // width 1.0 / 2.0

    // Test 3: empty time series
    result = minWidthRatioPerStep({}, {}, {});
    assert(result.empty());

    // Test 4: multiple variables, take minimum across them
    timePoints = {0.0};
    outer = {{{0.0, 4.0}, {0.0, 10.0}}};
    inner = {{{1.0, 3.0}, {4.0, 6.0}}}; // widths: 2.0/4.0=0.5, 2.0/10.0=0.2 => min 0.2
    result = minWidthRatioPerStep(timePoints, outer, inner);
    assert(result.size() == 1);
    assert(result[0] == 0.2);

    return 0;
}
