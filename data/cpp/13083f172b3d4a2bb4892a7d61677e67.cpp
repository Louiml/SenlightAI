// Write a standalone C++ function named `findKClosestPoints` that takes a vector of 2D integer points (`std::vector<std::vector<int>>`) and an integer `k`, and returns a new vector containing the `k` points closest to the origin (0, 0) using Euclidean distance. The order of the returned points can be arbitrary, and the input is guaranteed to have unique distances (no ties). The function should handle edge cases such as `k` equal to 0 (returning an empty result) and must be efficient for up to 104 points. Do not use a class; provide a free function with appropriate `const` correctness.

// The optimal approach uses a max-heap (priority queue) of size `k` to track the smallest distances. For each point, we compute its squared Euclidean distance (avoiding costly square roots) and push it into the heap. If the heap exceeds size `k`, we pop the largest distance, ensuring the heap only contains the `k` closest points seen so far. After processing all points, we extract the coordinates from the heap into a result vector (order does not matter). This approach handles `k = 0` by returning an empty vector immediately. Edge cases include duplicate distances (though guaranteed unique), and when `k` equals the number of points (all points returned). Time complexity is O(n log k) where n is the number of points, and space complexity is O(k) for the heap plus O(k) for the output (excluding input).

#include <vector>
#include <queue>

// Return the k points closest to the origin using squared Euclidean distance.
std::vector<std::vector<int>> findKClosestPoints(const std::vector<std::vector<int>>& points, int k) {
    std::vector<std::vector<int>> result;
    if (k == 0) return result;

    // Max-heap storing pairs of (distance, point indices) to keep the smallest k distances.
    auto compare = [](const std::pair<long long, int>& a, const std::pair<long long, int>& b) {
        return a.first < b.first; // max-heap by distance
    };
    std::priority_queue<std::pair<long long, int>, std::vector<std::pair<long long, int>>, decltype(compare)> heap(compare);

    for (int i = 0; i < static_cast<int>(points.size()); ++i) {
        long long dist = static_cast<long long>(points[i][0]) * points[i][0] +
                         static_cast<long long>(points[i][1]) * points[i][1];
        heap.emplace(dist, i);

        if (static_cast<int>(heap.size()) > k) {
            heap.pop();
        }
    }

    result.reserve(heap.size());
    while (!heap.empty()) {
        int idx = heap.top().second;
        result.push_back(points[idx]);
        heap.pop();
    }

    return result;
}

#include <cassert>
#include <vector>

// Helper to check if two results contain the same points regardless of order.
bool sameMultiset(const std::vector<std::vector<int>>& a, const std::vector<std::vector<int>>& b) {
    if (a.size() != b.size()) return false;
    for (const auto& pa : a) {
        bool found = false;
        for (const auto& pb : b) {
            if (pa == pb) { found = true; break; }
        }
        if (!found) return false;
    }
    return true;
}

int main() {
    // Example 1
    std::vector<std::vector<int>> points1 = {{1,3}, {-2,2}};
    auto res1 = findKClosestPoints(points1, 1);
    assert(sameMultiset(res1, {{-2,2}}));

    // Example 2
    std::vector<std::vector<int>> points2 = {{3,3}, {5,-1}, {-2,4}};
    auto res2 = findKClosestPoints(points2, 2);
    assert(sameMultiset(res2, {{3,3}, {-2,4}}));

    // k = 0 returns empty
    assert(findKClosestPoints(points1, 0).empty());

    // k equals number of points
    auto res3 = findKClosestPoints(points2, 3);
    assert(sameMultiset(res3, {{3,3}, {5,-1}, {-2,4}}));

    // Single point
    std::vector<std::vector<int>> points4 = {{0,0}};
    auto res4 = findKClosestPoints(points4, 1);
    assert(sameMultiset(res4, {{0,0}}));

    // Negative and positive coordinates
    std::vector<std::vector<int>> points5 = {{-1,-1}, {1,1}, {0,2}};
    auto res5 = findKClosestPoints(points5, 2);
    assert(sameMultiset(res5, {{-1,-1}, {1,1}}));

    // Larger test with 4 points
    std::vector<std::vector<int>> points6 = {{1,0}, {0,1}, {2,0}, {0,2}};
    auto res6 = findKClosestPoints(points6, 2);
    assert(sameMultiset(res6, {{1,0}, {0,1}}));

    return 0;
}
