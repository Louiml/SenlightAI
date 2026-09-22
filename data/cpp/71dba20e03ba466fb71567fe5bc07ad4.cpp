Write a C++ function `topKFrequent` that takes a vector of integers `nums` and an integer `k`, and returns a vector containing the `k` most frequent elements in any order. If there are ties in frequency, any of the tied elements may be chosen. The function must handle cases where `k` is less than or equal to the number of distinct elements in the input, and it must work for negative numbers, zeros, and duplicates. The returned vector should not contain duplicates, and its size must be exactly `k`. Assume `k` is always positive and not larger than the number of distinct elements.

#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Basic test.
    std::vector<int> nums1 = {1, 1, 1, 2, 2, 3};
    std::vector<int> res1 = topKFrequent(nums1, 2);
    std::sort(res1.begin(), res1.end());
    assert(res1 == std::vector<int>({1, 2}));

    // All same element.
    std::vector<int> nums2 = {5, 5, 5, 5};
    std::vector<int> res2 = topKFrequent(nums2, 1);
    assert(res2.size() == 1 && res2[0] == 5);

    // Negative numbers and k equals distinct count.
    std::vector<int> nums3 = {-1, -1, -2, -2, -2, 0};
    std::vector<int> res3 = topKFrequent(nums3, 3);
    std::sort(res3.begin(), res3.end());
    assert(res3 == std::vector<int>({-2, -1, 0}));

    // Ties in frequency, any valid selection works.
    std::vector<int> nums4 = {1, 2, 3, 4};
    std::vector<int> res4 = topKFrequent(nums4, 2);
    // Since all frequencies are 1, any two elements are acceptable.
    assert(res4.size() == 2);
    assert(res4[0] != res4[1]); // ensures distinct
    for (int val : res4) {
        assert(val >= 1 && val <= 4);
    }

    // Duplicate values with large k.
    std::vector<int> nums5 = {1, 1, 2, 2, 3, 3, 4, 4};
    std::vector<int> res5 = topKFrequent(nums5, 4);
    std::sort(res5.begin(), res5.end());
    assert(res5 == std::vector<int>({1, 2, 3, 4}));

    // Single element.
    std::vector<int> nums6 = {42};
    std::vector<int> res6 = topKFrequent(nums6, 1);
    assert(res6.size() == 1 && res6[0] == 42);

    // Large frequency difference.
    std::vector<int> nums7 = {10, 10, 10, 10, 20, 20, 30};
    std::vector<int> res7 = topKFrequent(nums7, 2);
    std::sort(res7.begin(), res7.end());
    assert(res7 == std::vector<int>({10, 20}));

    // k = 1 with a unique most frequent.
    std::vector<int> nums8 = {7, 7, 7, 8, 9, 9};
    std::vector<int> res8 = topKFrequent(nums8, 1);
    assert(res8.size() == 1 && res8[0] == 7);
}

#include <vector>
#include <unordered_map>
#include <queue>
#include <utility>

// Return the k most frequent elements from nums.
std::vector<int> topKFrequent(const std::vector<int>& nums, int k) {
    // Count frequencies for each element.
    std::unordered_map<int, int> frequency;
    for (const int num : nums) {
        ++frequency[num];
    }

    // Min-heap keyed by frequency. Stores pairs of (frequency, element).
    using Pair = std::pair<int, int>;
    auto cmp = [](const Pair& a, const Pair& b) {
        return a.first > b.first; // min-heap: smaller frequency at top
    };
    std::priority_queue<Pair, std::vector<Pair>, decltype(cmp)> heap(cmp);

    for (const auto& entry : frequency) {
        // entry is (element, frequency)
        Pair current = {entry.second, entry.first}; // (freq, element)
        if (static_cast<int>(heap.size()) < k) {
            heap.push(current);
        } else if (current.first > heap.top().first) {
            heap.pop();
            heap.push(current);
        }
    }

    // Extract the elements from the heap.
    std::vector<int> result;
    result.reserve(k);
    while (!heap.empty()) {
        result.push_back(heap.top().second);
        heap.pop();
    }
    return result;
}

// The solution uses a two-step approach: first, build a frequency map (`unordered_map<int,int>`) by iterating through the input vector, counting each element's occurrences in O(n) time. Second, use a min-heap (priority queue) of size `k` to track the `k` most frequent elements. The heap is ordered by frequency (the smallest frequency is at the top). For each entry in the frequency map, if the heap is not full, push the entry; otherwise, if the current entry's frequency is greater than the heap's top frequency, pop the top and push the current entry. This ensures the heap always contains the top `k` frequencies. At the end, extract the elements (the keys) from the heap into a result vector. Edge cases: when `k` equals the number of distinct elements, the heap will contain all. When frequencies are equal, any selection is valid. Time complexity: O(n) for counting, O(m log k) for heap operations, where m is number of distinct elements (m ≤ n). Space complexity: O(m) for the map and O(k) for the heap. The solution handles negative numbers naturally since `unordered_map` supports any integer key.
