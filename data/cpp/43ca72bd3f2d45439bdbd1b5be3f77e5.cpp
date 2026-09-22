Write a C++ function named `maxUniqueSubarraySum` that takes a non-empty vector of integers and returns the maximum possible sum of a contiguous subarray in which all elements are distinct (no repeated values within the subarray). The subarray must be contiguous, meaning its elements appear consecutively in the original vector. The function should handle negative numbers, zeros, positives, and duplicate values throughout the array. For example, given `[4,2,4,5,6]`, the maximum sum of a distinct-element subarray is `17` (from subarray `[2,4,5,6]`), and for `[5,2,1,2,5,3,1]` the answer is `13` (from `[3,1]` or `[5,3,1]`). The function must not modify the input vector and should return an `int` result. Calculate time and space complexity in your analysis.
#include <cassert>
#include <vector>

// Function prototype (already defined above)
int maxUniqueSubarraySum(const std::vector<int>& nums);

int main() {
    // Basic cases
    assert(maxUniqueSubarraySum({4, 2, 4, 5, 6}) == 17);      // subarray [2,4,5,6] sum=17
    assert(maxUniqueSubarraySum({1, 2, 3, 4}) == 10);         // entire array, all distinct
    assert(maxUniqueSubarraySum({5, 2, 1, 2, 5, 3, 1}) == 13); // [5,3,1] or [3,1] -> 13
    assert(maxUniqueSubarraySum({1, 1, 1, 1}) == 1);          // single element subarray
    assert(maxUniqueSubarraySum({-5, -2, 4, -1, 2}) == 5);    // [4, -1, 2] sum=5
    assert(maxUniqueSubarraySum({10}) == 10);                 // single element
    assert(maxUniqueSubarraySum({3, -2, 3, 4, -1}) == 7);     // [3,4] sum=7 (not 3 + -2 + 3)
    assert(maxUniqueSubarraySum({0, 0, 0}) == 0);             // only zeros
    assert(maxUniqueSubarraySum({100, 100, 100, 50, 60}) == 210); // [100,50,60] sum=210
    assert(maxUniqueSubarraySum({-1, -2, -3, 10}) == 10);     // [10] alone is best

    return 0;
}
#include <vector>
#include <unordered_map>
#include <algorithm>

// Returns the maximum sum of a contiguous subarray with all distinct elements.
int maxUniqueSubarraySum(const std::vector<int>& nums) {
    std::unordered_map<int, int> lastIndex; // stores the most recent index of each element
    int left = 0;
    int currentSum = 0;
    int maxSum = 0;
    const int n = static_cast<int>(nums.size());

    for (int right = 0; right < n; ++right) {
        int value = nums[right];

        // If the value is already in the current window, shrink from the left
        if (lastIndex.find(value) != lastIndex.end()) {
            int repeatIndex = lastIndex[value];
            while (left <= repeatIndex) {
                lastIndex.erase(nums[left]);
                currentSum -= nums[left];
                ++left;
            }
        }

        // Now the window [left, right] has unique elements
        lastIndex[value] = right;
        currentSum += value;
        maxSum = std::max(maxSum, currentSum);
    }

    return maxSum;
}
// The main idea is to use a sliding window technique with two pointers (`left` and `right`). Maintain a running sum of elements in the current window and a hash map that records the last index at which each element was seen. As we move the `right` pointer from index 0 to the end, we check if the current element already exists in the map (meaning we’ve seen it at or after the `left` pointer). If so, we shrink the window from the left by moving `left` up to and including the stored index of the repeated element, removing those elements from the sum and erasing them from the map to ensure the window contains only distinct elements. Then we update the map with the new index of the current element, add it to the sum, and update the answer with the maximum sum seen so far. This ensures the window always contains unique elements, and we try all possible valid subarrays. Edge cases include an array with all identical elements (answer is that single element), an array with only one element, and cases where the maximum sum comes from a subarray that starts late (e.g., all negative numbers except one positive). Time complexity is O(n) because each element is added to and removed from the window at most once. Space complexity is O(n) in the worst case for the map, though typically O(k) where k is the number of distinct elements in the current window, but bounded by n.
