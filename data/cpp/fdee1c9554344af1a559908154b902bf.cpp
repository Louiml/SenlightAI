/*
Write a C++ function `int splitArrayIntoMPieces(int arr[], int n, int m)` that takes an array of `n` non-negative integers and an integer `m` (1 ≤ m ≤ n), and returns the minimum possible largest sum among `m` contiguous subarrays (pieces) when the array is split into exactly `m` parts. Each part must be a contiguous segment of the original array, and every element must belong to exactly one part. The function should compute this value using a binary search over the possible maximum sum range, and must handle cases where `m` equals 1 (the whole array) or `n` (each element alone), as well as arrays with zeros or duplicate values.
*/
#include <vector>
#include <algorithm>

// Returns the minimum possible largest sum when splitting arr into exactly m contiguous pieces.
// Assumes m >= 1 and m <= n. The array arr has n non-negative integers.
int splitArrayIntoMPieces(const int arr[], int n, int m) {
    int low = 0;   // at least the maximum element
    int high = 0;  // at most the total sum
    for (int i = 0; i < n; ++i) {
        low = std::max(low, arr[i]);
        high += arr[i];
    }

    while (low < high) {
        int mid = low + (high - low) / 2;

        // Greedy count how many pieces are needed if each piece sum <= mid
        int currentSum = 0;
        int pieces = 1;  // at least one piece
        for (int i = 0; i < n; ++i) {
            if (currentSum + arr[i] > mid) {
                // Need to start a new piece
                currentSum = arr[i];
                ++pieces;
                if (pieces > m) break; // early exit optimization
            } else {
                currentSum += arr[i];
            }
        }

        if (pieces > m) {
            // Too many pieces, need to increase allowed sum
            low = mid + 1;
        } else {
            // Could achieve with m or fewer pieces, try smaller sum
            high = mid;
        }
    }

    return low;
}
#include <cassert>

int main() {
    // Example 1: simple array, m=2
    int arr1[] = {7, 2, 5, 10, 8};
    assert(splitArrayIntoMPieces(arr1, 5, 2) == 18); // [7,2,5] and [10,8] -> 14 and 18, min max = 18

    // Example 2: m=1 -> whole sum
    int arr2[] = {1, 2, 3, 4, 5};
    assert(splitArrayIntoMPieces(arr2, 5, 1) == 15); // single piece

    // Example 3: m=n -> each element alone
    int arr3[] = {5, 3, 9, 1};
    assert(splitArrayIntoMPieces(arr3, 4, 4) == 9); // max element

    // Example 4: all zeros
    int arr4[] = {0, 0, 0, 0};
    assert(splitArrayIntoMPieces(arr4, 4, 2) == 0); // all sums zero

    // Example 5: array with duplicates and zeros
    int arr5[] = {10, 0, 10, 0, 10};
    assert(splitArrayIntoMPieces(arr5, 5, 3) == 10); // each 10 alone, zeros can attach

    // Example 6: large array where answer is not sum or max
    int arr6[] = {1, 4, 4};
    assert(splitArrayIntoMPieces(arr6, 3, 3) == 4); // each alone
    assert(splitArrayIntoMPieces(arr6, 3, 2) == 5); // [1,4] and [4] -> max 5

    // Example 7: single element
    int arr7[] = {42};
    assert(splitArrayIntoMPieces(arr7, 1, 1) == 42);

    return 0;
}
// The problem is a classic "split array largest sum" (LeetCode 410). The goal is to minimize the maximum sum of `m` contiguous partitions. The solution uses binary search on the answer: the possible minimum largest sum ranges from `max(arr)` (when each piece is at least one element, so the largest single element must be <= the answer) to `sum(arr)` (when all elements are in one piece). For a candidate `mid`, we greedily partition the array into contiguous pieces such that each piece sum does not exceed `mid`, counting how many pieces are needed. If the number of pieces is greater than `m`, then `mid` is too small (we need to allow larger sums to reduce piece count), so we search the upper half. If pieces ≤ `m`, `mid` is feasible (we can split into fewer pieces, and then further split some to reach exactly `m`), so we search the lower half (including `mid` as a possible answer). The greedy partition is correct because minimizing the number of pieces under a sum cap is achieved by packing as many elements as possible into each piece. Edge cases: `m=1` returns sum of all; `m=n` returns max element; zeros in the array do not affect the logic since sum can stay same; duplicate values are fine. Time complexity: O(n log(sum)) where sum is total array sum (since binary search performs O(log(sum)) iterations, each scanning the array in O(n)). Space complexity: O(1) auxiliary.
