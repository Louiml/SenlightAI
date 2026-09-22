// Write a C++ function named `computeSGBMCostVolume` that, given two grayscale images (`left` and `right`) of equal size (`width × height`), a minimum disparity `minD`, a maximum disparity `maxD` (exclusive), and a penalty constant `P1`, computes a raw pixel-wise matching cost volume using the Birchfield–Tomasi (BT) dissimilarity measure. The function must return a `std::vector<short>` of size `width * height * (maxD - minD)`, laid out in row-major order of pixels (`y * width + x`), and for each pixel, the disparity dimension is contiguous with `d` ranging from `minD` to `maxD-1`. The cost for a pixel `(x,y)` and disparity `d` is defined as the minimum of two directional BT costs: one comparing the left pixel's intensity with the right pixel's interpolated intensity range at `x-d`, and the other comparing the right pixel's intensity with the left pixel's interpolated range, each clamped to zero and summed over the three color channels (here single channel, so just the one value). For pixels where `x-d` falls outside the right image boundaries, the cost must be set to `SHRT_MAX`. The function must be self-contained with no external dependencies beyond the C++ standard library, and must handle arbitrary dimensions and disparity ranges, including `minD` potentially being negative.
// The solution computes the BT cost directly without any precomputed lookup tables or SIMD optimizations. For each pixel `(x,y)` in the left image, we iterate over each disparity `d` in `[minD, maxD)`. The right pixel index is `x2 = x - d`. If `x2 < 0` or `x2 >= width`, we set the cost to `SHRT_MAX` (a sentinel for invalid matches). Otherwise, we use the Birchfield–Tomasi dissimilarity: we compute the interpolated left and right intensity values `I_left_min` and `I_left_max` by considering the pixel's left and right neighbors (clipped at borders) and averaging with the center intensity. Similarly for the right pixel. Then we compute `cost1 = max(0, I_left - I_right_max, I_right_min - I_left)` and `cost2 = max(0, I_right - I_left_max, I_left_min - I_right)`, and the final cost is `min(cost1, cost2)`. This yields an integer cost that is non-negative. For a single-channel image, this is directly the cost. The time complexity is `O(width * height * (maxD - minD))`, and the space complexity is the same for the output vector. Edge cases include: `minD` negative (so `x-d` can exceed width), `x-d` out of bounds on either side, and border pixels where neighbors do not exist (we clamp to the border pixel value). We also handle the case where `maxD <= minD` by returning an empty vector.
#include <vector>
#include <algorithm>
#include <cstddef>
#include <limits>

// Compute a Birchfield-Tomasi raw matching cost volume for two grayscale images.
// left and right are 8-bit grayscale images of equal dimensions.
// Returns a vector of size height * width * (maxD - minD).
// Layout: for pixel (x,y), disparity d in [minD, maxD), index = ((y * width + x) * numDisp + (d - minD)).
// Out-of-bound matches get cost SHRT_MAX.
std::vector<short> computeSGBMCostVolume(
    const std::vector<unsigned char>& left,
    const std::vector<unsigned char>& right,
    int width,
    int height,
    int minD,
    int maxD)
{
    // Validate inputs
    if (width <= 0 || height <= 0 || maxD <= minD) return {};
    if (left.size() != static_cast<size_t>(width) * height) return {};
    if (right.size() != static_cast<size_t>(width) * height) return {};

    const int numDisp = maxD - minD;
    const size_t totalCosts = static_cast<size_t>(width) * height * numDisp;
    std::vector<short> costVolume(totalCosts, 0);
    const short OUT_OF_BOUNDS = std::numeric_limits<short>::max();

    const int maxX = width - 1;
    const int maxY = height - 1;

    for (int y = 0; y < height; ++y) {
        const size_t rowOffset = static_cast<size_t>(y) * width;
        for (int x = 0; x < width; ++x) {
            const size_t pixelIndex = rowOffset + x;
            const size_t costBase = pixelIndex * numDisp;

            const unsigned char il = left[pixelIndex];
            // Left pixel neighbors (clamp at borders)
            const unsigned char il_left  = (x > 0) ? left[rowOffset + x - 1] : il;
            const unsigned char il_right = (x < maxX) ? left[rowOffset + x + 1] : il;
            // Interpolated range for left pixel
            const int il_min = std::min(il, std::min((il + il_left) / 2, (il + il_right) / 2));
            const int il_max = std::max(il, std::max((il + il_left) / 2, (il + il_right) / 2));

            for (int d = minD; d < maxD; ++d) {
                const int x2 = x - d;
                if (x2 < 0 || x2 >= width) {
                    costVolume[costBase + (d - minD)] = OUT_OF_BOUNDS;
                    continue;
                }

                const size_t rightPixel = static_cast<size_t>(y) * width + x2;
                const unsigned char ir = right[rightPixel];
                // Right pixel neighbors (clamp at borders)
                const unsigned char ir_left  = (x2 > 0) ? right[static_cast<size_t>(y) * width + x2 - 1] : ir;
                const unsigned char ir_right = (x2 < maxX) ? right[static_cast<size_t>(y) * width + x2 + 1] : ir;
                // Interpolated range for right pixel
                const int ir_min = std::min(ir, std::min((ir + ir_left) / 2, (ir + ir_right) / 2));
                const int ir_max = std::max(ir, std::max((ir + ir_left) / 2, (ir + ir_right) / 2));

                // Birchfield-Tomasi dissimilarity
                const int cost1 = std::max(0, std::max(il - ir_max, ir_min - il));
                const int cost2 = std::max(0, std::max(ir - il_max, il_min - ir));
                const int cost = std::min(cost1, cost2);

                costVolume[costBase + (d - minD)] = static_cast<short>(cost);
            }
        }
    }

    return costVolume;
}
#include <cassert>
#include <vector>
#include <limits>

// The solution function is assumed to be available here.
// (Include the function code above or include a header.)

int main() {
    // Simple 2x2 test: left and right identical, disparity 0 should have zero cost.
    std::vector<unsigned char> left = {10, 20, 30, 40};
    std::vector<unsigned char> right = {10, 20, 30, 40};
    int width = 2, height = 2;
    int minD = 0, maxD = 1;
    auto costs = computeSGBMCostVolume(left, right, width, height, minD, maxD);
    assert(costs.size() == 4);
    // All valid matches (d=0) should have zero or near-zero cost (exact zero for constant neighborhoods).
    for (short c : costs) assert(c == 0);

    // Test out-of-bound disparity: shift one pixel, disparity 1 should be valid only for x>=1.
    left = {100, 200, 100, 200};
    right = {200, 100, 200, 100};
    minD = 1; maxD = 2;
    costs = computeSGBMCostVolume(left, right, width, height, minD, maxD);
    // Pixel (0,0) with d=1 -> x2=-1 invalid -> SHRT_MAX
    assert(costs[0] == std::numeric_limits<short>::max());
    // Pixel (1,0) with d=1 -> x2=0 valid, cost>0
    assert(costs[1] >= 0 && costs[1] < std::numeric_limits<short>::max());
    // Pixel (0,1) with d=1 -> x2=-1 invalid
    assert(costs[2] == std::numeric_limits<short>::max());
    // Pixel (1,1) with d=1 -> x2=0 valid
    assert(costs[3] >= 0 && costs[3] < std::numeric_limits<short>::max());

    // Test negative disparity and larger range.
    left = {5, 6, 7, 8};
    right = {5, 6, 7, 8};
    minD = -1; maxD = 2; // disparities -1,0,1
    costs = computeSGBMCostVolume(left, right, width, height, minD, maxD);
    assert(costs.size() == 2 * 2 * 3); // 12
    // For identical images, d=0 should have zero cost at all valid pixels.
    // Check pixel (0,0): d=-1 -> x2=1 valid; d=0 -> x2=0 valid; d=1 -> x2=-1 invalid
    assert(costs[0] >= 0 && costs[0] < std::numeric_limits<short>::max()); // d=-1
    assert(costs[1] == 0); // d=0
    assert(costs[2] == std::numeric_limits<short>::max()); // d=1

    // Edge case: single pixel image
    left = {42};
    right = {42};
    width = 1; height = 1;
    minD = 0; maxD = 1;
    costs = computeSGBMCostVolume(left, right, width, height, minD, maxD);
    assert(costs.size() == 1);
    assert(costs[0] == 0);

    // Edge case: maxD <= minD returns empty
    costs = computeSGBMCostVolume(left, right, width, height, 2, 2);
    assert(costs.empty());

    // Edge case: mismatched image size returns empty
    std::vector<unsigned char> small = {1, 2, 3};
    costs = computeSGBMCostVolume(left, small, width, height, 0, 1);
    assert(costs.empty());

    return 0;
}
