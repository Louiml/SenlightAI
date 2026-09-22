Write a C++ function `findLineWithMostPoints` that takes a `std::vector<std::pair<double, double>>` of Cartesian points (with at least 2 distinct points, and no two points sharing the same x-coordinate, to avoid vertical-line division-by-zero issues) and returns a `std::pair<Line, int>` where `Line` is a simple struct with `double slope` and `double yintercept`, and the `int` is the maximum number of points that lie exactly on that line (within an epsilon of `1e-5` for slope comparisons). If multiple lines tie for the maximum, any valid line and its count is acceptable. The function must not modify the input vector and must work for any number of points up to 10 (for practical performance, since enumeration is used), but must handle the case where the maximum is 2 (i.e., no three points are collinear) by returning the line through the first two points.

// The core idea is to enumerate all subsets of points of size `k` starting from the largest possible `k` down to 3, and check if the points in that subset are collinear. To enumerate subsets, we use a recursive helper that builds a bitmask-like binary vector of length `n` indicating which points are selected, and when exactly `k` are selected, we store the indices in a result vector. For each subset of size `k`, we take the first two points, compute the slope (since all x-coordinates are distinct, no division by zero), then check that every other point in the subset has the same slope (within epsilon). If all match, we return the line and `k`. We iterate `k` from `n` down to 3, so the first successful `k` gives the maximum. If no subset of size ≥3 is collinear, we fall back to returning the line through the first two points with count 2. Complexity: The number of subsets is `2^n`, and for each subset of size `k` we do O(k) work, so worst-case time is O(n * 2^n), which is fine for n ≤ 10. Space complexity is O(n) for recursion depth and temporary storage. Edge cases: The problem statement guarantees at least 2 points and distinct x-coordinates; we also handle the case where all points are collinear (then largest k = n) and the case where only pairs are collinear (fallback to 2). Floating-point comparisons use absolute difference < 1e-5.

#include <vector>
#include <utility>
#include <cmath>
#include <cstddef>

struct Line {
    double slope;
    double yintercept;
    Line(double s, double y) : slope(s), yintercept(y) {}
};

// Helper: recursively enumerate all subsets of size k from indices 0..n-1
static void enumerateSubsets(int n, int k, int idx, std::vector<int>& current,
                             std::vector<std::vector<int>>& results) {
    if (static_cast<int>(current.size()) == k) {
        results.push_back(current);
        return;
    }
    if (idx == n) return;
    // Include idx
    current.push_back(idx);
    enumerateSubsets(n, k, idx + 1, current, results);
    // Exclude idx
    current.pop_back();
    enumerateSubsets(n, k, idx + 1, current, results);
}

// Main function: find the line that crosses the most points
std::pair<Line, int> findLineWithMostPoints(const std::vector<std::pair<double, double>>& points) {
    const int n = static_cast<int>(points.size());
    const double EPS = 1e-5;

    // Try all possible subset sizes from largest to smallest (down to 3)
    for (int k = n; k >= 3; --k) {
        std::vector<std::vector<int>> subsets;
        std::vector<int> current;
        enumerateSubsets(n, k, 0, current, subsets);

        for (const auto& subset : subsets) {
            // Use first two points to define the line
            const auto& a = points[subset[0]];
            const auto& b = points[subset[1]];
            double slope = (b.second - a.second) / (b.first - a.first);
            bool collinear = true;
            for (int i = 2; i < k; ++i) {
                const auto& c = points[subset[i]];
                double slope2 = (c.second - a.second) / (c.first - a.first);
                if (std::abs(slope - slope2) > EPS) {
                    collinear = false;
                    break;
                }
            }
            if (collinear) {
                double yintercept = a.second - slope * a.first;
                return std::make_pair(Line(slope, yintercept), k);
            }
        }
    }

    // Fallback: return line through the first two points (maximum is 2)
    const auto& a = points[0];
    const auto& b = points[1];
    double slope = (b.second - a.second) / (b.first - a.first);
    double yintercept = a.second - slope * a.first;
    return std::make_pair(Line(slope, yintercept), 2);
}

#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// Assume the Line struct and findLineWithMostPoints are defined above (or included)
// Placeholder: include the solution code here or link it.

int main() {
    const double EPS = 1e-5;

    // Test 1: All points collinear on y = x
    std::vector<std::pair<double, double>> p1 = {{0,0},{1,1},{2,2},{3,3}};
    auto res1 = findLineWithMostPoints(p1);
    assert(res1.second == 4);
    assert(std::abs(res1.first.slope - 1.0) < EPS);
    assert(std::abs(res1.first.yintercept - 0.0) < EPS);

    // Test 2: Three points on y = 2x+1, others off
    std::vector<std::pair<double, double>> p2 = {{0,1},{1,3},{2,5},{0,0},{1,1}};
    auto res2 = findLineWithMostPoints(p2);
    assert(res2.second == 3);
    assert(std::abs(res2.first.slope - 2.0) < EPS);
    assert(std::abs(res2.first.yintercept - 1.0) < EPS);

    // Test 3: No three collinear (max is 2)
    std::vector<std::pair<double, double>> p3 = {{0,0},{1,1},{1,3},{2,0}};
    auto res3 = findLineWithMostPoints(p3);
    assert(res3.second == 2);

    // Test 4: Four points on a horizontal line y=2
    std::vector<std::pair<double, double>> p4 = {{-1,2},{0,2},{1,2},{5,2}};
    auto res4 = findLineWithMostPoints(p4);
    assert(res4.second == 4);
    assert(std::abs(res4.first.slope - 0.0) < EPS);
    assert(std::abs(res4.first.yintercept - 2.0) < EPS);

    // Test 5: Two points only
    std::vector<std::pair<double, double>> p5 = {{2,3},{4,7}};
    auto res5 = findLineWithMostPoints(p5);
    assert(res5.second == 2);
    assert(std::abs(res5.first.slope - 2.0) < EPS);
    assert(std::abs(res5.first.yintercept - (-1.0)) < EPS);

    return 0;
}
