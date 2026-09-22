/*
Write a standalone C++ function named `resizeNearestNeighbor` that takes a 2D image represented as a `std::vector<std::vector<float>>` where each inner vector is a row of grayscale pixel values in the range [0,1], and resizes it to a new width `newWidth` and height `newHeight` using nearest‑neighbor interpolation. The function must handle any non‑negative dimensions, including upscaling and downscaling, and should return a new image with the specified dimensions. If either `newWidth` or `newHeight` is 0, return an empty vector. The mapping from destination pixel `(dx, dy)` to source pixel `(sx, sy)` should use integer rounding: `sx = (int)floor(dx * (srcWidth / (float)newWidth))` and similarly for `sy`, ensuring that the entire source image is covered (i.e., the last destination column/row maps to the last source column/row). The source image is guaranteed to be non‑empty; however, edge cases such as resizing to 1×1 or to the same size must work correctly. The function must not modify the input image.
*/

#include <vector>
#include <cstddef>
#include <cmath>
#include <algorithm>

// Resize a grayscale image (vector of rows) to new dimensions using nearest-neighbor.
// The input is a non-empty vector of rows, each row a vector of floats in [0,1].
// Returns a vector of `newHeight` rows, each with `newWidth` floats.
// If newWidth or newHeight is 0, returns an empty vector.
std::vector<std::vector<float>> resizeNearestNeighbor(
    const std::vector<std::vector<float>>& src,
    int newWidth,
    int newHeight) 
{
    if (newWidth <= 0 || newHeight <= 0) {
        return {};
    }

    const int srcWidth = static_cast<int>(src[0].size());
    const int srcHeight = static_cast<int>(src.size());

    // Scaling factors. Cast to float to avoid integer division truncation.
    const float scaleX = static_cast<float>(srcWidth) / static_cast<float>(newWidth);
    const float scaleY = static_cast<float>(srcHeight) / static_cast<float>(newHeight);

    std::vector<std::vector<float>> dst(newHeight, std::vector<float>(newWidth));

    for (int y = 0; y < newHeight; ++y) {
        // Nearest neighbor: floor(dy * scaleY).
        // Clamp to [0, srcHeight-1] for safety (especially when newHeight == 1).
        int sy = static_cast<int>(std::floor(y * scaleY));
        sy = std::max(0, std::min(sy, srcHeight - 1));

        const std::vector<float>& srcRow = src[sy];

        for (int x = 0; x < newWidth; ++x) {
            int sx = static_cast<int>(std::floor(x * scaleX));
            sx = std::max(0, std::min(sx, srcWidth - 1));
            dst[y][x] = srcRow[sx];
        }
    }

    return dst;
}

#include <cassert>
#include <vector>

// The solution function is defined above (or included here for completeness).
// For brevity, the function definition is assumed to be visible.

int main() {
    // Test 1: Simple 2x2 image upscaled to 4x4.
    std::vector<std::vector<float>> img2x2 = {
        {0.0f, 1.0f},
        {1.0f, 0.0f}
    };
    auto upscaled = resizeNearestNeighbor(img2x2, 4, 4);
    assert(upscaled.size() == 4);
    assert(upscaled[0].size() == 4);
    // Expected: each 2x2 block repeats the source pixel.
    // Using floor(dx * 2/4) = floor(dx*0.5) gives 0,0,1,1 for x=0..3.
    assert(upscaled[0][0] == 0.0f);
    assert(upscaled[0][1] == 0.0f);
    assert(upscaled[0][2] == 1.0f);
    assert(upscaled[0][3] == 1.0f);
    assert(upscaled[3][0] == 1.0f);
    assert(upscaled[3][1] == 1.0f);
    assert(upscaled[3][2] == 0.0f);
    assert(upscaled[3][3] == 0.0f);

    // Test 2: Downscale 4x4 to 2x2.
    std::vector<std::vector<float>> img4x4 = {
        {0, 1, 2, 3},
        {4, 5, 6, 7},
        {8, 9, 10, 11},
        {12, 13, 14, 15}
    };
    // Use values as floats for simplicity (they are representable).
    auto downscaled = resizeNearestNeighbor(img4x4, 2, 2);
    assert(downscaled.size() == 2);
    assert(downscaled[0].size() == 2);
    // floor(0*1.0)=0, floor(1*1.0)=1 -> picks (0,0) and (0,1)
    assert(downscaled[0][0] == 0.0f);
    assert(downscaled[0][1] == 1.0f);
    assert(downscaled[1][0] == 4.0f);
    assert(downscaled[1][1] == 5.0f);

    // Test 3: Resize to same size (copy)
    auto same = resizeNearestNeighbor(img2x2, 2, 2);
    assert(same == img2x2);

    // Test 4: Resize to 1x1 (should pick top-left pixel)
    auto one = resizeNearestNeighbor(img2x2, 1, 1);
    assert(one.size() == 1);
    assert(one[0].size() == 1);
    assert(one[0][0] == 0.0f);

    // Test 5: Resize to zero dimensions returns empty
    auto empty0 = resizeNearestNeighbor(img2x2, 0, 0);
    assert(empty0.empty());
    auto emptyW = resizeNearestNeighbor(img2x2, 0, 3);
    assert(emptyW.empty());
    auto emptyH = resizeNearestNeighbor(img2x2, 3, 0);
    assert(emptyH.empty());

    // Test 6: Single‐pixel image upscaled to 3x3 (all same value)
    std::vector<std::vector<float>> onePix = {{0.5f}};
    auto big = resizeNearestNeighbor(onePix, 3, 3);
    assert(big.size() == 3);
    for (const auto& row : big) {
        assert(row.size() == 3);
        for (float v : row) assert(v == 0.5f);
    }

    // Test 7: Non-square source (2x3) to 3x2
    std::vector<std::vector<float>> rect = {
        {0, 1, 2},
        {3, 4, 5}
    };
    auto resized = resizeNearestNeighbor(rect, 2, 3);
    // Expect height 3, width 2.
    assert(resized.size() == 3);
    assert(resized[0].size() == 2);
    // Source height 2, new height 3 -> scaleY = 2/3 = 0.6667.
    // y=0 -> floor(0)=0; y=1 -> floor(0.6667)=0; y=2 -> floor(1.333)=1.
    // Source width 3, new width 2 -> scaleX = 1.5.
    // x=0 -> floor(0)=0; x=1 -> floor(1.5)=1.
    assert(resized[0][0] == 0.0f);
    assert(resized[0][1] == 1.0f);
    assert(resized[1][0] == 0.0f);
    assert(resized[1][1] == 1.0f);
    assert(resized[2][0] == 3.0f);
    assert(resized[2][1] == 4.0f);

    // Test 8: Check that input is not modified.
    auto original = img2x2;
    resizeNearestNeighbor(img2x2, 10, 10);
    assert(img2x2 == original);

    return 0;
}

// The solution is a straightforward implementation of nearest‑neighbor interpolation. For each destination pixel at `(x,y)`, compute the corresponding source coordinates by scaling the destination index by the ratio of source dimension to destination dimension. Using integer truncation (`(int)`) is appropriate because destination coordinate 0 maps to source 0, and destination coordinate `newWidth-1` maps to `(newWidth-1) * (srcWidth/newWidth)`, which may be slightly less than `srcWidth-1`; to guarantee the last source pixel is used, we can clamp the result to `srcWidth-1` or use `floor` and then clamp. However, using integer division with `float` scaling and truncation yields the correct nearest neighbor for typical cases; for exact coverage, add a clamp at the end. The algorithm has time complexity `O(newWidth * newHeight)` because each destination pixel is computed in constant time. Space complexity is `O(newWidth * newHeight)` for the output image. Edge cases include: resizing to 0 dimensions (return empty), resizing from a non‑empty source to any positive size, and resizing to the same size (should produce a copy). Since the source is non‑empty, no division by zero occurs. The function should be `const`‑correct by taking the source by const reference.
