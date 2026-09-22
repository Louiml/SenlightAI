Write a standalone C++ function that computes the forward pass of a 2D max pooling operation with configurable kernel size, stride, and zero-padding. The function must take as input: a 1D vector (flattened row-major tensor) representing the input feature map of shape (batch, channels, height, width), along with integer parameters for batch, channels, input height/width, kernel height/width, stride height/width, and padding height/width. It must return a 1D vector for the output feature map of shape (batch, channels, output_height, output_width), where each output element is the maximum over the valid (non-padded) region of the corresponding kernel window. Padding adds zeros around the input, and any kernel positions that fall entirely outside the input (i.e., the valid region is empty) are ignored (set to the lowest possible value for the type). The output dimensions are computed as floor((input + 2*padding - kernel) / stride) + 1, assuming this value is positive for all dimensions. Use `int` for all data values, and initialize output maxima to `std::numeric_limits<int>::min()`. The function must be named `max_pool2d_forward` and must be `const`-correct (take inputs by const reference, return by value). Test with several configurations including cases with padding, stride > 1, and non-square inputs.
// The solution follows the classic sliding-window max pooling algorithm. For each output position (ob, oc, oh, ow), we compute the corresponding input window start positions: `ih_start = oh * stride_h - pad_h` and `iw_start = ow * stride_w - pad_w`. Then we iterate over the kernel size `kh` and `kw`, calculating actual input indices `ih = ih_start + kh` and `iw = iw_start + kw`. If the input index is within bounds (0 <= ih < input_h and 0 <= iw < input_w), we compare with the running maximum and update if larger. Output dimensions: `output_h = (input_h + 2*pad_h - kernel_h) / stride_h + 1` using integer division (floor). Similarly for width. The function must handle the case where padding makes some kernel positions outside the input—these are skipped; if no valid position exists (possible when kernel is larger than the valid area, but given the formula, output dimensions are positive only if the valid area is at least partially covered; still, we handle gracefully by leaving the output as minimum). Edge cases: stride > kernel causing non-overlapping windows; padding larger than kernel producing all-valid or all-invalid windows; exact division. Time complexity: for each of B*C*output_h*output_w outputs, we do up to kernel_h*kernel_w comparisons → O(B*C*OH*OW*KH*KW). Space complexity: O(B*C*OH*OW) for the output vector, plus O(1) auxiliary.
#include <vector>
#include <cstddef>
#include <algorithm>
#include <limits>

// Forward max pooling on a flattened 2D feature map.
// Input is row-major: [batch][channel][height][width].
// Returns flattened output: [batch][channel][out_h][out_w].
// Padding adds zeros around the input; kernel positions outside are ignored.
std::vector<int> max_pool2d_forward(
        const std::vector<int>& input,
        int batch, int channels,
        int input_h, int input_w,
        int kernel_h, int kernel_w,
        int stride_h, int stride_w,
        int pad_h, int pad_w) {
    // Compute output dimensions.
    int out_h = (input_h + 2 * pad_h - kernel_h) / stride_h + 1;
    int out_w = (input_w + 2 * pad_w - kernel_w) / stride_w + 1;
    if (out_h <= 0 || out_w <= 0) return {};  // invalid configuration

    std::vector<int> output(batch * channels * out_h * out_w);
    int min_val = std::numeric_limits<int>::min();

    for (int b = 0; b < batch; ++b) {
        for (int c = 0; c < channels; ++c) {
            // Base offset for this (b,c) plane.
            int plane_offset_in = (b * channels + c) * (input_h * input_w);
            int plane_offset_out = (b * channels + c) * (out_h * out_w);

            for (int oh = 0; oh < out_h; ++oh) {
                for (int ow = 0; ow < out_w; ++ow) {
                    int ih_start = oh * stride_h - pad_h;
                    int iw_start = ow * stride_w - pad_w;
                    int max_val = min_val;

                    for (int kh = 0; kh < kernel_h; ++kh) {
                        int ih = ih_start + kh;
                        if (ih < 0 || ih >= input_h) continue;
                        for (int kw = 0; kw < kernel_w; ++kw) {
                            int iw = iw_start + kw;
                            if (iw < 0 || iw >= input_w) continue;
                            int idx = plane_offset_in + ih * input_w + iw;
                            max_val = std::max(max_val, input[idx]);
                        }
                    }

                    int out_idx = plane_offset_out + oh * out_w + ow;
                    output[out_idx] = max_val;
                }
            }
        }
    }
    return output;
}
#include <cassert>
#include <vector>

// Include the function definition above (or link).

int main() {
    // 1x1x3x3 input, kernel 2x2, stride 1, pad 0
    std::vector<int> inp1 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto out1 = max_pool2d_forward(inp1, 1, 1, 3, 3, 2, 2, 1, 1, 0, 0);
    assert(out1.size() == 4);
    assert(out1[0] == 5);  // max of 1,2,4,5
    assert(out1[1] == 6);  // max of 2,3,5,6
    assert(out1[2] == 8);  // max of 4,5,7,8
    assert(out1[3] == 9);  // max of 5,6,8,9

    // 1x1x3x3 input, kernel 2x2, stride 2, pad 0
    auto out2 = max_pool2d_forward(inp1, 1, 1, 3, 3, 2, 2, 2, 2, 0, 0);
    assert(out2.size() == 1);
    assert(out2[0] == 5);  // only window at (0,0)

    // 1x1x3x3 input, kernel 3x3, stride 1, pad 1
    auto out3 = max_pool2d_forward(inp1, 1, 1, 3, 3, 3, 3, 1, 1, 1, 1);
    assert(out3.size() == 9);
    // For each output position, max of valid entries in 3x3 window.
    // (0,0) window covers rows -1..1, cols -1..1, only valid entries: 1,2,4,5 → max 5
    assert(out3[0] == 5);
    // (0,1) window covers rows -1..1, cols 0..2, valid: 1,2,3,4,5,6 → max 6
    assert(out3[1] == 6);
    // (0,2) window covers rows -1..1, cols 1..3, valid: 2,3,5,6 → max 6
    assert(out3[2] == 6);
    // (1,0) window covers rows 0..2, cols -1..1, valid: 1,2,4,5,7,8 → max 8
    assert(out3[3] == 8);
    // (1,1) full window → max 9
    assert(out3[4] == 9);
    // (1,2) window covers rows 0..2, cols 1..3, valid: 2,3,5,6,8,9 → max 9
    assert(out3[5] == 9);
    // (2,0) window covers rows 1..3, cols -1..1, valid: 4,5,7,8 → max 8
    assert(out3[6] == 8);
    // (2,1) window covers rows 1..3, cols 0..2, valid: 4,5,6,7,8,9 → max 9
    assert(out3[7] == 9);
    // (2,2) window covers rows 1..3, cols 1..3, valid: 5,6,8,9 → max 9
    assert(out3[8] == 9);

    // 1x2x2x2 input, kernel 1x1, stride 1, pad 0
    std::vector<int> inp4 = {1, 2, 3, 4, 5, 6, 7, 8};
    auto out4 = max_pool2d_forward(inp4, 1, 2, 2, 2, 1, 1, 1, 1, 0, 0);
    assert(out4.size() == 8);
    for (size_t i = 0; i < out4.size(); ++i) assert(out4[i] == inp4[i]);

    // 2x1x1x1 input, kernel 1x1, stride 1, pad 0
    std::vector<int> inp5 = {10, -5};
    auto out5 = max_pool2d_forward(inp5, 2, 1, 1, 1, 1, 1, 1, 1, 0, 0);
    assert(out5.size() == 2);
    assert(out5[0] == 10);
    assert(out5[1] == -5);

    // Edge case: kernel larger than input with padding 0 → output invalid (empty)
    std::vector<int> inp6 = {1, 2, 3, 4};
    auto out6 = max_pool2d_forward(inp6, 1, 1, 2, 2, 3, 3, 1, 1, 0, 0);
    assert(out6.empty());

    // Edge case: stride larger than kernel, non-square input
    std::vector<int> inp7(16);
    for (int i = 0; i < 16; ++i) inp7[i] = i + 1;  // 1..16, shape 4x4
    auto out7 = max_pool2d_forward(inp7, 1, 1, 4, 4, 2, 2, 3, 3, 0, 0);
    // out_h = (4-2)/3+1 = 1, out_w = 1 → size 1
    assert(out7.size() == 1);
    // window at (0,0) covers rows 0-1, cols 0-1 → max = 6
    assert(out7[0] == 6);

    // Test with padding larger than kernel: still works
    std::vector<int> inp8 = {5};
    auto out8 = max_pool2d_forward(inp8, 1, 1, 1, 1, 3, 3, 1, 1, 2, 2);
    // out_h = (1+4-3)/1+1 = 3, out_w = 3 → 9 outputs
    assert(out8.size() == 9);
    // All windows include the center 5, so all outputs are 5
    for (int v : out8) assert(v == 5);

    return 0;
}
