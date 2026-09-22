/*
Create a C++ function named `depthwise_convolution_3x3` that performs a depthwise 3x3 convolution over an input matrix (2D single-channel), producing an output matrix. The function must accept the input matrix as a `std::vector<std::vector<float>>`, a 3x3 weight kernel as `std::vector<std::vector<float>>`, a stride (positive integer), and an optional bias (default 0.0f). The stride must be 1, 2, or 3; no padding is applied; and for simplicity, assume the input is large enough so that the kernel fits entirely within the input for the given stride (i.e., no boundary clamping is needed). The output dimensions must be computed as `floor((input_dim - 3) / stride) + 1` for each spatial dimension. The function should return a new matrix of the computed output dimensions where each element is the sum of element-wise multiplication of the kernel with the corresponding input region, plus the bias. The function should use `const` references for inputs and be efficient, with no unnecessary copies.
*/

#include <vector>

// Performs a depthwise 3x3 convolution with a given stride and optional bias.
// The input and weights are 2D matrices of floats. No padding is applied.
// Assumes the kernel fits entirely within the input for the given stride.
std::vector<std::vector<float>> depthwise_convolution_3x3(
    const std::vector<std::vector<float>>& input,
    const std::vector<std::vector<float>>& weights,
    int stride,
    float bias = 0.0f) {

    // Validate basic assumptions (optional but good practice)
    // Input must be non-empty, weights must be 3x3, stride must be 1,2, or 3
    const int input_rows = static_cast<int>(input.size());
    const int input_cols = (input_rows > 0) ? static_cast<int>(input[0].size()) : 0;
    const int kernel_size = 3;

    // Compute output dimensions
    const int output_rows = (input_rows - kernel_size) / stride + 1;
    const int output_cols = (input_cols - kernel_size) / stride + 1;

    // Initialize output matrix with zeros
    std::vector<std::vector<float>> output(output_rows, std::vector<float>(output_cols, 0.0f));

    // Perform convolution
    for (int oy = 0; oy < output_rows; ++oy) {
        const int start_y = oy * stride;
        for (int ox = 0; ox < output_cols; ++ox) {
            const int start_x = ox * stride;
            float sum = 0.0f;
            for (int ky = 0; ky < kernel_size; ++ky) {
                for (int kx = 0; kx < kernel_size; ++kx) {
                    sum += weights[ky][kx] * input[start_y + ky][start_x + kx];
                }
            }
            output[oy][ox] = sum + bias;
        }
    }

    return output;
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: Identity-like kernel with stride 1
    std::vector<std::vector<float>> input1 = {
        {1.0f, 2.0f, 3.0f},
        {4.0f, 5.0f, 6.0f},
        {7.0f, 8.0f, 9.0f}
    };
    std::vector<std::vector<float>> weights_identity = {
        {0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f}
    };
    auto result1 = depthwise_convolution_3x3(input1, weights_identity, 1, 0.0f);
    assert(result1.size() == 1);
    assert(result1[0].size() == 1);
    assert(result1[0][0] == 5.0f); // center value

    // Test 2: Stride 2 on a 5x5 input
    std::vector<std::vector<float>> input2 = {
        {1, 2, 3, 4, 5},
        {6, 7, 8, 9, 10},
        {11, 12, 13, 14, 15},
        {16, 17, 18, 19, 20},
        {21, 22, 23, 24, 25}
    };
    std::vector<std::vector<float>> weights2 = {
        {1, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    // stride 2: output size = (5-3)/2 + 1 = 2x2
    auto result2 = depthwise_convolution_3x3(input2, weights2, 2, 0.0f);
    assert(result2.size() == 2);
    assert(result2[0].size() == 2);
    // Patch at (0,0): top-left 3x3 → sum with kernel [1,0,0;0,0,0;0,0,0] = input[0][0] = 1
    assert(result2[0][0] == 1.0f);
    // Patch at (0,1): start col = 2 → input[0][2]=3
    assert(result2[0][1] == 3.0f);
    // Patch at (1,0): start row = 2 → input[2][0]=11
    assert(result2[1][0] == 11.0f);
    // Patch at (1,1): start (2,2) → input[2][2]=13
    assert(result2[1][1] == 13.0f);

    // Test 3: Stride 3 on a 6x6 input, all ones kernel, bias=5
    std::vector<std::vector<float>> input3(6, std::vector<float>(6, 1.0f));
    std::vector<std::vector<float>> weights_ones(3, std::vector<float>(3, 1.0f));
    // output size = (6-3)/3 + 1 = 2x2
    auto result3 = depthwise_convolution_3x3(input3, weights_ones, 3, 5.0f);
    // Each patch sum = 9*1 = 9, plus bias 5 => 14
    assert(result3.size() == 2);
    assert(result3[0].size() == 2);
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            assert(result3[i][j] == 14.0f);

    // Test 4: Stride 1 with bias
    std::vector<std::vector<float>> input4 = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    std::vector<std::vector<float>> weights_avg = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    // Sum of all 9 = 45, plus bias 10 = 55
    auto result4 = depthwise_convolution_3x3(input4, weights_avg, 1, 10.0f);
    assert(result4.size() == 1);
    assert(result4[0].size() == 1);
    assert(result4[0][0] == 55.0f);

    // Test 5: Stride 2 on a 7x7 input with a specific kernel
    std::vector<std::vector<float>> input5(7, std::vector<float>(7, 2.0f));
    std::vector<std::vector<float>> weights5 = {
        {1, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    // output size = (7-3)/2 + 1 = 3x3
    auto result5 = depthwise_convolution_3x3(input5, weights5, 2, 0.0f);
    assert(result5.size() == 3);
    assert(result5[0].size() == 3);
    // Every patch top-left element is 2.0
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            assert(result5[i][j] == 2.0f);

    return 0;
}

// The solution directly implements the definition of a depthwise convolution: each output pixel is the dot product of the 3x3 kernel with the corresponding 3x3 input patch, plus bias. For a given output row `oy` and column `ox`, the corresponding input patch starts at (row = `oy * stride`, col = `ox * stride`). Loop over the kernel height (0..2) and width (0..2), multiply `weights[ky][kx]` by `input[y + ky][x + kx]`, accumulate, then add bias. The main edge cases are: stride of 1 (standard convolution with no padding), stride of 2 (halves output dimensions), and stride of 3 (output dimensions computed exactly). Since the problem guarantees the kernel fits entirely, we do not handle boundary conditions. The implementation uses `const` references and preallocates the output matrix with the correct dimensions. Time complexity is O(output_rows * output_cols * 9) = O(n*m*9), which is O(n*m) for constant kernel size. Space complexity is O(output_rows * output_cols) for the output matrix.
