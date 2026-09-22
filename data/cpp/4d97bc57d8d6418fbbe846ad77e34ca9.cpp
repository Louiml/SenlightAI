// Write a C++ function named `maxSubarraySum` that takes a `const std::vector<int>&` and returns the maximum sum of any non-empty contiguous subarray (Kadane's algorithm). The input vector will contain at least one element and may include negative numbers, zeros, and duplicates. The function must handle the case where all elements are negative (returning the largest single element) and where the maximum subarray is the entire array. The solution must be implemented as a standalone free function with proper `const` correctness and include all necessary headers.
The core idea is Kadane’s algorithm, which maintains a running sum of the current subarray (`currentSum`) and resets it to the current element whenever the running sum becomes smaller than that element alone. This works because if the previous subarray sum is negative, adding it to the next element only reduces the total, so we start fresh from that element. Simultaneously, we track the best sum seen so far (`bestSum`) by updating it whenever the current sum exceeds it. Edge cases:  
- If all elements are negative, the algorithm still finds the maximum (least negative) single element because the reset condition (`currentSum = std::max(currentSum + value, value)`) will always pick the larger (less negative) value.  
- If the array has one element, it is both the current and best sum.  
- Zeros are handled naturally (they may extend a subarray without reducing the sum).  
Time complexity is \(O(n)\) with a single pass, and space complexity is \(O(1)\) auxiliary, ignoring the input vector.
#include <vector>
#include <algorithm>

// Return the maximum sum of any non-empty contiguous subarray.
int maxSubarraySum(const std::vector<int>& nums) {
    int currentSum = nums[0];
    int bestSum = nums[0];

    for (size_t i = 1; i < nums.size(); ++i) {
        int value = nums[i];
        currentSum = std::max(currentSum + value, value);
        bestSum = std::max(bestSum, currentSum);
    }

    return bestSum;
}
#include <cassert>
#include <vector>

// Include the solution function here (or link it).

int main() {
    std::vector<int> arr1 = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    assert(maxSubarraySum(arr1) == 6);

    std::vector<int> arr2 = {1};
    assert(maxSubarraySum(arr2) == 1);

    std::vector<int> arr3 = {-1, -2, -3};
    assert(maxSubarraySum(arr3) == -1);

    std::vector<int> arr4 = {5, 4, -1, 7, 8};
    assert(maxSubarraySum(arr4) == 23);

    std::vector<int> arr5 = {0, 0, 0};
    assert(maxSubarraySum(arr5) == 0);

    std::vector<int> arr6 = {-2, -1};
    assert(maxSubarraySum(arr6) == -1);

    std::vector<int> arr7 = {10, -2, 3, -1, 5};
    assert(maxSubarraySum(arr7) == 15);

    std::vector<int> arr8 = {-5};
    assert(maxSubarraySum(arr8) == -5);

    std::vector<int> arr9 = {2, -1, 2};
    assert(maxSubarraySum(arr9) == 3);

    std::vector<int> arr10 = {-1, 0, -1};
    assert(maxSubarraySum(arr10) == 0);

    return 0;
}
