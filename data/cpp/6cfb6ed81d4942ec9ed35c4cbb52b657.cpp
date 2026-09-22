Write a C++ function that takes an array of integers and its size, and returns the maximum sum of any contiguous subarray (non-empty) within the array. The solution must handle arrays containing all negative numbers, all positive numbers, and a mix of both. Return an `int` representing the maximum subarray sum. The function should be named `maxSubarraySum` and should accept a `const int*` array and an integer size. Do not modify the input array.

The problem is classic "Maximum Subarray Sum" (Kadane's algorithm). The optimal approach is to iterate through the array once, maintaining a running `currentSum` that represents the best sum of a subarray ending at the current index, and a `bestSum` that tracks the maximum sum seen so far. For each element, update `currentSum` as the maximum of (the element itself, or `currentSum + element`). This single rule works for all cases: if all elements are negative, the element itself will be chosen, resulting in the least negative number; if there are positives, the running sum will drop when negatives occur but reset if the running sum becomes worse than starting fresh. Thus, no special handling for all-negative or mixed cases is needed, unlike the original snippet. Edge cases: an array of size 1 returns that single element; an array with all negatives returns the maximum (least negative) value. Time complexity is O(n) with O(1) auxiliary space.

#include <algorithm>

// Returns the maximum sum of a non-empty contiguous subarray.
int maxSubarraySum(const int* arr, int n) {
    if (n <= 0) return 0; // defensive, though problem expects n > 0

    int bestSum = arr[0];
    int currentSum = arr[0];

    for (int i = 1; i < n; ++i) {
        currentSum = std::max(arr[i], currentSum + arr[i]);
        bestSum = std::max(bestSum, currentSum);
    }

    return bestSum;
}

#include <cassert>

int main() {
    int arr1[] = {1, 2, 3, 4};
    assert(maxSubarraySum(arr1, 4) == 10);

    int arr2[] = {-1, -2, -3};
    assert(maxSubarraySum(arr2, 3) == -1);

    int arr3[] = {1, -2, 3, 5, -1, 2};
    assert(maxSubarraySum(arr3, 6) == 9); // subarray {3,5,-1,2} = 9

    int arr4[] = {-5};
    assert(maxSubarraySum(arr4, 1) == -5);

    int arr5[] = {0, 0, 0};
    assert(maxSubarraySum(arr5, 3) == 0);

    int arr6[] = {2, -1, 2, -1, 2};
    assert(maxSubarraySum(arr6, 5) == 4); // {2,-1,2,-1,2} = 4

    int arr7[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    assert(maxSubarraySum(arr7, 9) == 6); // {4,-1,2,1} = 6

    int arr8[] = {5, -1, -2, 10};
    assert(maxSubarraySum(arr8, 4) == 12); // {5,-1,-2,10} = 12

    int arr9[] = {100, -1, 0};
    assert(maxSubarraySum(arr9, 3) == 100); // {100} alone

    int arr10[] = {-1, -2, -3, -4};
    assert(maxSubarraySum(arr10, 4) == -1);
}
