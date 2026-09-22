// Write a C++ function `int longestZeroSumSubarrayLength(const int* arr, int size)` that takes an array of positive and negative integers and returns the length of the longest continuous (contiguous) subarray whose sum is exactly zero. The array may contain duplicates, large positive/negative values, and subarrays of length 1 (which sum to zero only if the element itself is zero). Return 0 if no zero-sum subarray exists. The function must be efficient enough to handle array sizes up to 10^5 within typical time limits, so avoid the O(n²) brute-force approach.

The optimal approach uses a hash map (e.g., `std::unordered_map`) to store the first occurrence index of each prefix sum. We iterate through the array once, maintaining a running `prefixSum`. For each position `i`, we insert `prefixSum` into the map if it has not been seen before. If `prefixSum` is already in the map, then the subarray from `(firstOccurrenceIndex + 1)` to `i` has sum zero, because the prefix sums are equal. We compute the length as `i - firstOccurrenceIndex` and track the maximum. This works because if the same prefix sum appears at two different indices, all elements between them sum to zero (due to cancellation). Edge cases: (1) If the entire array sum is zero, the first occurrence index of 0 will be -1 (which we must explicitly handle by initializing the map with `{0, -1}`), allowing a length of `i - (-1) = i+1`. (2) Single zero element gives prefix sum 0 at index 0, and with initial map entry `{0,-1}`, we get length `0 - (-1) = 1`. (3) If no zero-sum subarray exists, the map never finds a repeating prefix sum, and the function returns 0. Time complexity is O(n) average, O(n) worst-case due to hash collisions, and space complexity is O(n) for the map.

#include <unordered_map>

// Returns the length of the longest contiguous subarray with sum 0.
int longestZeroSumSubarrayLength(const int* arr, int size) {
    std::unordered_map<int, int> prefixFirstOccurrence;
    prefixFirstOccurrence.reserve(size * 2);
    prefixFirstOccurrence[0] = -1;  // handles subarray starting at index 0

    int prefixSum = 0;
    int maxLength = 0;

    for (int i = 0; i < size; ++i) {
        prefixSum += arr[i];

        auto it = prefixFirstOccurrence.find(prefixSum);
        if (it != prefixFirstOccurrence.end()) {
            int length = i - it->second;
            if (length > maxLength) {
                maxLength = length;
            }
        } else {
            prefixFirstOccurrence[prefixSum] = i;
        }
    }

    return maxLength;
}

#include <cassert>

int main() {
    // Basic test from prompt
    int arr1[] = {95, -97, -387, -435, -5, -70, 897, 127, 23, 284};
    assert(longestZeroSumSubarrayLength(arr1, 10) == 5);

    // Single zero element
    int arr2[] = {0};
    assert(longestZeroSumSubarrayLength(arr2, 1) == 1);

    // No zero-sum subarray
    int arr3[] = {1, 2, 3};
    assert(longestZeroSumSubarrayLength(arr3, 3) == 0);

    // Entire array sums to zero
    int arr4[] = {1, 2, -3};
    assert(longestZeroSumSubarrayLength(arr4, 3) == 3);

    // All zeros
    int arr5[] = {0, 0, 0, 0};
    assert(longestZeroSumSubarrayLength(arr5, 4) == 4);

    // Negative and positive balance
    int arr6[] = {5, -5, 10, -10, 3, -3, 7};
    // 5,-5 len 2; 10,-10 len 2; 3,-3 len 2; but 5,-5,10,-10 len 4; so max = 4
    assert(longestZeroSumSubarrayLength(arr6, 7) == 4);

    // Zero at end with previous prefix sum
    int arr7[] = {1, 2, -5, 2};
    // prefix sums: 1,3,-2,0; subarray (1,2,-5,2) len 4
    assert(longestZeroSumSubarrayLength(arr7, 4) == 4);

    // Mixed with zeros inside
    int arr8[] = {1, -1, 0, 5, -5};
    // 1,-1 len 2; 1,-1,0 len 3; 5,-5 len 2; max=3
    assert(longestZeroSumSubarrayLength(arr8, 5) == 3);

    // Empty array (size 0)
    int arr9[] = {};
    assert(longestZeroSumSubarrayLength(arr9, 0) == 0);

    return 0;
}
