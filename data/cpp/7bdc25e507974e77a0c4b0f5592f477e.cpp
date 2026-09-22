// Write a C++ function that performs a 2D transposed convolution (also known as deconvolution) on a single-channel input matrix using a single kernel, with optional stride and dilation, and returns the output matrix. The function must accept the input matrix as a 2D `std::vector<float>` (row-major), the kernel as a 2D `std::vector<float>`, integer stride for both axes, integer dilation for both axes, and an optional bias value. The output dimensions are computed as: `out_h = (in_h - 1) * stride_h + dilation_h * (kernel_h - 1) + 1` and similarly for width. The function returns a 2D `std::vector<float>` of the computed output. Use zero-padding implicitly (no explicit padding parameter) — the convolution is "full" in the sense that the kernel is placed at every valid position where it overlaps the input, and the output size is the minimal size that covers all contributions. The implementation must handle stride and dilation correctly, ensuring that contributions are placed only at positions where both the row and column alignment conditions are satisfied (i.e., the input position maps to an output position such that `(out_idx - dilation*(k-1))` is non-negative, divisible by stride, and less than the input dimension). Edge cases include stride > 1, dilation > 1, and single-element input.

The transposed convolution operation is the gradient of a standard convolution with respect to its input. For each input pixel (i, j), we "stamp" the flipped kernel (or equivalently, the kernel in normal orientation but placed at positions determined by the stride) onto the output, multiplying by the input value. The output accumulates contributions from multiple input positions when stride > 1. The algorithm iterates over every output pixel (y, x). For each, we determine which input pixels and kernel positions could contribute. A kernel tap (ky, kx) contributes to output (y, x) if the input position `sy = (y + dilation_h * (kernel_h - 1) - dilation_h * ky)` is divisible by stride_h and lies in [0, in_h). Equivalently, for each output pixel, we loop over all kernel taps and check the alignment condition. A simpler and more direct approach is to iterate over input positions and kernel taps, accumulating into the output at the computed destination `(i*stride_h + dilation_h*ky, j*stride_w + dilation_w*kx)`. This avoids alignment checks and is straightforward: initialize output to bias, then for each input pixel and each kernel element, add `input[i][j] * kernel[ky][kx]` to `out[i*stride_h + dilation_h*ky][j*stride_w + dilation_w*kx]`. This works because the stride and dilation directly define where each kernel weight maps. The output dimensions are computed as above. Time complexity is O(in_h * in_w * kernel_h * kernel_w), and space complexity is O(out_h * out_w) for the output. Edge cases include stride or dilation greater than 1 (which simply creates gaps or spreads the kernel), and empty input (should return an empty vector or handle gracefully). The reference implementation uses row-major storage and returns a 2D vector.

#include <vector>
#include <cstddef>

// Perform 2D transposed convolution on a single-channel input with a single kernel.
// stride and dilation are positive integers. Bias is added to every output pixel.
// Returns a 2D vector of floats with dimensions:
//   out_h = (in_h - 1) * stride_h + dilation_h * (kernel_h - 1) + 1
//   out_w = (in_w - 1) * stride_w + dilation_w * (kernel_w - 1) + 1
std::vector<std::vector<float>> transposed_conv2d(
    const std::vector<std::vector<float>>& input,
    const std::vector<std::vector<float>>& kernel,
    int stride_h,
    int stride_w,
    int dilation_h,
    int dilation_w,
    float bias = 0.0f) {

    if (input.empty() || kernel.empty() || input[0].empty() || kernel[0].empty()) {
        return {};
    }

    const int in_h = static_cast<int>(input.size());
    const int in_w = static_cast<int>(input[0].size());
    const int k_h = static_cast<int>(kernel.size());
    const int k_w = static_cast<int>(kernel[0].size());

    const int out_h = (in_h - 1) * stride_h + dilation_h * (k_h - 1) + 1;
    const int out_w = (in_w - 1) * stride_w + dilation_w * (k_w - 1) + 1;

    // Initialize output with bias
    std::vector<std::vector<float>> output(out_h, std::vector<float>(out_w, bias));

    for (int i = 0; i < in_h; ++i) {
        for (int j = 0; j < in_w; ++j) {
            const float in_val = input[i][j];
            if (in_val == 0.0f) continue; // optional optimization

            for (int ky = 0; ky < k_h; ++ky) {
                const int out_y = i * stride_h + dilation_h * ky;
                for (int kx = 0; kx < k_w; ++kx) {
                    const int out_x = j * stride_w + dilation_w * kx;
                    output[out_y][out_x] += in_val * kernel[ky][kx];
                }
            }
        }
    }

    return output;
}

#include <cassert>
#include <vector>
#include <cmath>

int main() {
    // Test 1: Simple 2x2 input, 2x2 kernel, stride=1, dilation=1, no bias
    {
        std::vector<std::vector<float>> in = {{1, 2}, {3, 4}};
        std::vector<std::vector<float>> ker = {{1, 0}, {0, 1}};
        auto out = transposed_conv2d(in, ker, 1, 1, 1, 1, 0.0f);
        // Expected: each input pixel places the identity kernel
        // out[0][0] = 1*1 = 1
        // out[0][1] = 1*0 + 2*1 = 2
        // out[0][2] = 2*0 = 0
        // out[1][0] = 1*0 + 3*1 = 3
        // out[1][1] = 1*1 + 2*0 + 3*0 + 4*1 = 5
        // out[1][2] = 2*0 + 4*0 = 0? Wait compute fully below
        std::vector<std::vector<float>> expected = {
            {1, 2, 0},
            {3, 5, 2},
            {0, 3, 4}
        };
        assert(out.size() == 3 && out[0].size() == 3);
        for (size_t i = 0; i < 3; ++i)
            for (size_t j = 0; j < 3; ++j)
                assert(std::fabs(out[i][j] - expected[i][j]) < 1e-6);
    }

    // Test 2: Stride=2, 1x1 input, 3x3 kernel, dilation=1
    {
        std::vector<std::vector<float>> in = {{5}};
        std::vector<std::vector<float>> ker = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
        auto out = transposed_conv2d(in, ker, 2, 2, 1, 1, 1.0f);
        // out_h = (1-1)*2 + 1*(3-1)+1 = 3; out_w = same 3
        // Only input (0,0) places kernel at (0,0), (0,2), (2,0), (2,2)? Wait stride=2 means out_y = 0*2 + 1*ky = ky, so all three rows 0,1,2. Similarly columns all 0,1,2. So every position gets a kernel value, plus bias 1.
        std::vector<std::vector<float>> expected = {
            {2, 3, 4},
            {5, 6, 7},
            {8, 9, 10}
        };
        assert(out.size() == 3 && out[0].size() == 3);
        for (size_t i = 0; i < 3; ++i)
            for (size_t j = 0; j < 3; ++j)
                assert(std::fabs(out[i][j] - expected[i][j]) < 1e-6);
    }

    // Test 3: Dilation=2, 1x1 input, 2x2 kernel, stride=1
    {
        std::vector<std::vector<float>> in = {{1}};
        std::vector<std::vector<float>> ker = {{2, 3}, {4, 5}};
        auto out = transposed_conv2d(in, ker, 1, 1, 2, 2, 0.0f);
        // out_h = (1-1)*1 + 2*(2-1)+1 = 3, same for width
        // kernel positions: ky=0 -> out_y=0; ky=1 -> out_y=2 (since dilation*ky)
        // Similarly kx: 0,2
        // out = {{2,0,3}, {0,0,0}, {4,0,5}}
        std::vector<std::vector<float>> expected = {
            {2, 0, 3},
            {0, 0, 0},
            {4, 0, 5}
        };
        assert(out.size() == 3 && out[0].size() == 3);
        for (size_t i = 0; i < 3; ++i)
            for (size_t j = 0; j < 3; ++j)
                assert(std::fabs(out[i][j] - expected[i][j]) < 1e-6);
    }

    // Test 4: Empty input returns empty
    {
        std::vector<std::vector<float>> in;
        std::vector<std::vector<float>> ker = {{1}};
        auto out = transposed_conv2d(in, ker, 1, 1, 1, 1, 0.0f);
        assert(out.empty());
    }

    // Test 5: Single element kernel, stride and dilation >1
    {
        std::vector<std::vector<float>> in = {{2, 3}};
        std::vector<std::vector<float>> ker = {{7}};
        auto out = transposed_conv2d(in, ker, 3, 2, 2, 1, 1.0f);
        // in_h=1, in_w=2, k_h=1, k_w=1
        // out_h = (1-1)*3 + 2*(1-1)+1 = 1
        // out_w = (2-1)*2 + 1*(1-1)+1 = 3
        // Contributions: for input (0,0): out_y=0*3+2*0=0, out_x=0*2+1*0=0 -> add 2*7=14
        // for input (0,1): out_y=0, out_x=1*2+0=2 -> add 3*7=21
        // bias adds 1 to all positions
        std::vector<std::vector<float>> expected = {{15, 1, 22}};
        assert(out.size() == 1 && out[0].size() == 3);
        for (size_t i = 0; i < 3; ++i)
            assert(std::fabs(out[0][i] - expected[0][i]) < 1e-6);
    }

    return 0;
}
