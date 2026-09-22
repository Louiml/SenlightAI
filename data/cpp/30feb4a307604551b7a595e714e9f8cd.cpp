/*
Write a C++ function named `maxSubarrayLengthAfterOneRemoval` that takes a non-empty `std::vector<int>` containing only 0s and 1s, and returns the length of the longest contiguous subarray consisting entirely of 1s that can be obtained by deleting exactly one element from the original array. The deletion must remove exactly one element (any position, not necessarily a 0), and the resulting subarray of 1s must come from the remaining contiguous elements. If no such subarray can be formed (i.e., all elements are 0 or the array has size 1 that must be deleted), return 0. For example, for `{1,1,0,1}`, deleting the 0 gives a subarray of length 3, but deleting the first 1 gives `{1,0,1}` with no contiguous 1s longer than 1, so the answer is 3. For `{0,0,0}`, any deletion leaves only zeros, so return 0.
*/
#include <vector>
#include <algorithm>

// Returns the length of the longest contiguous subarray of 1s obtainable by deleting exactly one element.
int maxSubarrayLengthAfterOneRemoval(const std::vector<int>& nums) {
    int low = 0;
    int high = 0;
    int zeroCount = 0;
    int n = static_cast<int>(nums.size());
    
    while (high < n) {
        if (nums[high] == 0) {
            ++zeroCount;
        }
        if (zeroCount > 1) {
            if (nums[low] == 0) {
                --zeroCount;
            }
            ++low;
        }
        ++high;
    }
    
    // The window [low, high-1] contains at most one zero.
    // After deleting exactly one element from this window, the max ones is (window size - 1).
    int windowSize = high - low;
    return std::max(0, windowSize - 1);
}
#include <cassert>
#include <vector>

int main() {
    // All ones: must delete one, so length = n-1
    assert(maxSubarrayLengthAfterOneRemoval({1,1,1}) == 2);
    // Provided example
    assert(maxSubarrayLengthAfterOneRemoval({1,1,0,1}) == 3);
    // All zeros: any deletion leaves only zeros -> 0
    assert(maxSubarrayLengthAfterOneRemoval({0,0,0}) == 0);
    // Single one: must delete it, so 0
    assert(maxSubarrayLengthAfterOneRemoval({1}) == 0);
    // Single zero: must delete it, result 0
    assert(maxSubarrayLengthAfterOneRemoval({0}) == 0);
    // Mixed: delete middle zero to get length 2
    assert(maxSubarrayLengthAfterOneRemoval({1,0,1}) == 1);
    // Larger case: delete the first 0 to get length 4
    assert(maxSubarrayLengthAfterOneRemoval({0,1,1,1,1,0,1}) == 4);
    // Edge: two zeros separated by ones, best is side segment after one deletion
    assert(maxSubarrayLengthAfterOneRemoval({1,0,1,0,1}) == 2);
    // All ones with one zero at end
    assert(maxSubarrayLengthAfterOneRemoval({1,1,1,0}) == 3);
    // Long sequence with many ones and one zero in middle
    assert(maxSubarrayLengthAfterOneRemoval({1,1,1,0,1,1}) == 4);
    
    return 0;
}
// The solution uses a sliding window with two pointers (`low` and `high`) that maintains a window containing at most one zero. The right pointer `high` expands the window each iteration. When the number of zeros in the window exceeds 1, the left pointer `low` moves right, and if the element at `low` is a zero, the zero count decreases. After adjusting, the window size is `high - low`, but because we must delete exactly one element (the one zero inside the window if present, or an arbitrary 1 if no zero), the maximum valid subarray of 1s after deletion is `high - low - 1`. This works because the window may contain at most one zero; if the window has exactly one zero, we delete that zero and get `high - low - 1` ones. If the window has zero zeros, we must still delete one element, so we delete one of the 1s, leaving `high - low - 1` ones. Edge case: if all elements are 1s, the window will cover the entire array with zero zeros, and the result is `n - 1` (since we must delete one 1). If the array size is 1 and contains 1, the result is 0; if it contains 0, result is 0. Time complexity is O(n) since each pointer moves at most n times; space complexity is O(1).
