/*
Write a C++ function `computeAverage` that takes a vector of floating-point numbers and returns their arithmetic mean as a `double`. The function should handle an empty vector by returning `0.0`. For a non-empty vector, compute the sum of all elements and divide by the number of elements. The function must be `const`-correct (accept a `const std::vector<float>&`), use only standard library headers, and avoid modifying the input. The task requires a robust standalone implementation, not a full program, and must be suitable for unit testing with assertions.
*/
#include <vector>

// Compute the arithmetic mean of a vector of floats.
// Returns 0.0 for an empty vector.
double computeAverage(const std::vector<float>& data) {
    if (data.empty()) {
        return 0.0;
    }

    double sum = 0.0;
    for (float value : data) {
        sum += static_cast<double>(value);
    }

    return sum / static_cast<double>(data.size());
}
#include <cassert>
#include <vector>
#include <cmath>

// Function prototype for testing
double computeAverage(const std::vector<float>& data);

int main() {
    // Empty vector
    assert(computeAverage({}) == 0.0);

    // Single element
    assert(computeAverage({5.0f}) == 5.0);

    // Simple case
    assert(std::abs(computeAverage({1.0f, 2.0f, 3.0f, 4.0f}) - 2.5) < 1e-6);

    // Negative numbers
    assert(std::abs(computeAverage({-1.0f, -2.0f, -3.0f}) - (-2.0)) < 1e-6);

    // Mixed signs
    assert(std::abs(computeAverage({-10.0f, 5.0f, 15.0f}) - 3.333333) < 1e-5);

    // Large vector (1000 elements of 2.5)
    std::vector<float> big(1000, 2.5f);
    assert(std::abs(computeAverage(big) - 2.5) < 1e-6);

    // Values that may lose precision in float but fine in double
    assert(std::abs(computeAverage({0.1f, 0.2f, 0.3f}) - 0.2) < 1e-6);

    // All zeros
    assert(computeAverage({0.0f, 0.0f, 0.0f}) == 0.0);

    return 0;
}
// The solution iterates through the vector using a range-based for loop over a `const` reference to avoid copying. A `double` accumulator is used for the sum to reduce floating-point rounding errors compared to using `float`. For an empty vector, the function returns `0.0` as a defined edge case; otherwise, it returns `sum / static_cast<double>(data.size())`. The main algorithm is a single pass, giving O(n) time complexity (where n is the vector size) and O(1) auxiliary space, since we only store the sum and use the input vector by reference. Edge cases include an empty input, a single element (the average equals that element), negative numbers, and large magnitudes that may cause overflow if summed with a `float`—using `double` mitigates this partially. No special handling is required for `NaN` or infinity unless specified, but the implementation would naturally propagate them.
