Write a C++ function named `downsampleImage` that takes a source grayscale image represented as a flat `std::vector<uint8_t>` of pixel values in row-major order, along with its width and height, and an integer scale factor `nScale`. The function should return a new `std::vector<uint8_t>` representing the downsampled image where each output pixel is the average (integer division) of the corresponding `nScale × nScale` block of source pixels. The function must handle cases where the source dimensions are not perfectly divisible by the scale factor by ignoring the leftover pixels at the right and bottom edges (i.e., only full blocks are processed). The output dimensions should be `sourceWidth / nScale` and `sourceHeight / nScale` (using integer division). If the scale factor is less than or equal to 0, throw a `std::invalid_argument`. If the scale factor is 1, the output should be a copy of the input. The function must be standalone, not rely on any external libraries (like OpenCV), and include proper `const` correctness.
// The core algorithm is a block-based averaging operation. For each output pixel at position `(outRow, outCol)`, we compute the sum of pixel values from the source image within the rectangle starting at `(outRow * nScale, outCol * nScale)` and extending `nScale` rows and columns. The average is computed using integer division (truncating toward zero). To handle leftover pixels when dimensions are not multiples of the scale, we simply loop only over full blocks — this means we compute `outHeight = srcHeight / nScale` and `outWidth = srcWidth / nScale`, and for each output pixel, we ensure the inner loops over `nScale` rows and columns stay within bounds implicitly because the starting position plus `nScale` never exceeds the source dimension (since we truncated). Edge cases: scale=1 simply copies; scale>min(width,height) yields an output of size 1x1 or even 0x0 (if the source dimension is smaller than the scale, integer division gives 0, so we must return an empty vector). We also need to validate that the input is non-empty and width/height are positive. Time complexity is O(srcWidth * srcHeight) because each source pixel is read exactly once. Space complexity is O(outWidth * outHeight) for the output vector.
#include <vector>
#include <stdexcept>

// Downsamples a grayscale image by averaging nScale x nScale blocks.
// Input: flat pixel data in row-major order, width, height, scale factor.
// Output: new flat vector of averaged pixels, dimensions: width/nScale, height/nScale.
// Ignores leftover pixels on right/bottom edges if dimensions not divisible by scale.
// Throws std::invalid_argument if scale <= 0 or if dimensions are invalid.
std::vector<uint8_t> downsampleImage(
    const std::vector<uint8_t>& src,
    int width,
    int height,
    int nScale
) {
    if (nScale <= 0) {
        throw std::invalid_argument("Scale factor must be positive");
    }
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Width and height must be positive");
    }
    if (src.size() != static_cast<size_t>(width) * height) {
        throw std::invalid_argument("Source data size does not match dimensions");
    }

    int outWidth = width / nScale;
    int outHeight = height / nScale;

    std::vector<uint8_t> dest(outWidth * outHeight);

    if (nScale == 1) {
        // Copy directly
        dest = src;
        return dest;
    }

    // Iterate over output pixels
    for (int outY = 0; outY < outHeight; ++outY) {
        for (int outX = 0; outX < outWidth; ++outX) {
            int sum = 0;
            int startY = outY * nScale;
            int startX = outX * nScale;

            // Sum the nScale x nScale block
            for (int dy = 0; dy < nScale; ++dy) {
                int srcY = startY + dy;
                for (int dx = 0; dx < nScale; ++dx) {
                    int srcX = startX + dx;
                    sum += src[srcY * width + srcX];
                }
            }

            int average = sum / (nScale * nScale);
            dest[outY * outWidth + outX] = static_cast<uint8_t>(average);
        }
    }

    return dest;
}
#include <cassert>
#include <vector>
#include <stdexcept>

// Declare the function being tested (typically comes from the header, but here inline for test)
std::vector<uint8_t> downsampleImage(
    const std::vector<uint8_t>& src,
    int width,
    int height,
    int nScale
);

int main() {
    // Test 1: Scale 1 returns copy
    std::vector<uint8_t> img1 = {1, 2, 3, 4};
    auto out1 = downsampleImage(img1, 2, 2, 1);
    assert(out1 == img1);

    // Test 2: 2x2 block average.
    // Source 4x4: all ones -> output 2x2 all 1
    std::vector<uint8_t> img2(16, 1);
    auto out2 = downsampleImage(img2, 4, 4, 2);
    assert(out2.size() == 4);
    for (auto v : out2) assert(v == 1);

    // Test 3: Source 2x2, scale 2 -> one output pixel = average of all four.
    std::vector<uint8_t> img3 = {0, 10, 20, 30};
    auto out3 = downsampleImage(img3, 2, 2, 2);
    assert(out3.size() == 1);
    assert(out3[0] == 15); // (0+10+20+30)/4 = 15

    // Test 4: Non-divisible dimensions, leftover ignored.
    // 3x3 source with scale 2: output 1x1 (3/2=1 each), average of top-left 2x2 block.
    std::vector<uint8_t> img4 = {
        1, 2, 100,
        3, 4, 100,
        100, 100, 100
    };
    auto out4 = downsampleImage(img4, 3, 3, 2);
    assert(out4.size() == 1);
    assert(out4[0] == 2); // (1+2+3+4)/4 = 10/4 = 2 (integer division)

    // Test 5: Scale larger than dimension -> output empty
    std::vector<uint8_t> img5 = {1, 2, 3, 4};
    auto out5 = downsampleImage(img5, 2, 2, 5);
    assert(out5.empty());

    // Test 6: Invalid scale throws
    bool threw = false;
    try {
        downsampleImage(img1, 2, 2, 0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test 7: Invalid dimensions throw
    threw = false;
    try {
        downsampleImage(img1, 0, 2, 1);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test 8: Mismatched data size throws
    threw = false;
    try {
        std::vector<uint8_t> bad = {1, 2, 3};
        downsampleImage(bad, 2, 2, 1);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test 9: Larger averaging with a 4x4 gradient, scale 2
    std::vector<uint8_t> img9 = {
        0, 1, 2, 3,
        4, 5, 6, 7,
        8, 9, 10, 11,
        12, 13, 14, 15
    };
    auto out9 = downsampleImage(img9, 4, 4, 2);
    // Block1: (0+1+4+5)/4 = 10/4=2
    // Block2: (2+3+6+7)/4 = 18/4=4
    // Block3: (8+9+12+13)/4 = 42/4=10
    // Block4: (10+11+14+15)/4 = 50/4=12
    assert(out9.size() == 4);
    assert(out9[0] == 2);
    assert(out9[1] == 4);
    assert(out9[2] == 10);
    assert(out9[3] == 12);

    return 0;
}
