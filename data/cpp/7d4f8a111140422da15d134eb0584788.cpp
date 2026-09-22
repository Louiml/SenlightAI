// Write a C++ function `kSmallestPairs` that takes two sorted integer vectors `nums1` and `nums2`, and an integer `k`, and returns a vector of pairs representing the `k` smallest sum pairs `(nums1[i], nums2[j])`. A pair's sum is defined as `nums1[i] + nums2[j]`. The result must be ordered by the sum in ascending order; if two sums are equal, the order between them does not matter. The input vectors are non-empty and sorted in non-decreasing order. Return at most `k` pairs; if the total number of possible pairs is less than `k`, return all possible pairs. The function must handle cases where `k` is larger than the number of possible combinations.
#include <cassert>
#include <vector>
#include <utility>

// Declaration of the function (assume it is defined above or included)
std::vector<std::pair<int, int>> kSmallestPairs(
    const std::vector<int>& nums1,
    const std::vector<int>& nums2,
    int k);

int main() {
    // Basic case
    std::vector<int> nums1 = {1, 7, 11};
    std::vector<int> nums2 = {2, 4, 6};
    auto result = kSmallestPairs(nums1, nums2, 3);
    std::vector<std::pair<int, int>> expected = {{1, 2}, {1, 4}, {1, 6}};
    assert(result == expected);

    // k larger than possible pairs
    nums1 = {1, 2};
    nums2 = {3};
    result = kSmallestPairs(nums1, nums2, 5);
    expected = {{1, 3}, {2, 3}};
    assert(result == expected);

    // k = 0
    result = kSmallestPairs(nums1, nums2, 0);
    assert(result.empty());

    // Single-element vectors
    nums1 = {1};
    nums2 = {1};
    result = kSmallestPairs(nums1, nums2, 1);
    expected = {{1, 1}};
    assert(result == expected);

    // Equal sums (order-independent, but we test size and content)
    nums1 = {1, 1};
    nums2 = {1, 1};
    result = kSmallestPairs(nums1, nums2, 4);
    assert(result.size() == 4);
    assert(result[0] == std::make_pair(1, 1));
    assert(result[1] == std::make_pair(1, 1));
    assert(result[2] == std::make_pair(1, 1));
    assert(result[3] == std::make_pair(1, 1));

    // k limited but more pairs available
    nums1 = {1, 2, 3};
    nums2 = {1, 2};
    result = kSmallestPairs(nums1, nums2, 2);
    expected = {{1, 1}, {1, 2}};
    assert(result == expected);
}
#include <vector>
#include <queue>
#include <tuple>

// Returns the k smallest sum pairs from two sorted vectors.
std::vector<std::pair<int, int>> kSmallestPairs(
    const std::vector<int>& nums1,
    const std::vector<int>& nums2,
    int k) {
    std::vector<std::pair<int, int>> result;
    if (nums1.empty() || nums2.empty() || k <= 0) {
        return result;
    }

    // Min-heap stores (sum, index in nums1, index in nums2)
    using HeapNode = std::tuple<int, int, int>;
    std::priority_queue<HeapNode, std::vector<HeapNode>, std::greater<HeapNode>> minHeap;

    // Initialize heap with first element of nums2 paired with each nums1[i] (up to k)
    for (int i = 0; i < nums1.size() && i < k; ++i) {
        minHeap.emplace(nums1[i] + nums2[0], i, 0);
    }

    while (!minHeap.empty() && result.size() < static_cast<size_t>(k)) {
        auto [sum, i, j] = minHeap.top();
        minHeap.pop();
        result.emplace_back(nums1[i], nums2[j]);

        // If there is a next element in nums2 for the same nums1[i], push it
        if (j + 1 < nums2.size()) {
            minHeap.emplace(nums1[i] + nums2[j + 1], i, j + 1);
        }
    }

    return result;
}
// The problem is a classic "k smallest sums" from two sorted arrays. The straightforward brute-force would generate all `n*m` pairs and sort them, but that is inefficient for large inputs. The optimal approach uses a min-heap (priority queue) seeded with the first pair `(nums1[0], nums2[0])`. Because both arrays are sorted, the smallest sum pair after any popped pair `(i,j)` can only be one of `(i+1, j)` or `(i, j+1)` — but we must be careful not to push duplicates. To avoid duplicates, we can use a visited set (or push only `(i+1, j)` and have a separate mechanism). A common trick: initialize the heap with `(nums1[i], nums2[0])` for all `i` from 0 to `min(k, n)-1`, then each pop produces pair `(nums1[i], nums2[j])`, and we push `(nums1[i], nums2[j+1])` if `j+1 < m`. This avoids duplicates because we only move forward in `nums2` for each fixed `i`. The heap orders by the sum. We pop `k` times or until heap empty. Edge cases: one or both vectors may be empty? The problem states non-empty, but handle empty gracefully. If `k` is 0, return empty. Time complexity: O(k log min(k, n)) for heap operations, but since we push at most `k` elements and each insertion/deletion is O(log k), overall O(k log k). Space: O(min(k, n)) for the heap and O(k) for result. If `k` can be very large (e.g., up to n*m), this is still efficient because we stop early.
