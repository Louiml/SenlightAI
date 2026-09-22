/*
Write a standalone C++ function named `downsampleMin4x4` that takes a `cv::Mat` source image of 16-bit unsigned integers (`CV_16U`) and returns a new `cv::Mat` that is the result of downsampling the source by a factor of 4 in both dimensions. For each 4×4 block in the source, the output pixel value must be the minimum of all non-zero values within that block; if all values in the block are zero, the output pixel must be zero. The dimensions of the source must be: width divisible by 8, height divisible by 4, and the output should have dimensions `source.rows/4` by `source.cols/4`. Use a straightforward O(16) per output pixel scan of each 4×4 block, without any platform-specific SIMD optimizations, to keep the solution portable and self-contained. The function must not modify the input and should handle any valid input size that meets the divisibility requirements.
*/
#include <opencv2/opencv.hpp>
#include <cstdint>
#include <limits>

// Downsample a CV_16U image by factor 4 using min-of-nonzero per 4x4 block.
cv::Mat downsampleMin4x4(const cv::Mat& source) {
    // Validate input types and dimensions
    CV_Assert(source.type() == CV_16U);
    CV_Assert(source.cols % 8 == 0);
    CV_Assert(source.rows % 4 == 0);

    const int outRows = source.rows / 4;
    const int outCols = source.cols / 4;
    cv::Mat result(outRows, outCols, CV_16U);

    const uint16_t maxVal = std::numeric_limits<uint16_t>::max();

    for (int y = 0; y < outRows; ++y) {
        for (int x = 0; x < outCols; ++x) {
            uint16_t minVal = maxVal; // sentinel meaning "no non-zero yet"
            const int baseY = y * 4;
            const int baseX = x * 4;

            // Scan the 4x4 block
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    uint16_t pixel = source.at<uint16_t>(baseY + i, baseX + j);
                    if (pixel != 0 && pixel < minVal) {
                        minVal = pixel;
                    }
                }
            }

            // If block contained only zeros, output 0
            if (minVal == maxVal) {
                result.at<uint16_t>(y, x) = 0;
            } else {
                result.at<uint16_t>(y, x) = minVal;
            }
        }
    }

    return result;
}
#include <cassert>
#include <opencv2/opencv.hpp>
#include <cstdint>

// The function to test is assumed declared above.
// For testing, we include the implementation here or link it.

int main() {
    // Test 1: Simple 4x4 block with mixed values
    {
        cv::Mat src(4, 4, CV_16U);
        uint16_t data[16] = {
            1, 2, 3, 4,
            0, 5, 6, 7,
            8, 9, 10, 11,
            12, 0, 13, 14
        };
        memcpy(src.data, data, sizeof(data));
        cv::Mat dst = downsampleMin4x4(src);
        assert(dst.rows == 1 && dst.cols == 1);
        // Non-zero minimum is 1
        assert(dst.at<uint16_t>(0, 0) == 1);
    }

    // Test 2: All zero block
    {
        cv::Mat src(4, 8, CV_16U, cv::Scalar(0));
        cv::Mat dst = downsampleMin4x4(src);
        assert(dst.rows == 1 && dst.cols == 2);
        assert(dst.at<uint16_t>(0, 0) == 0);
        assert(dst.at<uint16_t>(0, 1) == 0);
    }

    // Test 3: Larger image with known pattern
    {
        cv::Mat src(8, 16, CV_16U);
        // Fill with values 100..115 etc. Just test dimensions and a known block
        // We'll set first 4x4 block to have min 5, second block to have min 0 (all zeros), etc.
        // Simpler: create pattern manually
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 16; ++c) {
                // For block starting at (0,0): all values = 10 except one 5
                if (r < 4 && c < 4) {
                    src.at<uint16_t>(r, c) = (r == 0 && c == 0) ? 5 : 10;
                } else {
                    // Everything else zero
                    src.at<uint16_t>(r, c) = 0;
                }
            }
        }
        cv::Mat dst = downsampleMin4x4(src);
        assert(dst.rows == 2 && dst.cols == 4);
        // Block (0,0) -> min 5
        assert(dst.at<uint16_t>(0, 0) == 5);
        // Other blocks -> 0
        assert(dst.at<uint16_t>(0, 1) == 0);
        assert(dst.at<uint16_t>(0, 2) == 0);
        assert(dst.at<uint16_t>(0, 3) == 0);
        assert(dst.at<uint16_t>(1, 0) == 0);
        assert(dst.at<uint16_t>(1, 1) == 0);
        assert(dst.at<uint16_t>(1, 2) == 0);
        assert(dst.at<uint16_t>(1, 3) == 0);
    }

    // Test 4: Edge case where block has only one non-zero
    {
        cv::Mat src(4, 4, CV_16U, cv::Scalar(0));
        src.at<uint16_t>(1, 2) = 42;
        cv::Mat dst = downsampleMin4x4(src);
        assert(dst.at<uint16_t>(0, 0) == 42);
    }

    // Test 5: Verify input not modified (const correctness)
    {
        cv::Mat src(4, 4, CV_16U, cv::Scalar(7));
        cv::Mat srcCopy = src.clone();
        cv::Mat dst = downsampleMin4x4(src);
        assert(cv::countNonZero(src != srcCopy) == 0); // src unchanged
        assert(dst.at<uint16_t>(0, 0) == 7);
    }

    return 0;
}
// The algorithm iterates over each output pixel at `(y, x)` where `y` ranges from `0` to `source.rows/4 - 1` and `x` ranges from `0` to `source.cols/4 - 1`. For each such output position, it scans the corresponding 4×4 input block starting at `(y*4, x*4)`. For each element in the block, it checks if the value is non-zero; if so, it updates the minimum so far. A variable `initMin` is set to a sentinel value (e.g., `std::numeric_limits<uint16_t>::max()`) to detect when no non‑zero value has been found. After scanning the block, if the sentinel is still present, we set the output to 0; otherwise we set it to the found minimum. This handles the edge case where all pixels in a block are zero. The time complexity is O(rows * cols) because each input pixel is visited exactly once (16 operations per output pixel, but total work is proportional to input area). Space complexity is O(1) extra beyond the output matrix, which itself is O(rows*cols/16). No special handling of negative numbers is needed because the type is unsigned 16-bit. The function is `const`‑correct and does not modify the input.
