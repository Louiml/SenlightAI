// Write a C++ function `std::vector<cv::Vec3f> detectGreenCircles(const cv::Mat& inputImage)` that takes a color image as input and returns the circles detected in green regions using the following pipeline: convert the image to HSV, apply a median blur, threshold the green color range (lower bound: (34, 30, 214), upper bound: (142, 123, 255)) using `inRange`, apply a Gaussian blur to the mask, and then use Hough circle detection with parameters `dp=1.8, minDist=10, param1=20, param2=53, minRadius=18, maxRadius=26`. The function should return an empty vector if no circles are found or if the input is empty. Do not modify the input image; use a cloned copy internally. The function should be robust to images with no green regions, and it should handle grayscale or single-channel inputs gracefully by returning an empty vector.

The solution follows the exact processing steps from the snippet: first check that the input image is non-empty and has three channels (otherwise return an empty vector). Create a copy to avoid modifying the original. Apply `medianBlur` with kernel size 3 to reduce noise. Convert the blurred image to HSV using `COLOR_BGR2HSV`. Define the lower and upper Scalar bounds for the green color range (note the snippet uses HSV bounds; ensure the correct order). Use `inRange` to produce a binary mask where pixels within the range are white (255). Apply `GaussianBlur` with a 9x9 kernel and sigmaX/Y = 2 to smooth the mask edges. Use `HoughCircles` with `HOUGH_GRADIENT` and the specified parameters: dp=1.8, minDist=10, param1=20 (Canny high threshold), param2=53 (accumulator threshold), minRadius=18, maxRadius=26. The function returns the vector of `Vec3f` circles. Edge cases: if the input is empty (rows or cols == 0) return empty; if the input has only one channel, return empty because HSV conversion requires BGR. Also, if HoughCircles finds zero circles, the returned vector is empty. Time complexity: each OpenCV operation is linear in the number of pixels for blur and threshold, and Hough transform is roughly O(n * r) where n is number of edge pixels and r is radius range; overall O(rows*cols). Space complexity is O(rows*cols) for the intermediate matrices.

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <vector>

// Detect circles in green regions of a BGR image.
// Returns a vector of circles (x, y, radius). Empty if none or invalid input.
std::vector<cv::Vec3f> detectGreenCircles(const cv::Mat& inputImage) {
    // Handle invalid input: empty or non-3-channel image.
    if (inputImage.empty() || inputImage.channels() != 3) {
        return {};
    }

    // Work on a copy to avoid modifying the original.
    cv::Mat workingImage = inputImage.clone();

    // Step 1: Apply median blur to reduce noise.
    cv::medianBlur(workingImage, workingImage, 3);

    // Step 2: Convert to HSV color space.
    cv::Mat hsvImage;
    cv::cvtColor(workingImage, hsvImage, cv::COLOR_BGR2HSV);

    // Step 3: Threshold for green color range.
    cv::Scalar lower(34, 30, 214);
    cv::Scalar upper(142, 123, 255);
    cv::Mat mask;
    cv::inRange(hsvImage, lower, upper, mask);

    // Step 4: Gaussian blur to smooth mask edges.
    cv::GaussianBlur(mask, mask, cv::Size(9, 9), 2, 2);

    // Step 5: Hough circle detection.
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(mask, circles, cv::HOUGH_GRADIENT, 1.8, 10, 20, 53, 18, 26);

    return circles;
}

#include <cassert>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <vector>

// Declare the function under test (should be defined above or in separate header).
std::vector<cv::Vec3f> detectGreenCircles(const cv::Mat& inputImage);

int main() {
    // Create a synthetic image: a single green circle on black background.
    cv::Mat testImg(100, 100, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::circle(testImg, cv::Point(50, 50), 20, cv::Scalar(0, 255, 0), -1); // BGR green
    auto circles = detectGreenCircles(testImg);
    assert(!circles.empty());
    // Expect center near (50,50) and radius between 18 and 26.
    assert(std::abs(circles[0][0] - 50) < 10);
    assert(std::abs(circles[0][1] - 50) < 10);
    assert(circles[0][2] >= 18 && circles[0][2] <= 26);

    // Empty image should return empty.
    cv::Mat emptyImg;
    assert(detectGreenCircles(emptyImg).empty());

    // Grayscale image should return empty (not 3-channel).
    cv::Mat grayImg(50, 50, CV_8UC1, cv::Scalar(128));
    assert(detectGreenCircles(grayImg).empty());

    // Image with no green region should return empty.
    cv::Mat redImg(100, 100, CV_8UC3, cv::Scalar(0, 0, 255)); // red BGR
    assert(detectGreenCircles(redImg).empty());

    // Image with a small green dot (radius < minRadius) should return empty.
    cv::Mat smallDot(80, 80, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::circle(smallDot, cv::Point(40, 40), 5, cv::Scalar(0, 255, 0), -1);
    assert(detectGreenCircles(smallDot).empty());

    // Image with a large green circle (radius > maxRadius) should return empty.
    cv::Mat largeCircle(200, 200, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::circle(largeCircle, cv::Point(100, 100), 40, cv::Scalar(0, 255, 0), -1);
    assert(detectGreenCircles(largeCircle).empty());

    return 0;
}
