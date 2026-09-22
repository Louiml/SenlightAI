Write a standalone C++ function named `refineWithIterativeLeastSquares` that takes a vector of 2D points as `std::vector<std::pair<double,double>>`, a candidate line model represented by a `std::pair<double,double>` (slope and intercept), a squared error threshold, an integer number of iterations, and returns the refined model (as a `std::pair<double,double>`) after applying an iterative re-weighted least squares (IRLS) approach. In each iteration, compute squared residuals of all points relative to the current line, assign weights using a truncated quadratic cost (weight = 1 for residuals ≤ threshold, otherwise 0 for that iteration — i.e., hard inlier selection), then refit the line via least squares using only the inlier points. Stop early if the set of inliers does not change between consecutive iterations or if fewer than two points are inliers (in which case return the input model). The final returned model must be the one from the last successful least squares fit that had at least two inliers, unless no refinement occurs, in which case return the original model.
#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// Declare the function (or include the solution file)
std::pair<double, double> refineWithIterativeLeastSquares(
    const std::vector<std::pair<double, double>>& points,
    std::pair<double, double> model,
    double squared_threshold,
    int iterations);

int main() {
    // Perfect line y = 2x + 1, all points exact
    std::vector<std::pair<double, double>> pts1 = {{0,1}, {1,3}, {2,5}, {3,7}};
    auto res1 = refineWithIterativeLeastSquares(pts1, {0,0}, 1e-6, 5);
    assert(std::fabs(res1.first - 2.0) < 1e-9);
    assert(std::fabs(res1.second - 1.0) < 1e-9);

    // With noise, should approximate the true line
    std::vector<std::pair<double, double>> pts2 = {{0,1.1}, {1,2.9}, {2,5.2}, {3,6.8}, {10,100}}; // outlier
    auto res2 = refineWithIterativeLeastSquares(pts2, {1.0, 0.0}, 0.5, 10);
    // Outlier (10,100) far away, threshold small, so inliers are first four points
    assert(std::fabs(res2.first - 2.0) < 0.5);
    assert(std::fabs(res2.second - 1.0) < 0.5);

    // Too few points to refine
    std::vector<std::pair<double, double>> pts3 = {{0,0}, {1,1}};
    auto res3 = refineWithIterativeLeastSquares(pts3, {5, -2}, 1e-6, 3);
    assert(res3.first == 5.0 && res3.second == -2.0); // unchanged because 0 inliers

    // Vertical line case: all x same, should return original model
    std::vector<std::pair<double, double>> pts4 = {{0,0}, {0,1}, {0,2}};
    auto res4 = refineWithIterativeLeastSquares(pts4, {3, 4}, 10, 5);
    assert(res4.first == 3.0 && res4.second == 4.0);

    // Convergence: inliers stop changing after first iteration
    std::vector<std::pair<double, double>> pts5 = {{0,0}, {1,1}, {2,2}, {3,3}, {4,10}};
    auto res5 = refineWithIterativeLeastSquares(pts5, {0.5, 0.5}, 1.0, 20);
    // Should fit line to first four points, outlier excluded
    assert(std::fabs(res5.first - 1.0) < 1e-9);
    assert(std::fabs(res5.second - 0.0) < 1e-9);

    return 0;
}
#include <vector>
#include <utility>
#include <cmath>
#include <algorithm>

// Refine a line model (slope, intercept) by iterative least squares on inliers.
// Returns the refined model, or the original if refinement is not possible.
std::pair<double, double> refineWithIterativeLeastSquares(
    const std::vector<std::pair<double, double>>& points,
    std::pair<double, double> model,
    double squared_threshold,
    int iterations) {
    
    int n = static_cast<int>(points.size());
    if (n < 2) return model;

    double m = model.first;
    double b = model.second;
    std::vector<int> prev_inliers;
    std::vector<int> curr_inliers;

    // Compute initial inliers based on input model
    for (int i = 0; i < n; ++i) {
        double residual = points[i].second - (m * points[i].first + b);
        if (residual * residual <= squared_threshold) {
            curr_inliers.push_back(i);
        }
    }

    // If fewer than 2 inliers initially, no refinement possible
    if (curr_inliers.size() < 2) return model;

    bool inliers_changed = true;

    for (int iter = 0; iter < iterations && inliers_changed; ++iter) {
        // Fit line to current inliers using least squares
        double sum_x = 0, sum_y = 0, sum_xy = 0, sum_xx = 0;
        int k = static_cast<int>(curr_inliers.size());
        for (int i = 0; i < k; ++i) {
            int idx = curr_inliers[i];
            double x = points[idx].first;
            double y = points[idx].second;
            sum_x += x;
            sum_y += y;
            sum_xy += x * y;
            sum_xx += x * x;
        }

        double denominator = k * sum_xx - sum_x * sum_x;
        // Degenerate case: all points have same x (vertical line) → cannot fit y=mx+b
        if (std::fabs(denominator) < 1e-12) {
            return model;
        }

        double new_m = (k * sum_xy - sum_x * sum_y) / denominator;
        double new_b = (sum_y - new_m * sum_x) / k;

        // Update model
        m = new_m;
        b = new_b;

        // Compute new inlier set
        prev_inliers = curr_inliers;
        curr_inliers.clear();
        for (int i = 0; i < n; ++i) {
            double residual = points[i].second - (m * points[i].first + b);
            if (residual * residual <= squared_threshold) {
                curr_inliers.push_back(i);
            }
        }

        // Check if inlier set changed
        if (curr_inliers.size() != prev_inliers.size()) {
            inliers_changed = true;
        } else {
            inliers_changed = false;
            for (size_t i = 0; i < curr_inliers.size(); ++i) {
                if (curr_inliers[i] != prev_inliers[i]) {
                    inliers_changed = true;
                    break;
                }
            }
        }

        // If too few inliers, stop and keep previous model
        if (curr_inliers.size() < 2) {
            return model;
        }
    }

    return {m, b};
}
// The solution uses an iterative scheme: start with the input model, and repeatedly compute residuals from all points. Points with squared error ≤ the given threshold are considered inliers. If the inlier count is below 2, break and keep the previous model. Otherwise, perform a least squares fit on the inlier points to compute new slope `m` and intercept `b` using standard formulas: `m = (nΣ(xy) − ΣxΣy) / (nΣ(x²) − (Σx)²)` and `b = (Σy − mΣx) / n`, where `n` is the number of inliers. After fitting, assign the new model and check if the inlier set is identical to the previous iteration’s set; if so, break. The loop runs at most `iterations` times, or until the model converges. Edge cases: vertical lines would cause division by zero in the slope formula; since we represent lines as y = mx + b, such cases are not properly handled, so we assume non-vertical lines or detect degenerate denominator and return the current model. Also, if the threshold is so large that all points are inliers, the fit still works. Time complexity: O(iterations × N) where N is the number of points, and space complexity O(N) for residual storage.
