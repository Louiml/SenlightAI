Write a C++ function `nmppiCompareGtC` that takes a source image buffer of signed 16-bit pixels (`nm16s` is a typedef for `short`), a per-row byte stride for the source, a destination buffer of signed 16-bit values, a per-row byte stride for the destination, a pointer to an array of exactly 4 threshold values (`short`), and a width and height. The function must compare each pixel in the source against a threshold selected by the pixel's x-coordinate modulo 4 (i.e., threshold index = x % 4). If the pixel value is strictly greater than that threshold, write `0x00FF` (i.e., 255) to the corresponding destination location; otherwise write `0x0000`. The strides are given in bytes, not elements, so you must convert them to element offsets (each `short` is 2 bytes). The function must process the image row by row and preserve the width (number of pixels per row) and height (number of rows). Assume width is a multiple of 4, and all buffers are valid and non-overlapping. The function should return 0 on success. Implement it with appropriate `const` correctness for read-only inputs.

#include <cassert>
#include <vector>

int main() {
    // Test 1: Simple 4x1 image with thresholds {10, 20, 30, 40}
    {
        const int width = 4, height = 1;
        std::vector<nm16s> src = {5, 25, 35, 45};
        std::vector<nm16s> dst(4, -1);
        nm16s thresholds[4] = {10, 20, 30, 40};
        int srcStrideBytes = width * 2;  // 8 bytes
        int dstStrideBytes = width * 2;
        nmppiCompareGtC(src.data(), srcStrideBytes, dst.data(), dstStrideBytes, thresholds, width, height);
        assert(dst[0] == 0 && dst[1] == 255 && dst[2] == 255 && dst[3] == 255);
    }

    // Test 2: 2 rows with non-multiple-of-4? But width must be multiple of 4, use 4x2
    {
        const int width = 4, height = 2;
        std::vector<nm16s> src = {1, 2, 3, 4, 5, 6, 7, 8};
        std::vector<nm16s> dst(8, -1);
        nm16s thresholds[4] = {2, 3, 4, 5};
        int srcStrideBytes = width * 2;  // 8
        int dstStrideBytes = width * 2;
        nmppiCompareGtC(src.data(), srcStrideBytes, dst.data(), dstStrideBytes, thresholds, width, height);
        // Row0: (1>2?0), (2>3?0), (3>4?0), (4>5?0) => all 0
        // Row1: (5>2?255), (6>3?255), (7>4?255), (8>5?255) => all 255
        for (int i = 0; i < 4; ++i) assert(dst[i] == 0);
        for (int i = 4; i < 8; ++i) assert(dst[i] == 255);
    }

    // Test 3: Edge case with threshold equal to pixel value (not greater, so 0)
    {
        const int width = 4, height = 1;
        std::vector<nm16s> src = {100, 200, 300, 400};
        std::vector<nm16s> dst(4, -1);
        nm16s thresholds[4] = {100, 200, 300, 400};
        int srcStrideBytes = width * 2;
        int dstStrideBytes = width * 2;
        nmppiCompareGtC(src.data(), srcStrideBytes, dst.data(), dstStrideBytes, thresholds, width, height);
        for (int i = 0; i < 4; ++i) assert(dst[i] == 0);
    }

    // Test 4: Different strides (e.g., padding) to ensure stride handling
    {
        const int width = 4, height = 1;
        std::vector<nm16s> src = {10, 20, 30, 40, 999}; // extra padding at end
        std::vector<nm16s> dst(6, -1);
        nm16s thresholds[4] = {5, 5, 5, 5};
        int srcStrideBytes = 10; // 5 elements per row (4+1 padding) -> 10 bytes
        int dstStrideBytes = 12; // 6 elements per row -> 12 bytes
        nmppiCompareGtC(src.data(), srcStrideBytes, dst.data(), dstStrideBytes, thresholds, width, height);
        assert(dst[0] == 255 && dst[1] == 255 && dst[2] == 255 && dst[3] == 255);
        // dst[4] and dst[5] should remain -1 (untouched) because width=4
        assert(dst[4] == -1 && dst[5] == -1);
    }

    // Test 5: Negative values
    {
        const int width = 4, height = 1;
        std::vector<nm16s> src = {-10, -20, -30, -40};
        std::vector<nm16s> dst(4, -1);
        nm16s thresholds[4] = {-5, -15, -25, -35};
        int srcStrideBytes = width * 2;
        int dstStrideBytes = width * 2;
        nmppiCompareGtC(src.data(), srcStrideBytes, dst.data(), dstStrideBytes, thresholds, width, height);
        // -10 > -5? No -> 0
        // -20 > -15? No -> 0
        // -30 > -25? No -> 0
        // -40 > -35? No -> 0
        assert(dst[0] == 0 && dst[1] == 0 && dst[2] == 0 && dst[3] == 0);
    }

    return 0;
}

#include <cstddef>  // for size_t

// Type alias for 16-bit signed integers (as used in the original snippet)
using nm16s = short;

/**
 * Compares each pixel in a 16-bit grayscale image against a threshold selected by column index modulo 4.
 * If pixel > threshold, writes 0x00FF (255) to destination; otherwise writes 0.
 *
 * @param pSrcImg     Pointer to source image data (16-bit pixels).
 * @param nSrcStride  Byte stride between consecutive rows in the source.
 * @param pDst        Pointer to destination buffer (16-bit values).
 * @param nDstStride  Byte stride between consecutive rows in the destination.
 * @param pThreshold  Pointer to exactly 4 threshold values (used cyclically).
 * @param nWidth      Number of pixels per row (must be a multiple of 4).
 * @param nHeight     Number of rows.
 * @return 0 on success.
 */
int nmppiCompareGtC(const nm16s* pSrcImg, int nSrcStride,
                    nm16s* pDst, int nDstStride,
                    const nm16s* pThreshold,
                    int nWidth, int nHeight)
{
    // Convert byte strides to element strides (each nm16s is 2 bytes).
    const int srcElementStride = nSrcStride / static_cast<int>(sizeof(nm16s));
    const int dstElementStride = nDstStride / static_cast<int>(sizeof(nm16s));

    for (int y = 0; y < nHeight; ++y) {
        const nm16s* srcRow = pSrcImg + y * srcElementStride;
        nm16s* dstRow = pDst + y * dstElementStride;

        // Process columns in groups of 4, using the 4 thresholds directly.
        for (int x = 0; x < nWidth; x += 4) {
            for (int i = 0; i < 4; ++i) {
                dstRow[x + i] = (srcRow[x + i] > pThreshold[i]) ? 0x00FF : 0x0000;
            }
        }
    }

    return 0;
}

// The solution requires iterating over every pixel in the image, computing the row offset in elements as `(y * stride_in_bytes) / 2`, and the column offset as `x`. Since width is a multiple of 4, we can process columns in groups of 4 to match the threshold array of size 4. For each group of 4 pixels, apply thresholds `pThreshold[0]` to `pThreshold[3]` respectively. For each pixel, compare the source value with the corresponding threshold; if greater, store 255 (`0x00FF`) in the destination, else store 0 (`0x0000`). Important edge cases: strides may be negative or aligned differently, so always compute the element offset using integer division after converting bytes to elements (since stride is always even? Actually stride is in bytes for 16-bit data, so it must be even; but we handle it generically by using `stride / 2` for element offset). The function must not assume row-contiguous data; it uses stride to jump to the next row. Time complexity is `O(width * height)`, space complexity is `O(1)` auxiliary (excluding input/output buffers). The output is stored in-place as a 16-bit value; note that `0x00FF` as a `short` is 255, and `0x0000` is 0.
