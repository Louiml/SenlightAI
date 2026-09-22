// Write a C++ function `std::vector<std::pair<double, double>> movingAverage(const std::vector<std::pair<double, double>>& data, double width)` that takes a sequence of (x, y) coordinate pairs sorted by x and a positive window width `width`. For each valid point that has at least one neighbor on both the left and right within a horizontal distance of `width/2`, compute the average y-value of all points (including itself) whose x-coordinates lie within [x - width/2, x + width/2]. The function should ignore any points with null/NaN x or y values (treat NaN as missing data and skip those points entirely). The output should be a vector of (x, y) pairs where x is the original x-coordinate of the central point and y is the computed moving average. Points that do not have at least one valid neighbor on both sides within the window should be omitted from the output. Ensure the result preserves the original order of x-coordinates.
The algorithm iterates over each valid (non-NaN) point in the input. For each such point at index `i`, we expand leftward (decrementing index) and rightward (incrementing index) while the horizontal distance from the central x does not exceed `width/2`. During this expansion, we skip any point whose x or y is NaN. We accumulate the sum of y-values and count of valid points. To determine whether the central point has a neighbor on both sides, we track whether we successfully included at least one valid point to the left and at least one to the right. If both sides have at least one valid point and the central point itself is valid, we append to the result the pair `(x_i, sum/count)`. Important edge cases: an empty input returns an empty vector; if all points have NaN, the result is empty; if a point has no valid neighbor on one side (e.g., the first point with no left neighbor), it is excluded. The time complexity is O(n^2) in the worst case when the window covers many points per central point, but typically O(n * k) where k is the average number of points in a window. Space complexity is O(m) for the output, where m is the number of output points.
#include <vector>
#include <cmath>
#include <utility>

// Compute the moving average of y-values within a horizontal window of given width.
// data: vector of (x, y) pairs sorted by x; NaN values in x or y are treated as missing.
// width: positive window width; for each point, average is taken over points with
//        |x - x_center| <= width/2, ignoring NaN values.
// Returns: vector of (x, y) pairs for points that have at least one valid neighbor
//          on both the left and right within the window. Output preserves order.
std::vector<std::pair<double, double>> movingAverage(
    const std::vector<std::pair<double, double>>& data, double width) {
    
    std::vector<std::pair<double, double>> result;
    if (data.empty() || width <= 0.0) return result;
    
    double halfWidth = width / 2.0;
    const double nanValue = std::nan("");
    
    for (std::size_t i = 0; i < data.size(); ++i) {
        double xCenter = data[i].first;
        double yCenter = data[i].second;
        // Skip central point if it is invalid (NaN).
        if (std::isnan(xCenter) || std::isnan(yCenter)) continue;
        
        bool hasLeft = false;
        bool hasRight = false;
        double sum = yCenter;
        int count = 1;
        
        // Scan left.
        for (std::size_t j = i; j-- > 0;) {
            double xNext = data[j].first;
            if (std::isnan(xNext)) continue;
            if ((xCenter - xNext) > halfWidth) break;
            double yNext = data[j].second;
            if (std::isnan(yNext)) continue;
            sum += yNext;
            count++;
            hasLeft = true;
        }
        
        // Scan right.
        for (std::size_t j = i + 1; j < data.size(); ++j) {
            double xNext = data[j].first;
            if (std::isnan(xNext)) continue;
            if ((xNext - xCenter) > halfWidth) break;
            double yNext = data[j].second;
            if (std::isnan(yNext)) continue;
            sum += yNext;
            count++;
            hasRight = true;
        }
        
        // Only include points with at least one valid neighbor on both sides.
        if (hasLeft && hasRight) {
            result.emplace_back(xCenter, sum / count);
        }
    }
    return result;
}
#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// The solution function is declared above (include it directly in the same file).
std::vector<std::pair<double, double>> movingAverage(
    const std::vector<std::pair<double, double>>& data, double width);

int main() {
    // Basic case: window covers all points evenly.
    std::vector<std::pair<double, double>> d1 = {{0.0, 1.0}, {1.0, 3.0}, {2.0, 5.0}};
    auto r1 = movingAverage(d1, 4.0);
    assert(r1.size() == 1);
    assert(std::abs(r1[0].first - 1.0) < 1e-9);
    assert(std::abs(r1[0].second - 3.0) < 1e-9); // (1+3+5)/3 = 3

    // Two points: neither has a neighbor on both sides, result empty.
    std::vector<std::pair<double, double>> d2 = {{0.0, 1.0}, {2.0, 3.0}};
    auto r2 = movingAverage(d2, 2.0);
    assert(r2.empty());

    // With NaN values: skip them but still use valid neighbors.
    std::vector<std::pair<double, double>> d3 = {
        {0.0, 1.0}, {1.0, std::nan("")}, {2.0, 3.0}, {3.0, 5.0}
    };
    auto r3 = movingAverage(d3, 4.0);
    assert(r3.size() == 1); // Only point at x=2 has both left and right valid
    assert(std::abs(r3[0].first - 2.0) < 1e-9);
    // Left valid: (0,1), right valid: (3,5), center (2,3): avg = (1+3+5)/3 = 3
    assert(std::abs(r3[0].second - 3.0) < 1e-9);

    // Point at the beginning: no left neighbor, excluded.
    std::vector<std::pair<double, double>> d4 = {{0.0, 1.0}, {1.0, 2.0}, {2.0, 3.0}};
    auto r4 = movingAverage(d4, 1.0); // halfWidth = 0.5
    // Only point at x=1 has neighbors within 0.5: avg = (1+2+3)/3 = 2
    assert(r4.size() == 1);
    assert(std::abs(r4[0].first - 1.0) < 1e-9);
    assert(std::abs(r4[0].second - 2.0) < 1e-9);

    // Empty input.
    std::vector<std::pair<double, double>> d5;
    assert(movingAverage(d5, 1.0).empty());

    // All NaN central points: result empty.
    std::vector<std::pair<double, double>> d6 = {
        {0.0, std::nan("")}, {1.0, 2.0}, {2.0, std::nan("")}
    };
    assert(movingAverage(d6, 3.0).empty());

    // Check that width=0 returns empty.
    std::vector<std::pair<double, double>> d7 = {{0.0, 1.0}, {1.0, 2.0}};
    assert(movingAverage(d7, 0.0).empty());

    return 0;
}
