// Write a C++ function that converts a packed BGR (blue-green-red) image buffer to an RGB (red-green-blue) buffer. The function must process an entire image raster scan, where each pixel consists of three consecutive bytes in BGR order (byte order: B, G, R) in the source and must be written as three consecutive bytes in RGB order (byte order: R, G, B) in the destination. The input and output buffers have potentially different row strides (bytes per row). The function should handle widths that are not multiples of 16 pixels and must correctly convert leftover pixels at the end of each row. It must process each row independently, starting at the row's left edge, and must not read or write beyond the allocated buffers. The function signature is: `void bgrToRgb(const uint8_t* bgr, size_t width, size_t height, size_t bgrStride, uint8_t* rgb, size_t rgbStride);`. Assume all pointers are valid and `width > 0`, `height > 0`.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <vector>

int main() {
    // Helper to compare two buffers
    auto buffersEqual = [](const std::vector<uint8_t>& a, const std::vector<uint8_t>& b) {
        return a.size() == b.size() && std::memcmp(a.data(), b.data(), a.size()) == 0;
    };

    // Test 1: 1x1 pixel, no stride padding
    {
        std::vector<uint8_t> bgr = {10, 20, 30};
        std::vector<uint8_t> rgb(3, 0);
        bgrToRgb(bgr.data(), 1, 1, 3, rgb.data(), 3);
        assert(rgb == std::vector<uint8_t>({30, 20, 10}));
    }

    // Test 2: 2x1 pixels, no stride padding
    {
        std::vector<uint8_t> bgr = {1, 2, 3, 4, 5, 6};
        std::vector<uint8_t> rgb(6, 0);
        bgrToRgb(bgr.data(), 2, 1, 6, rgb.data(), 6);
        assert(rgb == std::vector<uint8_t>({3, 2, 1, 6, 5, 4}));
    }

    // Test 3: 1x2 rows, stride with padding (bgrStride=5, rgbStride=5)
    {
        std::vector<uint8_t> bgr = {1, 2, 3, 99, 99,
                                    4, 5, 6, 99, 99};
        std::vector<uint8_t> rgb(10, 0);
        std::vector<uint8_t> expected = {3, 2, 1, 99, 99,
                                         6, 5, 4, 99, 99};
        bgrToRgb(bgr.data(), 1, 2, 5, rgb.data(), 5);
        assert(rgb == expected);
    }

    // Test 4: 3x1 pixels, verify all color swaps
    {
        std::vector<uint8_t> bgr = {255, 0, 0, 0, 255, 0, 0, 0, 255};
        std::vector<uint8_t> rgb(9, 0);
        std::vector<uint8_t> expected = {0, 0, 255, 0, 255, 0, 255, 0, 0};
        bgrToRgb(bgr.data(), 3, 1, 9, rgb.data(), 9);
        assert(rgb == expected);
    }

    // Test 5: width not multiple of anything (7 pixels), single row, stride=21
    {
        std::vector<uint8_t> bgr(21);
        std::vector<uint8_t> rgb(21, 0);
        std::vector<uint8_t> expected(21);
        for (int i = 0; i < 7; ++i) {
            bgr[i*3 + 0] = i*10 + 1;
            bgr[i*3 + 1] = i*10 + 2;
            bgr[i*3 + 2] = i*10 + 3;
            expected[i*3 + 0] = i*10 + 3;
            expected[i*3 + 1] = i*10 + 2;
            expected[i*3 + 2] = i*10 + 1;
        }
        bgrToRgb(bgr.data(), 7, 1, 21, rgb.data(), 21);
        assert(rgb == expected);
    }

    // Test 6: Different strides for source and destination
    {
        std::vector<uint8_t> bgr = {1, 2, 3, 99, 99,  // row0 stride=5
                                    4, 5, 6, 99, 99}; // row1 stride=5
        std::vector<uint8_t> rgb(8, 0); // row0 stride=3, row1 stride=5 (since rgbStride=5)
        std::vector<uint8_t> expected = {3, 2, 1, 99,
                                         6, 5, 4, 99};
        bgrToRgb(bgr.data(), 1, 2, 5, rgb.data(), 5);
        assert(rgb == expected);
    }

    // Test 7: Larger image with both strides, verify output size
    {
        const size_t width = 16, height = 4;
        const size_t bgrStride = width*3 + 3;  // padding
        const size_t rgbStride = width*3 + 7;  // different padding
        std::vector<uint8_t> bgr(bgrStride * height, 0);
        std::vector<uint8_t> rgb(rgbStride * height, 0);
        // Fill BGR with recognizable pattern
        for (size_t r = 0; r < height; ++r) {
            for (size_t x = 0; x < width; ++x) {
                bgr[r*bgrStride + x*3 + 0] = static_cast<uint8_t>(x + r);
                bgr[r*bgrStride + x*3 + 1] = static_cast<uint8_t>(x * 2 + r);
                bgr[r*bgrStride + x*3 + 2] = static_cast<uint8_t>(x * 3 + r);
            }
        }
        bgrToRgb(bgr.data(), width, height, bgrStride, rgb.data(), rgbStride);
        for (size_t r = 0; r < height; ++r) {
            for (size_t x = 0; x < width; ++x) {
                assert(rgb[r*rgbStride + x*3 + 0] == static_cast<uint8_t>(x * 3 + r));
                assert(rgb[r*rgbStride + x*3 + 1] == static_cast<uint8_t>(x * 2 + r));
                assert(rgb[r*rgbStride + x*3 + 2] == static_cast<uint8_t>(x + r));
            }
        }
    }

    return 0;
}

#include <cstddef>
#include <cstdint>

// Convert a packed BGR image to packed RGB.
// Each pixel occupies 3 bytes: source order B,G,R; destination order R,G,B.
// Strides are in bytes and may include padding.
void bgrToRgb(const uint8_t* bgr, size_t width, size_t height,
              size_t bgrStride, uint8_t* rgb, size_t rgbStride) {
    for (size_t row = 0; row < height; ++row) {
        const uint8_t* srcRow = bgr + row * bgrStride;
        uint8_t* dstRow = rgb + row * rgbStride;
        for (size_t x = 0; x < width; ++x) {
            const size_t idx = x * 3;
            dstRow[idx + 0] = srcRow[idx + 2]; // R = original B
            dstRow[idx + 1] = srcRow[idx + 1]; // G unchanged
            dstRow[idx + 2] = srcRow[idx + 0]; // B = original R
        }
    }
}

// The core transformation is a 3-byte-to-3-byte per-pixel permutation. The straightforward approach iterates over every pixel, copying three bytes with indices swapped. A naive implementation reads `bgr[i]`, `bgr[i+1]`, `bgr[i+2]` and writes them to `rgb[i]`, `rgb[i+1]`, `rgb[i+2]` as `rgb[i] = bgr[i+2]`, `rgb[i+1] = bgr[i+1]`, `rgb[i+2] = bgr[i]`. This is O(width * height) and uses O(1) extra space. Edge cases include non-contiguous rows due to stride padding; the function must advance pointers by the stride, not by 3*width, after each row. For performance, one could process 16 pixels at a time using 128-bit SIMD loads and shuffles (as in the snippet), but a portable scalar implementation is correct for all platforms. The provided snippet uses SSE4.1 intrinsics to load 16 bytes (5⅓ pixels) at a time and shuffle bytes to reorder BGR to RGB, handling alignment and a tail block. For a self-contained task without requiring SIMD, we implement a clean scalar loop that processes each row's pixels in order, respecting strides. The main complexity is handling stride differences; we iterate row by row, and inside each row we iterate pixel by pixel. For leftover pixels when width is not a multiple of 16, a scalar tail loop is sufficient. Complexity: O(width * height) time, O(1) extra space.
