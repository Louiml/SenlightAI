/*
Write a C++ function `findKDistantIndices` that, given a vector of integers `nums`, an integer `key`, and a non-negative integer `k`, returns a sorted vector of all indices `i` (0 ≤ i < nums.size()) such that there exists some index `j` where `nums[j] == key` and `|i - j| ≤ k`. The returned vector must be in increasing order. If no such index exists, return an empty vector. The function should handle empty input, k = 0, and duplicate keys correctly.
*/

#include <vector>
#include <algorithm>

// Returns indices i such that there exists an index j with nums[j]==key and |i-j| <= k.
std::vector<int> findKDistantIndices(const std::vector<int>& nums, int key, int k) {
    const int n = static_cast<int>(nums.size());
    std::vector<bool> take(n, false);
    
    for (int j = 0; j < n; ++j) {
        if (nums[j] == key) {
            int left = std::max(0, j - k);
            int right = std::min(n - 1, j + k);
            for (int i = left; i <= right; ++i) {
                take[i] = true;
            }
        }
    }
    
    std::vector<int> result;
    for (int i = 0; i < n; ++i) {
        if (take[i]) {
            result.push_back(i);
        }
    }
    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Basic case
    std::vector<int> nums1 = {3,4,9,1,3,9,5};
    assert(findKDistantIndices(nums1, 9, 1) == std::vector<int>({1,2,3,4,5,6}));
    
    // k = 0
    std::vector<int> nums2 = {1,2,3,2,2,1};
    assert(findKDistantIndices(nums2, 2, 0) == std::vector<int>({1,3,4}));
    
    // No key present
    std::vector<int> nums3 = {5,6,7};
    assert(findKDistantIndices(nums3, 1, 2).empty());
    
    // Large k covers entire array
    std::vector<int> nums4 = {1,2,3};
    assert(findKDistantIndices(nums4, 2, 5) == std::vector<int>({0,1,2}));
    
    // Empty input
    std::vector<int> nums5;
    assert(findKDistantIndices(nums5, 7, 2).empty());
    
    // Multiple overlapping keys
    std::vector<int> nums6 = {1,2,1,2,1};
    assert(findKDistantIndices(nums6, 1, 1) == std::vector<int>({0,1,2,3,4}));
    
    // Single element input
    std::vector<int> nums7 = {5};
    assert(findKDistantIndices(nums7, 5, 1) == std::vector<int>({0}));
    
    // k larger than array size, key at one end
    std::vector<int> nums8 = {9,1,1,1};
    assert(findKDistantIndices(nums8, 9, 10) == std::vector<int>({0,1,2,3}));
    
    // Edge: key at boundaries
    std::vector<int> nums9 = {2,1,1,1,2};
    assert(findKDistantIndices(nums9, 2, 1) == std::vector<int>({0,1,3,4}));
    
    // Duplicate keys with wide range
    std::vector<int> nums10 = {1,0,0,1,0,0,1};
    assert(findKDistantIndices(nums10, 1, 2) == std::vector<int>({0,1,2,3,4,5,6}));
    
    return 0;
}

// The problem asks for all indices that are within distance `k` of at least one occurrence of `key`. A straightforward solution is to iterate over each index `j` where `nums[j] == key`, and then mark all indices from `max(0, j-k)` to `min(n-1, j+k)` as valid. Using a boolean vector `shouldTake` of size `n` (initialized to `false`), we set these ranges to `true`. After processing all key occurrences, we scan the boolean vector and collect indices that are marked `true`. This guarantees increasing order because we scan from left to right.  
// Edge cases:  
// - If `nums` is empty, the loop over `i` does nothing, the boolean vector is empty, and the return is an empty vector.  
// - If `k` is 0, only indices where `nums[i] == key` are marked.  
// - If `nums` contains multiple occurrences of `key`, overlapping ranges are naturally handled by OR-ing the marks.  
// - The distance condition is symmetric, so marking ranges correctly handles all indices that are within `k` steps of any key occurrence.  
//
// Time complexity: O(n + m * k) in the worst case, where m is the number of key occurrences. In practice, if k is large relative to n, this reduces to O(n * m) but at most O(n^2). Space complexity: O(n) for the boolean vector and O(n) for the result vector. A more optimal approach using a difference array could achieve O(n), but the simple marking is acceptable for typical constraints.
