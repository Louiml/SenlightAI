Write a C++ function that takes a vector of 2D points (as `cv::Point2f`) and a binary threshold value, and returns a `cv::RotatedRect` that best fits an ellipse to the points using the Direct Least Squares (Fitzgibbon 1999) method. The function must first filter out any points that are too close to the image border (within a margin of 2 pixels), then downsample the remaining points by keeping only every 20th point (starting with the first valid point), and finally call OpenCV's `fitEllipseDirect` on the downsampled set. If fewer than 6 points remain after filtering, or if the resulting ellipse has a width or height that is zero, negative, or whose larger dimension exceeds the smaller by a factor of 30, the function should return a default-constructed `cv::RotatedRect` (with center (0,0), size (0,0), angle 0). The input vector is non-empty, but may contain points outside the image bounds—those should be skipped entirely during filtering.

#include <cassert>
#include <opencv2/core.hpp>
#include <vector>

// Forward declaration of the function under test.
cv::RotatedRect fitEllipseDirectRobust(const std::vector<cv::Point2f>& pts,
                                       int imageWidth, int imageHeight,
                                       int margin = 2, int downsampleStep = 20,
                                       double maxRatio = 30.0);

int main() {
    // Test 1: Too few points (fewer than 6) after filtering returns default.
    std::vector<cv::Point2f> fewPoints = {
        cv::Point2f(10, 10), cv::Point2f(20, 20), cv::Point2f(30, 15)
    };
    cv::RotatedRect r1 = fitEllipseDirectRobust(fewPoints, 100, 100);
    assert(r1.center == cv::Point2f(0, 0));
    assert(r1.size.width == 0.0f && r1.size.height == 0.0f);

    // Test 2: Points all near the border are filtered out.
    std::vector<cv::Point2f> borderPoints = {
        cv::Point2f(1, 1), cv::Point2f(2, 2), cv::Point2f(3, 3),
        cv::Point2f(1, 5), cv::Point2f(5, 1), cv::Point2f(0, 0),
        cv::Point2f(99, 99), cv::Point2f(98, 98)
    };
    cv::RotatedRect r2 = fitEllipseDirectRobust(borderPoints, 100, 100);
    assert(r2.center == cv::Point2f(0, 0));

    // Test 3: Points forming a circle (many points) should yield a sensible ellipse.
    std::vector<cv::Point2f> circlePts;
    for (int i = 0; i < 360; i += 5) {
        double angle = i * CV_PI / 180.0;
        circlePts.emplace_back(50.0f + 30.0f * std::cos(angle),
                               50.0f + 30.0f * std::sin(angle));
    }
    cv::RotatedRect r3 = fitEllipseDirectRobust(circlePts, 100, 100);
    assert(r3.size.width > 0.0f && r3.size.height > 0.0f);
    assert(std::abs(r3.size.width - r3.size.height) < 5.0f);

    // Test 4: Points forming an elongated ellipse within ratio limit.
    std::vector<cv::Point2f> ellipsePts;
    for (int i = 0; i < 360; i += 3) {
        double angle = i * CV_PI / 180.0;
        ellipsePts.emplace_back(50.0f + 40.0f * std::cos(angle),
                                50.0f + 10.0f * std::sin(angle));
    }
    cv::RotatedRect r4 = fitEllipseDirectRobust(ellipsePts, 100, 100);
    assert(r4.size.width > 0.0f && r4.size.height > 0.0f);
    assert(std::abs(r4.size.width / r4.size.height - 4.0f) < 1.0f);

    // Test 5: Extremely elongated points (ratio > 30) are rejected.
    std::vector<cv::Point2f> flatPts;
    for (int i = 0; i < 50; ++i) {
        flatPts.emplace_back(10.0f + i * 2.0f, 50.0f);
    }
    // Add a few off-line points to make it a very flat ellipse.
    flatPts.emplace_back(10, 50);
    flatPts.emplace_back(12, 50);
    flatPts.emplace_back(14, 50);
    cv::RotatedRect r5 = fitEllipseDirectRobust(flatPts, 200, 200);
    assert(r5.center == cv::Point2f(0, 0)); // Should be default due to invalid ratio.

    return 0;
}

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

// Fit an ellipse to a set of 2D points using the Direct Least Squares method.
// Points near the border are discarded, and remaining points are downsampled
// (every 20th valid point is kept). Returns a default RotatedRect if the
// fitting fails or produces an invalid ellipse.
cv::RotatedRect fitEllipseDirectRobust(const std::vector<cv::Point2f>& pts,
                                       int imageWidth, int imageHeight,
                                       int margin = 2, int downsampleStep = 20,
                                       double maxRatio = 30.0) {
    const double ratioLimit = maxRatio;
    const int marginPx = margin;
    const int step = downsampleStep;

    std::vector<cv::Point2f> filtered;
    filtered.reserve(pts.size() / step + 1);

    int validIndex = 0;
    for (const auto& p : pts) {
        if (p.x > marginPx && p.y > marginPx &&
            p.x < imageWidth - marginPx && p.y < imageHeight - marginPx) {
            if (validIndex % step == 0) {
                filtered.push_back(p);
            }
            ++validIndex;
        }
    }

    if (filtered.size() < 6) {
        return cv::RotatedRect();
    }

    cv::RotatedRect box = cv::fitEllipseDirect(filtered);

    double maxDim = std::max(box.size.width, box.size.height);
    double minDim = std::min(box.size.width, box.size.height);
    if (maxDim <= 0.0 || minDim <= 0.0 || maxDim > minDim * ratioLimit) {
        return cv::RotatedRect();
    }

    return box;
}

// The solution processes the input points in a single pass to build a filtered and downsampled list. For each point, we first check if it lies strictly inside the image margins: we require `x > margin`, `y > margin`, `x < imageWidth - margin`, `y < imageHeight - margin`. Points failing this check are ignored. Among valid points, we keep every 20th one using an index counter that increments only for valid points—this ensures uniform sampling across the valid set. If after this filtering the retained vector has fewer than 6 points (OpenCV's minimum for `fitEllipseDirect`), we return a default `RotatedRect`. Otherwise, we call `fitEllipseDirect` on the retained points. Post-validation checks the returned rectangle: if either dimension is non-positive or the max/min ratio exceeds 30, we return the default rectangle. Time complexity is O(n) where n is the input size, since each point is visited once and `fitEllipseDirect` runs in O(m) for m retained points (m ≤ n/20). Space complexity is O(m) for the retained points vector, which is O(n) in the worst case but typically much smaller.
