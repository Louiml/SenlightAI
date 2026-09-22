// Write a C++ function named `maximumSubarraySum` that takes a vector of integers and returns the maximum possible sum of any contiguous subarray (Kadane's algorithm). The input may contain all negative numbers, all positive numbers, mixed positive/negative values, zeros, and the vector will have at least one element. The function should handle empty input by returning 0 (though the problem guarantees non-empty input, make the function robust), and should not modify the input vector. The return type should be `long long` to safely handle large inputs, and the function should be declared with proper `const` and reference parameters.
The solution uses Kadane's algorithm, which iterates through the array once while maintaining a running sum of the current subarray (`currentSum`). At each element, we add the element to `currentSum`. If `currentSum` exceeds the best sum seen so far (`bestSum`), we update `bestSum`. If `currentSum` becomes negative, we reset it to 0, because starting a new subarray from the current position will always yield a better sum than continuing a negative prefix. For arrays with all negative numbers, this reset ensures we pick the least negative element (since `bestSum` starts at the first element or at the minimum possible value). Edge cases: single-element arrays (return that element), all zeros (return 0), and large sums (use `long long` to prevent overflow). Time complexity: O(n) where n is the number of elements, and space complexity: O(1) extra space.
#include <vector>
#include <algorithm>
#include <cstdint>

// Returns the maximum sum of any contiguous subarray in the given vector.
// Handles empty input by returning 0. Uses Kadane's algorithm.
long long maximumSubarraySum(const std::vector<int>& nums) {
    if (nums.empty()) {
        return 0;
    }
    
    long long bestSum = nums[0];
    long long currentSum = 0;
    
    for (const int value : nums) {
        currentSum += value;
        bestSum = std::max(bestSum, currentSum);
        if (currentSum < 0) {
            currentSum = 0;
        }
    }
    
    return bestSum;
}
#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(maximumSubarraySum(std::vector<int>{-2, 1, -3, 4, -1, 2, 1, -5, 4}) == 6);
    assert(maximumSubarraySum(std::vector<int>{1}) == 1);
    
    // All positive
    assert(maximumSubarraySum(std::vector<int>{1, 2, 3, 4}) == 10);
    
    // All negative
    assert(maximumSubarraySum(std::vector<int>{-1, -2, -3}) == -1);
    assert(maximumSubarraySum(std::vector<int>{-5, -1, -10}) == -1);
    
    // Mixed with zeros
    assert(maximumSubarraySum(std::vector<int>{0, 5, -2, 0, 3}) == 6);
    
    // Large values
    assert(maximumSubarraySum(std::vector<int>{1000000, -1, 1000000}) == 2000000);
    
    // Empty
    assert(maximumSubarraySum(std::vector<int>{}) == 0);
    
    // All zeros
    assert(maximumSubarraySum(std::vector<int>{0, 0, 0}) == 0);
    
    return 0;
}
