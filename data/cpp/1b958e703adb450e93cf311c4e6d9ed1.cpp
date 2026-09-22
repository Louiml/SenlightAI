// Write a C++ function `vector<int> topKFrequent(const vector<int>& nums, int k)` that returns the `k` most frequently occurring integers in the input vector `nums`. The result should be ordered by decreasing frequency; if two numbers have the same frequency, their relative order does not matter. You may assume `k` is valid: `1 <= k <= number of distinct elements`. The input vector is not necessarily sorted and may contain duplicates, zero, negative numbers, and large values. The function should not modify the input vector.

// The solution uses a three-step approach: (1) Count frequencies using an `unordered_map<int, int>`, where each key is a number and its value is the number of occurrences. This requires a single pass over the input, resulting in `O(n)` time. (2) Build a "bucket" array of vectors, where the index represents a frequency and the vector at that index contains all numbers having that frequency. Since the maximum possible frequency is `nums.size()`, we allocate `nums.size() + 1` buckets. Populating the buckets requires iterating over all unique entries in the map, so `O(u)` time where `u` is the number of distinct elements. (3) Traverse the buckets from the highest frequency down to the lowest, collecting numbers into the result vector until we have gathered exactly `k` elements. This step is `O(n)` in the worst case since we may traverse all buckets but stop early once `k` elements are collected. Edge cases include `k` equal to the number of distinct elements (we collect all), duplicate frequencies (order within the same bucket is arbitrary, which is acceptable), and all elements identical (only one bucket containing one number). The total time complexity is `O(n)` on average due to the hash map, and the space complexity is `O(n)` for the map and buckets, plus `O(k)` for the result.

#include <vector>
#include <unordered_map>

// Return the k most frequent elements, ordered by decreasing frequency.
std::vector<int> topKFrequent(const std::vector<int>& nums, int k) {
    // Count frequency of each number.
    std::unordered_map<int, int> frequency;
    for (int num : nums) {
        ++frequency[num];
    }
    
    // Create buckets: index = frequency, value = list of numbers with that frequency.
    std::vector<std::vector<int>> buckets(nums.size() + 1);
    for (const auto& entry : frequency) {
        buckets[entry.second].push_back(entry.first);
    }
    
    // Collect the top k elements by iterating buckets from highest frequency downward.
    std::vector<int> result;
    for (int freq = buckets.size() - 1; freq > 0 && result.size() < static_cast<size_t>(k); --freq) {
        for (int num : buckets[freq]) {
            result.push_back(num);
            if (result.size() == static_cast<size_t>(k)) {
                return result;
            }
        }
    }
    return result;
}

#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Basic case: frequencies 2, 2, 1; k=2 should return [1,2] or [2,1] but order by frequency descending works.
    std::vector<int> nums1 = {1, 1, 2, 2, 3};
    std::vector<int> res1 = topKFrequent(nums1, 2);
    assert(res1.size() == 2);
    assert(std::find(res1.begin(), res1.end(), 1) != res1.end());
    assert(std::find(res1.begin(), res1.end(), 2) != res1.end());

    // All same elements: only one distinct, k=1 returns that element.
    std::vector<int> nums2 = {5, 5, 5, 5};
    assert(topKFrequent(nums2, 1) == std::vector<int>({5}));

    // k equals the number of distinct elements.
    std::vector<int> nums3 = {4, 1, 2};
    std::vector<int> res3 = topKFrequent(nums3, 3);
    assert(res3.size() == 3);
    sort(res3.begin(), res3.end());
    assert(res3 == std::vector<int>({1, 2, 4}));

    // Negative numbers and zero.
    std::vector<int> nums4 = {-1, -1, 0, 0, 0, 2};
    std::vector<int> res4 = topKFrequent(nums4, 2);
    assert(res4.size() == 2);
    assert(res4[0] == 0); // frequency 3 (highest)
    assert(res4[1] == -1); // frequency 2 (next)

    // Large input with many duplicates: k=1 returns the most frequent.
    std::vector<int> nums5 = {3, 3, 3, 2, 2, 1, 1, 1, 1};
    assert(topKFrequent(nums5, 1) == std::vector<int>({1}));

    // Single element, k=1.
    std::vector<int> nums6 = {42};
    assert(topKFrequent(nums6, 1) == std::vector<int>({42}));

    // Empty vector is not allowed per spec (k>=1), but test defensive behavior implied? skip as spec says valid.

    // Check that input is not modified.
    std::vector<int> original = {1, 1, 2};
    std::vector<int> copy = original;
    topKFrequent(original, 2);
    assert(original == copy);

    return 0;
}
