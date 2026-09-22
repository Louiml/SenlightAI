/*
Write a C++ function named `slidingWindowMinMax` that takes a vector of integers `data`, a window size `k`, and returns a `std::pair<std::vector<int>, std::vector<int>>` where the first vector contains the minimum value of each sliding window of size `k` (in order of window start position from 0 to `data.size()-k`) and the second vector contains the maximum value for each corresponding window. If `k` is larger than the size of `data`, the function should treat the entire array as a single window and return one element for min and one for max. The function must handle empty input gracefully by returning two empty vectors. You may assume `k` is positive. The implementation must use a segment tree (as in the given snippet) for range min/max queries, not a deque or other optimization.
*/
#include <vector>
#include <algorithm>
#include <utility>

// Segment tree node storing min and max of its segment.
struct Node {
    int minVal;
    int maxVal;
};

// Build the segment tree recursively.
// tree is a vector of Node, indexed by 1-based segment tree index.
// data is the original array (1-indexed for convenience).
void buildTree(int idx, int left, int right, const std::vector<int>& data, std::vector<Node>& tree) {
    if (left == right) {
        tree[idx].minVal = data[left];
        tree[idx].maxVal = data[left];
        return;
    }
    int mid = (left + right) / 2;
    buildTree(idx * 2, left, mid, data, tree);
    buildTree(idx * 2 + 1, mid + 1, right, data, tree);
    tree[idx].minVal = std::min(tree[idx*2].minVal, tree[idx*2+1].minVal);
    tree[idx].maxVal = std::max(tree[idx*2].maxVal, tree[idx*2+1].maxVal);
}

// Query range [ql, qr] and update the current min/max in qMin and qMax.
void queryRange(int idx, int left, int right, int ql, int qr, const std::vector<Node>& tree, int& qMin, int& qMax) {
    if (ql <= left && right <= qr) {
        qMin = std::min(qMin, tree[idx].minVal);
        qMax = std::max(qMax, tree[idx].maxVal);
        return;
    }
    int mid = (left + right) / 2;
    if (qr <= mid) {
        queryRange(idx * 2, left, mid, ql, qr, tree, qMin, qMax);
    } else if (ql > mid) {
        queryRange(idx * 2 + 1, mid + 1, right, ql, qr, tree, qMin, qMax);
    } else {
        queryRange(idx * 2, left, mid, ql, mid, tree, qMin, qMax);
        queryRange(idx * 2 + 1, mid + 1, right, mid + 1, qr, tree, qMin, qMax);
    }
}

// Main function: returns a pair of vectors (minima, maxima) for each sliding window.
std::pair<std::vector<int>, std::vector<int>> slidingWindowMinMax(const std::vector<int>& data, int k) {
    int n = static_cast<int>(data.size());
    if (n == 0 || k <= 0) {
        return {{}, {}};
    }
    // If k is larger than n, use the whole array as one window.
    if (k > n) k = n;
    
    // Build 1-indexed data array for easier indexing.
    std::vector<int> arr(n + 1);
    for (int i = 0; i < n; ++i) arr[i+1] = data[i];
    
    // Tree size: 4 * n is safe for recursive segment trees.
    std::vector<Node> tree(4 * n + 1);
    buildTree(1, 1, n, arr, tree);
    
    int numWindows = n - k + 1;
    std::vector<int> mins(numWindows);
    std::vector<int> maxs(numWindows);
    
    for (int start = 0; start < numWindows; ++start) {
        int left = start + 1;
        int right = start + k;
        int qMin = 2000000000; // large initial value
        int qMax = -2000000000; // small initial value
        queryRange(1, 1, n, left, right, tree, qMin, qMax);
        mins[start] = qMin;
        maxs[start] = qMax;
    }
    
    return {mins, maxs};
}
#include <cassert>
#include <vector>
#include <utility>

// Function declaration – assume the solution code above is included before this main.
std::pair<std::vector<int>, std::vector<int>> slidingWindowMinMax(const std::vector<int>& data, int k);

int main() {
    // Basic test with k=3
    {
        std::vector<int> data = {1, 3, -1, -3, 5, 3, 6, 7};
        auto result = slidingWindowMinMax(data, 3);
        std::vector<int> expectedMin = {-1, -3, -3, -3, 3, 3};
        std::vector<int> expectedMax = {3, 3, 5, 5, 6, 7};
        assert(result.first == expectedMin);
        assert(result.second == expectedMax);
    }
    // k=1: each element is its own window
    {
        std::vector<int> data = {5, -1, 9};
        auto result = slidingWindowMinMax(data, 1);
        assert(result.first == data);
        assert(result.second == data);
    }
    // k > n: whole array as one window
    {
        std::vector<int> data = {2, 8, -4};
        auto result = slidingWindowMinMax(data, 10);
        assert(result.first == std::vector<int>{-4});
        assert(result.second == std::vector<int>{8});
    }
    // All equal values
    {
        std::vector<int> data = {7, 7, 7, 7};
        auto result = slidingWindowMinMax(data, 2);
        assert(result.first == std::vector<int>(3, 7));
        assert(result.second == std::vector<int>(3, 7));
    }
    // Empty input
    {
        std::vector<int> data;
        auto result = slidingWindowMinMax(data, 3);
        assert(result.first.empty());
        assert(result.second.empty());
    }
    // Single element
    {
        std::vector<int> data = {42};
        auto result = slidingWindowMinMax(data, 1);
        assert(result.first == std::vector<int>{42});
        assert(result.second == std::vector<int>{42});
    }
    // k=2 with negative numbers
    {
        std::vector<int> data = {-10, -20, -30, 0, 10};
        auto result = slidingWindowMinMax(data, 2);
        std::vector<int> expectedMin = {-20, -30, -30, 0};
        std::vector<int> expectedMax = {-10, -20, 0, 10};
        assert(result.first == expectedMin);
        assert(result.second == expectedMax);
    }
    // k=5 for n=5 (full window)
    {
        std::vector<int> data = {3, 1, 4, 1, 5};
        auto result = slidingWindowMinMax(data, 5);
        assert(result.first == std::vector<int>{1});
        assert(result.second == std::vector<int>{5});
    }
    // k=4 for n=5 (two windows)
    {
        std::vector<int> data = {1, 2, 3, 4, 5};
        auto result = slidingWindowMinMax(data, 4);
        assert(result.first == std::vector<int>{1, 2});
        assert(result.second == std::vector<int>{4, 5});
    }
    // k=3 with long sequence ensuring correctness
    {
        std::vector<int> data = {9, 8, 7, 6, 5, 4, 3, 2, 1};
        auto result = slidingWindowMinMax(data, 3);
        std::vector<int> expectedMin = {7, 6, 5, 4, 3, 2, 1};
        std::vector<int> expectedMax = {9, 8, 7, 6, 5, 4, 3};
        assert(result.first == expectedMin);
        assert(result.second == expectedMax);
    }
    return 0;
}
// The problem requires computing the minimum and maximum for every contiguous subarray (window) of length `k`. A straightforward approach scans each window separately, costing \(O(nk)\), which can be too slow for large `n` and `k`. The provided snippet uses a segment tree to answer range min/max queries in \(O(\log n)\) time per query. We first build a segment tree over the entire array where each node stores both the minimum and maximum of its segment. To answer each window query, we call a range-query function that aggregates min and max across the overlapping tree nodes. With `n-k+1` windows, total time is \(O(n + (n-k+1)\log n)\), which simplifies to \(O(n\log n)\). Space complexity is \(O(n)\) for the tree array (we use a 4*size array). Edge cases: (1) `k > n`: we set `k = n` and produce exactly one window covering the whole array. (2) Empty input: return two empty vectors. (3) `n=1` or `k=1`: windows are single elements, so min=max=that element. (4) All elements identical: min and max are the same value. The segment tree must be built and updated correctly; since we only build once and never update after that, we can implement the build and query functions as in the snippet. We must also handle partial overlaps correctly during range queries by splitting the query when it crosses the mid of a node.
