// Write a C++ function named `kClosestPoints` that takes a vector of 2D points (each point represented as a vector of two integers) and an integer `K`, and returns a vector containing the `K` points closest to the origin `(0,0)`, where distance is measured by squared Euclidean distance (i.e., \(x^2 + y^2\)). The returned order does not matter, but the selection must be correct: if multiple points have equal distance, any subset of size `K` is acceptable. The input vector will contain at least `K` points, and `K > 0`. The function should be efficient, using a max-heap of size `K` to keep track of the smallest distances seen so far.

// The problem asks to select the `K` smallest squared distances from the origin among all points. A naive approach would sort all points by distance and take the first `K`, which is `O(n log n)`. To achieve better performance when `K` is significantly smaller than `n`, use a max-heap (priority queue) of size `K`. The algorithm is:
// 1. Initialize a max-heap that stores pairs of `(squared_distance, index)`.
// 2. Push the first `K` points into the heap.
// 3. For each remaining point, compute its squared distance. If that distance is smaller than the largest distance in the heap (the top element), replace the top by popping it and pushing the new point.
// 4. After processing all points, extract all elements from the heap (which are the indices of the `K` smallest distances) and build the result vector by indexing into the original point list.
// Edge cases: `K` equal to the total number of points (all points included); duplicate distances (any valid subset works). Time complexity is `O(n log K)` because each heap operation is `O(log K)` and we do `n` push/pop operations in the worst case. Space complexity is `O(K)` for the heap plus `O(K)` for the result vector.

#include <vector>
#include <queue>
#include <utility>

// Return the K points closest to the origin (0,0) based on squared distance.
std::vector<std::vector<int>> kClosestPoints(const std::vector<std::vector<int>>& points, int K) {
    // Max-heap of pairs {squared_distance, index}
    std::priority_queue<std::pair<int, int>> maxHeap;

    // Insert first K points into the heap
    for (int i = 0; i < K; ++i) {
        int dist = points[i][0] * points[i][0] + points[i][1] * points[i][1];
        maxHeap.push({dist, i});
    }

    // Process remaining points
    for (int i = K; i < (int)points.size(); ++i) {
        int dist = points[i][0] * points[i][0] + points[i][1] * points[i][1];
        if (dist < maxHeap.top().first) {
            maxHeap.pop();
            maxHeap.push({dist, i});
        }
    }

    // Extract the K closest points
    std::vector<std::vector<int>> result;
    result.reserve(K);
    while (!maxHeap.empty()) {
        result.push_back(points[maxHeap.top().second]);
        maxHeap.pop();
    }
    return result;
}

#include <cassert>
#include <vector>
#include <algorithm>

// Helper to check if two vectors of points contain the same multiset of points (ignoring order).
bool samePoints(const std::vector<std::vector<int>>& a, const std::vector<std::vector<int>>& b) {
    if (a.size() != b.size()) return false;
    std::vector<std::vector<int>> a_sorted = a;
    std::vector<std::vector<int>> b_sorted = b;
    std::sort(a_sorted.begin(), a_sorted.end());
    std::sort(b_sorted.begin(), b_sorted.end());
    return a_sorted == b_sorted;
}

int main() {
    // Basic test: 3 points, K=2
    std::vector<std::vector<int>> points1 = {{1,1}, {2,2}, {0,1}}; // distances: 2,8,1
    auto result1 = kClosestPoints(points1, 2);
    assert(samePoints(result1, {{1,1}, {0,1}}));

    // All points included when K equals size
    std::vector<std::vector<int>> points2 = {{-1,0}, {0,1}, {2,0}};
    auto result2 = kClosestPoints(points2, 3);
    assert(samePoints(result2, {{-1,0}, {0,1}, {2,0}}));

    // K=1 with negative coordinates
    std::vector<std::vector<int>> points3 = {{-3,4}, {5,-12}, {1,1}}; // distances: 25,169,2
    auto result3 = kClosestPoints(points3, 1);
    assert(samePoints(result3, {{1,1}}));

    // Duplicate distances: choose any valid subset of size 2 from points all at distance 1
    std::vector<std::vector<int>> points4 = {{1,0}, {0,1}, {-1,0}, {0,-1}};
    auto result4 = kClosestPoints(points4, 2);
    assert(result4.size() == 2);
    for (const auto& p : result4) {
        assert(p[0]*p[0] + p[1]*p[1] == 1);
    }

    // K equals 1 from a larger set
    std::vector<std::vector<int>> points5 = {{10,10}, {0,1}, {2,0}};
    auto result5 = kClosestPoints(points5, 1);
    assert(samePoints(result5, {{0,1}}));

    return 0;
}
