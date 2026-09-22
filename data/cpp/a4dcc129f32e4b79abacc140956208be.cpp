Write a C++ function named `collectKDistantIndices` that takes a non-empty vector of integers `nums`, an integer `key`, and a non-negative integer `k`, and returns a vector containing all indices `j` (in ascending order) such that there exists at least one index `i` with `nums[i] == key` and `|i - j| <= k`. The returned vector must have strictly increasing values, with no duplicates, and must include every index that satisfies the condition. You may assume the input vector is not empty, but `k` can be zero, and `key` may appear multiple times or not at all (in which case the result is empty).

#include <cassert>
#include <vector>

int main() {
    // Basic case: key at index 2, k=2 covers indices 0..4
    std::vector<int> nums1 = {3,4,5,2,1,6};
    assert(collectKDistantIndices(nums1, 2, 2) == std::vector<int>({0,1,2,3,4}));
    
    // k=0: only the exact index where key appears
    std::vector<int> nums2 = {1,2,3,4,5};
    assert(collectKDistantIndices(nums2, 3, 0) == std::vector<int>({2}));
    
    // key appears multiple times, overlapping ranges
    std::vector<int> nums3 = {1,2,1,2,1};
    assert(collectKDistantIndices(nums3, 1, 1) == std::vector<int>({0,1,2,3,4}));
    
    // key not present -> empty result
    std::vector<int> nums4 = {5,6,7};
    assert(collectKDistantIndices(nums4, 1, 3).empty());
    
    // k larger than array size -> all indices included if key present
    std::vector<int> nums5 = {1,2,3,4};
    assert(collectKDistantIndices(nums5, 3, 100) == std::vector<int>({0,1,2,3}));
    
    // Single element array, key matches
    std::vector<int> nums6 = {7};
    assert(collectKDistantIndices(nums6, 7, 0) == std::vector<int>({0}));
    
    // Edge: key at beginning, k=2
    std::vector<int> nums7 = {2,5,6,1,3};
    assert(collectKDistantIndices(nums7, 2, 2) == std::vector<int>({0,1,2}));
    
    // Edge: key at end, k=2
    std::vector<int> nums8 = {1,3,5,7,2};
    assert(collectKDistantIndices(nums8, 2, 2) == std::vector<int>({3,4}));
    
    // Multiple keys far apart
    std::vector<int> nums9 = {1,2,1,3,1};
    assert(collectKDistantIndices(nums9, 1, 1) == std::vector<int>({0,1,2,3,4}));
    
    // Duplicate keys close, k=1
    std::vector<int> nums10 = {1,1,1};
    assert(collectKDistantIndices(nums10, 1, 0) == std::vector<int>({0,1,2}));
    
    return 0;
}

#include <vector>
#include <algorithm>

// Collect all indices j such that there exists an i with nums[i] == key and |i - j| <= k.
// Returns indices in strictly increasing order, with no duplicates.
std::vector<int> collectKDistantIndices(const std::vector<int>& nums, int key, int k) {
    const int n = static_cast<int>(nums.size());
    std::vector<bool> valid(n, false);
    
    // Mark all indices within distance k of any occurrence of key
    for (int i = 0; i < n; ++i) {
        if (nums[i] == key) {
            int start = std::max(0, i - k);
            int end = std::min(n - 1, i + k);
            for (int j = start; j <= end; ++j) {
                valid[j] = true;
            }
        }
    }
    
    // Collect marked indices in ascending order
    std::vector<int> result;
    for (int j = 0; j < n; ++j) {
        if (valid[j]) {
            result.push_back(j);
        }
    }
    return result;
}

// The solution scans the array once to find every position `i` where `nums[i] == key`. For each such occurrence, any index within the range `[i - k, i + k]` is valid, but we must clamp this range to the valid bounds of the array (`0` to `n-1`). Since multiple occurrences of `key` can produce overlapping ranges, we need to avoid duplicates and return indices in sorted order. The simplest approach is to collect all candidate indices into a `std::set<int>`, which automatically sorts and deduplicates them. Alternatively, we can use a boolean `visited` array of size `n` to mark indices and then iterate through all indices in ascending order to output those marked, which is more memory-efficient and avoids the overhead of a set. The main edge cases are: `k` larger than the array length, `key` not present, duplicates of `key` close together, and `k == 0` (only the exact index where `key` appears is included). Time complexity is O(n) because each index is visited at most once (using a marking array and then a single pass to collect). Space complexity is O(n) for the marking array plus the output vector.
