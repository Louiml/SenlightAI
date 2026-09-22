/*
Write a C++ function named `binarySearchSorted` that takes a sorted integer vector (`std::vector<int>`) in non-decreasing order and a target integer, and returns the index of the target if it exists in the vector, or -1 if it does not. The function must implement the classic binary search algorithm with a half-open interval `[left, right)` — that is, `left` is inclusive and `right` is exclusive. It should handle edge cases such as an empty input vector, a target smaller than all elements, larger than all elements, duplicates (return the index of any occurrence, not necessarily the first), and a vector with a single element. The function must be declared as `int binarySearchSorted(const std::vector<int>& nums, int target);` and must not modify the input vector (use `const` reference). Do not use standard library search functions like `std::binary_search` or `std::find`; implement the algorithm manually.
*/
#include <vector>

// Perform binary search on a sorted vector (non-decreasing order).
// Returns the index of the target if found, otherwise returns -1.
// Uses half-open interval [left, right).
int binarySearchSorted(const std::vector<int>& nums, int target) {
    int left = 0;
    int right = static_cast<int>(nums.size()); // exclusive upper bound

    while (left < right) {
        int mid = left + (right - left) / 2; // avoid overflow
        if (nums[mid] == target) {
            return mid;
        } else if (target > nums[mid]) {
            left = mid + 1; // search right half
        } else {
            right = mid;    // search left half, excluding mid
        }
    }

    return -1; // not found
}
#include <cassert>
#include <vector>

// Declare the function (or include the header where it is defined).
int binarySearchSorted(const std::vector<int>& nums, int target);

int main() {
    // Basic cases
    std::vector<int> arr1 = {1, 2, 3, 4, 5};
    assert(binarySearchSorted(arr1, 3) == 2);
    assert(binarySearchSorted(arr1, 1) == 0);
    assert(binarySearchSorted(arr1, 5) == 4);
    assert(binarySearchSorted(arr1, 0) == -1);
    assert(binarySearchSorted(arr1, 6) == -1);

    // Empty vector
    std::vector<int> empty;
    assert(binarySearchSorted(empty, 10) == -1);

    // Single element
    std::vector<int> single = {7};
    assert(binarySearchSorted(single, 7) == 0);
    assert(binarySearchSorted(single, 5) == -1);

    // Duplicates - any index is acceptable, but verify it returns a valid index
    std::vector<int> dup = {1, 2, 2, 2, 3};
    int idx = binarySearchSorted(dup, 2);
    assert(idx >= 1 && idx <= 3);
    assert(dup[idx] == 2);

    // Negative numbers and large size
    std::vector<int> neg = {-10, -5, -1, 0, 4};
    assert(binarySearchSorted(neg, -5) == 1);
    assert(binarySearchSorted(neg, -1) == 2);
    assert(binarySearchSorted(neg, 3) == -1);

    // All elements greater than target
    std::vector<int> all_big = {5, 6, 7};
    assert(binarySearchSorted(all_big, 4) == -1);

    // All elements less than target
    std::vector<int> all_small = {1, 2, 3};
    assert(binarySearchSorted(all_small, 9) == -1);

    // Larger random check with even and odd sizes
    std::vector<int> even = {2, 4, 6, 8};
    assert(binarySearchSorted(even, 4) == 1);
    assert(binarySearchSorted(even, 8) == 3);
    assert(binarySearchSorted(even, 5) == -1);

    std::vector<int> odd = {2, 4, 6};
    assert(binarySearchSorted(odd, 6) == 2);
    assert(binarySearchSorted(odd, 2) == 0);

    return 0;
}
// The solution uses an iterative binary search over the half-open interval `[left, right)`. Initially set `left = 0` and `right = nums.size()` (one past the last index). While `left < right`, compute `mid = left + (right - left) / 2` to avoid integer overflow. Compare `nums[mid]` with the target:  
// - If equal, return `mid` immediately.  
// - If target is greater than `nums[mid]`, move `left = mid + 1` to search the right half, because `mid` is already excluded.  
// - Otherwise (target is less), set `right = mid` to search the left half, since `mid` is not included in the new interval.  
// This approach works for sorted arrays with duplicates because it returns any matching index. Edge cases: empty vector returns -1 immediately (size 0). Target smaller than first element or larger than last element will cause the loop to terminate naturally with `left` possibly becoming equal to `size`, and the function returns -1. A single-element vector works because `left=0, right=1`, `mid=0`, and the comparison determines the result. Time complexity is O(log n) per search, and space complexity is O(1) auxiliary.
