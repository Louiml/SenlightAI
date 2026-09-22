// Write a C++ function `double estimateSimilarityTransform(const std::vector<std::pair<double,double>>& source, const std::vector<std::pair<double,double>>& target)` that computes the least mean square (LMS) error of the best similarity transform (scale, rotation, translation) mapping a set of source 2D points to target 2D points. Assume both vectors have exactly 3 points (non-collinear is not guaranteed), and return the non-negative LMS residual error as defined by `CalculateTransformationLMS3_0` in the provided snippet. The function should handle edge cases where the denominator (sum of variances of source points) is zero (e.g., all source points identical) by returning `0.0` to avoid division by zero. Points are given as `(x, y)` pairs; use doubles internally for precision, but inputs are integer-like doubles. The similarity transform must allow arbitrary rotation and uniform scaling, and the error measures how well the source points can be aligned to the target points after an optimal similarity transformation.
// The core idea is to compute the optimal similarity transform parameters (scale `s`, rotation angle `θ`, and translation `(tx, ty)`) in a closed-form least-squares sense for two sets of three corresponding 2D points. The algorithm follows the standard approach for absolute orientation with similarity transform (also known as Umeyama's method simplified for points with equal weights). First, compute the centroids of both point sets. Then, compute the covariance-like quantities: the sums of products of centered coordinates (`XtXs`, `YtYs`, `XtYs`, `YtXs`) and the variances of the source points (`XsXs + YsYs`). The optimal rotation is given by `atan2(XtYs - YtXs, XtXs + YtYs)`. The optimal scale is derived by projecting the cross-covariance onto the rotation and dividing by the source variance. The LMS residual error is the total variance of the target points minus the squared Frobenius norm of the cross-covariance divided by the source variance. This residual is always non-negative due to the Cauchy–Schwarz inequality; numerical rounding might produce tiny negative values, so we clamp to zero. Edge cases: if the source variance is zero (all source points identical), the denominator is zero; in that case, any scale/rotation gives the same error, but the optimal translation maps the source centroid to the target centroid, and the error becomes the variance of the target points. However, the provided function `CalculateTransformationLMS3_0` returns `dbLMS` without ever calculating translation; when `del == 0`, it returns `dbLMS = 0` because the formula for `dbLMS` is only set inside the `if` block, and it is initialized to 0. So our function should mimic that: if `del == 0`, return `0.0`. For three points, the algorithm runs in O(1) time and uses O(1) auxiliary space.
#include <vector>
#include <utility>
#include <cmath>
#include <cassert>

// Compute the LMS residual error of the best similarity transform
// mapping 'source' (3 points) to 'target' (3 points).
// Returns the non-negative residual error as defined in the original
// CalculateTransformationLMS3_0 function.
double estimateSimilarityTransform(
    const std::vector<std::pair<double, double>>& source,
    const std::vector<std::pair<double, double>>& target) 
{
    assert(source.size() == 3 && target.size() == 3);

    // Compute centroids of both point sets.
    double xt = (source[0].first + source[1].first + source[2].first) / 3.0;
    double yt = (source[0].second + source[1].second + source[2].second) / 3.0;
    double xs = (target[0].first + target[1].first + target[2].first) / 3.0;
    double ys = (target[0].second + target[1].second + target[2].second) / 3.0;

    // Compute centered second-order moments.
    double xt_xt = (source[0].first * source[0].first + 
                     source[1].first * source[1].first + 
                     source[2].first * source[2].first) / 3.0;
    double yt_yt = (source[0].second * source[0].second + 
                     source[1].second * source[1].second + 
                     source[2].second * source[2].second) / 3.0;
    double xs_xs = (target[0].first * target[0].first + 
                     target[1].first * target[1].first + 
                     target[2].first * target[2].first) / 3.0;
    double ys_ys = (target[0].second * target[0].second + 
                     target[1].second * target[1].second + 
                     target[2].second * target[2].second) / 3.0;

    double xt_xs = (source[0].first * target[0].first + 
                     source[1].first * target[1].first + 
                     source[2].first * target[2].first) / 3.0;
    double yt_ys = (source[0].second * target[0].second + 
                     source[1].second * target[1].second + 
                     source[2].second * target[2].second) / 3.0;
    double xt_ys = (source[0].first * target[0].second + 
                     source[1].first * target[1].second + 
                     source[2].first * target[2].second) / 3.0;
    double yt_xs = (source[0].second * target[0].first + 
                     source[1].second * target[1].first + 
                     source[2].second * target[2].first) / 3.0;

    xt_xt -= xt * xt;
    yt_yt -= yt * yt;
    xs_xs -= xs * xs;
    ys_ys -= ys * ys;
    xt_xs -= xt * xs;
    yt_ys -= yt * ys;
    xt_ys -= xt * ys;
    yt_xs -= yt * xs;

    // Optimal rotation angle.
    double rotate = std::atan2(xt_ys - yt_xs, xt_xs + yt_ys);
    double cos_r = std::cos(rotate);
    double sin_r = std::sin(rotate);

    double del = xs_xs + ys_ys;
    double lms = 0.0;

    if (del != 0.0) {
        // Residual error formula from the original function.
        double numerator = (xt_xs + yt_ys) * (xt_xs + yt_ys) +
                           (xt_ys - yt_xs) * (xt_ys - yt_xs);
        lms = xt_xt + yt_yt - numerator / del;
        // Guard against tiny negative values due to floating-point precision.
        if (lms < 0.0) lms = 0.0;
    }
    // If del == 0, the function returns 0.0 as in the original implementation.

    return lms;
}
#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// Function declaration (assume it is included from the solution above)
double estimateSimilarityTransform(
    const std::vector<std::pair<double, double>>& source,
    const std::vector<std::pair<double, double>>& target);

int main() {
    // Identity transform: points unchanged -> LMS error is 0.
    std::vector<std::pair<double,double>> pts1 = {{0,0}, {1,0}, {0,1}};
    assert(std::fabs(estimateSimilarityTransform(pts1, pts1)) < 1e-9);

    // Pure translation: shift all points by (5, -3).
    std::vector<std::pair<double,double>> target1 = {{5,-3}, {6,-3}, {5,-2}};
    assert(std::fabs(estimateSimilarityTransform(pts1, target1)) < 1e-9);

    // Scaled by 2 and rotated by 90 degrees: (x,y) -> (-2y, 2x).
    // Original points: (0,0)->(0,0), (1,0)->(0,2), (0,1)->(-2,0).
    std::vector<std::pair<double,double>> target2 = {{0,0}, {0,2}, {-2,0}};
    assert(std::fabs(estimateSimilarityTransform(pts1, target2)) < 1e-9);

    // Non-collinear points with a known perfect transform.
    // Source: (10,20), (30,40), (50,60) not collinear? Actually they are collinear.
    // Use non-collinear: (0,0), (2,0), (0,3).
    std::vector<std::pair<double,double>> src3 = {{0,0}, {2,0}, {0,3}};
    // Apply rotation 30 degrees and scale 1.5.
    double angle = 30.0 * M_PI / 180.0;
    double s = 1.5;
    double cx = 100.0, cy = 50.0;
    std::vector<std::pair<double,double>> dst3;
    for (const auto& p : src3) {
        double x = p.first * s * std::cos(angle) - p.second * s * std::sin(angle) + cx;
        double y = p.first * s * std::sin(angle) + p.second * s * std::cos(angle) + cy;
        dst3.push_back({x, y});
    }
    assert(std::fabs(estimateSimilarityTransform(src3, dst3)) < 1e-6);

    // Edge case: all source points identical -> denominator zero.
    std::vector<std::pair<double,double>> src_degenerate = {{3,4}, {3,4}, {3,4}};
    std::vector<std::pair<double,double>> dst_any = {{1,2}, {5,6}, {7,8}};
    // Original function returns 0 in this case.
    assert(std::fabs(estimateSimilarityTransform(src_degenerate, dst_any)) < 1e-9);

    // Non-zero error when transform is not exactly similarity.
    // Template: triangle with sides 1,1,sqrt(2), target is a squashed version.
    std::vector<std::pair<double,double>> src4 = {{0,0}, {1,0}, {0,1}};
    std::vector<std::pair<double,double>> dst4 = {{0,0}, {2,0}, {0,1}}; // not a similarity (aspect ratio changed)
    double err = estimateSimilarityTransform(src4, dst4);
    assert(err > 0.0 && err < 10.0); // should be a positive residual

    return 0;
}
