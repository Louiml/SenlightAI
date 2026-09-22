Write a standalone C++ function named `computeTcaScore` that takes two grayscale image matrices (as `cv::Mat` containing 16-bit unsigned integer pixels, type `CV_16UC1`), a vector of four polynomial coefficients `{a, b, c, d}` for a radial distortion model `rd = a*r + b*r^2 + c*r^3 + d*r^4`, and a boolean `isExrMode`. The function must compute a Transverse Chromatic Aberration (TCA) metric based on the original code snippet, but simplified to operate on a single pair of input images without the survey loop, progress bar, or file I/O. Specifically: (1) generate a binary mask of "valid" pixels — if `isExrMode` is true, mark pixels inside a diamond-shaped CFA region defined by the four inequalities `i + j >= cols - 1`, `j > i - rows - 1`, `i + j < cols + 2*rows - 1`, and `i > j - cols`; if false, mark all pixels as valid; (2) compute remap grids `map_x` and `map_y` of type `CV_32FC1` using the distortion model, normalizing coordinates by the shorter image dimension, with pixel centers offset by 0.5 and output coordinates mapped back to pixel space via `(dim * coord * rd / r + dim) / 2`; (3) apply two copies of `cv::remap` — one to the mask and one to the input image — with linear interpolation and border constant zero; (4) compute the mean absolute difference between the remapped image and the original reference image, but only over pixels where the remapped mask value is 65535 (i.e., valid after remapping). Return this mean as a `double`. The function must handle both landscape and portrait images by using `width >= height` vs `width < height` branches for normalization. Assume input images have identical dimensions and are 16-bit grayscale. Use OpenCV only, no OpenMP required, and write clean, readable code with comments.

#include <cassert>
#include <vector>
#include <opencv2/opencv.hpp>

// Declaration of the function under test (usually in a header)
double computeTcaScore(const cv::Mat& ref_plane, const cv::Mat& work_plane,
                       const std::vector<double>& coeffs, bool isExrMode);

int main() {
    // Test 1: Identical images with zero distortion -> TCA = 0
    cv::Mat img1(10, 10, CV_16UC1, cv::Scalar(1000));
    assert(computeTcaScore(img1, img1, {0,0,0,0}, false) == 0.0);

    // Test 2: Different but constant images, zero distortion -> mean abs diff = 500
    cv::Mat img2(10, 10, CV_16UC1, cv::Scalar(2000));
    assert(computeTcaScore(img1, img2, {0,0,0,0}, false) == 1000.0);

    // Test 3: Non-EXR mode, all pixels valid, same as test 1
    assert(computeTcaScore(img1, img1, {0.1,0.05,0.001,0.0001}, false) == 0.0);

    // Test 4: EXR mode with identical images still zero
    assert(computeTcaScore(img1, img1, {0,0,0,0}, true) == 0.0);

    // Test 5: EXR mode with different images, non-zero result, verify it's finite
    double score = computeTcaScore(img1, img2, {0.1,0.02,0.001,0.0005}, true);
    assert(score >= 0.0 && score <= 65535.0);

    // Test 6: Portrait orientation (height > width) with identical images
    cv::Mat imgPortrait(20, 5, CV_16UC1, cv::Scalar(500));
    assert(computeTcaScore(imgPortrait, imgPortrait, {0.01,0.0,0.0,0.0}, false) == 0.0);

    // Test 7: Landscape orientation (width > height) with slightly different images
    cv::Mat imgLandscape(5, 20, CV_16UC1, cv::Scalar(3000));
    cv::Mat imgLandscape2(5, 20, CV_16UC1, cv::Scalar(3001));
    double landScore = computeTcaScore(imgLandscape, imgLandscape2, {0.0,0.0,0.0,0.0}, false);
    assert(landScore > 0.0 && landScore < 1.0); // Should be exactly 1.0 on average if all pixels valid
    // Actually with zero distortion, valids all, diff = 1 per pixel -> score = 1.0
    assert(landScore == 1.0);

    // Test 8: Check that non-EXR mode with a simple remap that moves pixels slightly
    // still produces a consistent result (just check it runs)
    cv::Mat imgA(8, 8, CV_16UC1, cv::Scalar(100));
    cv::Mat imgB(8, 8, CV_16UC1, cv::Scalar(200));
    double score8 = computeTcaScore(imgA, imgB, {0.001, 0.0001, 0.0, 0.0}, true);
    assert(score8 >= 0.0);

    // Test 9: Zero valid pixels in EXR mode for a tiny image? Ensure no crash
    cv::Mat tiny(3, 3, CV_16UC1, cv::Scalar(10));
    double score9 = computeTcaScore(tiny, tiny, {0.5,0.1,0.01,0.001}, true);
    // The diamond region may be empty for 3x3, so quot could be zero; function returns 0
    assert(score9 >= 0.0);

    // Test 10: Different coefficient values produce different scores (sanity check)
    double s1 = computeTcaScore(imgA, imgB, {0.0,0.0,0.0,0.0}, false);
    double s2 = computeTcaScore(imgA, imgB, {0.01,0.0,0.0,0.0}, false);
    // Zero distortion gives constant difference 100; non-zero distortion may alter it
    assert(s1 == 100.0);
    // With a tiny distortion, the mean difference should be close to 100 but not necessarily equal
    assert(s2 != s1);

    return 0;
}

#include <opencv2/opencv.hpp>
#include <cmath>
#include <cstdint>
#include <vector>

/**
 * Compute TCA metric between a reference and a distorted work image.
 * 
 * @param ref_plane  Reference 16-bit grayscale image (CV_16UC1).
 * @param work_plane Work 16-bit grayscale image (CV_16UC1), same dimensions.
 * @param coeffs     Vector {a, b, c, d} for radial distortion polynomial.
 * @param isExrMode  If true, restrict valid pixels to diamond-shaped CFA region.
 * @return Mean absolute difference over valid pixels after remapping.
 */
double computeTcaScore(const cv::Mat& ref_plane, const cv::Mat& work_plane,
                       const std::vector<double>& coeffs, bool isExrMode) {
    // Validate inputs
    CV_Assert(ref_plane.type() == CV_16UC1 && work_plane.type() == CV_16UC1);
    CV_Assert(ref_plane.size() == work_plane.size());
    CV_Assert(coeffs.size() == 4);

    int width = work_plane.cols;
    int height = work_plane.rows;
    double a = coeffs[0], b = coeffs[1], c = coeffs[2], d = coeffs[3];

    // Step 1: Build binary mask (valid pixels)
    cv::Mat blank(height, width, CV_16UC1);
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            if (isExrMode) {
                bool valid = (i + j >= width - 1) &&
                             (j > i - height - 1) &&
                             (i + j < width + 2 * height - 1) &&
                             (i > j - width);
                blank.at<ushort>(j, i) = valid ? 65535 : 0;
            } else {
                blank.at<ushort>(j, i) = 65535;
            }
        }
    }

    // Step 2: Prepare output and map matrices
    cv::Mat dst, mask, map_x, map_y;
    dst.create(work_plane.size(), work_plane.type());
    map_x.create(work_plane.size(), CV_32FC1);
    map_y.create(work_plane.size(), CV_32FC1);

    // Step 3: Compute remap grids
    if (width >= height) {
        // Landscape orientation: normalize by height
        for (int j = 0; j < height; ++j) {
            double y = (2 * (j + 0.5) - height) / height;
            for (int i = 0; i < width; ++i) {
                if (blank.at<ushort>(j, i) == 65535) {
                    double x = (2 * (i + 0.5) - width) / height;
                    double r = std::sqrt(x * x + y * y);
                    double rd = a * r + b * std::pow(r, 2) + c * std::pow(r, 3) + d * std::pow(r, 4);
                    map_x.at<float>(j, i) = static_cast<float>((height * x * rd / r + width) / 2);
                    map_y.at<float>(j, i) = static_cast<float>((height * y * rd / r + height) / 2);
                }
            }
        }
    } else {
        // Portrait orientation: normalize by width
        for (int j = 0; j < height; ++j) {
            double y = (2 * (j + 0.5) - height) / width;
            for (int i = 0; i < width; ++i) {
                if (blank.at<ushort>(j, i) == 65535) {
                    double x = (2 * (i + 0.5) - width) / width;
                    double r = std::sqrt(x * x + y * y);
                    double rd = a * r + b * std::pow(r, 2) + c * std::pow(r, 3) + d * std::pow(r, 4);
                    map_x.at<float>(j, i) = static_cast<float>((width * x * rd / r + width) / 2);
                    map_y.at<float>(j, i) = static_cast<float>((width * y * rd / r + height) / 2);
                }
            }
        }
    }

    // Step 4: Remap mask and work image
    cv::remap(blank, mask, map_x, map_y, cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(0));
    cv::remap(work_plane, dst, map_x, map_y, cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(0));

    // Step 5: Compute mean absolute difference over valid pixels
    unsigned long num = 0;
    unsigned long quot = 0;
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            if (mask.at<ushort>(j, i) == 65535) {
                num += std::abs(static_cast<int>(dst.at<ushort>(j, i)) -
                                static_cast<int>(ref_plane.at<ushort>(j, i)));
                ++quot;
            }
        }
    }
    return (quot > 0) ? (1.0 * num / quot) : 0.0;
}

// The solution mirrors the core computational steps of the snippet but strips away the survey, parallelism, and UI components. The main algorithm: first, construct a `blank` mask of type `CV_16UC1` with the same dimensions as the work image. For EXR mode, a pixel at row `i`, column `j` is valid if it lies inside a diamond-shaped region; the four inequalities are derived from the CFA pattern boundaries. For non-EXR mode, all pixels are valid. Next, allocate `dst` (same type as input) and `map_x`, `map_y` as `CV_32FC1`. Then compute the remap coordinates: for each pixel (row `j`, column `i`), normalize `x` and `y` by dividing by the shorter dimension (`height` for landscape, `width` for portrait) after centering with 0.5 offset, compute `r = sqrt(x*x + y*y)`, then `rd` from the polynomial, and finally set the corresponding map entries using the formula `(dim * coord * rd / r + dim) / 2`, where `dim` is the normalization dimension (height for landscape maps, width for portrait maps for both `map_x` and `map_y`, but note the output row scaling uses `height` in landscape and `width` in portrait for `map_y` as in the original). After building the grids, call `cv::remap` twice: once on `blank` to produce `mask`, once on `work_plane` to produce `dst`, both with `cv::INTER_LINEAR` and `cv::BORDER_CONSTANT` with zero scalar. Finally, iterate over all pixels, and where `mask.at<ushort>(j,i) == 65535`, accumulate the absolute difference between `dst.at<ushort>(j,i)` and `ref_plane.at<ushort>(j,i)`, counting the number of such pixels. Return `num / quot` as a double. Edge cases: if `quot` is zero (no valid pixels after remapping), return 0.0 to avoid division by zero. The normalization formulas must be correct for both orientations — in landscape, the `x` coordinate is divided by `height` (not width) while `y` is also divided by `height`; in portrait, both are divided by `width`. The `map_y` output row coordinate uses `height` as the outer dimension in landscape and `width` in portrait (as per original code, which uses `height` in landscape and `width` in portrait for `map_y`; note this is a quirk of the snippet, preserved for fidelity). Time complexity is O(W*H) for mask construction, O(W*H) for map computation, O(W*H) for each remap (assuming linear interpolation), and O(W*H) for difference accumulation, total O(W*H). Space complexity is O(W*H) for the mask, destination, and two map matrices. The solution uses only standard C++ and OpenCV core modules; no parallel constructs are required.
