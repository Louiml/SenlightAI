Given a sequence of positive integers where each integer is between 1 and 1,000,000 inclusive, write a C++ function that accepts a vector of integers and a target integer `k` (where `k >= 1`), and returns a vector of the `k` largest distinct values from the input, sorted in descending order. If the input contains fewer than `k` distinct values, return all distinct values sorted in descending order. The function must not modify the input vector and should handle large inputs efficiently (e.g., up to 10^5 elements). For example, given `{5, 3, 8, 5, 2, 9, 8, 1}` and `k=3`, the result should be `{9, 8, 5}`. Duplicate values are ignored when counting toward the `k` largest distinct elements.
// The problem reduces to finding the top `k` largest distinct elements. The simplest robust approach is to first remove duplicates by inserting all elements into a `std::set` (which stores elements in ascending order) or by using `std::unordered_set` and then sorting. However, to achieve optimal time and memory, we can use a min-heap (priority queue) of size at most `k` that stores the largest `k` distinct values seen so far. First, we use an unordered_set to track seen values and avoid duplicates. For each new distinct value, we push it into a min-heap. If the heap size exceeds `k`, we pop the smallest. After processing all elements, the heap contains the top `k` distinct values, but in heap order (not sorted). We extract them into a vector and sort in descending order. Edge cases: if `k` is larger than the number of distinct values, the heap contains all distinct values; the heap may also be empty if the input is empty (then return empty vector). Time complexity: O(n log k) for heap operations (where n is input size), plus O(n) for the unordered_set lookups, and O(k log k) for final sort. Space complexity: O(n) in the worst case for the unordered_set (to track duplicates) and O(k) for the heap. Alternative: use a `std::set` (balanced BST) and iterate backward – but the heap approach is more memory-efficient for large n.
#include <vector>
#include <unordered_set>
#include <queue>
#include <algorithm>

// Return the k largest distinct values from nums, sorted descending.
// If fewer than k distinct values exist, return all distinct values sorted descending.
std::vector<int> topKLargestDistinct(const std::vector<int>& nums, int k) {
    std::unordered_set<int> seen;
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap; // stores largest k distinct values

    for (int value : nums) {
        if (seen.insert(value).second) { // new distinct value
            minHeap.push(value);
            if (static_cast<int>(minHeap.size()) > k) {
                minHeap.pop(); // remove smallest among the top k
            }
        }
    }

    std::vector<int> result;
    result.reserve(minHeap.size());
    while (!minHeap.empty()) {
        result.push_back(minHeap.top());
        minHeap.pop();
    }
    std::sort(result.begin(), result.end(), std::greater<int>()); // descending order
    return result;
}
#include <cassert>
#include <vector>

int main() {
    // Basic case
    std::vector<int> nums1 = {5, 3, 8, 5, 2, 9, 8, 1};
    std::vector<int> res1 = topKLargestDistinct(nums1, 3);
    assert(res1 == (std::vector<int>{9, 8, 5}));

    // k larger than distinct count
    std::vector<int> nums2 = {7, 7, 7, 2};
    std::vector<int> res2 = topKLargestDistinct(nums2, 5);
    assert(res2 == (std::vector<int>{7, 2}));

    // Empty input
    std::vector<int> nums3 = {};
    std::vector<int> res3 = topKLargestDistinct(nums3, 2);
    assert(res3.empty());

    // All same duplicates
    std::vector<int> nums4 = {4, 4, 4};
    std::vector<int> res4 = topKLargestDistinct(nums4, 1);
    assert(res4 == (std::vector<int>{4}));

    // Large k with all distinct
    std::vector<int> nums5 = {10, 20, 30, 40};
    std::vector<int> res5 = topKLargestDistinct(nums5, 4);
    assert(res5 == (std::vector<int>{40, 30, 20, 10}));

    // k = 1
    std::vector<int> nums6 = {1, 5, 3, 5, 2, 9};
    std::vector<int> res6 = topKLargestDistinct(nums6, 1);
    assert(res6 == (std::vector<int>{9}));

    // Negative and zero not allowed per spec, but test positive only: use random
    std::vector<int> nums7 = {100, 1, 99, 1, 100, 50, 75};
    std::vector<int> res7 = topKLargestDistinct(nums7, 2);
    assert(res7 == (std::vector<int>{100, 99}));

    return 0;
}
