/*
Write a C++ function that, given an array of integers containing only 0s and 1s, its size `n`, and a non-negative integer `m`, returns the length of the longest contiguous subarray that can be formed by flipping at most `m` zeros to ones. The function must handle cases where `m` is larger than the total number of zeros, where the array has all zeros or all ones, and where `m` is zero. The function signature should be `int longestOnes(const std::vector<int>& arr, int m)`, and it should be implemented efficiently in a single pass using a sliding window.
*/
#include <vector>
#include <algorithm>

// Returns the length of the longest contiguous subarray that can be made all ones
// by flipping at most m zeros to ones.
int longestOnes(const std::vector<int>& arr, int m) {
    int n = static_cast<int>(arr.size());
    int left = 0;
    int zeroCount = 0;
    int maxLength = 0;

    for (int right = 0; right < n; ++right) {
        if (arr[right] == 0) {
            ++zeroCount;
        }

        // Shrink the window from the left until zeroCount <= m
        while (zeroCount > m) {
            if (arr[left] == 0) {
                --zeroCount;
            }
            ++left;
        }

        // Update the maximum length found so far
        maxLength = std::max(maxLength, right - left + 1);
    }

    return maxLength;
}
#include <cassert>
#include <vector>

// Forward declaration of the solution function (already defined above)
int longestOnes(const std::vector<int>& arr, int m);

int main() {
    // Basic cases
    assert(longestOnes({1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0}, 2) == 6);
    assert(longestOnes({0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1}, 3) == 10);

    // m = 0: longest run of consecutive ones
    assert(longestOnes({1, 0, 1, 1, 0, 1, 1, 1, 0}, 0) == 3);
    assert(longestOnes({0, 0, 0}, 0) == 0);
    assert(longestOnes({1, 1, 1}, 0) == 3);

    // All zeros
    assert(longestOnes({0, 0, 0, 0}, 2) == 2);
    assert(longestOnes({0, 0, 0, 0}, 5) == 4);

    // All ones
    assert(longestOnes({1, 1, 1, 1}, 0) == 4);
    assert(longestOnes({1, 1, 1, 1}, 10) == 4);

    // Empty array
    assert(longestOnes({}, 2) == 0);

    // Single element
    assert(longestOnes({0}, 1) == 1);
    assert(longestOnes({0}, 0) == 0);
    assert(longestOnes({1}, 0) == 1);

    // m larger than total zeros
    assert(longestOnes({1, 0, 1, 0, 1}, 10) == 5);

    return 0;
}
// The optimal solution uses the sliding window (two‑pointer) technique. Maintain a window defined by left index `left` and right index `right`. Expand the window to the right by including `arr[right]`. If the newly added element is `0`, increment a counter `zeroCount` representing the number of zeros in the current window. If `zeroCount` exceeds `m`, shrink the window from the left: while `zeroCount > m`, if the element at `left` is `0`, decrement `zeroCount`, then increment `left`. At each step, update the answer with the current window length `right - left + 1`. This works because the window always satisfies the condition of having at most `m` zeros, and we never shrink the window unnecessarily—only when forced to. Edge cases: if the array is empty, return 0. If `m` is greater than or equal to the total zeros, the answer is simply `n`. If all elements are ones, the answer is `n`. If `m` is zero, the window can contain no zeros, so it finds the longest run of consecutive ones. The time complexity is O(n) because each element is added and removed from the window at most once, and space complexity is O(1) beyond the input vector.
