/*
Write a C++ function named `boundedPrefixSum` that takes a constant integer array, its length, and a non-negative integer `limit`. The function must compute the sum of the first `(limit + 1)` elements of the array using a recursive approach that mirrors the logic of the given snippet, but with a cleaner and more robust design. Specifically, the function should return the sum of elements from index 0 up to index `limit` inclusive, but if `limit >= length`, return the sum of the entire array. The recursive base case is when only one element remains in the subarray under consideration (i.e., the recursion has reached index 0), at which point that element is returned. The function must not modify the input array and must use recursion only (no loops). For example, for `arr = {1, 3, 4, 6, 7}`, `length = 5`, and `limit = 3`, the function should return `1+3+4+6 = 14`.
*/
#include <cstddef> // for size_t

// Helper recursive function that processes from the end down to index 0.
// currentIndex is the index currently being considered (from high to low).
// limit is the maximum index that should be included in the sum.
static int boundedPrefixSumHelper(const int arr[], int currentIndex, int limit) {
    if (currentIndex == 0) {
        // Base case: only the first element remains.
        return arr[0];
    }
    // Recurse for the subarray ending at currentIndex-1.
    int sum = boundedPrefixSumHelper(arr, currentIndex - 1, limit);
    // Include current element only if its index is within the allowed limit.
    if (currentIndex <= limit) {
        sum += arr[currentIndex];
    }
    return sum;
}

// Computes the sum of arr[0..limit] (inclusive), or the entire array if limit is too large.
int boundedPrefixSum(const int arr[], int length, int limit) {
    // If limit is negative, we treat it as 0 (still include arr[0])? 
    // But per spec, limit is non-negative. For safety, clamp to 0.
    if (limit < 0) limit = 0;
    // If length <= 0, return 0 (empty array).
    if (length <= 0) return 0;
    // Clamp limit to length-1 to avoid out-of-bounds, but we still recurse through full length.
    if (limit >= length) limit = length - 1;
    return boundedPrefixSumHelper(arr, length - 1, limit);
}
#include <cassert>

int main() {
    int arr1[] = {1, 3, 4, 6, 7};
    assert(boundedPrefixSum(arr1, 5, 3) == 14); // 1+3+4+6
    assert(boundedPrefixSum(arr1, 5, 0) == 1);  // only first element
    assert(boundedPrefixSum(arr1, 5, 4) == 21); // all elements
    assert(boundedPrefixSum(arr1, 5, 10) == 21); // limit larger than length
    int arr2[] = {5};
    assert(boundedPrefixSum(arr2, 1, 0) == 5);
    assert(boundedPrefixSum(arr2, 1, 5) == 5);
    int arr3[] = {-2, 0, 3, -1};
    assert(boundedPrefixSum(arr3, 4, 2) == 1); // -2+0+3
    assert(boundedPrefixSum(arr3, 4, 3) == 0); // -2+0+3-1
    int arr4[] = {10, 20};
    assert(boundedPrefixSum(arr4, 2, 1) == 30);
    assert(boundedPrefixSum(arr4, 2, 0) == 10);
    return 0;
}
// The solution uses recursion to walk from the last element of the array back to index 0, while tracking the current index. A helper recursive function `recursiveHelper(arr, currentIndex, limit)` is used. At each recursive call, we reduce `currentIndex` by 1. The base case is `currentIndex == 0`, returning `arr[0]`. After the recursive call returns the sum for indices `0` to `currentIndex-1`, we check whether the current index is allowed to be included: we include `arr[currentIndex]` only if `currentIndex <= limit`. This directly implements the idea: for indices above `limit`, we skip adding them. However, the recursion still processes them to reach the base case. Edge cases: if `limit >= length-1`, all elements are included. If `limit < 0` (though constraint says non-negative, but for safety) we could return 0, but we assume it's non-negative. Time complexity is O(n) because we recurse through all n elements once. Space complexity is O(n) due to recursion stack depth. The solution uses `const int*` to ensure the array is not modified.
