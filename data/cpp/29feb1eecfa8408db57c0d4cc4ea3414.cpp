Write a standalone C++ function named `computeInt8MaxPool2D` that implements 2D max pooling on an 8-bit signed integer image with a rectangular kernel, stride, and explicit padding. The function takes a contiguous `std::vector<int8_t>` representing the input image in row-major order (height × width), along with the input height, input width, kernel height, kernel width, stride height, stride width, padding top, padding left, padding bottom, padding right, and an output zero-point value. The function must compute the output dimensions using the formula `output_dim = floor((input_dim + pad_begin + pad_end - kernel_dim) / stride) + 1`, and produce a contiguous `std::vector<int8_t>` of size (output_height × output_width) where each output element is the maximum of all valid input values within the corresponding kernel window. Positions where the kernel window falls entirely outside the padded input region (i.e., no valid input pixel overlaps) must be set to the output zero-point. Padding regions are ignored (not considered as valid values); only actual input pixels are considered for the maximum. The function must handle edge cases where the kernel window partially extends beyond the padded boundary, and must operate on `int8_t` values (which wrap naturally via signed integer comparison).

The solution requires iterating over each output position `(oy, ox)` and computing the corresponding input window starting at `(iy_start, ix_start) = (oy * stride_h - pad_top, ox * stride_w - pad_left)`. For each output position, we scan all offsets within the kernel dimensions `(kh, kw)`, computing the actual input indices `iy = iy_start + kh_offset`, `ix = ix_start + kw_offset`. If both `iy` is in `[0, input_height)` and `ix` is in `[0, input_width)`, we consider that input pixel for the maximum. If at least one valid pixel is found, the output is the maximum of those `int8_t` values; otherwise, the output is set to the zero-point. Because `int8_t` comparisons work naturally with signed values, we initialize the candidate maximum to the smallest possible `int8_t` (`-128`). The output dimensions are computed per the formula, ensuring integer arithmetic with `floor`. Key edge cases: (1) kernel window entirely outside the padded input (e.g., large padding) yields zero-point; (2) partial overlap must only count valid pixels; (3) stride values can be larger than kernel, causing skipped regions; (4) padding may be asymmetric. Time complexity is `O(output_height * output_width * kernel_h * kernel_w)` per element, which is `O(N * K)` where `N` is output size and `K` is kernel area. Space complexity is `O(output_height * output_width)` for the output vector, plus constant auxiliary space.

#include <vector>
#include <cstdint>
#include <algorithm>
#include <cassert>

/**
 * @brief Compute 2D max pooling on an int8_t image with explicit padding.
 * 
 * @param input       Contiguous input image in row-major order (height x width).
 * @param input_h     Input height.
 * @param input_w     Input width.
 * @param kernel_h    Kernel height.
 * @param kernel_w    Kernel width.
 * @param stride_h    Vertical stride.
 * @param stride_w    Horizontal stride.
 * @param pad_top     Padding above the image.
 * @param pad_left    Padding left of the image.
 * @param pad_bottom  Padding below the image.
 * @param pad_right   Padding right of the image.
 * @param zero_point  Output value for regions with no valid input overlap.
 * @return std::vector<int8_t> Pooled output in row-major order (output_h x output_w).
 */
std::vector<int8_t> computeInt8MaxPool2D(
    const std::vector<int8_t>& input,
    int input_h,
    int input_w,
    int kernel_h,
    int kernel_w,
    int stride_h,
    int stride_w,
    int pad_top,
    int pad_left,
    int pad_bottom,
    int pad_right,
    int8_t zero_point)
{
    // Validate inputs.
    assert(input.size() == static_cast<size_t>(input_h) * input_w);
    assert(kernel_h > 0 && kernel_w > 0);
    assert(stride_h > 0 && stride_w > 0);
    assert(pad_top >= 0 && pad_left >= 0 && pad_bottom >= 0 && pad_right >= 0);

    // Compute output dimensions.
    int output_h = (input_h + pad_top + pad_bottom - kernel_h) / stride_h + 1;
    int output_w = (input_w + pad_left + pad_right - kernel_w) / stride_w + 1;
    assert(output_h > 0 && output_w > 0);

    // Prepare output buffer.
    std::vector<int8_t> output(static_cast<size_t>(output_h) * output_w);

    for (int oy = 0; oy < output_h; ++oy) {
        for (int ox = 0; ox < output_w; ++ox) {
            // Starting position of the kernel in the input (can be negative due to padding).
            int iy_start = oy * stride_h - pad_top;
            int ix_start = ox * stride_w - pad_left;

            int8_t max_val = -128;  // Smallest int8_t value.
            bool found = false;

            for (int kh = 0; kh < kernel_h; ++kh) {
                int iy = iy_start + kh;
                if (iy < 0 || iy >= input_h) continue;  // Outside valid rows.

                for (int kw = 0; kw < kernel_w; ++kw) {
                    int ix = ix_start + kw;
                    if (ix < 0 || ix >= input_w) continue;  // Outside valid columns.

                    // Valid input pixel found.
                    found = true;
                    int8_t val = input[static_cast<size_t>(iy) * input_w + ix];
                    max_val = std::max(max_val, val);
                }
            }

            output[static_cast<size_t>(oy) * output_w + ox] = found ? max_val : zero_point;
        }
    }

    return output;
}

#include <cassert>
#include <vector>
#include <cstdint>

// Declare the solution function (assume it's defined above).
std::vector<int8_t> computeInt8MaxPool2D(
    const std::vector<int8_t>& input,
    int input_h, int input_w,
    int kernel_h, int kernel_w,
    int stride_h, int stride_w,
    int pad_top, int pad_left,
    int pad_bottom, int pad_right,
    int8_t zero_point);

int main() {
    // Test 1: Simple 2x2 max pool on a 4x4 image with stride 2, no padding.
    {
        std::vector<int8_t> in = {1, 2, 3, 4,
                                  5, 6, 7, 8,
                                  9, 10, 11, 12,
                                  13, 14, 15, 16};
        auto out = computeInt8MaxPool2D(in, 4, 4, 2, 2, 2, 2, 0, 0, 0, 0, 0);
        std::vector<int8_t> expected = {6, 8, 14, 16};
        assert(out == expected);
    }

    // Test 2: Max pool with padding on a 2x2 image, kernel 3x3, stride 1.
    {
        std::vector<int8_t> in = {1, 2,
                                  3, 4};
        // Output dimension: (2 + 1 + 1 - 3)/1 + 1 = 1 + 1 = 2? Let's compute: (2+1+1-3)=1, /1=1, +1=2.
        // Actually: (2+1+1-3)=1, /1=1, +1=2. So output is 2x2.
        // Each 3x3 window covers a region that may include padding (ignored).
        // For each output position, compute manually.
        auto out = computeInt8MaxPool2D(in, 2, 2, 3, 3, 1, 1, 1, 1, 1, 1, -128);
        // Output positions (oy,ox):
        // (0,0): window rows -1..1, cols -1..1 => valid pixels (0,0)=1, (0,1)=2, (1,0)=3, (1,1)=4 => max=4
        // (0,1): window rows -1..1, cols 0..2 => valid pixels (0,0)=1,(0,1)=2,(1,0)=3,(1,1)=4 => max=4
        // (1,0): window rows 0..2, cols -1..1 => same valid set => max=4
        // (1,1): rows 0..2, cols 0..2 => same => max=4
        std::vector<int8_t> expected = {4, 4, 4, 4};
        assert(out == expected);
    }

    // Test 3: Window completely outside padded area (large padding).
    {
        std::vector<int8_t> in = {5};
        // Kernel 3x3, stride 1, pad_top=5, pad_left=5, pad_bottom=0, pad_right=0.
        // Output dims: (1+5+0-3)/1+1 = 3+1=4? Actually (1+5+0-3)=3, /1=3, +1=4. So output 4x4.
        // The first output (0,0) window starts at row -5, col -5, covers rows -5..-3, cols -5..-3, no valid pixels.
        auto out = computeInt8MaxPool2D(in, 1, 1, 3, 3, 1, 1, 5, 5, 0, 0, -10);
        // Output (0,0) should be -10.
        assert(out[0] == -10);
        // Output (3,3) window starts at row 3-5=-2, col 3-5=-2, covers rows -2..0, cols -2..0 => includes pixel (0,0) => max=5
        assert(out[3*4+3] == 5);
    }

    // Test 4: Stride larger than kernel.
    {
        std::vector<int8_t> in = {1, 2, 3,
                                  4, 5, 6,
                                  7, 8, 9};
        // Kernel 1x1, stride 2, no padding => output 2x2 (since (3+0+0-1)/2+1=2).
        auto out = computeInt8MaxPool2D(in, 3, 3, 1, 1, 2, 2, 0, 0, 0, 0, 0);
        std::vector<int8_t> expected = {1, 3, 7, 9};
        assert(out == expected);
    }

    // Test 5: Kernel partially extends beyond right/bottom with padding.
    {
        std::vector<int8_t> in = {1, 2, 3,
                                  4, 5, 6,
                                  7, 8, 9};
        // Kernel 2x2, stride 1, pad_top=0, pad_left=0, pad_bottom=1, pad_right=1.
        // Output dims: (3+0+1-2)/1+1 = 2+1=3? (3+1-2)=2, /1=2, +1=3. So output 3x3.
        auto out = computeInt8MaxPool2D(in, 3, 3, 2, 2, 1, 1, 0, 0, 1, 1, -99);
        // For (2,2): window rows 2..3 (row 3 is padding), cols 2..3 (col 3 padding). Valid pixels: row2 col2=9 only => max=9.
        assert(out[2*3+2] == 9);
        // For (2,0): rows 2..3, cols 0..1 => valid rows 2, cols 0,1 => values 7,8 => max=8.
        assert(out[2*3+0] == 8);
        // For (0,0): rows 0..1, cols 0..1 => values 1,2,4,5 => max=5.
        assert(out[0] == 5);
    }

    // Test 6: All output positions fall outside image (kernel too large relative to output).
    {
        std::vector<int8_t> in = {1, 2};
        // Kernel 5x5, stride 1, pad_top=10, pad_left=10, pad_bottom=0, pad_right=0.
        // Output dims: (2+10+0-5)/1+1 = 7+1=8.
        auto out = computeInt8MaxPool2D(in, 1, 2, 5, 5, 1, 1, 10, 10, 0, 0, -1);
        // All output positions should be -1 because window never overlaps valid input? Actually some may, but for safety check first.
        assert(out[0] == -1);
        // The last output (7,7) window starts at row 7-10=-3, col 7-10=-3, covers rows -3..1, cols -3..1 => includes input rows 0..1? row 0 is valid, row1 is valid (index 1), cols include 0 and1? Actually input width is 2, so valid columns 0,1. The window cols -3..1 includes 0,1, so valid pixel exists. So the last output is max(1,2)=2.
        assert(out[7*8+7] == 2);
    }

    // Test 7: Negative int8_t values.
    {
        std::vector<int8_t> in = {-5, -10, 20, -1, 3, 0, -7, 8, -2};
        // 3x3, kernel 2x2, stride 2, no padding => output 1x1? (3+0+0-2)/2+1=1/2? (1)/2 integer =0, +1=1. Actually (3-2)=1, /2=0, +1=1. So output 1x1.
        auto out = computeInt8MaxPool2D(in, 3, 3, 2, 2, 2, 2, 0, 0, 0, 0, -128);
        // Window rows 0..1, cols 0..1 => values -5,-10,-1,3 => max=3.
        std::vector<int8_t> expected = {3};
        assert(out == expected);
    }

    return 0;
}
