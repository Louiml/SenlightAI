// Given a vector of non-negative integers `nums` and an integer `k`, write a C++ function `int minimumSubarrayLength(const std::vector<int>& nums, int k)` that returns the length of the shortest contiguous subarray whose bitwise OR is at least `k`. If no such subarray exists, return `-1`. The input vector is non-empty, and `k` is non-negative. The bitwise OR is computed over all elements of the subarray (e.g., `OR` of `[5, 2]` is `7`). The function must handle cases where the subarray length can be 1 or more, and where the total OR of the entire array is less than `k` (then return `-1`). Example: for `nums = [1, 2, 3]` and `k = 2`, the shortest subarray is `[2]` (length 1) because `2 >= 2`, but `[1]` (OR=1) is not enough. For `nums = [1, 2]` and `k = 4`, no subarray works, so return `-1`.
The core challenge is finding the shortest subarray with OR ≥ k efficiently. A brute-force approach checking all subarrays would be O(n²). Instead, we use a sliding window (two-pointer) method combined with a bit-count array to maintain the OR of the current window dynamically. The window is expanded by moving the right pointer, and for each new element, we update the OR and the bit counts (count of set bits at each of the 32 bit positions). After expanding, we try to shrink from the left as long as the current window's OR is still ≥ k, updating the answer with the current window length. When shrinking, we remove the left element's bits from the bit-count and recompute the OR from the bit-count (any bit with count > 0 is set). This recomputation is necessary because OR is not easily invertible when a bit disappears. Edge cases: (1) if `k == 0`, any subarray works; the shortest is length 1 (or 0 if we allow empty, but problem implies non-empty subarray, so length 1). (2) If the total OR of all elements is < k, the while loop never triggers, and we return -1. (3) The bit-count array has size 32 (for 32-bit integers; we assume non-negative, so no sign bit issues). Time complexity: each element is added and removed at most once, and each removal scans the 32 bits, so O(32n) = O(n) with a small constant. Space: O(1) auxiliary (just the bit-count array and a few variables).
#include <vector>
#include <climits>
#include <algorithm>

// Returns the length of the shortest contiguous subarray whose bitwise OR is at least k.
// If no such subarray exists, returns -1.
int minimumSubarrayLength(const std::vector<int>& nums, int k) {
    const int n = nums.size();
    std::vector<int> bitCount(32, 0); // Count of set bits at each bit position for current window
    int left = 0;
    int currentOR = 0;
    int minLength = INT_MAX;

    for (int right = 0; right < n; ++right) {
        // Expand window to include nums[right]
        currentOR |= nums[right];
        // Update bit counts for the new element
        for (int i = 0; i < 32; ++i) {
            if (nums[right] & (1 << i)) {
                ++bitCount[i];
            }
        }

        // Try to shrink from left while OR is still >= k
        while (left <= right && currentOR >= k) {
            minLength = std::min(minLength, right - left + 1);
            // Remove nums[left] from the window
            int updatedOR = 0;
            for (int i = 0; i < 32; ++i) {
                if (nums[left] & (1 << i)) {
                    --bitCount[i];
                }
                // Recompute OR from remaining bit counts
                if (bitCount[i] > 0) {
                    updatedOR |= (1 << i);
                }
            }
            currentOR = updatedOR;
            ++left;
        }
    }

    return (minLength == INT_MAX) ? -1 : minLength;
}
#include <cassert>
#include <vector>

// Function prototype (solution above; here we include a minimal declaration for testing)
int minimumSubarrayLength(const std::vector<int>& nums, int k);

int main() {
    // Basic cases
    assert(minimumSubarrayLength({1, 2, 3}, 2) == 1); // [2]
    assert(minimumSubarrayLength({1, 2, 3}, 4) == -1); // total OR=3 < 4
    assert(minimumSubarrayLength({1, 2}, 3) == 2); // [1,2] OR=3
    assert(minimumSubarrayLength({5, 1, 2}, 7) == 3); // [5,1,2] OR=7
    // k = 0 always works with any single element
    assert(minimumSubarrayLength({10, 20}, 0) == 1);
    // Single element sufficient
    assert(minimumSubarrayLength({8, 1, 2}, 8) == 1);
    // No single element, but combined works
    assert(minimumSubarrayLength({4, 2, 1}, 7) == 3);
    // Larger array with multiple valid subarrays
    assert(minimumSubarrayLength({1, 2, 4, 8}, 12) == 2); // [4,8] OR=12
    assert(minimumSubarrayLength({1, 2, 4, 8}, 14) == 3); // [2,4,8] OR=14
    // All zeros and k positive
    assert(minimumSubarrayLength({0, 0, 0}, 1) == -1);
    // Duplicates and k just above one element
    assert(minimumSubarrayLength({3, 3, 3}, 3) == 1);
    // Test where shrinking is tricky (rightmost element needed)
    assert(minimumSubarrayLength({1, 1, 2}, 2) == 1);
    return 0;
}
