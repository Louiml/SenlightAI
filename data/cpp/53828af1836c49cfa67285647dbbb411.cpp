// Write a C++ function named `apply3x3Convolution` that performs a 3×3 convolution with border handling on an 8-bit grayscale image. The function must accept the image dimensions, source and destination pointers with strides, a border mode (constant or replicate), a border value (used only for constant mode), a 9-element kernel array of signed 16-bit integers, and an integer scale factor (0–32). For each pixel, the convolution computes the sum of the products of the 3×3 neighborhood and kernel, then shifts the sum right by `scale` (using arithmetic right shift), and clamps the result to the unsigned 8-bit range [0, 255]. For border handling, when the border mode is constant, all pixels outside the image are treated as the given border value; when replicate, the edge pixels are repeated. The function must handle arbitrary image sizes (width and height at least 1) and must correctly process all pixels, including edges. The kernel indices follow row-major order: kernel[0] is the top-left, kernel[1] is top-center, kernel[2] is top-right, kernel[3] is middle-left, etc., with kernel[4] being the center. Strides are in bytes, and both source and destination pointers point to rows separated by their respective strides. The function should be efficient, but correctness is the priority. Provide the implementation in a self‑contained header‑only format with necessary includes.

// The solution computes a 3×3 convolution for each pixel. For each output pixel at (x, y), we need to access the 3×3 neighborhood centered at (x, y). For interior pixels, access is straightforward. For border pixels, we must resolve out‑of‑bounds coordinates using the border mode: constant (use borderValue) or replicate (clamp coordinates to [0, width-1] or [0, height-1]). We iterate row by row. For each row, we iterate column by column. To avoid redundant boundary checks, we can pre‑compute the row indices for the three source rows (y-1, y, y+1) using the border rule, and similarly for each column we can compute the three column indices (x-1, x, x+1). Then we perform a small loop over 9 kernel positions, accumulating the product of the source pixel and kernel coefficient. After accumulating, we apply the arithmetic right shift (using `>> scale` on a signed integer) and saturate to [0,255] using `std::clamp` with `uint8_t` conversion. The arithmetic shift on a negative value is implementation-defined in C++ for negative right shifts, but since we operate on `int32_t` (signed), the shift is arithmetic in practice on all common platforms; for strict safety we can cast to `int32_t` and then use a custom saturate that clamps after the shift. The time complexity is O(width * height * 9) = O(n) for n pixels, constant memory usage O(1) aside from the input and output buffers. Edge cases include single‑row or single‑column images, where replicate border must clamp to valid indices; constant border returns borderValue for missing rows/columns.

#include <cstdint>
#include <cstddef>
#include <algorithm>
#include <cassert>

// Performs a 3x3 convolution on an 8-bit grayscale image.
// Parameters:
//   width, height - image dimensions (>=1)
//   srcBase       - pointer to the first pixel of the source image
//   srcStride     - byte offset between consecutive source rows
//   dstBase       - pointer to the first pixel of the destination image
//   dstStride     - byte offset between consecutive destination rows
//   borderMode    - 0 for constant, 1 for replicate
//   borderValue   - value used when borderMode = 0 (constant)
//   kernel        - array of 9 signed 16-bit coefficients, row-major
//   scale         - shift amount (0 to 32) applied after accumulation
void apply3x3Convolution(
    std::size_t width, std::size_t height,
    const std::uint8_t* srcBase, std::ptrdiff_t srcStride,
    std::uint8_t* dstBase, std::ptrdiff_t dstStride,
    int borderMode, std::uint8_t borderValue,
    const std::int16_t* kernel, int scale)
{
    assert(width >= 1 && height >= 1);
    assert(kernel != nullptr);
    assert(scale >= 0 && scale <= 32);

    // Helper lambda to get a pixel with border handling.
    // row and col are the actual indices in the neighborhood (could be -1 or width, etc.)
    auto getPixel = [&](std::ptrdiff_t row, std::ptrdiff_t col) -> int {
        if (row < 0 || row >= static_cast<std::ptrdiff_t>(height) ||
            col < 0 || col >= static_cast<std::ptrdiff_t>(width)) {
            // Out of bounds
            if (borderMode == 0) {
                return borderValue;
            } else {
                // replicate (clamp)
                row = std::clamp<std::ptrdiff_t>(row, 0, static_cast<std::ptrdiff_t>(height - 1));
                col = std::clamp<std::ptrdiff_t>(col, 0, static_cast<std::ptrdiff_t>(width - 1));
            }
        }
        const std::uint8_t* rowPtr = srcBase + row * srcStride;
        return rowPtr[col];
    };

    for (std::size_t y = 0; y < height; ++y) {
        std::uint8_t* dstRow = dstBase + y * dstStride;
        for (std::size_t x = 0; x < width; ++x) {
            // Accumulate the convolution sum
            std::int32_t sum = 0;
            // Iterate over kernel positions (dy, dx)
            for (int dy = -1; dy <= 1; ++dy) {
                std::ptrdiff_t srcRow = static_cast<std::ptrdiff_t>(y) + dy;
                for (int dx = -1; dx <= 1; ++dx) {
                    std::ptrdiff_t srcCol = static_cast<std::ptrdiff_t>(x) + dx;
                    int pixel = getPixel(srcRow, srcCol);
                    int kernelIdx = (dy + 1) * 3 + (dx + 1);
                    sum += pixel * kernel[kernelIdx];
                }
            }
            // Apply arithmetic right shift (shift on signed value)
            std::int32_t shifted = sum >> scale;
            // Saturate to [0, 255]
            std::int32_t clamped = std::clamp<std::int32_t>(shifted, 0, 255);
            dstRow[x] = static_cast<std::uint8_t>(clamped);
        }
    }
}

#include <cassert>
#include <vector>
#include <cstdint>
#include <cstddef>

// The solution function is assumed to be declared above.
// We'll re-declare it here for the test file (in a real project it would be included).
void apply3x3Convolution(
    std::size_t width, std::size_t height,
    const std::uint8_t* srcBase, std::ptrdiff_t srcStride,
    std::uint8_t* dstBase, std::ptrdiff_t dstStride,
    int borderMode, std::uint8_t borderValue,
    const std::int16_t* kernel, int scale);

int main() {
    // Test 1: 3x3 image, identity kernel (only center = 1, others 0), scale=0, replicate border
    {
        std::size_t w = 3, h = 3;
        std::vector<std::uint8_t> src = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        std::uint8_t k[9] = {0, 0, 0, 0, 1, 0, 0, 0, 0};
        std::vector<std::uint8_t> dst(9, 0);
        apply3x3Convolution(w, h, src.data(), w, dst.data(), w, 1, 0, k, 0);
        // With identity, output equals input
        assert(dst == src);
    }

    // Test 2: 1x1 image, any kernel, constant border value 50, scale=0
    {
        std::size_t w = 1, h = 1;
        std::vector<std::uint8_t> src = {10};
        std::uint8_t k[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        std::vector<std::uint8_t> dst(1, 0);
        apply3x3Convolution(w, h, src.data(), w, dst.data(), w, 0, 50, k, 0);
        // All 3x3 neighbors are 50 except center 10
        // sum = 50*8 + 10*9 = 400 + 90 = 490, but saturates to 255
        assert(dst[0] == 255);
    }

    // Test 3: 3x3 image, kernel averaging (all 1/9 as integer? We'll use scale=3 and all kernel=1, then shift right 3 gives floor division by 8, not 9)
    // Better: scale=0, kernel that computes a simple sum of all 9 pixels
    {
        std::size_t w = 3, h = 3;
        std::vector<std::uint8_t> src = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        std::uint8_t k[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
        std::vector<std::uint8_t> dst(9, 0);
        apply3x3Convolution(w, h, src.data(), w, dst.data(), w, 1, 0, k, 0);
        // For interior pixel at (1,1), sum = all 9 values = 45, no saturation, so dst[4] = 45
        assert(dst[4] == 45);
        // For corner (0,0), using replicate border, the neighborhood is:
        // rows: 0,0,1 ; cols: 0,0,1
        // values: (0,0)=1, (0,0)=1, (0,1)=2
        //         (0,0)=1, (0,0)=1, (0,1)=2
        //         (1,0)=4, (1,0)=4, (1,1)=5
        // sum = 1+1+2+1+1+2+4+4+5 = 21
        assert(dst[0] == 21);
        // For corner (0,2), replicate:
        // rows 0,0,1 ; cols 2,2,1? Actually col clamp to 2 for x=2, then columns: 1,2,2
        // (0,1)=2, (0,2)=3, (0,2)=3
        // (0,1)=2, (0,2)=3, (0,2)=3
        // (1,1)=5, (1,2)=6, (1,2)=6
        // sum = 2+3+3+2+3+3+5+6+6 = 33
        assert(dst[2] == 33);
    }

    // Test 4: 1x3 image, kernel that ignores neighbors (center=1), replicate border, scale=0
    {
        std::size_t w = 3, h = 1;
        std::vector<std::uint8_t> src = {10, 20, 30};
        std::uint8_t k[9] = {0, 0, 0, 0, 1, 0, 0, 0, 0};
        std::vector<std::uint8_t> dst(3, 0);
        apply3x3Convolution(w, h, src.data(), w, dst.data(), w, 1, 0, k, 0);
        assert(dst[0] == 10 && dst[1] == 20 && dst[2] == 30);
    }

    // Test 5: Edge case with scale > 0 and negative sums
    {
        std::size_t w = 2, h = 2;
        std::vector<std::uint8_t> src = {0, 0, 0, 0};
        std::int16_t k[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
        // For constant border value=0, all pixels are 0, sum=0, scale any
        std::vector<std::uint8_t> dst(4, 255);
        apply3x3Convolution(w, h, src.data(), w, dst.data(), w, 0, 0, reinterpret_cast<const std::int16_t*>(k), 5);
        assert(dst[0] == 0);
        // Now use a kernel that gives negative sum: e.g., center=-1, all others 0
        std::int16_t kNeg[9] = {0, 0, 0, 0, -1, 0, 0, 0, 0};
        std::fill(dst.begin(), dst.end(), 255);
        apply3x3Convolution(w, h, src.data(), w, dst.data(), w, 0, 0, kNeg, 0);
        // sum = -1 * center pixel = -1, shift by 0 gives -1, clamp to 0
        assert(dst[0] == 0);
    }

    // Test 6: scale=32 with large values, saturation
    {
        std::size_t w = 1, h = 1;
        std::vector<std::uint8_t> src = {255};
        std::int16_t k[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
        // With constant border 0, sum = 255*9 = 2295, shift right 32 gives 0
        std::vector<std::uint8_t> dst(1, 255);
        apply3x3Convolution(w, h, src.data(), w, dst.data(), w, 0, 0, k, 32);
        assert(dst[0] == 0);
    }

    return 0;
}
