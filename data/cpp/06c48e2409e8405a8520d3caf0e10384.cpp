Write a standalone C++ function named `applyExponential` that takes a `std::vector<float>` as input and returns a new `std::vector<float>` where each element is the exponential (e^x) of the corresponding input element. The function should work for any non-empty vector (though it may also handle empty vectors gracefully by returning an empty vector), must not modify the input vector, and must use `std::exp` from `<cmath>` for the computation. The function signature should be `std::vector<float> applyExponential(const std::vector<float>& input)`. This task mimics the behavior of the TensorFlow Lite EXP op but in a simplified, self-contained form without any external dependencies.
#include <cassert>
#include <cmath>
#include <vector>

// Declaration of the function under test (included here for completeness).
std::vector<float> applyExponential(const std::vector<float>& input);

int main() {
    // Test with positive values
    std::vector<float> input1 = {0.0f, 1.0f, 2.0f};
    std::vector<float> result1 = applyExponential(input1);
    assert(result1.size() == 3);
    assert(std::fabs(result1[0] - 1.0f) < 1e-5);
    assert(std::fabs(result1[1] - std::exp(1.0f)) < 1e-5);
    assert(std::fabs(result1[2] - std::exp(2.0f)) < 1e-5);

    // Test with negative values (result between 0 and 1)
    std::vector<float> input2 = {-1.0f, -2.0f};
    std::vector<float> result2 = applyExponential(input2);
    assert(result2.size() == 2);
    assert(std::fabs(result2[0] - std::exp(-1.0f)) < 1e-5);
    assert(std::fabs(result2[1] - std::exp(-2.0f)) < 1e-5);

    // Test with zero and a large positive value (overflow to infinity is allowed)
    std::vector<float> input3 = {0.0f, 100.0f};
    std::vector<float> result3 = applyExponential(input3);
    assert(result3[0] == 1.0f);
    assert(std::isinf(result3[1]));

    // Test with an empty vector
    std::vector<float> input4;
    std::vector<float> result4 = applyExponential(input4);
    assert(result4.empty());

    // Test that the input is not modified (const correctness)
    std::vector<float> input5 = {3.0f, 4.0f};
    std::vector<float> original = input5;
    applyExponential(input5);
    assert(input5 == original);

    // Test a known value: exp(0.5f) ~ 1.64872
    std::vector<float> input6 = {0.5f};
    std::vector<float> result6 = applyExponential(input6);
    assert(std::fabs(result6[0] - 1.64872f) < 1e-4);

    return 0;
}
#include <vector>
#include <cmath>

// Compute the exponential of each element in the input vector.
// Returns a new vector where output[i] = exp(input[i]).
std::vector<float> applyExponential(const std::vector<float>& input) {
    std::vector<float> output;
    output.reserve(input.size());
    for (float value : input) {
        output.push_back(std::exp(value));
    }
    return output;
}
// The solution iterates over each element of the input vector and computes the exponential using `std::exp`, storing the result in a new output vector of the same size. Since the input is passed by const reference, we avoid copying for read access, and we allocate the output vector with the same size to preserve the tensor shape semantics. Edge cases include an empty input (return empty output), negative values (std::exp handles them naturally, producing values between 0 and 1), and large positive values that may overflow to infinity (which is acceptable behavior for floating-point, consistent with the original op). The time complexity is O(n) where n is the number of elements, and space complexity is O(n) for the output vector (no additional auxiliary structures). No special case handling is needed beyond simple iteration.
