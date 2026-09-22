Write a C++ function that takes a vector of grayscale `cv::Mat` images (all with the same dimensions) and returns a single `cv::Mat` that is the pixel-wise median across all input images (i.e., each pixel in the output is the median of the corresponding pixel values from all input images). The input vector may be empty or contain one image; handle these cases by returning an empty `cv::Mat` or a clone of the single image, respectively. The function must not modify the input images and must use `const` references where appropriate. Assume OpenCV is available and the function is a free function (no class).

// The solution processes each pixel location independently. For an output of size `H × W`, and `N` input images, we iterate over all `H × W` pixels. For each pixel, we collect the `N` intensity values (one from each image) into a temporary vector, sort it using `std::nth_element` (or full sort) to find the median, and assign the median to the output image at that location. Edge cases: (1) Empty input → return empty `cv::Mat`. (2) Single image → return a clone to avoid aliasing. (3) Images with different sizes → we can assume they are the same per specification, but the algorithm can still handle it by using the size of the first image and reading with bounds checks (though it’s safer to return empty if sizes mismatch). Time complexity: `O(H * W * N log N)` if using full sort, or `O(H * W * N)` average with `std::nth_element`. Space complexity: `O(N)` for the temporary pixel buffer plus output `O(H*W)`.

#include <opencv2/core.hpp>
#include <vector>
#include <algorithm>

// Compute the pixel-wise median of a vector of grayscale images (CV_8UC1).
// Returns an empty Mat if input is empty; returns a clone if only one image.
cv::Mat medianImages(const std::vector<cv::Mat>& images) {
    if (images.empty()) {
        return cv::Mat();
    }
    if (images.size() == 1) {
        return images[0].clone();
    }

    // Assume all images have the same size and type.
    const int rows = images[0].rows;
    const int cols = images[0].cols;
    const int type = images[0].type();
    cv::Mat result(rows, cols, type);

    const size_t n = images.size();
    std::vector<uchar> values(n);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // Collect pixel values from all images at (i, j)
            for (size_t k = 0; k < n; ++k) {
                values[k] = images[k].at<uchar>(i, j);
            }
            // Find median (nth_element for odd/even; for even we take lower median)
            std::nth_element(values.begin(), values.begin() + n / 2, values.end());
            result.at<uchar>(i, j) = values[n / 2];
        }
    }
    return result;
}

#include <opencv2/core.hpp>
#include <cassert>
#include <vector>

// Declare the solution function (from the task)
cv::Mat medianImages(const std::vector<cv::Mat>& images);

int main() {
    // Test 1: Empty input -> empty Mat
    {
        std::vector<cv::Mat> empty;
        cv::Mat out = medianImages(empty);
        assert(out.empty());
    }

    // Test 2: Single image -> clone
    {
        cv::Mat single = (cv::Mat_<uchar>(2, 2) << 1, 2, 3, 4);
        cv::Mat out = medianImages({single});
        assert(out.size() == single.size());
        assert(out.type() == single.type());
        // Ensure the output is a separate matrix (not the same data)
        out.at<uchar>(0, 0) = 200;
        assert(single.at<uchar>(0, 0) == 1);
    }

    // Test 3: Odd number of images (3 images, median is middle value)
    {
        cv::Mat a = (cv::Mat_<uchar>(2, 2) << 10, 20, 30, 40);
        cv::Mat b = (cv::Mat_<uchar>(2, 2) << 50, 60, 70, 80);
        cv::Mat c = (cv::Mat_<uchar>(2, 2) << 90, 100, 110, 120);
        cv::Mat expected = (cv::Mat_<uchar>(2, 2) << 50, 60, 70, 80);
        cv::Mat out = medianImages({a, b, c});
        assert(cv::countNonZero(out != expected) == 0);
    }

    // Test 4: Even number of images (4 images, lower median)
    {
        cv::Mat a = (cv::Mat_<uchar>(1, 2) << 1, 5);
        cv::Mat b = (cv::Mat_<uchar>(1, 2) << 2, 6);
        cv::Mat c = (cv::Mat_<uchar>(1, 2) << 3, 7);
        cv::Mat d = (cv::Mat_<uchar>(1, 2) << 4, 8);
        // For first pixel: values {1,2,3,4} -> lower median = 2 (element at index 2/2=1)
        // For second pixel: {5,6,7,8} -> lower median = 6
        cv::Mat expected = (cv::Mat_<uchar>(1, 2) << 2, 6);
        cv::Mat out = medianImages({a, b, c, d});
        assert(cv::countNonZero(out != expected) == 0);
    }

    // Test 5: Mixed values with duplicates
    {
        cv::Mat a = (cv::Mat_<uchar>(1, 1) << 7);
        cv::Mat b = (cv::Mat_<uchar>(1, 1) << 7);
        cv::Mat c = (cv::Mat_<uchar>(1, 1) << 9);
        cv::Mat expected = (cv::Mat_<uchar>(1, 1) << 7);
        cv::Mat out = medianImages({a, b, c});
        assert(out.at<uchar>(0, 0) == 7);
    }

    return 0;
}
