Write a C++ function named `countSubarraysWithSum` that takes a vector of integers and a target sum `m`, and returns the number of contiguous subarrays whose elements sum exactly to `m`. The vector contains only non-negative integers and its size is at least 1. The function must handle cases where no subarray matches (return 0), where the whole array sums to `m`, and where multiple subarrays share the same starting or ending elements. Do not use a sliding window with HashMap; instead, use the classic two-pointer sliding window technique because all elements are non‑negative, allowing the window sum to monotonically increase as it expands and decrease as it shrinks.

The main algorithm is a two-pointer sliding window over the array. Maintain two indices `start` and `end`, both initially 0, and a running `windowSum`. Repeatedly expand the window by adding `arr[end]` and incrementing `end` while `windowSum < m` and `end < n`. If `windowSum == m`, increment the answer counter, then shrink the window from the left (subtract `arr[start]`, increment `start`) to look for the next possible window. If `windowSum > m`, shrink the window from the left until the sum is ≤ `m`. Continue until `end` reaches `n` and the window cannot be expanded further. Since all numbers are non-negative, shrinking always reduces the sum, so the window remains valid. Edge cases: when `m = 0`, the algorithm must count all empty subarrays? The problem restricts to non‑empty subarrays; here the code snippet counts only subarrays with sum exactly `m`. With `m=0` and non‑negative numbers, only subarrays containing only zeros count; the sliding window correctly counts them because when `windowSum == 0` it increments and moves `start` forward. Also note that when the array contains many zeros, the window can shrink without expanding, so be careful to avoid infinite loops—always increment `start` after each count. Time complexity is O(n) because each element is added once and removed once. Space complexity is O(1) extra.

#include <vector>

// Count contiguous subarrays of non-negative integers whose sum equals m.
// Uses the sliding window technique. Assumes all elements are >= 0.
int countSubarraysWithSum(const std::vector<int>& arr, int m) {
    int n = static_cast<int>(arr.size());
    int start = 0;
    int end = 0;
    int windowSum = 0;
    int count = 0;

    while (true) {
        if (windowSum < m) {
            // Expand the window to the right.
            if (end == n) {
                // No more elements to add, and sum is too small.
                break;
            }
            windowSum += arr[end];
            ++end;
        } else if (windowSum == m) {
            // Found a matching subarray.
            ++count;
            // Move the left boundary to search for the next subarray.
            windowSum -= arr[start];
            ++start;
        } else { // windowSum > m
            // Shrink the window from the left to reduce the sum.
            windowSum -= arr[start];
            ++start;
        }
    }

    return count;
}

#include <cassert>
#include <vector>

// Forward declaration of the function being tested.
int countSubarraysWithSum(const std::vector<int>&, int);

int main() {
    // Basic cases
    assert(countSubarraysWithSum({1, 2, 3}, 3) == 2);      // [3] and [1,2]
    assert(countSubarraysWithSum({1, 1, 1}, 2) == 2);      // [1,1] at positions (0,1) and (1,2)
    assert(countSubarraysWithSum({5}, 5) == 1);            // whole array
    assert(countSubarraysWithSum({5}, 10) == 0);           // no match

    // All zeros
    assert(countSubarraysWithSum({0, 0, 0}, 0) == 3);      // each single zero subarray

    // Mixed zeros and positives
    assert(countSubarraysWithSum({0, 1, 0, 1}, 1) == 4);   // [1], [0,1], [1,0], [0,1] (last two zeros not counted)

    // Larger array
    assert(countSubarraysWithSum({2, 1, 3, 2, 1, 3}, 3) == 4); // [3], [1,2] (from indices 1-2), [2,1] (3-4), [3] at end

    // Negative target (not applicable, but just to verify no crash)
    assert(countSubarraysWithSum({1, 2, 3}, -1) == 0);

    return 0;
}
