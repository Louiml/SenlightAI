Write a C++ function that takes an integer array and its size, and determines whether the array contains a contiguous subarray whose elements sum to exactly zero. The function should return `true` if at least one such subarray exists, and `false` otherwise. A subarray is defined as a non-empty contiguous sequence of elements within the array. The array may contain positive numbers, negative numbers, and zeros. Handle edge cases such as an array with a single zero, an array with only non-zero elements, and arrays where the zero-sum subarray starts at the first element. The solution must be efficient for large arrays, with a time complexity of \(O(n)\) and auxiliary space of \(O(n)\). Provide the implementation as a standalone function named `hasZeroSumSubarray` that takes `const int arr[]` and `int n` as parameters, and returns a `bool`.

// The core idea is to use a prefix sum (cumulative sum) alongside a hash set (or hash map) to detect repeated sums. If the cumulative sum from the start of the array to index `i` equals the cumulative sum to some earlier index `j`, then the subarray between `j+1` and `i` sums to zero. Additionally, if the cumulative sum itself becomes zero at any point, then the subarray from index 0 to the current index sums to zero. A zero element in the array also trivially forms a zero-sum subarray of length 1. Therefore, we can iterate through the array once, maintaining a running sum. Before adding the current sum to the set, we check: (1) if the running sum is 0, (2) if the current element is 0, or (3) if the running sum has been seen before. If any condition holds, return true. Otherwise, insert the sum into the set. Important edge cases include an empty array (should return false), an array with a single zero, and arrays where a zero-sum subarray exists but not starting at index 0. The algorithm runs in \(O(n)\) time because each element is processed once, and each set insertion/lookup is \(O(1)\) on average. Space complexity is \(O(n)\) in the worst case due to storing up to `n` distinct prefix sums.

#include <unordered_set>

// Checks whether the array contains a contiguous subarray with sum zero.
bool hasZeroSumSubarray(const int arr[], int n) {
    std::unordered_set<int> prefixSums;
    int runningSum = 0;

    for (int i = 0; i < n; ++i) {
        runningSum += arr[i];

        // If current element is zero, or cumulative sum becomes zero,
        // or this cumulative sum was seen before, then a zero-sum subarray exists.
        if (arr[i] == 0 || runningSum == 0 || prefixSums.find(runningSum) != prefixSums.end()) {
            return true;
        }

        prefixSums.insert(runningSum);
    }

    return false;
}

#include <cassert>

int main() {
    // Basic cases
    int arr1[] = {4, 2, -3, 1, 6};
    assert(hasZeroSumSubarray(arr1, 5) == true);  // subarray {2, -3, 1}

    int arr2[] = {4, 2, 0, 1, 6};
    assert(hasZeroSumSubarray(arr2, 5) == true);  // single zero

    int arr3[] = {1, 2, 3};
    assert(hasZeroSumSubarray(arr3, 3) == false);

    // Zero-sum subarray starting at index 0
    int arr4[] = {-3, 1, 2, -4};
    assert(hasZeroSumSubarray(arr4, 4) == true);  // -3 + 1 + 2 = 0

    // Single element zero
    int arr5[] = {0};
    assert(hasZeroSumSubarray(arr5, 1) == true);

    // Single non-zero element
    int arr6[] = {5};
    assert(hasZeroSumSubarray(arr6, 1) == false);

    // Empty array (edge case)
    int arr7[] = {};
    assert(hasZeroSumSubarray(arr7, 0) == false);

    // All positive numbers
    int arr8[] = {2, 3, 4, 5};
    assert(hasZeroSumSubarray(arr8, 4) == false);

    // Repeated prefix sum not adjacent
    int arr9[] = {1, -1, 1, -1};
    assert(hasZeroSumSubarray(arr9, 4) == true);  // {1, -1} or {1, -1, 1, -1}

    // Large array with zero-sum in middle
    int arr10[] = {10, -5, 5, 7, -7};
    assert(hasZeroSumSubarray(arr10, 5) == true);  // {-5, 5} and {7, -7}
}
