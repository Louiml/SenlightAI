// Write a C++ function that takes a grayscale image represented as a `cv::Mat` (assumed to be of type `CV_8UC1`) and a Canny edge threshold value, and returns a `std::vector<std::pair<cv::Point2f, double>>` where each element corresponds to one detected contour. The pair should contain the contour's centroid (mass center computed from image moments) and its area computed via the moment `m00`. Use the following algorithm: blur the input image with a 3x3 Gaussian filter, apply Canny edge detection with the given threshold (and twice that threshold as the upper bound), find contours using `RETR_EXTERNAL` and `CHAIN_APPROX_SIMPLE`, compute moments for each contour, and derive the centroid as `(m10/m00, m01/m00)` and the area as `m00`. The function must handle the case of no contours by returning an empty vector, and it must avoid division by zero by adding a small epsilon (e.g., `1e-5`) to `m00` when computing the centroid. Do not include a main function; provide only the function implementation.
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <cassert>
#include <cmath>

// Forward declaration of the solution function (from the provided file).
std::vector<std::pair<cv::Point2f, double>> computeContourCentroidsAndAreas(const cv::Mat& src, int thresh);

int main() {
    // Test 1: Empty image -> no contours
    cv::Mat empty(10, 10, CV_8UC1, cv::Scalar(0));
    auto res1 = computeContourCentroidsAndAreas(empty, 100);
    assert(res1.empty());

    // Test 2: Single white rectangle on black background
    cv::Mat img2(20, 20, CV_8UC1, cv::Scalar(0));
    cv::rectangle(img2, cv::Rect(5, 5, 10, 10), cv::Scalar(255), cv::FILLED);
    auto res2 = computeContourCentroidsAndAreas(img2, 50);
    assert(res2.size() == 1);
    assert(std::fabs(res2[0].second - 100.0) < 1e-3); // area = 10*10
    assert(std::fabs(res2[0].first.x - 10.0) < 1e-3); // center x
    assert(std::fabs(res2[0].first.y - 10.0) < 1e-3); // center y

    // Test 3: Two separate rectangles
    cv::Mat img3(30, 30, CV_8UC1, cv::Scalar(0));
    cv::rectangle(img3, cv::Rect(2, 2, 6, 6), cv::Scalar(255), cv::FILLED); // area 36, center (5,5)
    cv::rectangle(img3, cv::Rect(20, 20, 4, 4), cv::Scalar(255), cv::FILLED); // area 16, center (22,22)
    auto res3 = computeContourCentroidsAndAreas(img3, 50);
    assert(res3.size() == 2);
    bool found_first = false, found_second = false;
    for (const auto& item : res3) {
        if (std::fabs(item.second - 36.0) < 1e-3 && std::fabs(item.first.x - 5.0) < 1e-3 && std::fabs(item.first.y - 5.0) < 1e-3) found_first = true;
        if (std::fabs(item.second - 16.0) < 1e-3 && std::fabs(item.first.x - 22.0) < 1e-3) found_second = true;
    }
    assert(found_first && found_second);

    // Test 4: Low threshold might detect, high threshold may produce no contours
    cv::Mat img4(10, 10, CV_8UC1, cv::Scalar(0));
    cv::circle(img4, cv::Point(5, 5), 3, cv::Scalar(255), cv::FILLED);
    auto res4_high = computeContourCentroidsAndAreas(img4, 255); // no edges
    assert(res4_high.empty());
    auto res4_low = computeContourCentroidsAndAreas(img4, 10); // edges exist
    assert(res4_low.size() >= 1);

    return 0;
}
#include <opencv2/imgproc.hpp>
#include <vector>
#include <utility>

// Compute centroids and areas of external contours in a grayscale image.
// Input: src - CV_8UC1 image, thresh - lower Canny threshold.
// Output: vector of pairs (centroid, area), area from moment m00.
std::vector<std::pair<cv::Point2f, double>>
computeContourCentroidsAndAreas(const cv::Mat& src, int thresh) {
    // Blur to reduce noise
    cv::Mat blurred;
    cv::GaussianBlur(src, blurred, cv::Size(3, 3), 0);

    // Edge detection
    cv::Mat edges;
    cv::Canny(blurred, edges, thresh, thresh * 2, 3);

    // Find external contours
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<std::pair<cv::Point2f, double>> result;
    result.reserve(contours.size());

    constexpr double eps = 1e-5; // avoid division by zero

    for (const auto& contour : contours) {
        cv::Moments mu = cv::moments(contour);
        double area = mu.m00;
        double cx = mu.m10 / (area + eps);
        double cy = mu.m01 / (area + eps);
        result.emplace_back(cv::Point2f(static_cast<float>(cx), static_cast<float>(cy)), area);
    }

    return result;
}
// The solution involves a standard OpenCV image processing pipeline: grayscale input assumed, smoothing to reduce noise, edge detection to highlight boundaries, contour extraction, and moment computation for geometric properties. The main algorithm steps are: (1) apply `cv::GaussianBlur` with a 3x3 kernel to smooth the image, (2) call `cv::Canny` with the provided threshold and `threshold * 2` as the upper bound, (3) find contours on the binary edge map using `RETR_EXTERNAL` to get only outer boundaries (avoids duplicate inner contours), (4) for each contour, compute moments via `cv::moments`, (5) compute the centroid using the spatial moments `m10` and `m01` normalized by the area moment `m00`; add `1e-5` to avoid division by zero (which could happen for degenerate contours with zero area), (6) store the centroid and `m00` area in a pair. Edge cases: empty contour vector (return empty), zero-area contours (use epsilon), and images with no edges (Canny output all zeros). Time complexity is O(N + M) where N is number of pixels and M is total contour points; space complexity is O(M) for storing contours and moments.
