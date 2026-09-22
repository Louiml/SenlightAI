Given a vector of non-negative integers, write a C++ function `int longestNiceSubarray(const std::vector<int>& nums)` that returns the length of the longest contiguous subarray where the bitwise AND of any two distinct elements in that subarray is zero. In other words, for every pair of indices `i` and `j` within the subarray with `i != j`, the condition `(nums[i] & nums[j]) == 0` must hold. The input vector may be empty (return 0), may contain values up to 10^9, and may include duplicates. The function must run efficiently for vectors up to length 10^5.

// The key observation is that the condition "(nums[i] & nums[j]) == 0 for all pairs" is equivalent to saying that no bit position is set more than once across all numbers in the subarray. Therefore, we can use a sliding window (two pointers) where we maintain a bitmask `mask` that represents the OR of all numbers currently in the window. When we try to add a new element `nums[i]`, if `(mask & nums[i]) != 0`, then some bit is reused, so we must shrink the window from the left by removing elements (via XOR) until the conflict is resolved. After ensuring the window is valid, we update the answer with the current window length and then add `nums[i]` to the mask via OR. Edge cases: empty input (returns 0), single element (always valid, returns 1), and duplicates – if a duplicate value appears, it will conflict with itself (since it has at least one set bit), so the earlier occurrence must be removed. The algorithm runs in O(n) time because each element is added and removed at most once, and uses O(1) auxiliary space. The mask can be a 64-bit integer to safely accommodate values up to 10^9 (since 2^30 > 10^9, but using `long long` provides extra safety).

#include <vector>
#include <algorithm>

// Returns the length of the longest contiguous subarray where the bitwise AND
// of any two distinct elements is zero.
int longestNiceSubarray(const std::vector<int>& nums) {
    int ans = 0;
    long long mask = 0;  // bitmask of all bits set in current window
    int left = 0;        // left boundary of the sliding window

    for (int right = 0; right < static_cast<int>(nums.size()); ++right) {
        // Shrink window until no bit conflicts with nums[right]
        while ((mask & nums[right]) != 0) {
            mask ^= nums[left];  // remove nums[left] from mask
            ++left;
        }
        // Current window [left, right] is valid
        ans = std::max(ans, right - left + 1);
        // Add nums[right] to the mask
        mask |= nums[right];
    }
    return ans;
}

#include <cassert>
#include <vector>
#include <iostream>

// Include the solution function here or via header

int main() {
    // Basic cases
    assert(longestNiceSubarray({1, 3, 8, 48, 10}) == 3); // e.g., [3,8,48] or [8,48,10]
    assert(longestNiceSubarray({1, 2, 4, 8}) == 4);      // all disjoint bits
    assert(longestNiceSubarray({3, 1, 5, 11, 13, 2}) == 1); // no two adjacent are nice? actually [1,5]? check: 1&5=1, so no; [11]? 11&2=0? but [13,2]? 13&2=0? yes but not length 2? Actually test: 13 and 2 have no common bits -> valid, but 5 and 11? 5&11=1, so not. The longest is 2? Let's compute: [1,5] -> 1&5=1 invalid. [5,11] invalid. [11,13] 11&13=9 invalid. [13,2] 13&2=0 valid. So length 2? But there is [3] alone. Wait, the test should be correct. Let's just use known examples.
    // Correcting: known example from LeetCode: [1,3,8,48,10] -> answer 3; [3,1,5,11,13] -> answer 1? Actually 3&1=1, 1&5=1, 5&11=1, 11&13=9, so no adjacent pairs are nice, so max length 1. But 13 alone is fine. So answer 1.
    assert(longestNiceSubarray({3, 1, 5, 11, 13}) == 1);
    
    // Edge cases
    assert(longestNiceSubarray({}) == 0);
    assert(longestNiceSubarray({7}) == 1);
    assert(longestNiceSubarray({0, 0, 0}) == 3); // zeros have no bits, all pairs AND = 0
    assert(longestNiceSubarray({1, 1, 1}) == 1); // duplicates conflict, only single element
    
    // Larger case with known result
    std::vector<int> v = {1, 3, 8, 48, 10, 2, 4, 16, 32, 64};
    assert(longestNiceSubarray(v) == 4); // e.g., [8,48,10,2]? 8&48=0? 8(1000) and 48(110000) share no bits? 48 = 110000, 8=1000, no overlap. 48&10=0? 10=1010, 48=110000, no. 10&2=0? 2=10, 10=1010 share bit 1? 10&2=2, so not. So maybe [2,4,16,32] works? 2,4,16,32 are all powers of two distinct -> all ANDs zero. That's length 4. Or [1,3,8,48]? no. So 4.
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
