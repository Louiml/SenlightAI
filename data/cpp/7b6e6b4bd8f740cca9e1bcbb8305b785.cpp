Write a standalone C++ function `gaussianPyramidScaleDown` that simulates the core subsampling logic of the Gaussian pyramid horizontal kernel (CLGaussianPyramidHorKernel) in a simplified, CPU-based form. The function takes a 1D vector of unsigned 8-bit input pixels (representing a single row of an image) and returns a vector of unsigned 16-bit output pixels (the scaled-down row). The horizontal operation applies a 1×5 Gaussian convolution [1, 4, 6, 4, 1] / 16 over the input, then subsamples by a factor of 2, selecting odd pixels (indices 1, 3, 5, ...) when the input width is even, and even pixels (indices 0, 2, 4, ...) when the input width is odd. The function must handle border pixels by clamping to the edge (replicate). The output length is `floor(input_width / 2)`. The input width is guaranteed to be at least 2. For example, for input width 4 (even), the output corresponds to input indices 1 and 3; for width 5 (odd), output corresponds to indices 0, 2, and 4. Note: the Gaussian kernel is applied at each subsampled position, computing the weighted sum over a 5-pixel window centered on the subsampled index (with clamping at boundaries), then dividing by 16 and rounding to nearest integer (using `std::lround` on the float division).

#include <cassert>
#include <vector>
#include <cstdint>

int main() {
    // Even width, offset=1, centers at 1 and 3
    // Input: [a,b,c,d] -> output at centers 1 and 3
    {
        std::vector<uint8_t> in = {10, 20, 30, 40};
        // center=1: pixels clamped [0,0,1,2,3] = [10,10,20,30,40]
        // sum = 1*10 +4*10 +6*20 +4*30 +1*40 = 10+40+120+120+40=330 -> 330/16=20.625 -> 21
        // center=3: pixels [1,2,3,3,3] = [20,30,40,40,40]
        // sum = 20+120+240+160+40=580 -> 580/16=36.25 -> 36
        auto out = gaussianPyramidScaleDown(in);
        assert(out.size() == 2);
        assert(out[0] == 21);
        assert(out[1] == 36);
    }

    // Odd width, offset=0, centers at 0,2,4
    {
        std::vector<uint8_t> in = {10, 20, 30, 40, 50};
        // center=0: pixels [0,0,0,1,2] = [10,10,10,20,30]
        // sum = 10+40+60+80+30=220 -> 220/16=13.75 -> 14
        // center=2: pixels [0,1,2,3,4] = [10,20,30,40,50]
        // sum = 10+80+180+160+50=480 -> 480/16=30
        // center=4: pixels [2,3,4,4,4] = [30,40,50,50,50]
        // sum = 30+160+300+200+50=740 -> 740/16=46.25 -> 46
        auto out = gaussianPyramidScaleDown(in);
        assert(out.size() == 2);  // 5/2 = 2
        assert(out[0] == 14);
        assert(out[1] == 30);
    }

    // Minimum width 2 (even)
    {
        std::vector<uint8_t> in = {0, 255};
        // center=1: pixels [0,0,0,1,1] = [0,0,0,255,255]
        // sum = 0+0+0+1020+255=1275 -> 1275/16=79.6875 -> 80
        auto out = gaussianPyramidScaleDown(in);
        assert(out.size() == 1);
        assert(out[0] == 80);
    }

    // All same values, even width
    {
        std::vector<uint8_t> in = {100, 100, 100, 100};
        // center=1: all clamped to 100 -> sum = (1+4+6+4+1)*100=1600 -> 100
        auto out = gaussianPyramidScaleDown(in);
        assert(out.size() == 2);
        assert(out[0] == 100);
        assert(out[1] == 100);
    }

    // Odd width with 3 elements (minimum odd)
    {
        std::vector<uint8_t> in = {5, 10, 15};
        // center=0: pixels [0,0,0,1,2] = [5,5,5,10,15]
        // sum = 5+20+30+40+15=110 -> 110/16=6.875 -> 7
        auto out = gaussianPyramidScaleDown(in);
        assert(out.size() == 1);
        assert(out[0] == 7);
    }

    return 0;
}

#include <vector>
#include <cstdint>
#include <cmath>
#include <algorithm>

/**
 * Simulates the horizontal Gaussian pyramid downsampling step.
 * Applies a 1x5 Gaussian kernel [1,4,6,4,1]/16 and subsamples every second pixel.
 * The subsampling offset depends on the parity of the input width:
 *   even width  -> select odd indices (1,3,5,...)
 *   odd width   -> select even indices (0,2,4,...)
 * Boundary handling uses clamping to the edge (replicate).
 *
 * @param input  A 1D vector of uint8_t pixels (row of an image)
 * @return A vector of uint16_t containing the scaled-down row
 */
std::vector<uint16_t> gaussianPyramidScaleDown(const std::vector<uint8_t>& input) {
    const size_t width = input.size();
    const size_t out_width = width / 2;
    std::vector<uint16_t> output(out_width);

    // Determine subsampling offset: 1 for even width, 0 for odd width
    const size_t offset = (width % 2 == 0) ? 1 : 0;

    // Gaussian kernel coefficients
    const int kernel[5] = {1, 4, 6, 4, 1};

    for (size_t i = 0; i < out_width; ++i) {
        const size_t center = i * 2 + offset;

        int sum = 0;
        for (int t = -2; t <= 2; ++t) {
            // Clamp index to valid range [0, width-1]
            const int idx = static_cast<int>(center) + t;
            const size_t clamped = static_cast<size_t>(std::clamp(idx, 0, static_cast<int>(width - 1)));
            sum += kernel[t + 2] * static_cast<int>(input[clamped]);
        }

        // Round to nearest using lround on float division
        output[i] = static_cast<uint16_t>(std::lround(static_cast<float>(sum) / 16.0f));
    }

    return output;
}

// The core idea is to first determine the subsampling offset based on the parity of the input width. If the width is even, we start at index 1 and take every second pixel (1, 3, 5, ...). If odd, we start at index 0 and take every second pixel (0, 2, 4, ...). For each selected output position `out_idx` (0-based), the corresponding input center index is `center = out_idx * 2 + offset`, where `offset = (width % 2 == 0) ? 1 : 0`. Now we apply the 5-tap Gaussian filter [1,4,6,4,1] over the input pixels from `center-2` to `center+2`, but since the input is 1D, we must handle out-of-bounds by clamping the index to the valid range [0, width-1]. For each tap `t` from -2 to 2, the input pixel is `input[std::clamp(center + t, 0, width-1)]`. The weighted sum is computed as `sum = 1*p0 + 4*p1 + 6*p2 + 4*p3 + 1*p4`. Then divide by 16 and round to nearest integer: `output_value = std::lround(static_cast<float>(sum) / 16.0f)`. Edge cases: when width is even and center is 1 (for out_idx=0), the left taps clamp to index 0; when center is near the right boundary, right taps clamp to width-1. For odd width, the first center is 0, so left taps clamp to 0. The output size is exactly `width / 2` using integer division. Time complexity is O(output_width * 5) = O(width) because the kernel taps are constant (5). Space complexity is O(1) extra for the output vector, which is O(width) by necessity.
