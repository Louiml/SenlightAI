// Write a C++ function that, given a vector of integers `nums` and a positive integer `k`, returns a vector containing the `k` most frequent elements in `nums`. If there is a tie in frequency, any order among the tied elements is acceptable. The input vector may contain duplicates, negative numbers, and the value of `k` is guaranteed to be between 1 and the number of distinct elements in `nums` (inclusive). The function should not modify the original input vector and should return the result as a new vector.

#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Basic test
    std::vector<int> nums1 = {1,1,1,2,2,3};
    std::vector<int> res1 = topKFrequent(nums1, 2);
    std::sort(res1.begin(), res1.end());
    assert(res1 == std::vector<int>({1,2}));

    // k = number of distinct elements
    std::vector<int> nums2 = {4,4,4,5,5,6};
    std::vector<int> res2 = topKFrequent(nums2, 3);
    std::sort(res2.begin(), res2.end());
    assert(res2 == std::vector<int>({4,5,6}));

    // k = 1
    std::vector<int> nums3 = {7,7,8,8,8,9};
    std::vector<int> res3 = topKFrequent(nums3, 1);
    assert(res3.size() == 1 && res3[0] == 8);

    // Negative numbers and ties (any order allowed, so check sorted)
    std::vector<int> nums4 = {-1,-1,-2,-2,-3,-3};
    std::vector<int> res4 = topKFrequent(nums4, 2);
    std::sort(res4.begin(), res4.end());
    assert(res4.size() == 2 && (res4[0] == -1 || res4[0] == -2 || res4[0] == -3));

    // All same numbers
    std::vector<int> nums5 = {10,10,10,10};
    std::vector<int> res5 = topKFrequent(nums5, 1);
    assert(res5 == std::vector<int>({10}));

    // Single element
    std::vector<int> nums6 = {42};
    std::vector<int> res6 = topKFrequent(nums6, 1);
    assert(res6 == std::vector<int>({42}));

    // Duplicate with mixed frequencies
    std::vector<int> nums7 = {1,2,2,3,3,3,4,4,4,4};
    std::vector<int> res7 = topKFrequent(nums7, 4);
    std::sort(res7.begin(), res7.end());
    assert(res7 == std::vector<int>({1,2,3,4}));

    // Large k equals distinct count
    std::vector<int> nums8 = {5,5,1,2,2,3,3,3};
    std::vector<int> res8 = topKFrequent(nums8, 4);
    std::sort(res8.begin(), res8.end());
    assert(res8 == std::vector<int>({1,2,3,5}));

    // Check original vector not modified
    std::vector<int> nums9 = {9,9,8,7,7,7};
    std::vector<int> original = nums9;
    topKFrequent(nums9, 2);
    assert(nums9 == original);

    // Ensure result size correct
    std::vector<int> nums10 = {1,1,2,2,3,4,4};
    std::vector<int> res10 = topKFrequent(nums10, 2);
    assert(res10.size() == 2);
}

#include <vector>
#include <unordered_map>
#include <algorithm>

// Returns the k most frequent elements from nums (order among ties is arbitrary).
std::vector<int> topKFrequent(const std::vector<int>& nums, int k) {
    // Step 1: Count frequencies.
    std::unordered_map<int, int> freq;
    for (int x : nums) {
        ++freq[x];
    }

    // Step 2: Convert map to vector of pairs (value, frequency).
    std::vector<std::pair<int, int>> freqVec;
    freqVec.reserve(freq.size());
    for (const auto& entry : freq) {
        freqVec.emplace_back(entry.first, entry.second);
    }

    // Step 3: Sort by frequency descending.
    std::sort(freqVec.begin(), freqVec.end(),
              [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                  return a.second > b.second;
              });

    // Step 4: Collect top k values.
    std::vector<int> ans;
    ans.reserve(k);
    for (int i = 0; i < k; ++i) {
        ans.push_back(freqVec[i].first);
    }
    return ans;
}

// The solution uses a hash map (`std::unordered_map`) to count the frequency of each distinct integer in the input vector, which takes O(n) time where n is the size of the input. After counting, we convert the map into a vector of pairs `(value, frequency)`. Then we sort this vector in descending order of frequency using a custom comparator (via a lambda or a free comparator function). Sorting the vector of size `m` (number of distinct elements) takes O(m log m) time. Finally, we take the first `k` elements from the sorted vector and push their values into the result. Edge cases include: `k` equal to the number of distinct elements (all are returned), or when there are ties—any ordering is allowed, so the sort just needs to group equal frequencies but not necessarily order them strictly. Space complexity is O(m) for the map and the vector of pairs, where m ≤ n. The overall time complexity is O(n + m log m), which is acceptable for typical constraints.
