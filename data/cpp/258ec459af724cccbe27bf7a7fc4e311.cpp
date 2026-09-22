// Given an integer array `nums` and an integer `k`, write a C++ function `containsNearbyDuplicate` that returns `true` if there exist two distinct indices `i` and `j` in the array such that `nums[i] == nums[j]` and `abs(i - j) <= k`, and `false` otherwise. The function must handle arrays with up to 10^5 elements, `k` may be 0 or larger, and values may be negative or large integers. The function should be efficient for large inputs.

#include <cassert>
#include <vector>

int main() {
    // Case 1: Duplicate within distance.
    assert(containsNearbyDuplicate({1, 2, 3, 1}, 3) == true);
    // Case 2: Duplicate distance exactly k.
    assert(containsNearbyDuplicate({1, 0, 1, 1}, 1) == true);
    // Case 3: Duplicate too far.
    assert(containsNearbyDuplicate({1, 2, 3, 1, 2, 3}, 2) == false);
    // Case 4: k = 0, no duplicates can be distinct indices.
    assert(containsNearbyDuplicate({1, 1}, 0) == false);
    // Case 5: Empty array.
    assert(containsNearbyDuplicate({}, 5) == false);
    // Case 6: Single element.
    assert(containsNearbyDuplicate({5}, 3) == false);
    // Case 7: Negative numbers.
    assert(containsNearbyDuplicate({-1, -1, 2}, 1) == true);
    // Case 8: No duplicates at all.
    assert(containsNearbyDuplicate({1, 2, 3}, 5) == false);
    // Case 9: Duplicate appears but only at the very end and within k.
    assert(containsNearbyDuplicate({1, 2, 3, 4, 5, 1}, 5) == true);
    // Case 10: k larger than array size.
    assert(containsNearbyDuplicate({1, 2, 3}, 10) == false);
    
    return 0;
}

#include <unordered_map>
#include <vector>
#include <cstdlib>

// Returns true if there are two distinct indices i and j such that
// nums[i] == nums[j] and abs(i - j) <= k.
bool containsNearbyDuplicate(const std::vector<int>& nums, int k) {
    // Map each value to the most recent index where it was seen.
    std::unordered_map<int, int> lastIndex;
    
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        int value = nums[i];
        
        auto it = lastIndex.find(value);
        if (it != lastIndex.end()) {
            // Found a duplicate; check distance.
            if (std::abs(it->second - i) <= k) {
                return true;
            }
            // Distance too large, update to current index.
            it->second = i;
        } else {
            lastIndex[value] = i;
        }
    }
    return false;
}

// The solution uses a hash map (unordered_map) to store the most recent index of each value encountered so far. Iterate through the array from left to right. For each element, check if it exists in the map. If it does, compute the absolute difference between the current index and the stored index; if that difference is ≤ k, immediately return `true`. If the difference is > k, update the stored index to the current index because the older occurrence is too far away, and any future occurrence will be even farther from that old one. If the element is not in the map, insert it with the current index. This ensures that for each value only its latest occurrence is tracked, which is sufficient because if any earlier occurrence were within distance k, it would have been detected when we processed that later occurrence. Edge cases: empty array returns false; k = 0: requires two distinct indices, so only true if there are duplicates at the exact same index—impossible, so for k=0 the function always returns false because `abs(i-j) <= 0` implies i=j, which is not allowed. Time complexity is O(n) since each element is processed once with O(1) average hash operations. Space complexity is O(n) in the worst case when all elements are distinct.
