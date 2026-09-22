// Write a C++ function `bool binarySearchDescending(const std::vector<int>& arr, int target)` that returns `true` if `target` exists in `arr`, and `false` otherwise, but with one twist: the input array is sorted in **strictly descending** order (e.g., `[10, 8, 5, 3, 1]`). The function must implement a recursive binary search that correctly adjusts the search intervals for descending order. The array may be empty (then return `false`), may contain duplicate values, and may contain negative numbers. The function must be `const`-correct, take the array by `const&`, and not use any standard library search functions (like `std::binary_search` or `std::find`). You must handle all edge cases cleanly without infinite recursion.
#include <cassert>
#include <vector>

// Assume the solution function is already declared above.

int main() {
    std::vector<int> arr1 = {10, 8, 5, 3, 1};
    assert(binarySearchDescending(arr1, 8) == true);
    assert(binarySearchDescending(arr1, 1) == true);
    assert(binarySearchDescending(arr1, 10) == true);
    assert(binarySearchDescending(arr1, 4) == false);
    assert(binarySearchDescending(arr1, 0) == false);
    
    std::vector<int> arr2 = {};  // empty
    assert(binarySearchDescending(arr2, 5) == false);
    
    std::vector<int> arr3 = {5};  // single element
    assert(binarySearchDescending(arr3, 5) == true);
    assert(binarySearchDescending(arr3, 6) == false);
    
    std::vector<int> arr4 = {9, 9, 7, 7, 7, 2};  // duplicates
    assert(binarySearchDescending(arr4, 7) == true);
    assert(binarySearchDescending(arr4, 9) == true);
    assert(binarySearchDescending(arr4, 2) == true);
    assert(binarySearchDescending(arr4, 8) == false);
    
    std::vector<int> arr5 = {-10, -20, -30};  // negatives
    assert(binarySearchDescending(arr5, -20) == true);
    assert(binarySearchDescending(arr5, -25) == false);
    
    return 0;
}
#include <vector>

// Recursively search for 'target' in a strictly descending sorted vector.
// Returns true if found, false otherwise. Handles empty vectors and duplicates.
bool binarySearchDescendingHelper(const std::vector<int>& arr, int left, int right, int target) {
    if (left > right) {
        return false;  // empty range
    }
    int mid = left + (right - left) / 2;  // avoid overflow
    
    if (arr[mid] == target) {
        return true;
    } else if (arr[mid] > target) {
        // Since descending, larger values are to the right.
        return binarySearchDescendingHelper(arr, mid + 1, right, target);
    } else { // arr[mid] < target
        // Smaller values are to the left.
        return binarySearchDescendingHelper(arr, left, mid - 1, target);
    }
}

// Public wrapper function.
bool binarySearchDescending(const std::vector<int>& arr, int target) {
    return binarySearchDescendingHelper(arr, 0, static_cast<int>(arr.size()) - 1, target);
}
// The core issue is that the provided code snippet mistakenly assumes ascending order (it checks `if(a[m] == x)`, `else if(a[m] > x) check(m+1, r)`, else `check(l, m-1)`) but the problem statement is ambiguous about order. Here we explicitly define descending order. For a descending array, if the middle element is greater than the target, the target must be to the **right** (since values decrease as index increases), so we recurse on `[mid+1, right]`. Conversely, if `a[mid] < target`, the target lies to the left, recurse on `[left, mid-1]`. The base case is `left > right` returning `false`. Important edge cases: empty vector (call with `left=0, right=-1` must return `false`), single element, duplicates (if `a[mid] == target`, return `true` immediately), and target outside the range (e.g., larger than the first element, or smaller than the last – the recursion will quickly hit base case). Time complexity: \(O(\log n)\) for successful and unsuccessful searches because we halve the interval each step. Space complexity: \(O(\log n)\) due to the recursion stack depth. The function must be implemented recursively exactly, using `const std::vector<int>&` to avoid copying.
