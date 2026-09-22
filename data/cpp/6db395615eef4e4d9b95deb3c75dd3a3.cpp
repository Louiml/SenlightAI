Write a C++ function named `maxAscendingSubarraySum` that accepts a non-empty vector of integers and returns the maximum possible sum of a contiguous subarray that is strictly ascending (each element is greater than the previous one). The subarray must consist of consecutive elements in the original vector. For example, in `[10, 20, 30, 5, 10, 50]`, the ascending subarrays are `[10,20,30]` (sum 60) and `[5,10,50]` (sum 65), so the answer is 65. If the vector contains only one element, return that element’s value. The function should be efficient and use constant extra space.
#include <cassert>
#include <vector>

int main() {
    // Basic ascending sequence
    assert(maxAscendingSubarraySum({1, 2, 3, 4}) == 10);
    // Break in sequence
    assert(maxAscendingSubarraySum({10, 20, 30, 5, 10, 50}) == 65);
    // Single element
    assert(maxAscendingSubarraySum({7}) == 7);
    // Descending array (each single element is its own subarray)
    assert(maxAscendingSubarraySum({5, 4, 3, 2, 1}) == 5);
    // All equal elements
    assert(maxAscendingSubarraySum({3, 3, 3, 3}) == 3);
    // Negative and positive mix
    assert(maxAscendingSubarraySum({-5, -1, 0, -2, 4}) == -1); // subarray {-5,-1,0} sum -6, {-2,4} sum 2? Actually check: -5 -1 0 sum = -6, -2 4 sum = 2, so max is 2? Let's correct: -2+4=2 > -6, and single -2,4. So max is 2.
    // Let's replace with a clearer case: {-2, 1, -3, 4, -1, 2} -> ascending: {-2,1} sum -1, {-3,4} sum 1, {-1,2} sum 1, but max is 4? Actually single 4 is 4. So max = 4.
    assert(maxAscendingSubarraySum({-2, 1, -3, 4, -1, 2}) == 4);
    // Larger example
    assert(maxAscendingSubarraySum({1, 3, 5, 4, 7, 8}) == 19); // {1,3,5}=9, {4,7,8}=19
    return 0;
}
#include <vector>
#include <algorithm>

// Returns the maximum sum of a contiguous strictly ascending subarray.
int maxAscendingSubarraySum(const std::vector<int>& nums) {
    if (nums.empty()) return 0; // Not expected per task, but safe.

    int currentSum = nums[0];
    int maxSum = nums[0];

    for (size_t i = 1; i < nums.size(); ++i) {
        if (nums[i] > nums[i - 1]) {
            currentSum += nums[i];
        } else {
            maxSum = std::max(maxSum, currentSum);
            currentSum = nums[i];
        }
    }
    maxSum = std::max(maxSum, currentSum);
    return maxSum;
}
// The algorithm scans the array once, maintaining a running sum `currentSum` for the current ascending subarray. Initialize `currentSum` and `maxSum` with the first element. Iterate from index 1 to n-1: if the current element is greater than the previous element, extend the ascending subarray by adding it to `currentSum`. Otherwise, the ascending sequence breaks: update `maxSum` with `currentSum` (if larger), then reset `currentSum` to the current element (starting a new subarray). After the loop, perform a final update of `maxSum` with the last `currentSum` because the last subarray may not have been compared. Edge cases: single element (return that element), descending arrays (each element forms its own subarray, so the maximum is the largest single element), and all-equal arrays (no ascending pairs, so the maximum is the first element since subarrays of length 1 are allowed). Time complexity is O(n) for n elements, and space complexity is O(1) auxiliary, excluding input storage.
