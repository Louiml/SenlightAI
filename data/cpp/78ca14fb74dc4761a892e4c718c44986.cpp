/*
Write a C++ function named `maxSubarraySum` that takes a non-empty array of integers (via a pointer and its size) and returns the maximum possible sum of any contiguous subarray. Your solution must handle arrays containing all negative numbers, all positive numbers, and mixed positive/negative values, and must run in linear time using constant extra space. The function should be `const`-correct, meaning it must not modify the input array.
*/

#include <algorithm> // for std::max

// Returns the maximum sum of any contiguous subarray in arr[0..n-1].
// Assumes n > 0.
int maxSubarraySum(const int* arr, int n) {
    int max_ending_here = 0;
    int max_so_far = arr[0]; // Initialize with first element to handle all-negative arrays

    for (int i = 0; i < n; ++i) {
        max_ending_here = std::max(arr[i], max_ending_here + arr[i]);
        max_so_far = std::max(max_so_far, max_ending_here);
    }
    return max_so_far;
}

#include <cassert>

int main() {
    // Mixed positive and negative
    int arr1[] = {3, 4, -5, 8, -12, 7, 6, -2};
    assert(maxSubarraySum(arr1, 8) == 15); // subarray [3,4,-5,8] or [7,6]

    // All negative
    int arr2[] = {-3, -1, -2};
    assert(maxSubarraySum(arr2, 3) == -1);

    // All positive
    int arr3[] = {1, 2, 3, 4};
    assert(maxSubarraySum(arr3, 4) == 10);

    // Single element
    int arr4[] = {5};
    assert(maxSubarraySum(arr4, 1) == 5);

    // Single negative
    int arr5[] = {-7};
    assert(maxSubarraySum(arr5, 1) == -7);

    // Mixed with zeros
    int arr6[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    assert(maxSubarraySum(arr6, 9) == 6); // subarray [4,-1,2,1]

    // Zero and negative
    int arr7[] = {0, -1, 0};
    assert(maxSubarraySum(arr7, 3) == 0);

    // Two elements
    int arr8[] = {-1, -2};
    assert(maxSubarraySum(arr8, 2) == -1);

    // Large numbers
    int arr9[] = {1000000, -1, 1000000};
    assert(maxSubarraySum(arr9, 3) == 1999999);

    // Alternating signs
    int arr10[] = {5, -2, 5};
    assert(maxSubarraySum(arr10, 3) == 8);

    return 0;
}

// The standard algorithm for this problem is Kadane's algorithm, which maintains a running prefix sum that is reset to zero whenever it becomes negative. This works because any negative prefix can never contribute positively to a future subarray, so discarding it guarantees we only consider subarrays that could yield a maximum sum. We track the largest prefix sum seen so far. Edge cases: if all numbers are negative, the algorithm will correctly return the least negative number (the largest value) because when the prefix becomes negative it resets, and the maximum is updated before the reset. If the array has a single element, that element is returned. Time complexity is O(n) with a single pass; space complexity is O(1) auxiliary. The function must not modify the input, so the parameter is `const int*`.
