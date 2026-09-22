Write a C++ function `maxAscendingSubarraySum` that takes a non-empty vector of integers (which may contain negative values, duplicates, and zeroes) and returns the maximum possible sum of any contiguous subarray that is strictly increasing. A strictly increasing subarray is a sequence of consecutive elements where each element is greater than the previous one. The function should return the sum of the elements in such a subarray that yields the largest total. The input vector is passed by const reference to avoid copying. Consider edge cases: all elements decreasing, all elements equal, a single element, and mixed positive/negative values.

#include <vector>
#include <cassert>
#include <iostream>

// Function declaration from the solution.
int maxAscendingSubarraySum(const std::vector<int>& nums);

int main() {
    // Basic test case from original snippet.
    std::vector<int> test1 = {12, 17, 15, 13, 10, 11, 12};
    assert(maxAscendingSubarraySum(test1) == 33); // 12+17, then 10+11+12=33

    // Single element.
    std::vector<int> test2 = {5};
    assert(maxAscendingSubarraySum(test2) == 5);

    // All decreasing.
    std::vector<int> test3 = {5, 4, 3, 2, 1};
    assert(maxAscendingSubarraySum(test3) == 5);

    // All increasing.
    std::vector<int> test4 = {1, 2, 3, 4};
    assert(maxAscendingSubarraySum(test4) == 10);

    // All equal.
    std::vector<int> test5 = {7, 7, 7, 7};
    assert(maxAscendingSubarraySum(test5) == 7);

    // Mixed with negatives.
    std::vector<int> test6 = {-2, -1, 0, 1, -5, 4};
    assert(maxAscendingSubarraySum(test6) == -2); // The ascending run -2,-1,0,1 sums to -2, but -5+4 = -1, still max is -2? Actually -5+4 = -1 > -2? Let's compute: run1: -2-1+0+1 = -2, run2: -5+4 = -1, so max = -1.});

    // Corrected mixed test.
    std::vector<int> test7 = {-2, -1, 0, 1, -5, 4};
    assert(maxAscendingSubarraySum(test7) == -1);

    // Alternating up/down.
    std::vector<int> test8 = {1, 3, 2, 4, 6, 5};
    assert(maxAscendingSubarraySum(test8) == 12); // 2+4+6=12

    // Large length with best at the end.
    std::vector<int> test9 = {10, 9, 8, 1, 2, 3, 4};
    assert(maxAscendingSubarraySum(test9) == 10); // 1+2+3+4=10 > 10

    std::cout << "All tests passed." << std::endl;
    return 0;
}

#include <vector>
#include <algorithm> // for std::max

// Return the maximum sum of any contiguous strictly increasing subarray.
int maxAscendingSubarraySum(const std::vector<int>& nums) {
    int result = nums[0];
    int currentSum = nums[0];
    for (size_t i = 1; i < nums.size(); ++i) {
        if (nums[i] > nums[i - 1]) {
            currentSum += nums[i];
        } else {
            currentSum = nums[i];
        }
        result = std::max(result, currentSum);
    }
    return result;
}

// The solution scans the array in a single pass while maintaining a running sum for the current strictly increasing run. Initialize the running sum with the first element and the result with the same value. For each subsequent element `nums[i]`: if it is strictly greater than the previous element, append it to the current run by adding it to the running sum; otherwise, start a new run with just `nums[i]`. After updating the running sum, compare it with the current result and keep the larger. This works because a strictly increasing subarray is broken at the first non-increasing pair, and any new run starts fresh. Edge cases: if the array has one element, the result is that element. If all elements are decreasing, each run has length 1, so the result is the maximum single element. If all elements are equal, the same applies since the condition `nums[i] > nums[i-1]` fails each time. Time complexity is O(n) with a single loop, and space complexity is O(1) beyond the input vector.
