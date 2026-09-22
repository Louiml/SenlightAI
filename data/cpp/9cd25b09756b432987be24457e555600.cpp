/*
Write a C++ function `selectFeatures` that takes a vector of 2D points (`std::vector<std::pair<double,double>>`), a minimum distance threshold (`double minDist`), and an image dimension pair (`int imgWidth, int imgHeight`). The function should simulate a feature selection strategy inspired by the `setMask` method: it must prioritize points that appear earlier in the input (simulating higher tracking counts), and greedily select points such that no two selected points are within `minDist` Euclidean distance of each other, while also discarding any point whose coordinates are outside the image borders (using border size 1, meaning valid x in `[1, imgWidth-2]` and y in `[1, imgHeight-2]`). The function returns a new vector containing the selected points in the order they were prioritized (i.e., the order of the input after filtering). Points are processed in the given order; when a point is selected, it prevents any later point within `minDist` from being selected. The output must preserve the relative order of the input after filtering.
*/
#include <vector>
#include <cmath>
#include <utility>

// Determine if a point is inside the image border (with border size 1)
bool isInBorder(double x, double y, int imgWidth, int imgHeight) {
    const int BORDER = 1;
    int ix = static_cast<int>(std::round(x));
    int iy = static_cast<int>(std::round(y));
    return BORDER <= ix && ix < imgWidth - BORDER && BORDER <= iy && iy < imgHeight - BORDER;
}

// Compute squared Euclidean distance between two points (avoids sqrt for efficiency)
double squaredDistance(const std::pair<double, double>& a, const std::pair<double, double>& b) {
    double dx = a.first - b.first;
    double dy = a.second - b.second;
    return dx * dx + dy * dy;
}

// Select features: prioritize earlier points, enforce minimum distance, respect borders
std::vector<std::pair<double, double>> selectFeatures(
    const std::vector<std::pair<double, double>>& points,
    double minDist,
    int imgWidth,
    int imgHeight)
{
    std::vector<std::pair<double, double>> selected;

    // First filter points that are outside the border
    std::vector<std::pair<double, double>> validPoints;
    for (const auto& pt : points) {
        if (isInBorder(pt.first, pt.second, imgWidth, imgHeight)) {
            validPoints.push_back(pt);
        }
    }

    double minDistSq = minDist * minDist;

    // Greedy selection: process in input order (priority)
    for (const auto& pt : validPoints) {
        bool tooClose = false;
        for (const auto& accepted : selected) {
            if (squaredDistance(pt, accepted) < minDistSq) {
                tooClose = true;
                break;
            }
        }
        if (!tooClose) {
            selected.push_back(pt);
        }
    }

    return selected;
}
#include <cassert>
#include <vector>
#include <utility>

// The solution function is assumed to be visible here (include or paste above)

int main() {
    // Test 1: Simple case with two far points
    std::vector<std::pair<double, double>> pts1 = {{10.0, 10.0}, {20.0, 20.0}};
    auto result1 = selectFeatures(pts1, 5.0, 50, 50);
    assert(result1.size() == 2);
    assert(result1[0] == std::make_pair(10.0, 10.0));
    assert(result1[1] == std::make_pair(20.0, 20.0));

    // Test 2: Two points too close, only first selected
    std::vector<std::pair<double, double>> pts2 = {{10.0, 10.0}, {12.0, 10.0}};
    auto result2 = selectFeatures(pts2, 3.0, 50, 50);
    assert(result2.size() == 1);
    assert(result2[0] == std::make_pair(10.0, 10.0));

    // Test 3: Border filtering, point outside is removed
    std::vector<std::pair<double, double>> pts3 = {{0.0, 5.0}, {5.0, 5.0}, {10.0, 5.0}};
    auto result3 = selectFeatures(pts3, 1.0, 10, 10);
    // Valid range: x in [1, 8], y in [1, 8]; (0,5) invalid, others valid and far apart
    assert(result3.size() == 2);
    assert(result3[0] == std::make_pair(5.0, 5.0));
    assert(result3[1] == std::make_pair(10.0, 5.0));

    // Test 4: Empty input
    std::vector<std::pair<double, double>> pts4;
    auto result4 = selectFeatures(pts4, 2.0, 100, 100);
    assert(result4.empty());

    // Test 5: minDist zero, all valid points selected
    std::vector<std::pair<double, double>> pts5 = {{5.0, 5.0}, {5.5, 5.5}, {6.0, 6.0}};
    auto result5 = selectFeatures(pts5, 0.0, 20, 20);
    assert(result5.size() == 3);

    // Test 6: Non-integer coordinates, border check uses rounding
    std::vector<std::pair<double, double>> pts6 = {{0.4, 5.0}, {1.5, 5.0}};
    auto result6 = selectFeatures(pts6, 1.0, 10, 10);
    // 0.4 rounds to 0, invalid; 1.5 rounds to 2, valid
    assert(result6.size() == 1);
    assert(result6[0] == std::make_pair(1.5, 5.0));

    // Test 7: Order preservation when skipping close later ones
    std::vector<std::pair<double, double>> pts7 = {{5.0, 5.0}, {10.0, 10.0}, {6.0, 5.0}};
    auto result7 = selectFeatures(pts7, 3.0, 100, 100);
    // (5,5) selected, (10,10) selected (distance ~7.07), (6,5) too close to (5,5) so rejected
    assert(result7.size() == 2);
    assert(result7[0] == std::make_pair(5.0, 5.0));
    assert(result7[1] == std::make_pair(10.0, 10.0));

    return 0;
}
// The solution follows a greedy approach similar to the `setMask` method. First, we filter out points that lie outside the border (coordinates not in `[1, imgWidth-2]` and `[1, imgHeight-2]`). Then we iterate through the filtered points in their original order (which represents priority). We maintain a list of accepted points. For each candidate point, we check if it is at least `minDist` away from every already-accepted point using Euclidean distance. If it is, we accept it and add it to the result; otherwise, we skip it. Because we process in input order, earlier points are always preferred over later ones, matching the "tracked for long time" preference. Edge cases: if `minDist` is zero or negative, all valid points are accepted; if the input is empty, the result is empty; points exactly at distance equal to `minDist` are considered too close (we require `> minDist` to accept), so we use `< minDist` to reject. Time complexity is O(n * m) in the worst case, where n is the number of valid input points and m is the number of accepted points (m ≤ n), so O(n²) worst-case. Space complexity is O(n) for the output vector.
