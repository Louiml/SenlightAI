Write a C++ function `bool containsNearbyDuplicate(const std::vector<int>& nums, int k)` that returns `true` if there exist two indices `i` and `j` (with `i != j`) in the given vector such that `nums[i] == nums[j]` and the absolute difference `|i - j| <= k`; otherwise return `false`. The function must handle an empty vector, a single-element vector, very large `k`, and duplicate values that are far apart. You may use any standard library containers, but do not modify the input vector.

// The algorithm uses a hash map to store the most recent index of each number as we iterate through the vector once. For each element at index `i`, we check whether the value has been seen before. If it has and the difference between the current index and the stored index is `<= k`, we immediately return `true`. If the difference is larger than `k`, we update the stored index to the current one because the older occurrence is no longer relevant for future duplicates (any future duplicate must be within `k` of the current index). If the value is new, we simply store its index. Edge cases: an empty vector or a vector with only one element has no valid pair, so the function correctly returns `false`. A very large `k` (larger than the vector size) will cause the condition to hold for any duplicate since the difference is always less than the vector size. The time complexity is \(O(n)\) because each element is processed once and hash map operations are amortized \(O(1)\). The space complexity is \(O(n)\) in the worst case when all elements are distinct.

#include <vector>
#include <unordered_map>
#include <cstdlib> // for std::abs

// Returns true if there exist two indices i and j (i != j) such that
// nums[i] == nums[j] and |i - j| <= k. Otherwise returns false.
bool containsNearbyDuplicate(const std::vector<int>& nums, int k) {
    std::unordered_map<int, int> lastIndex; // value -> most recent index
    const int n = static_cast<int>(nums.size());
    for (int i = 0; i < n; ++i) {
        auto it = lastIndex.find(nums[i]);
        if (it != lastIndex.end()) {
            if (std::abs(i - it->second) <= k) {
                return true;
            }
            // Update to current index because the old one is too far away
            it->second = i;
        } else {
            lastIndex[nums[i]] = i;
        }
    }
    return false;
}

#include <cassert>
#include <vector>

int main() {
    // Basic duplicate within range
    assert(containsNearbyDuplicate({1,2,3,1}, 3) == true);
    // Duplicate but distance exactly k
    assert(containsNearbyDuplicate({1,0,1,1}, 1) == true);
    // Duplicate exists but distance > k
    assert(containsNearbyDuplicate({1,2,3,1,2,3}, 2) == false);
    // Empty vector
    assert(containsNearbyDuplicate({}, 5) == false);
    // Single element
    assert(containsNearbyDuplicate({42}, 1) == false);
    // No duplicates
    assert(containsNearbyDuplicate({1,2,3,4,5}, 0) == false);
    // Large k catches any duplicate
    assert(containsNearbyDuplicate({1,2,3,1}, 10) == true);
    // Consecutive duplicates with k=0 (must be false because i != j required)
    assert(containsNearbyDuplicate({5,5}, 0) == false);
    // Duplicate at edges with exact distance k
    assert(containsNearbyDuplicate({1,2,3,4,1}, 4) == true);
    return 0;
}
