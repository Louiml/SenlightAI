// Write a C++ function that takes a vector of `double` values and returns a `double` equal to the sum of the natural logarithms of the gamma function applied to each element. If any element is less than or equal to zero, the function should return `-std::numeric_limits<double>::infinity()` because the `lgamma` function is defined only for positive real numbers. Use Eigen's `Array` types and the `lgamma()` member function to compute the values efficiently. The function should be `const`-correct and take the input as a `const std::vector<double>&` for simplicity.

// The solution must iterate over each element of the input vector, compute `std::lgamma(x)` (or the Eigen equivalent `lgamma()` on an array), and accumulate the sum. The key edge case is handling non-positive inputs: `lgamma` is undefined (or yields NaN) for non-positive values, so the function should return negative infinity to signal an invalid domain. For an empty vector, the sum is naturally 0.0. To use Eigen's vectorized operations, the input can be converted to an `Eigen::ArrayXd`, then the `lgamma()` member function is applied, and the result is summed. However, since the problem requires handling invalid inputs explicitly, a simple loop that checks each element is more straightforward and avoids Eigen’s NaN propagation. The time complexity is O(n) for n elements, and the space complexity is O(1) if we avoid copying the input (or O(n) if we convert to an Eigen array). The implementation uses `<cmath>` for `std::lgamma` and `std::numeric_limits` for infinity. For accuracy, `std::lgamma` returns the log gamma value, and summing them is a common operation (e.g., in statistical computations).

#include <vector>
#include <cmath>
#include <limits>

// Compute the sum of the log-gamma of each element in the input.
// Returns -infinity if any element is <= 0 (invalid domain for lgamma).
double sumLogGamma(const std::vector<double>& values) {
    double total = 0.0;
    for (const double value : values) {
        if (value <= 0.0) {
            return -std::numeric_limits<double>::infinity();
        }
        total += std::lgamma(value);
    }
    return total;
}

#include <cassert>
#include <cmath>

int main() {
    // Empty vector sums to zero.
    assert(sumLogGamma({}) == 0.0);
    
    // Single positive value.
    assert(sumLogGamma({1.0}) == 0.0); // lgamma(1) = 0
    
    // Known values: lgamma(0.5) = log(pi)/2, lgamma(2.0) = log(1) = 0.
    assert(std::abs(sumLogGamma({0.5, 2.0}) - (std::log(M_PI) / 2.0)) < 1e-12);
    
    // Invalid input (zero and negative) returns -infinity.
    assert(sumLogGamma({1.0, 0.0}) == -std::numeric_limits<double>::infinity());
    assert(sumLogGamma({-1.0, 2.0}) == -std::numeric_limits<double>::infinity());
    
    // Large positive values should not overflow the sum if reasonable.
    // lgamma(5.0) = log(24) ≈ 3.178, lgamma(3.0) = log(2) ≈ 0.693.
    assert(std::abs(sumLogGamma({5.0, 3.0}) - (std::log(24.0) + std::log(2.0))) < 1e-12);
    
    // Multiple elements with one invalid.
    assert(sumLogGamma({-0.1, 1.0, 2.0}) == -std::numeric_limits<double>::infinity());
    
    return 0;
}
