// Write a C++ function `bool canFormSubsetSum(const int arr[], int size, int target)` that determines whether there exists a subset of the given array whose elements sum exactly to a target value. The array may contain positive, negative, or zero integers, and the subset can be empty (so a target of `0` always yields `true`). The function must correctly handle arrays with duplicate values, large targets, and edge cases such as an empty array. You may use recursion or any iterative approach, but you must not modify the input array. Provide a self-contained implementation that is callable from any program.

// The core idea is to explore all possible subsets using recursion. For each element, we consider two decisions: either include it in the subset (reducing the target by the element's value) or exclude it (keeping the target unchanged). This binary decision tree explores the entire power set. The base case occurs when no elements remain: return `true` only if the remaining target is exactly `0`, otherwise `false`. To avoid unnecessary recursion, if the current element is greater than the current target, we cannot include it (since all elements are positive in the simplest version), but since the problem allows negative values, we must always try both include and exclude regardless of value. However, a more efficient variant assumes non-negative integers and prunes when `arr[n-1] > target`. The provided snippet uses that assumption, but for generality we should handle negative numbers by not pruning. Important edge cases: empty array and target 0 returns `true`; empty array and non-zero target returns `false`; a single element equal to target returns `true`; duplicates do not affect correctness. Time complexity is \(O(2^n)\) in the worst case (exponential), and space complexity is \(O(n)\) due to recursion stack depth. For a more efficient solution with positive integers, dynamic programming can reduce to \(O(n \times \text{target})\) but the task focuses on correctness and recursion.

#include <vector>
#include <algorithm>

// Determines whether a subset of arr[0..size-1] sums exactly to target.
// The array may contain negative, zero, or positive integers.
bool canFormSubsetSum(const int arr[], int size, int target) {
    // Base case: no elements left.
    if (size == 0) {
        return target == 0;
    }

    // Last element of the current array.
    int last = arr[size - 1];

    // Always consider both including and excluding the last element.
    // Including it reduces the target, excluding it keeps the target.
    bool include = canFormSubsetSum(arr, size - 1, target - last);
    bool exclude = canFormSubsetSum(arr, size - 1, target);

    return include || exclude;
}

#include <cassert>

int main() {
    int arr1[] = {1, 2, 4, 6};
    assert(canFormSubsetSum(arr1, 4, 7) == true);  // 1 + 6 or 1 + 2 + 4
    assert(canFormSubsetSum(arr1, 4, 5) == true);  // 1 + 4
    assert(canFormSubsetSum(arr1, 4, 3) == false); // no subset sums to 3

    int arr2[] = {3, -1, 4};
    assert(canFormSubsetSum(arr2, 3, 3) == true);  // 3 or -1+4
    assert(canFormSubsetSum(arr2, 3, 2) == true);  // 3 + -1
    assert(canFormSubsetSum(arr2, 3, 0) == true);  // empty subset

    int arr3[] = {0, 0, 0};
    assert(canFormSubsetSum(arr3, 3, 0) == true);
    assert(canFormSubsetSum(arr3, 3, 1) == false);

    int arr4[] = {5};
    assert(canFormSubsetSum(arr4, 1, 5) == true);
    assert(canFormSubsetSum(arr4, 1, 0) == true); // empty subset
    assert(canFormSubsetSum(arr4, 1, 6) == false);

    int arr5[] = {}; // empty array
    assert(canFormSubsetSum(arr5, 0, 0) == true);
    assert(canFormSubsetSum(arr5, 0, 1) == false);
}
