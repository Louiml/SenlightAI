// Write a standalone C++ function named `conv_output_size` that, given the input spatial dimension, kernel size, padding, stride, and dilation (all as `int`), returns the output spatial dimension of a 2D convolution for that single axis. The formula is: `output = floor((input + 2 * padding - dilation * (kernel - 1) - 1) / stride) + 1`. The function must handle the edge case where the computed output dimension is less than or equal to zero; in that case, return 0 to indicate an invalid configuration. The function should be `const`-correct and use only the standard library. No `main` function or entry-point wrapper is needed.

// The solution is straightforward arithmetic derived from the standard convolution output size formula, which appears in the given Caffe code snippet: `output_dim = (input_dim + 2 * pad - kernel_extent) / stride + 1`, where `kernel_extent = dilation * (kernel - 1) + 1`. Expanding and simplifying gives: `output = floor((input + 2 * pad - dilation * (kernel - 1) - 1) / stride) + 1`. In C++, integer division truncates toward zero, which for non-negative numerators matches `floor`. However, if the numerator is negative (e.g., when the kernel is too large for the padded input), the division yields a negative or zero result, and adding 1 could yield 0 or positive unexpectedly. To be safe and match the requirement of returning 0 for invalid configurations, we first compute the numerator as a signed `int` (the values are reasonably small in typical use). If the numerator is negative or the resulting output is less than or equal to zero, return 0. Otherwise, return the computed value. Edge cases include: kernel size 1 with dilation 1 (no effective expansion), zero padding, and large strides that reduce output. The function uses only a few arithmetic operations, so time complexity is O(1) and space complexity is O(1).

#include <algorithm>

// Compute the output spatial size of a convolution on one axis.
// Returns 0 if the configuration is invalid (non-positive output size).
int conv_output_size(int input_size, int kernel_size, int padding, int stride, int dilation) {
    // Ensure non-negative parameters; treat negative as 0 for safety.
    input_size = std::max(0, input_size);
    kernel_size = std::max(1, kernel_size);
    padding = std::max(0, padding);
    stride = std::max(1, stride);
    dilation = std::max(1, dilation);

    // Effective kernel extent after dilation.
    int kernel_extent = dilation * (kernel_size - 1) + 1;

    // Numerator of the output size formula.
    int numerator = input_size + 2 * padding - kernel_extent;

    // If numerator is negative, the kernel does not fit; invalid.
    if (numerator < 0) {
        return 0;
    }

    int output_size = numerator / stride + 1;

    // Ensure positive; if not, return 0 for invalid.
    return output_size > 0 ? output_size : 0;
}

#include <cassert>

int main() {
    // Basic valid cases.
    assert(conv_output_size(7, 3, 0, 1, 1) == 5); // 7 -> 5
    assert(conv_output_size(7, 3, 1, 1, 1) == 7); // Same padding
    assert(conv_output_size(7, 3, 0, 2, 1) == 3); // Stride 2
    assert(conv_output_size(7, 3, 0, 1, 2) == 3); // Dilation 2

    // Kernel larger than input (no padding).
    assert(conv_output_size(3, 5, 0, 1, 1) == 0); // invalid
    // Kernel with padding fits.
    assert(conv_output_size(3, 5, 2, 1, 1) == 3); // output = (3+4-5)/1+1 = 3

    // Edge cases: kernel size 1.
    assert(conv_output_size(10, 1, 0, 1, 1) == 10);
    assert(conv_output_size(10, 1, 0, 2, 1) == 5);

    // Large stride producing output 1.
    assert(conv_output_size(5, 3, 0, 4, 1) == 1); // (5-3)/4+1 = 1

    // Dilation with kernel size 1 has no effect.
    assert(conv_output_size(5, 1, 0, 1, 100) == 5);

    // Non-positive input or invalid parameters.
    assert(conv_output_size(0, 3, 0, 1, 1) == 0);
    assert(conv_output_size(5, 0, 0, 1, 1) == 5); // clamped to kernel 1
    assert(conv_output_size(5, 3, 0, 0, 1) == 3); // stride clamped to 1
}
