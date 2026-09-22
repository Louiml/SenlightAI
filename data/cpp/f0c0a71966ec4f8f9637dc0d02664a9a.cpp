/*
Given a sorted vector of integers `arr`, a positive integer `k`, and a target integer `x`, write a C++ function `findClosestElements` that returns a vector containing the `k` elements from `arr` that are closest to `x`. The result must be in ascending order (i.e., in the same relative order as they appear in the original sorted array). If two elements are equally close to `x`, prefer the one with the smaller value (i.e., the one appearing earlier in the sorted array). The function should handle the case where `k >= arr.size()` by returning the entire array, and must run in O(log n + k) time for a vector of size n (via binary search to find the starting position, then expanding outward). The array may contain duplicate values; duplicates are treated as distinct elements occupying different indices.
*/
#include <vector>
#include <cstdlib>
#include <algorithm>

// Returns k elements from sorted arr closest to x, in ascending order.
// Ties are broken toward smaller elements (left side).
std::vector<int> findClosestElements(const std::vector<int>& arr, int k, int x) {
    int n = arr.size();
    if (n <= k) return arr;

    // Binary search for the index of the element closest to x.
    int lo = 0, hi = n - 1;
    while (lo < hi - 1) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == x) {
            lo = mid;
            hi = mid;
            break;
        } else if (arr[mid] < x) {
            lo = mid;
        } else {
            hi = mid;
        }
    }

    // Choose the closer of lo and hi; on tie, choose lo (smaller index).
    int idx = (std::abs(arr[lo] - x) <= std::abs(arr[hi] - x)) ? lo : hi;

    // Expand the window around idx until it has k elements.
    int left = idx, right = idx;
    while (right - left + 1 < k) {
        if (left == 0) {
            right++;
        } else if (right == n - 1) {
            left--;
        } else {
            int distL = std::abs(arr[left - 1] - x);
            int distR = std::abs(arr[right + 1] - x);
            if (distL <= distR) {
                left--;
            } else {
                right++;
            }
        }
    }

    return std::vector<int>(arr.begin() + left, arr.begin() + right + 1);
}
#include <cassert>
#include <vector>

// The solution function declaration is assumed to be above.
// Include the provided solution code here before the main for testing.

int main() {
    std::vector<int> arr1 = {1, 2, 3, 4, 5};
    assert(findClosestElements(arr1, 4, 3) == std::vector<int>({1, 2, 3, 4}));
    assert(findClosestElements(arr1, 4, -1) == std::vector<int>({1, 2, 3, 4}));
    assert(findClosestElements(arr1, 2, 3) == std::vector<int>({2, 3}));
    assert(findClosestElements(arr1, 2, 1) == std::vector<int>({1, 2}));
    assert(findClosestElements(arr1, 5, 100) == std::vector<int>({1, 2, 3, 4, 5}));

    std::vector<int> arr2 = {1, 1, 1, 10, 10, 10};
    assert(findClosestElements(arr2, 3, 9) == std::vector<int>({1, 10, 10}));
    assert(findClosestElements(arr2, 3, 0) == std::vector<int>({1, 1, 1}));

    std::vector<int> arr3 = {0, 0, 1, 2, 3, 3, 4, 7, 7, 8};
    assert(findClosestElements(arr3, 4, 3) == std::vector<int>({1, 2, 3, 3}));
    assert(findClosestElements(arr3, 3, 5) == std::vector<int>({3, 4, 7}));
    assert(findClosestElements(arr3, 2, 9) == std::vector<int>({7, 8}));

    std::vector<int> arr4 = {1};
    assert(findClosestElements(arr4, 1, 100) == std::vector<int>({1}));
    assert(findClosestElements(arr4, 2, 0) == std::vector<int>({1}));

    return 0;
}
// The algorithm first finds the index of the element in `arr` that is closest to `x` using a custom binary search. This binary search narrows down to two candidate indices (`l` and `r` such that `l` is the largest index with `arr[l] <= x` and `r` is the smallest with `arr[r] >= x`) and then picks the nearer one, breaking ties by the smaller value (left index). Then we maintain a sliding window `[l, r]` initialized to that single index. While the window size is less than `k`, we expand either left or right depending on which neighbor is closer to `x`. If we are at the left boundary (cannot move left), we expand right; if at the right boundary, expand left. Otherwise compare `abs(arr[l-1] - x)` and `abs(arr[r+1] - x)`; if left is less than or equal, move left, else move right. The <= ensures tie-breaking toward the smaller value (left side). Once the window contains exactly `k` elements, return the subvector `arr[l .. r]`. This correctly handles edge cases: when `k == n` (or larger) we return the whole array; when `x` is outside the array's value range, the binary search still returns the closest endpoint; when there are duplicates, the expansion rules still respect ordering. Time complexity is O(log n) for the binary search plus O(k) for the expansion, giving O(log n + k). Space complexity is O(1) extra for indices, excluding the returned vector.
