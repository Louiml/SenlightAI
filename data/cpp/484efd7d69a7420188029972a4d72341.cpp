/*
Write a C++ function named `applyFloor` that takes a reference to a `std::vector<float>` representing input data and returns a `std::vector<float>` where each element is the mathematical floor of the corresponding input element (i.e., the largest integer value less than or equal to the input). The function should preserve the order and length of the input, handle empty vectors gracefully by returning an empty vector, and correctly process negative values (e.g., floor(-3.7) = -4.0). The function must be `const`-correct regarding the input, use `std::floor` from `<cmath>`, and not modify the input vector. The implementation should be self-contained with all necessary headers included.
*/

#include <vector>
#include <cmath>

// Apply mathematical floor to each element of the input vector.
// Returns a new vector with the same size as input, each element being floor(input[i]).
std::vector<float> applyFloor(const std::vector<float>& input) {
    std::vector<float> output;
    output.reserve(input.size());
    for (float value : input) {
        output.push_back(static_cast<float>(std::floor(value)));
    }
    return output;
}

#include <cassert>
#include <cmath>
#include <vector>

// The solution function is expected to be defined above this test.
// For completeness, we include a minimal declaration for testing.
std::vector<float> applyFloor(const std::vector<float>& input);

int main() {
    // Basic positive values
    std::vector<float> v1 = {1.5f, 2.9f, 0.0f, 3.0f};
    std::vector<float> r1 = applyFloor(v1);
    assert(r1.size() == 4);
    assert(r1[0] == 1.0f && r1[1] == 2.0f && r1[2] == 0.0f && r1[3] == 3.0f);

    // Negative and fractional values
    std::vector<float> v2 = {-1.5f, -0.5f, -2.0f, -0.0f};
    std::vector<float> r2 = applyFloor(v2);
    assert(r2.size() == 4);
    assert(r2[0] == -2.0f && r2[1] == -1.0f && r2[2] == -2.0f && r2[3] == 0.0f);

    // Empty input
    std::vector<float> v3;
    std::vector<float> r3 = applyFloor(v3);
    assert(r3.empty());

    // Single element
    std::vector<float> v4 = {42.42f};
    std::vector<float> r4 = applyFloor(v4);
    assert(r4.size() == 1 && r4[0] == 42.0f);

    // Very large numbers
    std::vector<float> v5 = {1e10f, -1e10f, 1e-5f};
    std::vector<float> r5 = applyFloor(v5);
    assert(r5[0] == 1e10f);
    assert(r5[1] == -1e10f);
    assert(r5[2] == 0.0f);
}

// The solution iterates over each element of the input vector and applies the standard library `std::floor` function, which returns a `double`; we cast this result to `float` to match the element type. The main algorithmic consideration is that `std::floor` correctly handles negative numbers, zero, positive numbers, and values like -0.5, returning the nearest integer less than or equal to the input. Edge cases include an empty input (return empty output), values like -2.0 (floor is -2.0), and large floats where precision matters; `std::floor` is required by the C++ standard to handle all representable float values correctly. Time complexity is O(n) where n is the number of elements, and space complexity is O(n) for the output vector.
