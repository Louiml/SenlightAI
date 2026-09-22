/*
Write a standalone C++ function that implements a simplified version of the touch-point sampling logic from the provided code snippet. Given a sequence of 2D input points (x, y) and a threshold distance, the function should downsample the points by removing intermediate points that are too close to their neighbors (within a distance threshold) while always keeping the first and last points. Specifically, iterate through the points; if the distance between the current point and the last kept point is less than the given threshold, skip the current point; otherwise, keep it. The function should return a vector of pairs (or a struct vector) containing the filtered points in original order. Handle edge cases: empty input (return empty vector), single point (return that point), and points exactly at the threshold distance (keep them, since the condition is strict less than). Use Euclidean distance for measuring distances. The function signature should be: `std::vector<std::pair<int,int>> filterClosePoints(const std::vector<std::pair<int,int>>& points, int threshold)`. Ensure proper `const` correctness and include necessary headers.
*/

#include <vector>
#include <utility>
#include <cstddef>

// Filter points that are too close to the previous kept point.
// Keeps the first and last points always.
std::vector<std::pair<int,int>> filterClosePoints(const std::vector<std::pair<int,int>>& points, int threshold) {
    if (points.empty()) return {};

    std::vector<std::pair<int,int>> result;
    result.reserve(points.size()); // reserve to avoid reallocations

    // Always keep the first point
    result.push_back(points.front());

    // Convert threshold to squared to avoid floating point and sqrt
    const long long thresholdSq = static_cast<long long>(threshold) * threshold;

    // Iterate from the second point
    for (size_t i = 1; i < points.size(); ++i) {
        const auto& p = points[i];
        const auto& last = result.back();

        long long dx = static_cast<long long>(p.first) - last.first;
        long long dy = static_cast<long long>(p.second) - last.second;
        long long distSq = dx*dx + dy*dy;

        // Keep if distance is >= threshold (strictly not less than)
        if (distSq >= thresholdSq) {
            result.push_back(p);
        }
    }

    // Ensure the last point is always kept if it was skipped and differs
    // from the last kept point.
    if (points.size() > 1) {
        const auto& lastInput = points.back();
        const auto& lastKept = result.back();
        if (lastInput.first != lastKept.first || lastInput.second != lastKept.second) {
            result.push_back(lastInput);
        }
    }

    return result;
}

#include <cassert>
#include <vector>
#include <utility>

// Function declaration (since we don't include the solution file)
std::vector<std::pair<int,int>> filterClosePoints(const std::vector<std::pair<int,int>>& points, int threshold);

int main() {
    // Empty input
    assert(filterClosePoints({}, 5).empty());

    // Single point
    std::vector<std::pair<int,int>> single = {{1,2}};
    assert(filterClosePoints(single, 10) == single);

    // All points far apart -> keep all
    std::vector<std::pair<int,int>> far = {{0,0}, {5,5}, {10,10}};
    assert(filterClosePoints(far, 3) == far);

    // Close points are filtered, but first and last kept
    std::vector<std::pair<int,int>> close = {{0,0}, {1,1}, {2,2}, {10,10}};
    auto filtered = filterClosePoints(close, 3);
    assert(filtered.size() == 3);
    assert(filtered[0] == std::make_pair(0,0));
    assert(filtered[1] == std::make_pair(2,2)); // (1,1) skipped
    assert(filtered[2] == std::make_pair(10,10));

    // Threshold exactly equal to distance -> keep (strict less than)
    std::vector<std::pair<int,int>> exact = {{0,0}, {3,4}}; // distance 5
    auto filteredExact = filterClosePoints(exact, 5);
    assert(filteredExact.size() == 2);

    // Threshold larger than all distances -> only first and last
    std::vector<std::pair<int,int>> allClose = {{0,0}, {1,0}, {2,0}, {3,0}};
    auto filteredAll = filterClosePoints(allClose, 10);
    assert(filteredAll.size() == 2);
    assert(filteredAll[0] == std::make_pair(0,0));
    assert(filteredAll[1] == std::make_pair(3,0));

    // Negative coordinates
    std::vector<std::pair<int,int>> neg = {{-10,0}, {-9,0}, {-8,0}, {-5,-5}};
    auto filteredNeg = filterClosePoints(neg, 2);
    assert(filteredNeg.size() == 3);
    assert(filteredNeg[0] == std::make_pair(-10,0));
    assert(filteredNeg[1] == std::make_pair(-8,0)); // -9 skipped
    assert(filteredNeg[2] == std::make_pair(-5,-5));

    // Threshold zero: all points kept
    std::vector<std::pair<int,int>> zeroThresh = {{0,0}, {1,1}, {2,2}};
    assert(filterClosePoints(zeroThresh, 0) == zeroThresh);

    // Same point repeated, threshold > 0 keeping only first and last identical? Last is same as first; should still keep only one? Actually last is same as first, so no duplicate added.
    std::vector<std::pair<int,int>> repeated = {{5,5}, {5,5}, {5,5}};
    auto filteredRepeat = filterClosePoints(repeated, 1);
    // First kept, then second skipped (distance 0 < 1), third skipped, but last is same as first, so no extra push
    assert(filteredRepeat.size() == 1);
    assert(filteredRepeat[0] == std::make_pair(5,5));

    return 0;
}

// The solution iterates through the input vector of points once, maintaining a result vector. The first point is always added. For each subsequent point, compute the squared Euclidean distance between it and the last point in the result vector. Since we only need to compare against a threshold, avoid floating-point precision issues by comparing squared distances: if `(dx*dx + dy*dy) < threshold*threshold`, skip the point; otherwise add it. The last point is automatically kept because the loop processes all points, and if it is too close to the previous kept point, it would be skipped—but we need to ensure the last point is always included. Therefore, after the main loop, if the last point was skipped, we explicitly add it to the result. Alternatively, track the last point and always push it at the end if it differs from the last kept point. Complexity: O(n) time where n is the number of input points, O(n) worst-case space for the result (which is the output). Edge cases: empty input returns empty; single point returns that point; threshold zero means no point is skipped (since distance >= 0, and only skip if distance < 0, which never happens, so all points kept); threshold large may skip many points, but first and last are always kept.
