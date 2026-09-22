// Implement a C++ function named `computeOutlierAdjustedVariance` that takes two parameters: a `std::vector<double>` representing the process noise variances for seven states (the diagonal of a 7x7 state covariance matrix) and a `double` outlier rate between 0.0 and 1.0 (inclusive). The function must return a new `std::vector<double>` where each input variance is scaled by a factor that depends on the outlier rate: if the outlier rate is less than 0.5, multiply each variance by `1.0 / (1.0 - outlier_rate)`; if the outlier rate is exactly 0.5, multiply each variance by `2.0`; and if the outlier rate is greater than 0.5, multiply each variance by `1.0 / (outlier_rate)` to avoid excessive inflation. The function must reject invalid inputs: if the vector is empty, has a size other than 7, or the outlier rate is outside [0,1], it should return an empty vector. Ensure the function uses `const` references for inputs and returns by value. Do not modify the input vector.

// The solution first validates the input: the vector must have exactly 7 elements, and the outlier rate must be within `[0.0, 1.0]`. If either condition fails, return an empty vector. Otherwise, compute a scaling factor based on the outlier rate using a piecewise formula. The key is to avoid division by zero: if the rate is 0.0, `1.0 / (1.0 - 0.0) = 1.0` works fine; if the rate is 1.0, then `1.0 / (1.0 - 1.0)` would be division by zero, so the branch for `> 0.5` uses `1.0 / rate`, which is `1.0 / 1.0 = 1.0` and is safe. For rates exactly 0.5, both formulas would give 2.0, but we handle it explicitly for clarity. The function iterates over the 7-element vector, multiplies each element by the scaling factor, and returns the new vector. Time complexity is O(7) = O(1) and space complexity is O(1) for the scaling factor plus O(7) for the output vector, which is constant. Edge cases include outlier rate exactly 0.0, exactly 1.0, exactly 0.5, and invalid vector sizes or negative rates.

#include <vector>
#include <cmath>

// Compute outlier-adjusted process noise variances for a 7-state EKF.
// Returns an empty vector if inputs are invalid.
std::vector<double> computeOutlierAdjustedVariance(
    const std::vector<double>& inputVariances,
    double outlierRate
) {
    // Validate inputs
    if (inputVariances.size() != 7 || outlierRate < 0.0 || outlierRate > 1.0) {
        return {};
    }

    // Determine scaling factor based on outlier rate
    double scale;
    if (outlierRate < 0.5) {
        scale = 1.0 / (1.0 - outlierRate);
    } else if (outlierRate == 0.5) {
        scale = 2.0;
    } else { // outlierRate > 0.5
        scale = 1.0 / outlierRate;
    }

    // Create scaled output
    std::vector<double> result;
    result.reserve(inputVariances.size());
    for (double variance : inputVariances) {
        result.push_back(variance * scale);
    }
    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Normal case with rate 0.2 -> scale = 1/(1-0.2)=1.25
    std::vector<double> input1(7, 0.1);
    auto out1 = computeOutlierAdjustedVariance(input1, 0.2);
    assert(out1.size() == 7);
    for (double v : out1) {
        assert(std::fabs(v - 0.125) < 1e-9);
    }

    // Rate exactly 0.5 -> scale = 2.0
    auto out2 = computeOutlierAdjustedVariance({1.0,2.0,3.0,4.0,5.0,6.0,7.0}, 0.5);
    assert(out2.size() == 7);
    assert(out2[0] == 2.0);
    assert(out2[1] == 4.0);
    assert(out2[6] == 14.0);

    // Rate 0.0 -> scale = 1.0
    auto out3 = computeOutlierAdjustedVariance({1.0,1.0,1.0,1.0,1.0,1.0,1.0}, 0.0);
    for (double v : out3) assert(v == 1.0);

    // Rate 1.0 -> scale = 1.0 (since 1/1.0)
    auto out4 = computeOutlierAdjustedVariance({2.0,2.0,2.0,2.0,2.0,2.0,2.0}, 1.0);
    for (double v : out4) assert(v == 2.0);

    // Invalid size
    assert(computeOutlierAdjustedVariance({1.0,2.0,3.0}, 0.1).empty());

    // Invalid outlier rate
    assert(computeOutlierAdjustedVariance({1.0,2.0,3.0,4.0,5.0,6.0,7.0}, -0.1).empty());
    assert(computeOutlierAdjustedVariance({1.0,2.0,3.0,4.0,5.0,6.0,7.0}, 1.1).empty());

    // Input not modified
    std::vector<double> original(7, 3.0);
    auto out5 = computeOutlierAdjustedVariance(original, 0.8);
    assert(original[0] == 3.0);
    assert(out5[0] == 3.0 / 0.8);
}
