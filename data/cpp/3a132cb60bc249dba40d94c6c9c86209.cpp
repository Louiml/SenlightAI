/*
Write a C++ function that performs a recursive binary search on a sorted integer array and returns the index (0-based) of a target value if found, or -1 if not found. The function must follow the recursive approach shown in the provided snippet but correct its logical and off-by-one errors: specifically, the original code has an incorrect base condition, a wrong recursive call range, and it prints output instead of returning a value. Your function should be pure (no printing, no global state), handle an empty array, and work correctly for arrays with even and odd lengths. Use `const` for the array parameter, and avoid variable-length arrays inside the function.
*/

#include <vector>

// Recursive binary search on a sorted array (ascending order).
// Returns the 0-based index of the target if found, otherwise -1.
// The array must be sorted in non-decreasing order.
int recursiveBinarySearch(const std::vector<int>& arr, int target, int start, int end) {
    // Base case: search space is empty
    if (start > end) {
        return -1;
    }
    
    int mid = start + (end - start) / 2; // avoids potential overflow
    
    if (arr[mid] == target) {
        return mid;
    } else if (arr[mid] < target) {
        // Search right half
        return recursiveBinarySearch(arr, target, mid + 1, end);
    } else {
        // Search left half (original snippet incorrectly used mid+1 here)
        return recursiveBinarySearch(arr, target, start, mid - 1);
    }
}

// Convenience wrapper to call the recursive function without specifying bounds.
int binarySearch(const std::vector<int>& arr, int target) {
    return recursiveBinarySearch(arr, target, 0, static_cast<int>(arr.size()) - 1);
}

#include <cassert>
#include <vector>

// Function declarations (already defined above in the solution)
int binarySearch(const std::vector<int>& arr, int target);

int main() {
    // Basic cases
    std::vector<int> arr1 = {1, 2, 3, 4, 5};
    assert(binarySearch(arr1, 1) == 0);
    assert(binarySearch(arr1, 5) == 4);
    assert(binarySearch(arr1, 3) == 2);
    assert(binarySearch(arr1, 6) == -1);
    assert(binarySearch(arr1, 0) == -1);

    // Even length
    std::vector<int> arr2 = {10, 20, 30, 40};
    assert(binarySearch(arr2, 10) == 0);
    assert(binarySearch(arr2, 40) == 3);
    assert(binarySearch(arr2, 25) == -1);

    // Single element
    std::vector<int> arr3 = {7};
    assert(binarySearch(arr3, 7) == 0);
    assert(binarySearch(arr3, 8) == -1);

    // Duplicate values (returns any matching index)
    std::vector<int> arr4 = {2, 2, 2, 2};
    int idx = binarySearch(arr4, 2);
    assert(idx >= 0 && idx < 4);
    assert(arr4[idx] == 2);

    // Negative numbers
    std::vector<int> arr5 = {-5, -3, 0, 2, 9};
    assert(binarySearch(arr5, -5) == 0);
    assert(binarySearch(arr5, 9) == 4);
    assert(binarySearch(arr5, -1) == -1);

    // Empty array
    std::vector<int> arr6;
    assert(binarySearch(arr6, 1) == -1);

    return 0;
}

// The solution uses a standard recursive binary search. The main algorithm: given a sorted array, compare the target with the middle element. If they match, return the middle index. If the target is smaller, recurse on the left half (start to mid-1). If larger, recurse on the right half (mid+1 to end). The base case is when the start index exceeds the end index, meaning the target is not present; return -1. Important edge cases: an empty array (start=0, end=-1) should immediately return -1; a single-element array works with the middle calculation; duplicate values—returns any one matching index. The original snippet had bugs: (1) base condition `b>e` was correct but it printed a message and returned 0, which could be confused with a valid index 0; (2) in the right-half call it used `m+1` correctly but in the left-half call it used `m+1` instead of `m-1`, causing infinite recursion; (3) it printed instead of returning. The corrected function returns an `int` index or -1. Time complexity is O(log n) worst-case, space complexity is O(log n) due to recursion stack.
