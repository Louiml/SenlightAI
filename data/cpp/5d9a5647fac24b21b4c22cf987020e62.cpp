Write a C++ function named `countKDiffPairs` that takes a vector of integers and an integer `k` as input, and returns the number of unique pairs of indices `(i, j)` (with `i != j`) such that the absolute difference between the two values is exactly `k`. The function must count each unordered pair only once, meaning that `(a, b)` and `(b, a)` are considered the same. The input vector can contain duplicate values, and the function should return the number of distinct value pairs that satisfy the condition. The vector length will not exceed 10,000, and all integers are in the range [-1e7, 1e7]. Note that `k` may be negative, zero, or positive, and if `k` is negative, treat it as its absolute value because the problem concerns absolute difference.

The algorithm begins by sorting the input vector to make binary search feasible and to allow easy handling of duplicates. We then iterate through the sorted array, skipping duplicate values at the current position (except for the first occurrence) to avoid counting the same pair multiple times. For each unique current value `nums[i]`, we use `binary_search` to check whether `nums[i] + k` exists in the rest of the array (starting from index `i+1`). If found, we increment the count. This ensures that each unique pair is counted exactly once because the smaller value is the one we iterate over, and the larger value is searched to the right. For `k = 0`, we need to check if the same value appears at least twice in the array; binary search for `nums[i]` starting from `i+1` will find a duplicate only if there are at least two occurrences, and by skipping consecutive duplicates in the loop, we count each pair once. Edge cases include an empty array (return 0), a negative `k` (take absolute value), and `k = 0` where duplicates must be present. Sorting takes `O(n log n)` time, and for each unique element we perform a binary search of `O(log n)`, so the overall time complexity is `O(n log n)`. Space complexity is `O(1)` auxiliary if we sort in-place, ignoring the input storage.

#include <vector>
#include <algorithm>
#include <cstdlib>

// Count the number of unique pairs whose absolute difference equals k.
// The input vector may contain duplicates, and pairs are unordered.
// If k is negative, its absolute value is used.
int countKDiffPairs(std::vector<int>& nums, int k) {
    if (nums.empty()) {
        return 0;
    }

    k = std::abs(k);
    std::sort(nums.begin(), nums.end());

    int count = 0;
    int previous = 0;

    for (size_t i = 0; i < nums.size() - 1; ++i) {
        // Process only the first occurrence of each distinct value
        if (i == 0 || nums[i] != previous) {
            // Search for nums[i] + k in the sorted suffix
            if (std::binary_search(nums.begin() + i + 1, nums.end(), nums[i] + k)) {
                ++count;
            }
            previous = nums[i];
        }
    }

    return count;
}

#include <cassert>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> nums1 = {3, 1, 4, 1, 5};
    assert(countKDiffPairs(nums1, 2) == 2);

    std::vector<int> nums2 = {1, 2, 3, 4, 5};
    assert(countKDiffPairs(nums2, 1) == 4);

    std::vector<int> nums3 = {1, 3, 1, 5, 4};
    assert(countKDiffPairs(nums3, 0) == 1);

    // Negative k is treated as absolute difference
    std::vector<int> nums4 = {1, 2, 3};
    assert(countKDiffPairs(nums4, -1) == 2);

    // Empty vector returns 0
    std::vector<int> nums5;
    assert(countKDiffPairs(nums5, 5) == 0);

    // Duplicate values with k=0, but only one unique value that appears twice
    std::vector<int> nums6 = {7, 7};
    assert(countKDiffPairs(nums6, 0) == 1);

    // Duplicate values with k=0, but only one occurrence -> no pairs
    std::vector<int> nums7 = {7};
    assert(countKDiffPairs(nums7, 0) == 0);

    // k larger than any possible difference
    std::vector<int> nums8 = {1, 2};
    assert(countKDiffPairs(nums8, 10) == 0);

    // Large values entering the range
    std::vector<int> nums9 = {-10000000, 10000000, 0};
    assert(countKDiffPairs(nums9, 10000000) == 2); // (-10000000,0) and (0,10000000)

    // Duplicate values with positive k
    std::vector<int> nums10 = {5, 5, 5, 7};
    assert(countKDiffPairs(nums10, 2) == 1); // only (5,7) counted once

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
