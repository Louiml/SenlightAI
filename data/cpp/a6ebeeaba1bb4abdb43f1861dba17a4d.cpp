// Write a standalone C++ function named `pool2d_avg_exclude_padding` that implements average pooling with a fixed 3×3 pooling window, stride 1, and configurable zero-padding on a 2D single-channel floating-point tensor represented as a contiguous `std::vector<float>` in row-major order (width × height). The function must accept the input vector, its width and height, and the left/top padding amounts (right/bottom padding are not used in this simplified task). For each output pixel at logical coordinates `(ox, oy)` (where output size equals input size due to stride 1), the function must compute the average of all source pixels within the 3×3 window centered at `(ox, oy)` after applying padding with value 0.0f, but only over those pixels that fall within the original unpadded source region. Specifically, if `exclude_padding` is true, the denominator is the count of valid (non-padding) source pixels in the window; if false, the denominator is always 9 (the full window size), and padded positions contribute 0.0f to the sum. The function must also handle partial windows at the borders correctly, returning a `std::vector<float>` of the same size as the input. Negative coordinate offsets larger than padding (which should never occur given the window is always fully contained after padding) can be safely ignored.

#include <cassert>
#include <vector>
#include <cmath>

int main() {
    // Test 1: 3x3 identity-like input, no padding, exclude_padding=true
    // Input:
    // 1 2 3
    // 4 5 6
    // 7 8 9
    // For center (1,1), window covers full 3x3, avg = (1+2+3+4+5+6+7+8+9)/9 = 5
    std::vector<float> img1 = {1,2,3, 4,5,6, 7,8,9};
    auto out1 = pool2d_avg_exclude_padding(img1, 3, 3, 0, 0, true);
    assert(out1.size() == 9);
    // The center output should be 5
    assert(std::abs(out1[4] - 5.0f) < 1e-5);
    // Corner (0,0): window covers top-left 2x2 => avg = (1+2+4+5)/4 = 3
    assert(std::abs(out1[0] - 3.0f) < 1e-5);

    // Test 2: Same input but exclude_padding=false
    auto out2 = pool2d_avg_exclude_padding(img1, 3, 3, 0, 0, false);
    // Corner: sum = 1+2+4+5 = 12, /9 = 1.333...
    assert(std::abs(out2[0] - 12.0f/9.0f) < 1e-5);
    // Center: sum = 45, /9 = 5
    assert(std::abs(out2[4] - 5.0f) < 1e-5);

    // Test 3: 1x1 input with padding 1 on all sides, exclude_padding=true
    // Input: [42], pad_left=1, pad_top=1
    // Only one output pixel at (0,0): window covers all padding, only center valid
    std::vector<float> img3 = {42.0f};
    auto out3 = pool2d_avg_exclude_padding(img3, 1, 1, 1, 1, true);
    assert(out3.size() == 1);
    assert(std::abs(out3[0] - 42.0f) < 1e-5); // count=1, sum=42

    // Test 4: Same 1x1 but exclude_padding=false
    auto out4 = pool2d_avg_exclude_padding(img3, 1, 1, 1, 1, false);
    assert(std::abs(out4[0] - 42.0f/9.0f) < 1e-5);

    // Test 5: 2x2 input, padding left=1 top=1, exclude_padding=true
    // Input:
    // 1 2
    // 3 4
    // Output 2x2. At (0,0): window covers source positions ranging x from -2 to 0, y from -2 to 0
    // Valid source pixels: (0,0) only => avg = 1
    std::vector<float> img5 = {1,2, 3,4};
    auto out5 = pool2d_avg_exclude_padding(img5, 2, 2, 1, 1, true);
    assert(out5.size() == 4);
    assert(std::abs(out5[0] - 1.0f) < 1e-5);

    // Test 6: Verify a specific non-trivial case manually
    // Input 2x2:
    // 10 20
    // 30 40
    // pad_left=1, pad_top=0, exclude_padding=false
    // Output 2x2. At output (0,0): window x from -1 to 1, y from -1 to 1
    // Valid xs: -1 (pad=0), 0, 1 ; valid ys: -1(pad),0,1
    // So valid pixels: (0,0)=10, (0,1)=20? wait careful:
    // sx = ox + dx - 1 - pad_left = ox+dx-1-1 = ox+dx-2 for dx=0,1,2
    // For ox=0: sx = -2,-1,0 -> only sx=0 valid (x=0)
    // sy = oy + dy - 0 - pad_top = oy+dy for dy=0,1,2
    // For oy=0: sy=0,1,2 -> only sy=0,1 valid (y=0,1)
    // So valid pixels: (sx=0,sy=0)=10, (sx=0,sy=1)=30
    // sum=40, /9 = 4.444...
    std::vector<float> img6 = {10,20, 30,40};
    auto out6 = pool2d_avg_exclude_padding(img6, 2, 2, 1, 0, false);
    assert(std::abs(out6[0] - 40.0f/9.0f) < 1e-5);

    // Test 7: Same but exclude_padding=true: sum=40, count=2 => 20
    auto out7 = pool2d_avg_exclude_padding(img6, 2, 2, 1, 0, true);
    assert(std::abs(out7[0] - 20.0f) < 1e-5);

    // Test 8: Empty input? Not specified, but size 0 should return empty
    std::vector<float> img8;
    auto out8 = pool2d_avg_exclude_padding(img8, 0, 0, 0, 0, true);
    assert(out8.empty());
}

#include <vector>
#include <cstddef>

// Perform 3x3 average pooling (stride 1, zero-padding) on a single-channel float image.
// Input: src (row-major, size width*height), src_w, src_h, pad_left, pad_top, exclude_padding.
// Output: vector of same size as input, where each element is the pooled result.
std::vector<float> pool2d_avg_exclude_padding(
    const std::vector<float>& src,
    int src_w,
    int src_h,
    int pad_left,
    int pad_top,
    bool exclude_padding)
{
    const int out_w = src_w; // stride 1, no output padding => same size
    const int out_h = src_h;

    std::vector<float> dst(out_w * out_h, 0.0f);

    const int pool_size = 3;
    const int center_offset = 1; // offset from window top-left to center

    for (int oy = 0; oy < out_h; ++oy) {
        for (int ox = 0; ox < out_w; ++ox) {
            float sum = 0.0f;
            int count = 0;

            // Iterate over 3x3 window
            for (int dy = 0; dy < pool_size; ++dy) {
                for (int dx = 0; dx < pool_size; ++dx) {
                    // Source coordinates after accounting for padding
                    int sx = ox + dx - center_offset - pad_left;
                    int sy = oy + dy - center_offset - pad_top;

                    // Check if this source coordinate is within the original image
                    if (sx >= 0 && sx < src_w && sy >= 0 && sy < src_h) {
                        sum += src[static_cast<size_t>(sy) * src_w + sx];
                        ++count;
                    }
                    // else: value is zero (padding), not counted
                }
            }

            // Compute average
            if (exclude_padding) {
                dst[static_cast<size_t>(oy) * out_w + ox] = sum / static_cast<float>(count);
            } else {
                dst[static_cast<size_t>(oy) * out_w + ox] = sum / 9.0f;
            }
        }
    }

    return dst;
}

// The core algorithm iterates over each output pixel `(oy, ox)` and examines a 3×3 neighborhood of source coordinates `(sy, sx)` where `sy = oy + dy - pad_top` and `sx = ox + dx - pad_left` for `dy, dx` in `{-1, 0, 1}`. For each such source coordinate, we determine if it lies inside the original image bounds `[0, height-1]` and `[0, width-1]`. If inside, the value is fetched from the input vector at `sy*width + sx`; if outside, the value is treated as 0.0f (padding). The sum accumulates these values, and the count tracks how many source pixels were inside bounds. If `exclude_padding` is true, the average is `sum / count` (where count ranges from 1 to 9); if false, the average is `sum / 9.0f` regardless of count. Edge cases include windows fully outside the image (only possible if padding is zero and output is at the very edge, but that still yields at least one valid pixel), and the output size equals the input size since stride is 1 and no output padding is considered. The time complexity is O(height * width * 9) = O(N) where N is the number of input pixels, and space complexity is O(1) auxiliary beyond the output vector.
