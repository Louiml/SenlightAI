/*
Write a C++ function that takes an OpenCV `Mat` image and returns a vector of four transformed images (scaled, translated, rotated, and perspective-transformed) produced by fixed geometric operations. The scaled image must be 75% of the original size using nearest-neighbor interpolation. The translated image must shift the image by 25 pixels down and 25 pixels to the right, preserving the original output size. The rotated image must rotate the image by 45 degrees clockwise around its center, and the perspective transform must map a specified quadrilateral (with corners at approximately (-30,-60), (cols+50,-50), (cols+100,rows+50), (-50,rows+50)) onto the full original image rectangle. The function must handle empty input images by returning an empty vector, and each output image must have the same number of channels as the input.
*/

#include <opencv2/opencv.hpp>
#include <vector>

// Apply four fixed geometric transformations to an input image.
// Returns {scaled, translated, rotated, perspective_transformed} in that order.
// If the input image is empty, returns an empty vector.
std::vector<cv::Mat> applyGeometricTransforms(const cv::Mat& input) {
    if (input.empty()) {
        return {};
    }

    const int cols = input.cols;
    const int rows = input.rows;

    // 1. Scale to 75% using nearest-neighbor interpolation.
    cv::Mat scaled;
    cv::resize(input, scaled, cv::Size(), 0.75, 0.75, cv::INTER_NEAREST);

    // 2. Translate by (25, 25) pixels, keeping original output size.
    cv::Mat translated;
    cv::Mat translationMatrix = (cv::Mat_<double>(2, 3) << 1, 0, 25.0, 0, 1, 25.0);
    cv::warpAffine(input, translated, translationMatrix, input.size());

    // 3. Rotate 45 degrees clockwise around the exact image center.
    cv::Mat rotated;
    const double angle = 45.0;
    cv::Point2f center((cols - 1) / 2.0f, (rows - 1) / 2.0f);
    cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
    cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), input.size(), angle).boundingRect2f();
    rot.at<double>(0, 2) += bbox.width / 2.0 - cols / 2.0;
    rot.at<double>(1, 2) += bbox.height / 2.0 - rows / 2.0;
    cv::warpAffine(input, rotated, rot, bbox.size());

    // 4. Perspective transform: map a larger quadrilateral onto the original rectangle.
    cv::Mat perspective;
    cv::Point2f inputQuad[4] = {
        cv::Point2f(-30, -60),
        cv::Point2f(cols + 50, -50),
        cv::Point2f(cols + 100, rows + 50),
        cv::Point2f(-50, rows + 50)
    };
    cv::Point2f outputQuad[4] = {
        cv::Point2f(0, 0),
        cv::Point2f(cols - 1, 0),
        cv::Point2f(cols - 1, rows - 1),
        cv::Point2f(0, rows - 1)
    };
    cv::Mat lambda = cv::getPerspectiveTransform(inputQuad, outputQuad);
    cv::warpPerspective(input, perspective, lambda, input.size());

    return {scaled, translated, rotated, perspective};
}

#include <cassert>
#include <opencv2/opencv.hpp>
#include <vector>

// Solution function declaration (or include the header).
std::vector<cv::Mat> applyGeometricTransforms(const cv::Mat& input);

int main() {
    // Create a small test image (e.g., 100x80 grayscale).
    cv::Mat img = cv::Mat::zeros(80, 100, CV_8UC1);
    cv::rectangle(img, cv::Rect(10, 10, 50, 30), cv::Scalar(255), -1);

    // Test with a valid image.
    std::vector<cv::Mat> results = applyGeometricTransforms(img);
    assert(results.size() == 4);
    assert(results[0].size() == cv::Size(75, 60)); // scaled: 0.75 * original
    assert(results[0].type() == img.type());
    assert(results[1].size() == img.size());       // translated: same size as input
    // Rotation output size should be the bounding box of a 45-degree rotated rectangle.
    cv::Size expected_rot_size = cv::RotatedRect(cv::Point2f(), img.size(), 45.0).boundingRect2f().size();
    assert(results[2].size() == expected_rot_size);
    assert(results[3].size() == img.size());       // perspective: same as input

    // Check a pixel after translation: the white rectangle moved by (25,25).
    // Original (10,10) -> (35,35) in translated image.
    assert(results[1].at<uchar>(35, 35) == 255);

    // Test empty input.
    std::vector<cv::Mat> empty_result = applyGeometricTransforms(cv::Mat());
    assert(empty_result.empty());

    // Test color image (3 channels).
    cv::Mat color_img = cv::Mat::zeros(100, 100, CV_8UC3);
    std::vector<cv::Mat> color_results = applyGeometricTransforms(color_img);
    assert(color_results.size() == 4);
    assert(color_results[0].channels() == 3);

    return 0;
}

// The solution uses OpenCV’s geometric transformation functions. For scaling, `cv::resize` with `INTER_NEAREST` generates the 0.75×0.75 version. For translation, a 2×3 affine matrix with `(1,0,25,0,1,25)` is applied via `cv::warpAffine`, preserving input dimensions. For rotation, `cv::getRotationMatrix2D` computes a matrix around the exact pixel center `((cols-1)/2, (rows-1)/2)`, then the matrix is adjusted so the output bounding box of the rotated rectangle is fully visible; the output size is the bounding box size. For perspective, four source points (slightly outside the image) are mapped to the four corners of the output via `cv::getPerspectiveTransform` and applied with `cv::warpPerspective`. Edge cases: an empty input (any dimension zero) returns an empty vector; all operations assume the input is a valid `CV_8UC` or similar multi-channel image (but not required for correctness—they work with any type). Time complexity is O(n) per transformation for an n-pixel image (since each operation is linear in pixel count), and auxiliary space is O(n) for the output images.
