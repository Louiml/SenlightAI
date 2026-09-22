// Write a C++ function named `computeApoptosisRate` that takes a vector of doubles representing the membrane voltage trajectory `V` (in millivolts) at equally spaced time points, a scalar `timeStep` (in milliseconds), and a double `threshold` (in millivolts). The function must return the average rate of change of the voltage over the entire trajectory, defined as the mean of the absolute values of all forward differences `abs(V[i+1] - V[i])` divided by `timeStep`. However, if any forward difference exceeds `threshold` in absolute value, treat that difference as a spike and replace it by `0.0` before computing the average. If the vector has fewer than two elements, return `0.0`. The function must be `const`-correct and not modify its inputs.

#include <cassert>
#include <vector>
#include <cmath>

// Since the function is defined above, we only need to test.
int main() {
    // Basic linear increase: each step 1.0 mV, timeStep 1 ms => rate 1.0
    std::vector<double> v1 = {0.0, 1.0, 2.0, 3.0};
    assert(std::fabs(computeApoptosisRate(v1, 1.0, 10.0) - 1.0) < 1e-9);

    // With threshold 0.5, all differences exceed -> all zero -> rate 0
    assert(std::fabs(computeApoptosisRate(v1, 1.0, 0.5) - 0.0) < 1e-9);

    // Mixed: differences 0.1, 5.0, 0.1, threshold 1.0 -> ignore 5.0, sum=0.2, divide by 3 => 0.066666...
    std::vector<double> v2 = {0.0, 0.1, 5.1, 5.2};
    double expected = (0.1 + 0.0 + 0.1) / (3.0 * 1.0); // 0.2/3
    assert(std::fabs(computeApoptosisRate(v2, 1.0, 1.0) - expected) < 1e-9);

    // Empty vector
    std::vector<double> v3;
    assert(computeApoptosisRate(v3, 1.0, 1.0) == 0.0);

    // Single element
    std::vector<double> v4 = {42.0};
    assert(computeApoptosisRate(v4, 1.0, 1.0) == 0.0);

    // Non-unit time step: steps of 2.0 mV over 0.5 ms => rate = 2.0/0.5 = 4.0
    std::vector<double> v5 = {0.0, 2.0, 4.0};
    assert(std::fabs(computeApoptosisRate(v5, 0.5, 10.0) - 4.0) < 1e-9);

    // Negative threshold: all differences are > negative -> all ignored -> 0
    assert(std::fabs(computeApoptosisRate(v1, 1.0, -1.0) - 0.0) < 1e-9);

    // Zero time step -> returns 0 (avoid division by zero)
    assert(computeApoptosisRate(v1, 0.0, 1.0) == 0.0);

    // Large spike exactly at threshold: not ignored (since >, not >=)
    std::vector<double> v6 = {0.0, 2.0, 2.0};
    assert(std::fabs(computeApoptosisRate(v6, 1.0, 2.0) - (2.0 + 0.0)/2.0) < 1e-9); // 1.0

    // All equal -> rate 0
    std::vector<double> v7 = {5.0, 5.0, 5.0};
    assert(computeApoptosisRate(v7, 1.0, 0.0) == 0.0);

    return 0;
}

#include <vector>
#include <cmath>

// Compute the mean absolute forward difference of a voltage trajectory,
// ignoring (zeroing) differences that exceed a given threshold.
double computeApoptosisRate(const std::vector<double>& V, double timeStep, double threshold) {
    if (V.size() < 2 || timeStep <= 0.0) {
        return 0.0;
    }

    double sum = 0.0;
    for (std::size_t i = 0; i + 1 < V.size(); ++i) {
        double diff = std::fabs(V[i + 1] - V[i]);
        if (diff > threshold) {
            diff = 0.0;  // spike, ignore
        }
        sum += diff;
    }

    return sum / (static_cast<double>(V.size() - 1) * timeStep);
}

// The solution iterates over the vector, computing each consecutive difference. For each difference, if its absolute value is greater than the given threshold, we set the contribution to zero (simulating spike removal). Otherwise, we keep the absolute difference. We accumulate these adjusted absolute differences. The average rate is the accumulated sum divided by the product `(n-1) * timeStep`, where `n` is the number of elements. This gives the mean absolute rate in units of millivolts per millisecond. Edge cases: vector size 0 or 1 returns 0.0; threshold negative? We treat any threshold as a non-negative threshold, but comparisons use `>` on absolute value, so negative threshold behaves like zero (all differences larger than negative are spikes). Time step must be positive; if timeStep <= 0, we can return 0.0 to avoid division by zero. Time complexity is O(n) because we scan once, space complexity O(1). The function must be free-standing, take a `const std::vector<double>&` to avoid copying, and `double` parameters by value.
