Write a standalone C++ function that takes a vector of `double` values and a scalar multiplier, and returns a new vector of the same size where each element is the product of the scalar and the corresponding input element. The function should accept a `const std::vector<double>&` for the input vector and a `double` for the multiplier, and return a `std::vector<double>`. The input vector may be empty, in which case the function must return an empty vector. Ensure the function is `const`-correct, does not modify the input, and uses only standard library facilities (no external libraries like Eigen). The solution must be self-contained and compilable with a standard C++ compiler.

The approach is straightforward: iterate over the input vector and multiply each element by the scalar, storing results in a new vector. The main edge case is an empty input, which should naturally yield an empty output without errors. There are no special considerations for negative values or zeros—multiplication is performed directly. The algorithm runs in O(n) time, where n is the number of elements in the input vector, and uses O(n) auxiliary space to store the result vector. A `const` reference avoids copying the input, and the function itself is non-modifying and can be marked `const` if desired (though as a free function, we just ensure parameters are `const`). To improve robustness, we might reserve capacity in the output vector to avoid reallocations, but it's optional given typical vector growth behavior.

#include <vector>

// Multiply each element of the input vector by the scalar and return the result.
std::vector<double> multiplyVectorByScalar(const std::vector<double>& input, double multiplier) {
    std::vector<double> result;
    result.reserve(input.size());
    for (double value : input) {
        result.push_back(value * multiplier);
    }
    return result;
}

#include <cassert>
#include <cmath>

int main() {
    // Basic test with positive values
    std::vector<double> v1 = {1.0, 2.0, 3.0};
    auto r1 = multiplyVectorByScalar(v1, 2.0);
    assert(r1.size() == 3);
    assert(std::fabs(r1[0] - 2.0) < 1e-9);
    assert(std::fabs(r1[1] - 4.0) < 1e-9);
    assert(std::fabs(r1[2] - 6.0) < 1e-9);

    // Test with negative multiplier
    std::vector<double> v2 = {1.0, -2.0, 3.5};
    auto r2 = multiplyVectorByScalar(v2, -1.5);
    assert(r2.size() == 3);
    assert(std::fabs(r2[0] + 1.5) < 1e-9);
    assert(std::fabs(r2[1] - 3.0) < 1e-9);
    assert(std::fabs(r2[2] + 5.25) < 1e-9);

    // Test with zero multiplier
    std::vector<double> v3 = {5.0, -7.0, 0.0};
    auto r3 = multiplyVectorByScalar(v3, 0.0);
    assert(r3.size() == 3);
    for (double val : r3) {
        assert(std::fabs(val) < 1e-9);
    }

    // Test with empty input
    std::vector<double> v4;
    auto r4 = multiplyVectorByScalar(v4, 10.0);
    assert(r4.empty());

    // Test with single element
    std::vector<double> v5 = {7.5};
    auto r5 = multiplyVectorByScalar(v5, 2.0);
    assert(r5.size() == 1);
    assert(std::fabs(r5[0] - 15.0) < 1e-9);
}
