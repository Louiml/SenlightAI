Write a standalone C++ function `evaluate_logistic_normalized` that takes a vector of real-valued parameters `x` (as `std::vector<double>`), applies a modified softmax transformation to produce normalized weights, and returns the sum of squared deviations of those weights from a target uniform distribution. Specifically, for each element `x[i]`, apply the logistic function `1/(1+exp(-x[i]))` to get a raw value in (0,1). Then normalize these raw values by dividing each by their total sum, so they form a probability-like vector `p`. The function must return the sum over all `i` of `(p[i] - 1/N)^2`, where `N` is the size of the input vector. Handle the edge case of an empty input vector by returning `0.0`. The function must be `const`-correct (i.e., not modify the input) and include appropriate `<vector>`, `<cmath>`, and `<numeric>` headers only.
The main algorithm is straightforward: compute the logistic sigmoid for each input element, sum them, normalize, and compute the squared deviation from a uniform target. For an input of size `N`, we need `N` calls to `exp` (or `std::exp`), one sum, `N` divisions, and `N` subtractions/squares/sums. Edge cases: empty input should immediately return `0.0` to avoid division by zero. For very large negative or positive `x[i]`, `exp(-x[i])` may underflow or overflow; using `1.0/(1.0+std::exp(-x[i]))` is numerically stable for most practical ranges, but if `x[i]` is extremely large (e.g., >700), `exp(-x[i])` underflows to 0 and the sigmoid becomes 1, which is acceptable. For extremely negative (e.g., < -700), `exp(-x[i])` overflows to infinity, making `1/(1+inf)` be 0; handling this directly with `1/(1+exp(-x))` works because the overflow yields `inf` and the result is 0.0. Time complexity is O(N), space is O(1) auxiliary (only a few doubles and the sum). No need to sort or modify the input. The solution uses `std::transform` or a manual loop for clarity.
#include <vector>
#include <cmath>
#include <numeric>

/**
 * @brief Compute the sum of squared deviations from a uniform distribution
 *        after applying a logistic (sigmoid) transformation and normalizing.
 * 
 * For each element x[i], compute s_i = 1/(1+exp(-x[i])). Then normalize
 * p_i = s_i / sum(s_j). Return sum_i (p_i - 1/N)^2. Empty input returns 0.0.
 * 
 * @param x Input vector of real numbers.
 * @return double The sum of squared deviations from uniform, or 0.0 if empty.
 */
double evaluate_logistic_normalized(const std::vector<double>& x) {
    if (x.empty()) {
        return 0.0;
    }
    
    const size_t N = x.size();
    std::vector<double> sigmoid(N);
    
    // Apply logistic function
    for (size_t i = 0; i < N; ++i) {
        sigmoid[i] = 1.0 / (1.0 + std::exp(-x[i]));
    }
    
    // Normalize
    double sum = std::accumulate(sigmoid.begin(), sigmoid.end(), 0.0);
    if (sum <= 0.0) {
        // Should never happen because sigmoid always > 0, but guard against
        // numeric issues and return a large penalty to indicate invalid.
        return std::numeric_limits<double>::infinity();
    }
    
    double target = 1.0 / static_cast<double>(N);
    double result = 0.0;
    for (size_t i = 0; i < N; ++i) {
        double p_i = sigmoid[i] / sum;
        double diff = p_i - target;
        result += diff * diff;
    }
    return result;
}
#include <cassert>
#include <cmath>

// forward declaration of the function under test
double evaluate_logistic_normalized(const std::vector<double>& x);

int main() {
    // Test empty input
    assert(std::abs(evaluate_logistic_normalized({}) - 0.0) < 1e-12);
    
    // Single element: sigmoid(x) normalizes to 1, deviation from 1 is 0.
    assert(std::abs(evaluate_logistic_normalized({0.0}) - 0.0) < 1e-12);
    assert(std::abs(evaluate_logistic_normalized({5.0}) - 0.0) < 1e-12);
    
    // Two elements: x = {0,0} => sigmoid each = 0.5, sum = 1.0, p = {0.5,0.5}, target=0.5 => 0.
    assert(std::abs(evaluate_logistic_normalized({0.0, 0.0}) - 0.0) < 1e-12);
    
    // Two elements: x = {10,0} => sigmoid(10)≈0.99995, sigmoid(0)=0.5, sum≈1.49995
    // p ≈ {0.66665, 0.33335}, target=0.5, deviations ≈ 0.02778 + 0.02778 ≈ 0.05556
    double val = evaluate_logistic_normalized({10.0, 0.0});
    double expected = 0.0;
    {
        double s0 = 1.0/(1.0+std::exp(-10.0));
        double s1 = 0.5;
        double sum = s0+s1;
        double p0 = s0/sum;
        double p1 = s1/sum;
        expected = (p0-0.5)*(p0-0.5) + (p1-0.5)*(p1-0.5);
    }
    assert(std::abs(val - expected) < 1e-9);
    
    // Three elements all equal: x = {2,2,2} => sigmoid same, normalized uniform => 0.
    assert(std::abs(evaluate_logistic_normalized({2.0,2.0,2.0}) - 0.0) < 1e-12);
    
    // Large values: x = {700, -700, 0} => sigmoids ≈ {1,0,0.5}, normalized ≈ {0.6667,0,0.3333}
    // target = 1/3. Compute approximate expected.
    double val2 = evaluate_logistic_normalized({700.0, -700.0, 0.0});
    {
        double s0 = 1.0; // exp(-700) underflows to 0
        double s1 = 0.0; // exp(700) overflows, 1/(1+inf)=0
        double s2 = 0.5;
        double sum = s0+s1+s2;
        double p0 = s0/sum;
        double p1 = s1/sum;
        double p2 = s2/sum;
        double t = 1.0/3.0;
        double expected2 = (p0-t)*(p0-t)+(p1-t)*(p1-t)+(p2-t)*(p2-t);
        assert(std::abs(val2 - expected2) < 1e-9);
    }
    
    return 0;
}
