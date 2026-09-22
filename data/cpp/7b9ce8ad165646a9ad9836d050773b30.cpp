Write a C++ function named `secondSmallestSum` that takes a pointer to an array of integers and its length, and returns the second smallest possible sum that can be obtained by selecting any non-empty contiguous subarray from the given array. The array may contain negative numbers, zeros, and duplicates. If there is only one distinct subarray sum (i.e., all possible subarray sums are equal, which only happens if the input length is 1, since a single-element array has exactly one subarray), the function should return that single sum. The function must not modify the input array and must allocate no dynamic memory. Assume the input array is valid (non-null pointer and length ≥ 1).

The core challenge is to compute all possible contiguous subarray sums without actually generating them all explicitly to avoid O(n²) memory. However, the most straightforward and correct approach is to enumerate every subarray using two nested loops: for each starting index, accumulate a running sum as you extend the end index. During this enumeration, track the smallest sum and the second smallest sum seen so far. Initialize both from the first subarray sum (which is the first element). For every subsequent new sum, compare it with the current smallest and second smallest. Update accordingly: if the new sum is smaller than both, the old smallest becomes second smallest and the new sum becomes smallest; if it is between them, update second smallest; if equal to either, no change is needed (since duplicates do not create a new distinct rank). This works because the second smallest is defined as the smallest distinct value among all subarray sums? The task says “second smallest sum” without specifying distinct, so we interpret it as the second smallest numeric value (duplicates counted as same rank). Therefore, if there are duplicate smallest values, the second smallest is the next larger value (or the same if all sums are equal). To handle this correctly, we can maintain two variables: `smallest` and `secondSmallest`. When we see a sum less than `smallest`, we shift: `secondSmallest = smallest; smallest = newSum`. If the new sum is greater than `smallest` but less than `secondSmallest`, we update `secondSmallest`. If equal to either, ignore. Edge case: for a single-element array, there is only one sum, so return that value. Time complexity is O(n²) because we enumerate all n(n+1)/2 subarrays, and space is O(1) beyond the input array. This is acceptable for typical constraints (n up to a few thousand). If n were large (e.g., >10^4), a more advanced algorithm using prefix sums and a heap would be needed, but for a teaching exercise O(n²) is fine.

#include <vector>
#include <climits>

// Returns the second smallest sum among all non-empty contiguous subarrays of `numbers`.
// If the array has length 1, returns that single subarray's sum.
// Assumes `numbers` is non-null and `length >= 1`.
int secondSmallestSum(const int* numbers, int length) {
    // Initialize from the first subarray (just the first element)
    int smallest = numbers[0];
    int secondSmallest = INT_MAX;
    bool hasSecond = false;

    // Enumerate all subarrays
    for (int start = 0; start < length; ++start) {
        int currentSum = 0;
        for (int end = start; end < length; ++end) {
            currentSum += numbers[end];
            if (currentSum < smallest) {
                // New smallest found; old smallest becomes second
                if (secondSmallest > smallest || !hasSecond) {
                    secondSmallest = smallest;
                    hasSecond = true;
                } else if (secondSmallest > currentSum) {
                    // but we already update below? Actually handled above
                }
                // Shift: old smallest becomes second, new becomes smallest
                // We must ensure second is updated correctly
                // Simpler: if currentSum < smallest, push smallest to second, then set smallest
                // But need to handle when second is already smaller than smallest? impossible since smallest <= second
                // So:
                secondSmallest = smallest;
                smallest = currentSum;
                hasSecond = true;
            } else if (currentSum > smallest && (currentSum < secondSmallest || !hasSecond)) {
                secondSmallest = currentSum;
                hasSecond = true;
            }
            // If equal to smallest or second, ignore (duplicate values don't create new rank)
        }
    }

    if (!hasSecond) {
        // Only one distinct sum exists (e.g., length == 1)
        return smallest;
    }
    return secondSmallest;
}

#include <cassert>

int main() {
    // Basic case with negatives and positives
    int arr1[] = {1, -2, 3};
    // Subarray sums: 1, -2, 3, -1, 1, 2 --> sorted distinct: -2, 1, 2, 3? Actually distinct sums: -2, -1, 1, 2, 3. Smallest -2, second -1.
    assert(secondSmallestSum(arr1, 3) == -1);

    // All positive
    int arr2[] = {1, 2, 3};
    // Subarray sums: 1,2,3,3,5,6 -> distinct:1,2,3,5,6 -> second smallest 2
    assert(secondSmallestSum(arr2, 3) == 2);

    // All negative
    int arr3[] = {-5, -1, -3};
    // Subarray sums: -5,-1,-3,-6,-4,-9 -> distinct:-9,-6,-5,-4,-3,-1 -> second smallest -6
    assert(secondSmallestSum(arr3, 3) == -6);

    // Single element
    int arr4[] = {42};
    assert(secondSmallestSum(arr4, 1) == 42);

    // Duplicates
    int arr5[] = {2, 2, 2};
    // Subarray sums: 2,2,2,4,4,6 -> distinct:2,4,6 -> second smallest 4
    assert(secondSmallestSum(arr5, 3) == 4);

    // All zeros
    int arr6[] = {0, 0, 0};
    // Subarray sums all 0 -> only one distinct, so return 0
    assert(secondSmallestSum(arr6, 3) == 0);

    // Mixed with zeros
    int arr7[] = {0, -1, 2};
    // Subarray sums: 0,-1,2,-1,1,1 -> distinct:-1,0,1,2 -> second smallest 0
    assert(secondSmallestSum(arr7, 3) == 0);

    return 0;
}
