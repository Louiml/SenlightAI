/*
Write a standalone C++ function named `applyEdgeHighlightFilter` that takes two `std::string` parameters: an input BMP image file path and an output BMP image file path. The function must load the image using the CImg library, apply a 3×3 Laplacian-like edge-highlighting filter with kernel `{-1,-1,-1, -1,9,-1, -1,-1,-1}` to the red channel of every interior pixel (excluding the 1-pixel border), and save the result as a 24-bit BMP to the output path. For each interior pixel, compute the convolution sum using the original image values; clamp the result to the range `[0, 255]` and assign this clamped value to all three RGB channels (making the output grayscale). The border pixels should remain unchanged from the input image. If the input file cannot be opened or the output file cannot be written, the function must throw a `std::logic_error` with the message `"There was a problem with opening or saving a file. Path not valid."` The function must not use global variables, must not read from standard input, and must be self-contained (include all necessary headers, but do not include a `main` function).
*/
#include <string>
#include <stdexcept>
#include "CImg.h"

using namespace cimg_library;

/**
 * Loads a BMP image, applies a 3x3 edge-highlighting filter to the red channel,
 * clamps the result to [0,255] and stores it as grayscale in all RGB channels,
 * then saves the result as a BMP. Throws std::logic_error on file I/O failure.
 * 
 * @param inputPath  Path to the input BMP file.
 * @param outputPath Path where the filtered BMP will be saved.
 */
void applyEdgeHighlightFilter(const std::string& inputPath, const std::string& outputPath) {
    cimg::exception_mode(0);
    try {
        CImg<unsigned char> image(inputPath.c_str());
        CImg<unsigned char> copy(image); // deep copy preserve original values

        const int width  = image.width();
        const int height = image.height();

        // Interior pixels only: skip border rows/columns (index 0 and last)
        for (int y = 1; y < height - 1; ++y) {
            for (int x = 1; x < width - 1; ++x) {
                // Compute convolution sum using original red channel values
                int sum = 9 * copy(x, y, 0)
                        - copy(x-1, y-1, 0) - copy(x, y-1, 0) - copy(x+1, y-1, 0)
                        - copy(x-1, y,   0)                   - copy(x+1, y,   0)
                        - copy(x-1, y+1, 0) - copy(x, y+1, 0) - copy(x+1, y+1, 0);

                // Clamp to [0, 255]
                unsigned char value;
                if (sum > 255) {
                    value = 255;
                } else if (sum < 0) {
                    value = 0;
                } else {
                    value = static_cast<unsigned char>(sum);
                }

                // Write grayscale value to all channels
                image(x, y, 0) = value;
                image(x, y, 1) = value;
                image(x, y, 2) = value;
            }
        }

        image.save_bmp(outputPath.c_str());
    } catch (CImgIOException&) {
        throw std::logic_error("There was a problem with opening or saving a file. Path not valid.");
    }
}
#include <cassert>
#include <string>
#include "CImg.h"

using namespace cimg_library;

// Forward declaration of the function under test
void applyEdgeHighlightFilter(const std::string& inputPath, const std::string& outputPath);

int main() {
    // Create a simple 5x5 test image with known values (all channels identical)
    const int w = 5, h = 5;
    CImg<unsigned char> input(w, h, 1, 3, 0); // 3 channels, initialized to 0
    // Fill with a pattern: 10 at all pixels except center = 50
    unsigned char val = 10;
    input.fill(10);
    input(2, 2, 0) = 50;
    input(2, 2, 1) = 50;
    input(2, 2, 2) = 50;

    input.save_bmp("test_input.bmp");

    // Apply filter
    applyEdgeHighlightFilter("test_input.bmp", "test_output.bmp");

    // Load output
    CImg<unsigned char> output("test_output.bmp");

    // Border pixels must remain unchanged
    for (int x = 0; x < w; ++x) {
        for (int y = 0; y < h; ++y) {
            if (x == 0 || x == w-1 || y == 0 || y == h-1) {
                assert(output(x, y, 0) == 10);
                assert(output(x, y, 1) == 10);
                assert(output(x, y, 2) == 10);
            }
        }
    }

    // Interior pixel (1,1): original neighborhood all 10 except center (1,1)=10
    // sum = 9*10 - 8*10 = 10 → expected 10
    assert(output(1, 1, 0) == 10);

    // Interior pixel (2,2): center original = 50, neighbors all 10
    // sum = 9*50 - 8*10 = 450 - 80 = 370 → clamp to 255
    assert(output(2, 2, 0) == 255);
    assert(output(2, 2, 1) == 255);
    assert(output(2, 2, 2) == 255);

    // Interior pixel (3,3): same as (1,1)
    assert(output(3, 3, 0) == 10);

    // Test with a dark pixel: create a 3x3 all zeros with center 0, sum = 0
    CImg<unsigned char> dark(3, 3, 1, 3, 0);
    dark.save_bmp("dark_input.bmp");
    applyEdgeHighlightFilter("dark_input.bmp", "dark_output.bmp");
    CImg<unsigned char> darkOut("dark_output.bmp");
    assert(darkOut(1, 1, 0) == 0);

    // Test error handling: non-existent file
    bool threw = false;
    try {
        applyEdgeHighlightFilter("nonexistent.bmp", "dummy.bmp");
    } catch (const std::logic_error&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
// The solution loads the input image and makes a deep copy of it via `CImg<unsigned char> copy(image)`. The original `image` will be modified in place, while `copy` preserves the original pixel values for convolution. For each interior pixel `(x,y)` with `1 <= x < width-1` and `1 <= y < height-1`, compute `sum = 9 * copy(x,y,0) - (copy(x-1,y-1,0) + copy(x,y-1,0) + copy(x+1,y-1,0) + copy(x-1,y,0) + copy(x+1,y,0) + copy(x-1,y+1,0) + copy(x,y+1,0) + copy(x+1,y+1,0))`. This is equivalent to applying the kernel to the red channel only. Then clamp: if `sum > 255` set to 255, if `sum < 0` set to 0, otherwise keep `sum`. Assign the clamped integer to all three channels of `image(x,y,0)`, `image(x,y,1)`, and `image(x,y,2)`. Border pixels are left untouched because the loops start at 1 and end at `width-1` and `height-1`. Complexity: For an image of width `W` and height `H`, the algorithm visits `(W-2)*(H-2)` interior pixels, each doing constant work (8 additions, 1 multiplication, a few comparisons), so time complexity is `O(W*H)`. Space complexity is `O(W*H)` for the copy of the image. The error handling is done by setting `cimg::exception_mode(0)` to disable CImg's default exception throwing, then catching `CImgIOException` and rethrowing as `std::logic_error` with the specified message. The function must be `void` or return nothing; it will save the output BMP using `image.save_bmp(outputPath.c_str())` after processing.
