// Write a C++ function named `searchNearlySorted` that takes a sorted array of integers (where each element may be misplaced by at most one position from its correct sorted index), its size, and a target value `x`. The function must return the index of `x` in the array if present, or -1 if `x` is not in the array. The array is guaranteed to be "nearly sorted" in the sense that for any valid index `i`, the element originally at sorted position `i` can only be found at index `i-1`, `i`, or `i+1` (within bounds). The function must use a modified binary search that leverages this property, checking the middle element and its immediate neighbors at each step, and recursing on appropriate subarrays excluding the already-checked middle neighborhood. The function must be recursive, handle an empty array (size 0) gracefully, and use `const` correctness for the array parameter.
// The solution adapts classic binary search to exploit the "nearly sorted" property. At each step, compute `mid` as `l + (r - l) / 2`. Instead of checking only `arr[mid]`, also check `arr[mid-1]` (if `mid > l`) and `arr[mid+1]` (if `mid < r`). If the target matches any of these three positions, return that index immediately. If the target is less than `arr[mid]`, then because the array is nearly sorted, the target could only reside in the left subarray ending at `mid-2` (since `mid-1` and `mid` positions have been ruled out). Similarly, if the target is greater than `arr[mid]`, search the right subarray starting at `mid+2`. The recursion terminates when `l > r`, returning -1. Important edge cases include: an empty array (size 0) where initial `l=0`, `r=-1`; a single-element array; target at the very first or last index (neighbor checks must respect bounds); and duplicated values—if duplicates exist, the function may return any valid occurrence, but the specified property guarantees at most one occurrence per three-position window, so this is not an issue. Time complexity: each recursive call reduces the search range by about half but skips up to 3 elements per step, so \(O(\log n)\) with a small constant factor; space complexity is \(O(\log n)\) due to recursion stack depth.
#include <vector>

// Recursive helper for searching a nearly sorted array.
// arr is sorted except each element may be at index i-1, i, or i+1 from its sorted position.
int searchNearlySortedHelper(const int arr[], int l, int r, int x) {
    if (l > r) {
        return -1;
    }
    int mid = l + (r - l) / 2;
    
    // Check middle and its immediate neighbors.
    if (arr[mid] == x) return mid;
    if (mid > l && arr[mid - 1] == x) return mid - 1;
    if (mid < r && arr[mid + 1] == x) return mid + 1;
    
    // If x is smaller than arr[mid], it must be in the left part (excluding checked neighbors).
    if (x < arr[mid]) {
        return searchNearlySortedHelper(arr, l, mid - 2, x);
    }
    // Otherwise, it must be in the right part.
    return searchNearlySortedHelper(arr, mid + 2, r, x);
}

// Public function: searches for x in a nearly sorted array of given size.
// Returns index of x, or -1 if not found.
int searchNearlySorted(const int arr[], int size, int x) {
    if (size <= 0) return -1;
    return searchNearlySortedHelper(arr, 0, size - 1, x);
}
#include <cassert>

int main() {
    // Basic cases
    int arr1[] = {10, 3, 40, 20, 50, 80, 70};
    int size1 = sizeof(arr1) / sizeof(arr1[0]);
    assert(searchNearlySorted(arr1, size1, 40) == 2);
    assert(searchNearlySorted(arr1, size1, 10) == 0);
    assert(searchNearlySorted(arr1, size1, 70) == 6);
    assert(searchNearlySorted(arr1, size1, 100) == -1);
    
    // Single element
    int arr2[] = {5};
    assert(searchNearlySorted(arr2, 1, 5) == 0);
    assert(searchNearlySorted(arr2, 1, 3) == -1);
    
    // Two elements (nearly sorted property still holds)
    int arr3[] = {2, 1};
    assert(searchNearlySorted(arr3, 2, 1) == 1);
    assert(searchNearlySorted(arr3, 2, 2) == 0);
    
    // Empty array
    int* arr4 = nullptr;
    assert(searchNearlySorted(arr4, 0, 10) == -1);
    
    // Target at a neighbor when mid is not equal
    int arr5[] = {1, 2, 3, 4, 5, 6};
    // This array is perfectly sorted, but still nearly sorted.
    assert(searchNearlySorted(arr5, 6, 4) == 3);
    assert(searchNearlySorted(arr5, 6, 2) == 1);
    
    // Larger test with values at edges
    int arr6[] = {3, 2, 1, 4, 5, 6};
    assert(searchNearlySorted(arr6, 6, 1) == 2);
    assert(searchNearlySorted(arr6, 6, 3) == 0);
    assert(searchNearlySorted(arr6, 6, 6) == 5);
    
    return 0;
}
