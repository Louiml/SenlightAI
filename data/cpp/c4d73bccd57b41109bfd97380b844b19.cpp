// Write a C++ function that, given a vector of 2D line segments (each represented by four doubles: x1, y1, x2, y2), finds all pairs of segments that are "nearly parallel" (i.e., the absolute difference in their slopes is less than a specified threshold `thetaCut`) AND whose perpendicular distance between their midpoints is less than a specified `distanceCut`. The function should return a vector of pairs of indices (i, j) with i < j for each such matching pair. Additionally, if two segments have the same slope difference and both satisfy the distance criterion, only the pair with the smallest midpoint distance should be retained for that combination of segments. If no pairs satisfy the criteria, return an empty vector. Handle edge cases: segments with infinite slope (vertical lines) should use the angle (π/2) for comparison, and duplicate or overlapping segments should be treated as valid matches if they meet the criteria. The function must be `const`-correct and take the threshold values as parameters with default values `thetaCut = 0.01` (radians) and `distanceCut = 1.0` (units).

// The solution sorts segments by their angle (computed as `atan2(dy, dx)` for the direction vector, which handles vertical lines naturally since `atan2` returns π/2 for x=0). After sorting, we only need to compare adjacent segments in sorted order because any two segments with angle difference less than `thetaCut` must be within one position of each other in the sorted array (since solutions with intermediate angles would be even closer). For each adjacent pair, compute the midpoint distance: find the perpendicular distance between the lines passing through each segment's midpoint. The perpendicular distance between two lines with direction vectors (dx1, dy1) and (dx2, dy2) can be computed using the formula: the distance between point P1 on line 1 and line 2 is `abs((P2 - P1) · n)` where `n` is the unit normal to line 2. However, since the segments are nearly parallel, we can approximate by using the Euclidean distance between the midpoints directly. To handle the "smallest distance" requirement, for each segment we track the best matching segment found so far (with smallest distance) among all pairs with same angle difference. This is done by iterating through sorted segments and for each pair, if it satisfies the angle criterion, compute distance; if distance < current best for either segment, update the pair. Finally, collect all unique pairs. Time complexity: O(n log n) for sorting (n = number of segments), plus O(n) for adjacent comparisons, giving O(n log n) overall. Space complexity: O(n) for the sorted vector and output storage.

#include <vector>
#include <cmath>
#include <algorithm>
#include <utility>

struct Segment {
    double x1, y1, x2, y2;
    double angle;
    double midX, midY;
};

// Return indices of all pairs of nearly parallel segments within distanceCut
std::vector<std::pair<size_t, size_t>> findParallelPairs(
    const std::vector<std::pair<double, double>>& p1,
    const std::vector<std::pair<double, double>>& p2,
    double thetaCut = 0.01,
    double distanceCut = 1.0) {
    
    size_t n = p1.size();
    std::vector<Segment> segs(n);
    for (size_t i = 0; i < n; ++i) {
        segs[i].x1 = p1[i].first;
        segs[i].y1 = p1[i].second;
        segs[i].x2 = p2[i].first;
        segs[i].y2 = p2[i].second;
        segs[i].angle = std::atan2(segs[i].y2 - segs[i].y1, segs[i].x2 - segs[i].x1);
        segs[i].midX = (segs[i].x1 + segs[i].x2) / 2.0;
        segs[i].midY = (segs[i].y1 + segs[i].y2) / 2.0;
    }
    
    // Sort by angle
    std::vector<size_t> idx(n);
    for (size_t i = 0; i < n; ++i) idx[i] = i;
    std::sort(idx.begin(), idx.end(), [&](size_t a, size_t b) {
        return segs[a].angle < segs[b].angle;
    });
    
    // For each segment, track best matching partner and distance
    std::vector<size_t> bestPartner(n, n); // n = sentinel for none
    std::vector<double> bestDist(n, distanceCut + 1.0); // sentinel
    
    // Compare only adjacent in sorted order
    for (size_t k = 0; k + 1 < n; ++k) {
        size_t i = idx[k];
        size_t j = idx[k+1];
        double angleDiff = std::fabs(segs[i].angle - segs[j].angle);
        // Handle wrap-around for angles near ±π
        if (angleDiff > M_PI) angleDiff = 2.0 * M_PI - angleDiff;
        if (angleDiff < thetaCut) {
            double dx = segs[i].midX - segs[j].midX;
            double dy = segs[i].midY - segs[j].midY;
            double dist = std::sqrt(dx*dx + dy*dy);
            if (dist < distanceCut) {
                if (dist < bestDist[i]) {
                    bestDist[i] = dist;
                    bestPartner[i] = j;
                }
                if (dist < bestDist[j]) {
                    bestDist[j] = dist;
                    bestPartner[j] = i;
                }
            }
        }
    }
    
    // Collect unique pairs (i, j) with i < j
    std::vector<std::pair<size_t, size_t>> result;
    for (size_t i = 0; i < n; ++i) {
        size_t j = bestPartner[i];
        if (j != n && i < j) {
            result.emplace_back(i, j);
        }
    }
    // Sort for deterministic output
    std::sort(result.begin(), result.end());
    return result;
}

#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Test 1: Two parallel horizontal segments, close enough
    std::vector<std::pair<double, double>> p1a = {{0.0, 0.0}, {2.0, 0.0}};
    std::vector<std::pair<double, double>> p2a = {{1.0, 0.0}, {3.0, 0.0}};
    auto result1 = findParallelPairs(p1a, p2a, 0.01, 1.0);
    assert(result1.size() == 1 && result1[0] == std::make_pair(0, 1));

    // Test 2: Nearly parallel but distance too large
    std::vector<std::pair<double, double>> p1b = {{0.0, 0.0}, {10.0, 0.0}};
    std::vector<std::pair<double, double>> p2b = {{0.0, 5.0}, {10.0, 5.0}};
    auto result2 = findParallelPairs(p1b, p2b, 0.01, 1.0);
    assert(result2.empty());

    // Test 3: Vertical lines (infinite slope)
    std::vector<std::pair<double, double>> p1c = {{0.0, 0.0}, {0.0, 5.0}};
    std::vector<std::pair<double, double>> p2c = {{0.1, 0.0}, {0.1, 5.0}};
    auto result3 = findParallelPairs(p1c, p2c, 0.01, 1.0);
    assert(result3.size() == 1 && result3[0] == std::make_pair(0, 1));

    // Test 4: Three segments, only one matching pair
    std::vector<std::pair<double, double>> p1d = {{0.0, 0.0}, {1.0, 0.0}, {0.0, 0.0}};
    std::vector<std::pair<double, double>> p2d = {{1.0, 0.0}, {2.0, 0.1}, {1.0, 0.0}};
    auto result4 = findParallelPairs(p1d, p2d, 0.01, 1.0);
    assert(result4.size() == 1 && result4[0] == std::make_pair(0, 2));

    // Test 5: Empty input
    std::vector<std::pair<double, double>> empty1, empty2;
    auto result5 = findParallelPairs(empty1, empty2);
    assert(result5.empty());

    // Test 6: Single segment
    std::vector<std::pair<double, double>> p1f = {{0.0, 0.0}};
    std::vector<std::pair<double, double>> p2f = {{1.0, 1.0}};
    auto result6 = findParallelPairs(p1f, p2f);
    assert(result6.empty());

    return 0;
}
