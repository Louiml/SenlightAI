Write a C++ function `int maximumSubarraySum(const std::vector<int>& nums)` that takes a non-empty vector of integers and returns the maximum sum of any non-empty contiguous subarray. A contiguous subarray is a consecutive sequence of one or more elements from the original array. The function must handle arrays containing all negative numbers correctly — in that case, the maximum subarray sum is the largest (least negative) element itself, since any non-empty subarray must include at least one element. Do not use any external libraries beyond the standard C++ library.

The problem is a classic Kadane's algorithm. The key idea is to maintain two variables while iterating through the array: `currentSum`, which stores the maximum sum of a subarray ending at the current position, and `bestSum`, which stores the maximum subarray sum seen so far. For each element, we update `currentSum` as `max(element, currentSum + element)` — this decides whether to start a new subarray at the current element (if adding it to the previous sum would make it smaller than the element itself, e.g., if the previous sum is negative) or to extend the existing subarray. Then we update `bestSum` as `max(bestSum, currentSum)`. This approach automatically handles all-negative arrays: for the first element, `currentSum` becomes that (negative) element, and `bestSum` becomes that value; for subsequent elements, `currentSum = max(element, currentSum + element)` will always be the element itself (since the sum of two negatives is smaller than the larger of them), so `bestSum` ends up as the maximum element. Edge cases include a single-element array (returns that element) and arrays with zeros or duplicate values. Time complexity is O(n) for n elements, and space complexity is O(1) auxiliary space (ignoring the input vector's storage). The algorithm works because it efficiently explores all possible contiguous subarrays in a single pass without needing to store intermediate results.

#include <vector>
#include <algorithm>

// Returns the maximum sum of any non-empty contiguous subarray.
// Handles arrays with all negative numbers by returning the maximum element.
int maximumSubarraySum(const std::vector<int>& nums) {
    if (nums.empty()) {
        return 0; // For empty input, return 0 to avoid undefined behavior.
        // The problem specification says non-empty, but we handle it gracefully.
    }

    int currentSum = nums[0]; // Maximum sum of a subarray ending at current index
    int bestSum = nums[0];    // Maximum sum of any subarray seen so far

    for (size_t i = 1; i < nums.size(); ++i) {
        // Either extend the existing subarray or start a new one at nums[i]
        currentSum = std::max(nums[i], currentSum + nums[i]);
        bestSum = std::max(bestSum, currentSum);
    }

    return bestSum;
}

#include <cassert>
#include <vector>

// The solution function is assumed to be defined above.
int main() {
    // Basic positive and mixed arrays
    assert(maximumSubarraySum({1, 2, 3}) == 6);
    assert(maximumSubarraySum({-2, 1, -3, 4, -1, 2, 1, -5, 4}) == 6); // subarray [4,-1,2,1]
    
    // All negative numbers — maximum is the largest (least negative) element
    assert(maximumSubarraySum({-1, -2, -3}) == -1);
    assert(maximumSubarraySum({-5, -1, -10}) == -1);
    
    // Single element
    assert(maximumSubarraySum({7}) == 7);
    assert(maximumSubarraySum({-4}) == -4);
    
    // Edge cases with zeros and duplicates
    assert(maximumSubarraySum({0, 0, 0}) == 0);
    assert(maximumSubarraySum({3, -1, 3}) == 5); // subarray [3,-1,3]
    assert(maximumSubarraySum({10, -20, 5, 5, 5}) == 15); // subarray [5,5,5]
    
    // Large positive values to ensure no overflow (within int range)
    assert(maximumSubarraySum({1000000, 1000000, -1, 1000000}) == 3000000);
    
    return 0;
}
