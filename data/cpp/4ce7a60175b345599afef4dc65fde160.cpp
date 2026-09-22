/*
Write a C++ function that computes and returns the mean and standard deviation of a sequence of floating-point collision-count values collected from multiple simulation samples. The function should take a `std::vector<unsigned int>` of raw collision counts (one per sample) and return a `std::pair<double,double>` where the first value is the sample mean and the second is the sample standard deviation (using the unbiased estimator with `n-1` in the denominator). Handle the edge case where the input vector has fewer than 2 elements: for an empty vector return `{0.0, 0.0}`, and for a single element return `{that value, 0.0}`. The function must be `const`-correct, use only standard library headers, and be independent of any external simulation code.
*/
#include <vector>
#include <cmath>
#include <utility>

// Compute the sample mean and sample standard deviation of collision counts.
// Returns {mean, std_dev}. For empty input returns {0.0, 0.0}.
// For a single element returns {value, 0.0}.
std::pair<double, double> collisionMetrics(const std::vector<unsigned int>& collisions) {
    if (collisions.empty()) {
        return {0.0, 0.0};
    }
    
    // Compute mean
    double sum = 0.0;
    for (unsigned int c : collisions) {
        sum += static_cast<double>(c);
    }
    double n = static_cast<double>(collisions.size());
    double mean = sum / n;

    if (collisions.size() == 1) {
        return {mean, 0.0};
    }

    // Compute sum of squared differences from mean
    double sum_sq = 0.0;
    for (unsigned int c : collisions) {
        double diff = static_cast<double>(c) - mean;
        sum_sq += diff * diff;
    }
    double variance = sum_sq / (n - 1.0);
    double std_dev = std::sqrt(variance);
    return {mean, std_dev};
}
#include <cassert>
#include <vector>
#include <cmath>
#include <utility>

// The solution function is assumed to be defined as above.
std::pair<double, double> collisionMetrics(const std::vector<unsigned int>& collisions);

int main() {
    // Empty vector
    auto res0 = collisionMetrics({});
    assert(res0.first == 0.0 && res0.second == 0.0);

    // Single element
    auto res1 = collisionMetrics({5});
    assert(res1.first == 5.0 && res1.second == 0.0);

    // Known values: {2, 4, 4, 4, 5, 5, 7, 9} -> mean = 5.0
    std::vector<unsigned int> data = {2, 4, 4, 4, 5, 5, 7, 9};
    auto res2 = collisionMetrics(data);
    assert(std::abs(res2.first - 5.0) < 1e-9);
    // Sample variance = 4.0, std = 2.0
    assert(std::abs(res2.second - 2.0) < 1e-9);

    // Two elements: {10, 20} -> mean=15, variance=50, std≈7.071
    auto res3 = collisionMetrics({10, 20});
    assert(std::abs(res3.first - 15.0) < 1e-9);
    assert(std::abs(res3.second - std::sqrt(50.0)) < 1e-9);

    // All identical values: std = 0
    auto res4 = collisionMetrics({7, 7, 7, 7});
    assert(res4.second == 0.0);

    // Larger values to check overflow safety (as doubles)
    std::vector<unsigned int> big = {4000000000u, 4000000000u, 4000000000u};
    auto res5 = collisionMetrics(big);
    assert(std::abs(res5.first - 4000000000.0) < 1e-6);
    assert(res5.second == 0.0);

    return 0;
}
// The task is straightforward: average the given unsigned integers as doubles and compute the standard deviation. The main algorithm: if the vector is empty, return `{0.0, 0.0}`. If it contains one element, return `{static_cast<double>(v[0]), 0.0}`. Otherwise, first pass computes the sum and then the mean `mean = sum / n` where `n = v.size()`. Second pass computes the sum of squared differences from the mean: `sum_sq = Σ (v[i] - mean)^2`. The sample standard deviation is `sqrt(sum_sq / (n - 1))`. Edge cases to consider: integer overflow when summing large unsigned values—cast to `double` early to avoid overflow. Also, division by zero is avoided by the explicit check for `n < 2`. Time complexity is `O(n)` for two passes (or one pass with Welford's algorithm for numerical stability, but for a teaching task two passes are clear and adequate). Space complexity is `O(1)` auxiliary. The solution must be a free function with a descriptive name, e.g., `collisionMetrics`, and must not include a `main` function.
