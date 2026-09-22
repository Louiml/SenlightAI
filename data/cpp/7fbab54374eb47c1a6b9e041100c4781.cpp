// Write a C++ function named `findEquilibriumIndex` that takes a `const std::vector<int>&` and returns the **pivot index** — the index where the sum of all elements strictly to the left equals the sum of all elements strictly to the right. If no such index exists, return `-1`. The input vector may be empty (then return `-1`), may contain negative numbers, and may have multiple valid pivot indices (return the leftmost one). The function must be `const`-correct (i.e., it does not modify the input), and must handle edge cases such as a single-element vector (pivot index `0` because both left and right sums are zero) and vectors where the pivot is at the first or last position (the sum on the respective side is zero).

// The core algorithm is straightforward: compute the total sum of the vector using `std::accumulate`. Then iterate through each index, maintaining a running `leftSum` (the sum of elements before the current index). For each index `i`, the right sum is `totalSum - leftSum - nums[i]`. Compare `rightSum` with `leftSum`; if they are equal, return `i`. Otherwise, add `nums[i]` to `leftSum` and continue. If no match is found, return `-1`.
//
// Edge cases:  
// - Empty vector: `totalSum = 0`, loop does not execute, returns `-1`.  
// - Single element: `totalSum = nums[0]`. For `i=0`, `leftSum=0`, `rightSum = totalSum - 0 - nums[0] = 0`, so returns `0`.  
// - Negative numbers: works because sums are computed arithmetically.  
// - Multiple pivots: since we return on the first match, we get the leftmost.  
// - Pivot at first index: requires `totalSum - nums[0] == 0`, i.e., `totalSum == nums[0]` (meaning rest sum is zero). Works.
//
// Time complexity: `O(n)` for the total sum computation plus `O(n)` for the loop, so overall `O(n)`. Space complexity: `O(1)` auxiliary space, not counting the input vector.

#include <vector>
#include <numeric>  // for std::accumulate

// Returns the leftmost pivot index where left sum equals right sum, or -1.
int findEquilibriumIndex(const std::vector<int>& nums) {
    const int totalSum = std::accumulate(nums.begin(), nums.end(), 0);
    int leftSum = 0;

    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        const int rightSum = totalSum - leftSum - nums[i];
        if (rightSum == leftSum) {
            return i;
        }
        leftSum += nums[i];
    }
    return -1;
}

#include <cassert>
#include <vector>

int main() {
    // Example from typical problem
    std::vector<int> nums1 = {1, 7, 3, 6, 5, 6};
    assert(findEquilibriumIndex(nums1) == 3);

    // No pivot exists
    std::vector<int> nums2 = {1, 2, 3};
    assert(findEquilibriumIndex(nums2) == -1);

    // Pivot at first index (left sum = 0)
    std::vector<int> nums3 = {2, -1, 1};
    assert(findEquilibriumIndex(nums3) == 0);

    // Pivot at last index (right sum = 0)
    std::vector<int> nums4 = {1, 1, -1, 0};
    assert(findEquilibriumIndex(nums4) == 3);

    // Single element
    std::vector<int> nums5 = {5};
    assert(findEquilibriumIndex(nums5) == 0);

    // Empty vector
    std::vector<int> nums6 = {};
    assert(findEquilibriumIndex(nums6) == -1);

    // Negative numbers and multiple pivots, returns leftmost
    std::vector<int> nums7 = {0, 0, 0, 0};
    assert(findEquilibriumIndex(nums7) == 0);

    // Typical with negatives
    std::vector<int> nums8 = {-1, -1, 0, 1, 1};
    assert(findEquilibriumIndex(nums8) == 2);

    // Larger vector, pivot in middle
    std::vector<int> nums9 = {1, 2, 3, 4, 5, 6, 21};
    assert(findEquilibriumIndex(nums9) == 5);

    return 0;
}
