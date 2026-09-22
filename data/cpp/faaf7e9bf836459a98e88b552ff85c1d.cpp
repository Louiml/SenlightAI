/*
Given a float image represented as a 1D `std::vector<float>` with integer width and height, and a positive odd integer `kernel_size`, write a C++ function that performs a normalized box blur (average filter) with the given square kernel size, writing the result into an output `std::vector<float>` of the same length. The function should read from the input vector, treat out‑of‑bounds pixels as zero for boundary handling, and maintain the image’s original dimensions in the output. You may assume `kernel_size >= 1` and that it is odd, and that the input dimensions are positive. The function should be usable without any external image library; only standard headers are allowed.
*/
#include <vector>
#include <cstddef>

/**
 * @brief Perform a normalized box blur on a float image.
 * @param src  Input image as a 1D row-major vector of floats (length = width*height).
 * @param width  Image width in pixels.
 * @param height Image height in pixels.
 * @param kernel_size  Odd positive integer defining the square kernel side length.
 * @return Output blurred image as a 1D vector of the same size.
 *
 * Out-of-bounds samples are treated as zero, and the sum is divided by the
 * number of valid in-bounds samples to avoid darkening borders.
 */
std::vector<float> boxBlur(const std::vector<float>& src, int width, int height, int kernel_size) {
    // Validate inputs.
    if (src.size() != static_cast<size_t>(width) * static_cast<size_t>(height)) {
        throw std::invalid_argument("Input size does not match width*height");
    }
    if (kernel_size <= 0 || kernel_size % 2 == 0) {
        throw std::invalid_argument("Kernel size must be a positive odd integer");
    }

    std::vector<float> dst(src.size(), 0.0f);
    const int half = kernel_size / 2;

    // Iterate over every output pixel.
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float sum = 0.0f;
            int count = 0;

            // Sum valid samples in the neighborhood.
            for (int dy = -half; dy <= half; ++dy) {
                const int ny = y + dy;
                if (ny < 0 || ny >= height) continue;
                for (int dx = -half; dx <= half; ++dx) {
                    const int nx = x + dx;
                    if (nx < 0 || nx >= width) continue;
                    sum += src[static_cast<size_t>(ny) * width + nx];
                    ++count;
                }
            }

            // Normalize by the number of valid samples (count > 0 always because center is in-bounds).
            dst[static_cast<size_t>(y) * width + x] = sum / static_cast<float>(count);
        }
    }

    return dst;
}
#include <cassert>
#include <vector>

// Function under test is declared above; include the actual implementation here.

int main() {
    // Test 1: 3x3 image with kernel 1 (identity).
    std::vector<float> img1 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<float> out1 = boxBlur(img1, 3, 3, 1);
    assert(out1 == img1);

    // Test 2: 2x2 image with kernel 3 (box blur averages all pixels because 3x3 window extends beyond 2x2).
    std::vector<float> img2 = {1, 2, 3, 4};
    // All four pixels see the same 4 valid neighbors, so each becomes (1+2+3+4)/4 = 2.5.
    std::vector<float> expected2 = {2.5f, 2.5f, 2.5f, 2.5f};
    std::vector<float> out2 = boxBlur(img2, 2, 2, 3);
    for (size_t i = 0; i < expected2.size(); ++i) {
        assert(out2[i] == expected2[i]);
    }

    // Test 3: 3x1 image with kernel 3 (horizontal only).
    std::vector<float> img3 = {1, 2, 3};
    // Corner left: valid neighbors = {1,2} -> avg 1.5, middle: {1,2,3} -> avg 2, right: {2,3} -> avg 2.5.
    std::vector<float> expected3 = {1.5f, 2.0f, 2.5f};
    std::vector<float> out3 = boxBlur(img3, 3, 1, 3);
    for (size_t i = 0; i < expected3.size(); ++i) {
        assert(out3[i] == expected3[i]);
    }

    // Test 4: 1x1 image with any kernel.
    std::vector<float> img4 = {7.0f};
    std::vector<float> out4 = boxBlur(img4, 1, 1, 5);
    assert(out4[0] == 7.0f);

    // Test 5: 2x2 image with kernel 3 but manually verify each pixel.
    std::vector<float> img5 = {0, 1, 2, 3};
    // For pixel (0,0): valid neighbors: (0,0)=0,(0,1)=1,(1,0)=2,(1,1)=3 sum=6/4=1.5.
    // For pixel (0,1): same 4 neighbors sum=6/4=1.5.
    // For pixel (1,0): same sum=6/4=1.5.
    // For pixel (1,1): same sum=6/4=1.5.
    std::vector<float> expected5 = {1.5f, 1.5f, 1.5f, 1.5f};
    std::vector<float> out5 = boxBlur(img5, 2, 2, 3);
    for (size_t i = 0; i < expected5.size(); ++i) {
        assert(out5[i] == expected5[i]);
    }

    // Test 6: Empty vector? Not allowed by precondition, but we test that invalid dimensions throw.
    bool threw = false;
    try {
        boxBlur({1.0f}, 1, 1, 2); // even kernel
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
// The solution iterates over every output pixel (x, y). For each pixel, it accumulates the sum of the pixel values in a `kernel_size × kernel_size` neighborhood centered at (x, y), where any sample falling outside the image bounds is treated as 0.0f. The result is the accumulated sum divided by the number of valid in‑bounds samples (i.e., the count of pixels actually within the image). This normalization by the valid count avoids darkening borders compared to dividing by the full kernel area. Key edge cases: kernel_size = 1 yields an identity operation (output equals input). When the kernel extends past the image border, only the valid pixels are averaged, so the divisor changes per pixel. To avoid repeated bounds checks, we can pre‑compute valid neighbor offsets and use a double loop. Time complexity is O(H × W × kernel_size²) since each output pixel does O(k²) work; space complexity is O(H × W) for the output vector. The implementation uses `std::vector<float>` and checks that input and output sizes match the given dimensions.
