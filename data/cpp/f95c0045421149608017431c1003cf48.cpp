Write a standalone C++ function named `dequantizeTensor` that takes four parameters: a `std::vector<int> quantizedValues` containing 8-bit unsigned integer values (0–255), a float `scale` (greater than zero), a float `zeroPoint` (the quantized value corresponding to real value 0.0), and a `std::vector<float>& output` (which will be resized and filled). The function should convert each quantized integer `q` to its floating-point representation using the formula `real = (q - zeroPoint) * scale`. The function must validate that `scale > 0` and that the output vector is empty before filling it; if either condition fails, throw an `std::invalid_argument` exception. The function should handle empty input gracefully by leaving the output vector empty. This is a simplified, data-only version of the TFLite dequantization kernel shown, focusing only on the numeric transformation and without tensor shape or type dispatch logic.

The solution loops over each element in the input vector, applies the affine dequantization formula `(q - zeroPoint) * scale`, and appends each result to the output vector. Before processing, we validate that `scale` is positive and that the output vector is empty (to prevent accidental overwriting of existing data). For an empty input, the loop is skipped and the output remains empty, which is safe. Edge cases: `zeroPoint` can be any float, including negative or fractional; the input values are guaranteed 0–255 by the type `int` but we don't need to enforce it unless spec says so—here we just use them as-is. Time complexity is O(n) where n is the size of the input, and space complexity is O(n) for the output vector. Constant memory for loop variables.

#include <vector>
#include <stdexcept>

// Dequantize a vector of quantized integer values using scale and zeroPoint.
// Throws std::invalid_argument if scale <= 0 or if output is not empty.
void dequantizeTensor(const std::vector<int>& quantizedValues,
                      float scale,
                      float zeroPoint,
                      std::vector<float>& output) {
    // Validate inputs
    if (scale <= 0.0f) {
        throw std::invalid_argument("Scale must be greater than zero");
    }
    if (!output.empty()) {
        throw std::invalid_argument("Output vector must be empty before call");
    }

    // Reserve space to avoid reallocations (optional but efficient)
    output.reserve(quantizedValues.size());

    // Perform dequantization for each value
    for (int q : quantizedValues) {
        float real = static_cast<float>(q - zeroPoint) * scale;
        output.push_back(real);
    }
}

#include <cassert>
#include <vector>
#include <cmath>

int main() {
    // Basic dequantization with zeroPoint = 0, scale = 1
    std::vector<int> q1 = {0, 1, 2, 3};
    std::vector<float> out1;
    dequantizeTensor(q1, 1.0f, 0.0f, out1);
    assert(out1.size() == 4);
    assert(std::fabs(out1[0] - 0.0f) < 1e-6);
    assert(std::fabs(out1[1] - 1.0f) < 1e-6);
    assert(std::fabs(out1[2] - 2.0f) < 1e-6);
    assert(std::fabs(out1[3] - 3.0f) < 1e-6);

    // Example with typical TFLite int8 parameters: zeroPoint=24, scale=0.05
    std::vector<int> q2 = {24, 44, 4, 124};
    std::vector<float> out2;
    dequantizeTensor(q2, 0.05f, 24.0f, out2);
    assert(std::fabs(out2[0] - 0.0f) < 1e-6);
    assert(std::fabs(out2[1] - 1.0f) < 1e-6);
    assert(std::fabs(out2[2] - (-1.0f)) < 1e-6);
    assert(std::fabs(out2[3] - 5.0f) < 1e-6);

    // Empty input gives empty output
    std::vector<int> q3;
    std::vector<float> out3;
    dequantizeTensor(q3, 1.0f, 0.0f, out3);
    assert(out3.empty());

    // Throw on scale <= 0
    bool threw = false;
    try {
        std::vector<float> out4;
        dequantizeTensor({0}, 0.0f, 0.0f, out4);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Throw on non-empty output
    threw = false;
    try {
        std::vector<float> out5 = {0.0f};
        dequantizeTensor({0}, 1.0f, 0.0f, out5);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
