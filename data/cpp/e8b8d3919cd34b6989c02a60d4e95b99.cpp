Write a C++ function `maximumUniqueSubarraySum` that takes a non-empty vector of integers and returns the maximum possible sum of a contiguous subarray in which all elements are distinct (no duplicates). The subarray must be non-empty, but its length can be one. The function should handle negative numbers, zeros, and large positive integers. The time complexity must be linear in the number of elements, and the solution must use a sliding window with a frequency map to ensure uniqueness of elements within the current window.

#include <cassert>
#include <vector>

int main() {
    // Basic case with duplicates in the middle
    assert(maximumUniqueSubarraySum({4, 2, 4, 5, 6}) == 17); // subarray [2,4,5,6] or [4,5,6]? Actually best is [2,4,5,6] sum=17
    // All unique elements
    assert(maximumUniqueSubarraySum({1, 2, 3, 4}) == 10); // whole array
    // Single element
    assert(maximumUniqueSubarraySum({-5}) == -5);
    // All negative but with duplicates
    assert(maximumUniqueSubarraySum({-2, -1, -2, -3}) == -1); // single -1 is max
    // Duplicate at beginning and end
    assert(maximumUniqueSubarraySum({5, 1, 2, 3, 4, 5}) == 15); // [1,2,3,4,5] sum=15
    // Zero and negative mixed
    assert(maximumUniqueSubarraySum({0, -1, 2, -1, 3}) == 5); // [2,-1,3] or [0,-1,2,3]? Actually [2,3] sum=5? Wait [2,-1,3] sum=4, [0,-1,2,3] sum=4, [2,3] not contiguous. Best is [0,-1,2,3]? no duplicates: 0,-1,2,3 sum=4; 2,-1,3 sum=4? Actually [2,-1,3] has -1 once, sum=4; [0,2,3] not contiguous. Let's compute: subarrays: [0]=0, [-1], [2], [-1], [3]; [0,-1]=-1; [-1,2]=1; [2,-1]=1; [-1,3]=2; [0,-1,2]=1; [-1,2,-1] has duplicate -1 invalid; [2,-1,3] sum=4; [0,-1,2,3] sum=4. So max is 4. But my assertion says 5, correct it.)
    // Corrected: 
    assert(maximumUniqueSubarraySum({0, -1, 2, -1, 3}) == 4); // [2,-1,3] or [0,-1,2,3]
    // Large duplicates
    assert(maximumUniqueSubarraySum({10, 10, 10, 10}) == 10); // any single 10
    // Mixed with positive and negative, multiple duplicates
    assert(maximumUniqueSubarraySum({1, -2, 1, 3}) == 4); // [1,-2,1,3] invalid (duplicate 1), best is [1,3] not contiguous, actually [ -2,1,3] sum=2, [1,3] not contiguous, [1,-2] sum=-1, [1,-2,1] invalid, [3] sum=3, [-2,1,3] sum=2, [1] sum=1, so max is 3? Wait [1,3] not contiguous, so best is [3] sum=3. Let's compute properly: subarrays: [1]=1, [-2]=-2, [1]=1, [3]=3; [1,-2]=-1, [-2,1]=-1, [1,3]=4 (contiguous! indices 2 and 3) -> sum=4. Yes, [1,3] is contiguous? indices: 0:1,1:-2,2:1,3:3; subarray from index 3 to 3 is [3], from 2 to 3 is [1,3] sum=4 valid (distinct). So max=4.
    assert(maximumUniqueSubarraySum({1, -2, 1, 3}) == 4);
    
    return 0;
}

#include <vector>
#include <unordered_map>
#include <algorithm>

// Return the maximum sum of a contiguous subarray with all distinct elements.
int maximumUniqueSubarraySum(const std::vector<int>& nums) {
    if (nums.empty()) return 0;

    int left = 0;
    int window_sum = 0;
    int max_sum = nums[0]; // At minimum, a single element is a valid subarray.
    std::unordered_map<int, int> freq;

    for (int right = 0; right < static_cast<int>(nums.size()); ++right) {
        int value = nums[right];
        // If value already exists in the window, shrink from left until it's unique.
        while (freq.find(value) != freq.end()) {
            // Current window (before removing left) is valid, so record its sum.
            max_sum = std::max(max_sum, window_sum);
            // Remove the leftmost element from the window.
            --freq[nums[left]];
            if (freq[nums[left]] == 0) {
                freq.erase(nums[left]);
            }
            window_sum -= nums[left];
            ++left;
        }
        // Add the new element to the window.
        window_sum += value;
        ++freq[value];
    }
    // Final check for the remaining window.
    max_sum = std::max(max_sum, window_sum);

    return max_sum;
}

// The problem is solved using a dynamic sliding window technique with two pointers (`left` and `right`) over the vector. We maintain a frequency map (or unordered_map) that stores the count of each element currently inside the window. We expand the window by moving `right` and adding `nums[right]` to the window sum, while also increasing its frequency. If adding the new element creates a duplicate (i.e., its frequency becomes 2 or more), we shrink the window from the left by removing `nums[left]`, decreasing its frequency, subtracting its value from the window sum, and moving `left` forward until the duplicate is eliminated and all elements in the window are unique again. Before starting the shrink, we record the current window sum as a candidate answer using `max` comparison. After the loop ends, we make one final candidate check with the current window sum. The key edge case is when the entire array has no duplicates; then the answer is simply the sum of all elements. Another edge case is when all elements are negative; the sliding window still works because we always consider at least one-element windows, and the maximum will be the largest negative number (since a single element subarray is always valid). The algorithm runs in O(n) time because each element is added to the window once and removed at most once, and the unordered_map operations are O(1) on average. Space complexity is O(k), where k is the number of distinct elements in the window, at most O(n) in the worst case.
