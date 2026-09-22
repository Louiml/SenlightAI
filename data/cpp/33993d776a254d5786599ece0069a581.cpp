Write a C++ function named `maxAverageSubarray` that takes a non-empty vector of integers `nums` and an integer `k` (where `1 <= k <= nums.size()`) and returns the maximum average value of any contiguous subarray of length exactly `k`. The function should return the result as a `double`. Handle edge cases such as a single-element array, negative numbers, and `k` equal to the full array length appropriately. The solution must use a sliding window approach for efficiency.
// The optimal approach uses a fixed-size sliding window of length `k`. Initialize a variable `sum` to the sum of the first `k` elements. Then, compute the initial average and treat it as the current maximum. Slide the window one element at a time by adding the next element and subtracting the element that leaves the window, updating the sum and comparing the new average with the current maximum. Continue until the window reaches the end of the array. Edge cases include: if `nums.size() == 1` (the only element is the average), if `k == nums.size()` (only one window), and arrays with negative values (averages can be negative, so initialize the maximum to a very small value like `-INFINITY`). Time complexity is \(O(n)\) where `n` is the size of `nums`, and space complexity is \(O(1)\) auxiliary.
#include <vector>
#include <limits>
#include <algorithm>

// Returns the maximum average of any contiguous subarray of length exactly k.
double maxAverageSubarray(const std::vector<int>& nums, int k) {
    double sum = 0.0;
    // Compute sum of the first k elements.
    for (int i = 0; i < k; ++i) {
        sum += nums[i];
    }
    double maxAverage = sum / k;

    // Slide the window from k to the end.
    for (int i = k; i < static_cast<int>(nums.size()); ++i) {
        sum += nums[i] - nums[i - k];
        maxAverage = std::max(maxAverage, sum / k);
    }

    return maxAverage;
}
#include <cassert>
#include <vector>

double maxAverageSubarray(const std::vector<int>& nums, int k);

int main() {
    // Basic test
    std::vector<int> nums1 = {1, 12, -5, -6, 50, 3};
    assert(maxAverageSubarray(nums1, 4) == 12.75);

    // Single element
    std::vector<int> nums2 = {5};
    assert(maxAverageSubarray(nums2, 1) == 5.0);

    // All negative numbers
    std::vector<int> nums3 = {-1, -2, -3, -4};
    assert(maxAverageSubarray(nums3, 2) == -1.5);

    // k equals full length
    std::vector<int> nums4 = {10, 20, 30};
    assert(maxAverageSubarray(nums4, 3) == 20.0);

    // Mixed positives and negatives with k=1 (should be the maximum element)
    std::vector<int> nums5 = {-5, 3, -1, 9};
    assert(maxAverageSubarray(nums5, 1) == 9.0);

    // Duplicate values
    std::vector<int> nums6 = {7, 7, 7};
    assert(maxAverageSubarray(nums6, 2) == 7.0);

    // Large k
    std::vector<int> nums7 = {1, 2, 3, 4, 5};
    assert(maxAverageSubarray(nums7, 4) == 3.5);

    // Check with floating-point precision
    std::vector<int> nums8 = {2, 4, 6};
    assert(maxAverageSubarray(nums8, 2) == 5.0);

    // Negative and positive mixed
    std::vector<int> nums9 = {-10, 5, -2, 8, -1};
    assert(maxAverageSubarray(nums9, 3) == 3.6666666666666665);

    // Empty edge case not needed, but ensure k=1 for a single negative
    std::vector<int> nums10 = {-3};
    assert(maxAverageSubarray(nums10, 1) == -3.0);

    return 0;
}
