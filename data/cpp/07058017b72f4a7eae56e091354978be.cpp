// Write a C++ function `longestArithmeticSubarrayLength` that takes a vector of integers representing an array and returns the length of the longest contiguous subarray where the difference between consecutive elements is constant (i.e., an arithmetic progression). The input array is guaranteed to have at least two elements. For example, given `[10, 7, 4, 6, 8, 10, 11]`, the longest arithmetic subarray is `[4, 6, 8, 10]` with common difference `2`, so the function should return `4`. The function should handle negative differences, zeros, and duplicate values correctly, and must not modify the input vector.
The solution iterates through the array once, maintaining the current common difference (`currentDiff`) and the length of the current arithmetic subarray (`currentLength`). Initialize `currentDiff` as the difference between the first two elements and `currentLength` as 2 (since any two elements form an arithmetic subarray of length 2). Then, for each subsequent index `i` starting from 2, compute the difference between `arr[i]` and `arr[i-1]`. If this difference equals the current common difference, increment `currentLength`; otherwise, reset `currentDiff` to the new difference and set `currentLength` back to 2. Keep track of the maximum `currentLength` seen so far and return it. Edge cases include all elements equal (difference 0, entire array is arithmetic), negative differences (handled naturally by direct comparison), and a strictly increasing/decreasing sequence. Complexity is O(n) time and O(1) extra space.
#include <vector>
#include <algorithm>

// Returns the length of the longest contiguous arithmetic subarray.
int longestArithmeticSubarrayLength(const std::vector<int>& arr) {
    const int n = static_cast<int>(arr.size());
    if (n < 2) return n;

    int maxLength = 2;
    int currentDiff = arr[1] - arr[0];
    int currentLength = 2;

    for (int i = 2; i < n; ++i) {
        const int nextDiff = arr[i] - arr[i - 1];
        if (nextDiff == currentDiff) {
            ++currentLength;
        } else {
            currentDiff = nextDiff;
            currentLength = 2;
        }
        maxLength = std::max(maxLength, currentLength);
    }
    return maxLength;
}
#include <cassert>
#include <vector>

int main() {
    assert(longestArithmeticSubarrayLength({10, 7, 4, 6, 8, 10, 11}) == 4);
    assert(longestArithmeticSubarrayLength({1, 2, 3, 4, 5}) == 5);
    assert(longestArithmeticSubarrayLength({5, 5, 5, 5}) == 4);
    assert(longestArithmeticSubarrayLength({-3, -1, 1, 3, 5}) == 5);
    assert(longestArithmeticSubarrayLength({1, 3, 2, 4, 6, 8}) == 4);
    assert(longestArithmeticSubarrayLength({100, 90, 80, 70}) == 4);
    assert(longestArithmeticSubarrayLength({2, 4, 6, 9, 12, 15, 18}) == 4);
    assert(longestArithmeticSubarrayLength({0, 0, 1, 2, 3}) == 3);
    assert(longestArithmeticSubarrayLength({1, 2}) == 2);
    assert(longestArithmeticSubarrayLength({7, 3, 5, 7, 9, 11, 13}) == 5);
    return 0;
}
