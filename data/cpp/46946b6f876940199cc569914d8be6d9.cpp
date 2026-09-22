Write a C++ function named `applyGranularSmoothing` that takes a grayscale `cv::Mat` image (`CV_8UC1`) and an integer `blockSize` as input, and returns a new `cv::Mat` where each pixel’s value is replaced by the **median** of all pixels within a square neighborhood of size `blockSize × blockSize` centered on that pixel (using border replication for edges). The function must handle invalid parameters (empty image or `blockSize` not a positive odd integer) by throwing a `std::invalid_argument`. The function should be `const`-correct and should not modify the input.
// The core algorithm is a sliding-window median filter. For each pixel `(r,c)`, define the half‑window `k = blockSize / 2`. Build the neighborhood by iterating rows from `max(0, r-k)` to `min(rows-1, r+k)` and columns similarly, extracting values using border replication (clamp indices to image bounds). Collect these values into a `std::vector<int>`, sort it, and take the middle element (if the vector size is even, take the lower middle to keep behavior deterministic). Edge cases: when `blockSize` is too large, the neighborhood simply includes all valid pixels; the median is still computed correctly. The time complexity is O(rows × cols × blockSize²) per pixel for copying and sorting, but in practice sorting a small vector is fast; overall O(rows·cols·blockSize²·log(blockSize²)). Space complexity is O(blockSize²) for the temporary vector per pixel, plus the output image size. Parameter validation must be performed first — throw if `input.empty()`, `blockSize <= 0`, or `blockSize % 2 == 0`. The function returns a new `cv::Mat` with the same dimensions and type.
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>
#include <stdexcept>

// Apply median filtering with a square neighborhood of given blockSize (odd positive integer).
// Uses border replication (clamping) for pixels near edges. Throws on invalid input.
cv::Mat applyGranularSmoothing(const cv::Mat& input, int blockSize) {
    if (input.empty()) {
        throw std::invalid_argument("Input image is empty.");
    }
    if (blockSize <= 0 || blockSize % 2 == 0) {
        throw std::invalid_argument("blockSize must be a positive odd integer.");
    }

    const int rows = input.rows;
    const int cols = input.cols;
    const int k = blockSize / 2;

    cv::Mat output(rows, cols, CV_8UC1);

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            // Collect neighborhood values
            std::vector<int> values;
            values.reserve(blockSize * blockSize);

            int rStart = std::max(0, r - k);
            int rEnd = std::min(rows - 1, r + k);
            int cStart = std::max(0, c - k);
            int cEnd = std::min(cols - 1, c + k);

            for (int rr = rStart; rr <= rEnd; ++rr) {
                for (int cc = cStart; cc <= cEnd; ++cc) {
                    values.push_back(static_cast<int>(input.at<uchar>(rr, cc)));
                }
            }

            // Median: sort and pick middle element (lower middle if even count)
            std::sort(values.begin(), values.end());
            int median = values[values.size() / 2];
            output.at<uchar>(r, c) = static_cast<uchar>(median);
        }
    }

    return output;
}
#include <cassert>
#include <opencv2/opencv.hpp>
#include <stdexcept>

// The solution function is declared above (include it here in actual usage).

int main() {
    // Test 1: Identity for 1x1 block on a 3x3 image
    cv::Mat img1 = (cv::Mat_<uchar>(3,3) << 10,20,30, 40,50,60, 70,80,90);
    cv::Mat result1 = applyGranularSmoothing(img1, 1);
    assert(cv::countNonZero(result1 != img1) == 0);

    // Test 2: Median of entire 3x3 image when blockSize=3 (center pixel only)
    cv::Mat img2 = (cv::Mat_<uchar>(3,3) << 1, 2, 3, 4, 5, 6, 7, 8, 9);
    cv::Mat result2 = applyGranularSmoothing(img2, 3);
    assert(result2.at<uchar>(1,1) == 5); // median of full image
    // Corners use replication: e.g., (0,0) median of values {1,1,2,1,1,2,4,4,5} => sorted {1,1,1,1,2,2,4,4,5} median=2
    assert(result2.at<uchar>(0,0) == 2);

    // Test 3: Edge replication for 1x1 image with blockSize=3
    cv::Mat img3 = (cv::Mat_<uchar>(1,1) << 42);
    cv::Mat result3 = applyGranularSmoothing(img3, 3);
    assert(result3.at<uchar>(0,0) == 42);

    // Test 4: Larger block on a single row image
    cv::Mat img4 = (cv::Mat_<uchar>(1,5) << 1, 5, 3, 9, 2);
    cv::Mat result4 = applyGranularSmoothing(img4, 3);
    // For pixel 0: values {1,1,5} sorted -> median=1
    // For pixel 1: values {1,5,3} sorted -> median=3
    // For pixel 2: values {5,3,9} sorted -> median=5
    // For pixel 3: values {3,9,2} sorted -> median=3
    // For pixel 4: values {9,2,2} sorted -> median=2
    assert(result4.at<uchar>(0,0) == 1);
    assert(result4.at<uchar>(0,1) == 3);
    assert(result4.at<uchar>(0,2) == 5);
    assert(result4.at<uchar>(0,3) == 3);
    assert(result4.at<uchar>(0,4) == 2);

    // Test 5: Even blockSize throws
    bool threw = false;
    try {
        applyGranularSmoothing(img1, 2);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test 6: Non-positive blockSize throws
    threw = false;
    try {
        applyGranularSmoothing(img1, 0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test 7: Empty image throws
    cv::Mat empty;
    threw = false;
    try {
        applyGranularSmoothing(empty, 3);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
