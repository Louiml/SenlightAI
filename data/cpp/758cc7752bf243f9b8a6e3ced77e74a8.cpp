Write a C++ function that simulates a simplified background subtraction algorithm for a grayscale image. Given a `cv::Mat` representing a single-channel grayscale image (with pixel values in the range 0–255) and a reference background image of the same size and type, the function should compute a foreground mask where a pixel is marked as foreground (value 255) if the absolute difference between the corresponding pixel values in the current frame and the background exceeds a fixed threshold of 25. Otherwise, the pixel is background (value 0). The function must return the foreground mask as a new `cv::Mat` with the same dimensions, using `CV_8UC1` type. The input images must not be modified, and the function should handle cases where the inputs are empty or have mismatched sizes by returning an empty `cv::Mat`. Implement the function with the signature `cv::Mat computeForegroundMask(const cv::Mat& currentFrame, const cv::Mat& backgroundImage, int threshold = 25)`.

#include <cassert>
#include <opencv2/core.hpp>

// Include the solution function (declaration above or from a header)
// cv::Mat computeForegroundMask(const cv::Mat& currentFrame, const cv::Mat& backgroundImage, int threshold = 25);

int main() {
    // Helper to create a grayscale image filled with a constant value
    auto makeGray = [](int rows, int cols, uint8_t value) {
        cv::Mat img(rows, cols, CV_8UC1, cv::Scalar(value));
        return img;
    };

    // Test 1: Single pixel identical background -> all background
    {
        cv::Mat bg = makeGray(1, 1, 100);
        cv::Mat frame = makeGray(1, 1, 100);
        cv::Mat mask = computeForegroundMask(frame, bg);
        assert(mask.size() == cv::Size(1, 1));
        assert(mask.type() == CV_8UC1);
        assert(mask.at<uint8_t>(0, 0) == 0);
    }

    // Test 2: Single pixel difference exactly at threshold (25) -> background
    {
        cv::Mat bg = makeGray(1, 1, 100);
        cv::Mat frame = makeGray(1, 1, 125);
        cv::Mat mask = computeForegroundMask(frame, bg);
        assert(mask.at<uint8_t>(0, 0) == 0);
    }

    // Test 3: Single pixel difference just above threshold (26) -> foreground
    {
        cv::Mat bg = makeGray(1, 1, 100);
        cv::Mat frame = makeGray(1, 1, 126);
        cv::Mat mask = computeForegroundMask(frame, bg);
        assert(mask.at<uint8_t>(0, 0) == 255);
    }

    // Test 4: 2x2 image with mixed differences
    {
        cv::Mat bg = (cv::Mat_<uint8_t>(2, 2) << 10, 20, 30, 40);
        cv::Mat frame = (cv::Mat_<uint8_t>(2, 2) << 10, 50, 30, 80);
        cv::Mat mask = computeForegroundMask(frame, bg); // default threshold 25
        // Differences: row0: [0, 30], row1: [0, 40] -> foreground at (0,1) and (1,1)
        assert(mask.at<uint8_t>(0, 0) == 0);
        assert(mask.at<uint8_t>(0, 1) == 255);
        assert(mask.at<uint8_t>(1, 0) == 0);
        assert(mask.at<uint8_t>(1, 1) == 255);
    }

    // Test 5: Custom threshold (e.g., 50) changes classification
    {
        cv::Mat bg = makeGray(1, 1, 0);
        cv::Mat frame = makeGray(1, 1, 60);
        cv::Mat mask = computeForegroundMask(frame, bg, 50);
        assert(mask.at<uint8_t>(0, 0) == 255); // diff=60 > 50
        cv::Mat mask2 = computeForegroundMask(frame, bg, 60);
        assert(mask2.at<uint8_t>(0, 0) == 0); // diff=60 not > 60
    }

    // Test 6: Empty input returns empty mask
    {
        cv::Mat bg, frame;
        cv::Mat mask = computeForegroundMask(frame, bg);
        assert(mask.empty());
    }

    // Test 7: Mismatched sizes return empty mask
    {
        cv::Mat bg = makeGray(2, 2, 0);
        cv::Mat frame = makeGray(3, 3, 0);
        cv::Mat mask = computeForegroundMask(frame, bg);
        assert(mask.empty());
    }

    // Test 8: Non-grayscale type (e.g., CV_8UC3) returns empty mask
    {
        cv::Mat bg(2, 2, CV_8UC3, cv::Scalar(0, 0, 0));
        cv::Mat frame(2, 2, CV_8UC3, cv::Scalar(0, 0, 0));
        cv::Mat mask = computeForegroundMask(frame, bg);
        assert(mask.empty());
    }

    // Test 9: All foreground where background is dark and frame is bright
    {
        cv::Mat bg = makeGray(3, 3, 0);
        cv::Mat frame = makeGray(3, 3, 255);
        cv::Mat mask = computeForegroundMask(frame, bg);
        assert(cv::countNonZero(mask) == 9); // all pixels foreground
    }

    // Test 10: Input images not modified
    {
        cv::Mat bg = (cv::Mat_<uint8_t>(1, 2) << 10, 20);
        cv::Mat frame = (cv::Mat_<uint8_t>(1, 2) << 11, 21);
        cv::Mat bgCopy = bg.clone();
        cv::Mat frameCopy = frame.clone();
        cv::Mat mask = computeForegroundMask(frame, bg);
        assert(cv::countNonZero(mask) == 0); // all differences <= 25
        assert(cv::norm(bg, bgCopy) == 0);
        assert(cv::norm(frame, frameCopy) == 0);
    }

    return 0;
}

#include <opencv2/core.hpp>
#include <cstdint>

// Compute a foreground mask by thresholding the absolute difference between
// a current grayscale frame and a reference background image.
// Returns an empty cv::Mat if inputs are invalid (empty, mismatched size/type).
cv::Mat computeForegroundMask(const cv::Mat& currentFrame, const cv::Mat& backgroundImage, int threshold = 25) {
    // Validate inputs: must be non-empty, same size, same type (CV_8UC1)
    if (currentFrame.empty() || backgroundImage.empty() ||
        currentFrame.size() != backgroundImage.size() ||
        currentFrame.type() != CV_8UC1 || backgroundImage.type() != CV_8UC1) {
        return cv::Mat();
    }

    // Create output mask initialized to zeros (background)
    cv::Mat foregroundMask = cv::Mat::zeros(currentFrame.size(), CV_8UC1);

    // Iterate over all pixels using raw pointers for efficiency
    const int rows = currentFrame.rows;
    const int cols = currentFrame.cols;
    const int channels = currentFrame.channels(); // Should be 1

    for (int i = 0; i < rows; ++i) {
        const uint8_t* framePtr = currentFrame.ptr<uint8_t>(i);
        const uint8_t* bgPtr = backgroundImage.ptr<uint8_t>(i);
        uint8_t* maskPtr = foregroundMask.ptr<uint8_t>(i);

        for (int j = 0; j < cols; ++j) {
            // Compute absolute difference for the grayscale channel
            int diff = std::abs(static_cast<int>(framePtr[j]) - static_cast<int>(bgPtr[j]));
            if (diff > threshold) {
                maskPtr[j] = 255; // Foreground
            }
            // else maskPtr[j] remains 0 (background)
        }
    }

    return foregroundMask;
}

// The solution iterates over every pixel of the two input images. For each pixel, it computes the absolute difference between the intensity values of the current frame and the background image. If this difference is strictly greater than the threshold (default 25), the corresponding pixel in the output mask is set to 255 (foreground); otherwise, it is set to 0 (background). The output is a new `cv::Mat` of type `CV_8UC1` with the same dimensions as the inputs. Edge cases: if either input is empty, or if their sizes or types do not match (both must be `CV_8UC1`), the function returns an empty `cv::Mat` to signal an error. The function uses `const` references for inputs to guarantee no modification, and the output is constructed independently. Time complexity is \(O(H \times W)\) for an image of height \(H\) and width \(W\), since every pixel is visited exactly once. Space complexity is \(O(H \times W)\) for the output mask, plus constant auxiliary space for the loop variables and threshold value. The implementation uses direct pointer access to raw data for efficiency, but also validates the inputs before processing.
