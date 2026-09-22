Write a C++ function that takes a vector of distinct integers and returns the minimum number of adjacent swaps required to sort the array in ascending order. The vector may be empty (return 0), but will never contain duplicate values. The function must operate in-place conceptually but should not modify the input vector—instead, work on a copy. The input size can be up to 10^5 elements, so the solution must be efficient. Additionally, the function should handle arrays that are already sorted (return 0) and reversed arrays (return n*(n-1)/2). The function signature should be `int minAdjacentSwapsToSort(const std::vector<int>& arr)`. Note: This is based on the classic problem of counting inversions or cycle decomposition, but here we specifically count adjacent swaps, which equals the number of inversions. However, the given snippet uses a different interpretation (minimum swaps of any two elements, not necessarily adjacent). To align with the new task, we will explicitly define that only adjacent swaps are allowed, and the goal is to sort the array.
#include <cassert>
#include <vector>

// (The solution function is assumed to be included above.)

int main() {
    // Empty array
    assert(minAdjacentSwapsToSort({}) == 0);
    // Single element
    assert(minAdjacentSwapsToSort({42}) == 0);
    // Already sorted
    assert(minAdjacentSwapsToSort({1,2,3,4,5}) == 0);
    // Reverse sorted
    std::vector<int> rev = {5,4,3,2,1};
    assert(minAdjacentSwapsToSort(rev) == 10); // n*(n-1)/2 = 5*4/2 = 10
    // Random permutation
    assert(minAdjacentSwapsToSort({3,1,2}) == 2); // 3 inversions? Actually: (3,1),(3,2) -> 2
    assert(minAdjacentSwapsToSort({2,3,1}) == 2); // (2,1),(3,1) -> 2
    assert(minAdjacentSwapsToSort({1,3,2}) == 1); // (3,2)
    assert(minAdjacentSwapsToSort({3,2,1}) == 3); // (3,2),(3,1),(2,1) -> 3
    // Larger test with distinct values
    assert(minAdjacentSwapsToSort({1,5,2,4,3}) == 4); // inversions: (5,2),(5,4),(5,3),(4,3) -> 4
    // Check original input is not modified
    std::vector<int> original = {5,1,4,2,3};
    int result = minAdjacentSwapsToSort(original);
    assert(result == 6); // count manually: inversions: (5,1),(5,4),(5,2),(5,3),(4,2),(4,3) -> 6
    assert(original == std::vector<int>({5,1,4,2,3})); // unchanged

    // Negative numbers work as well
    assert(minAdjacentSwapsToSort({-1, -2, 0}) == 1); // (-1,-2)
    return 0;
}
#include <vector>
#include <cstdint>

// Recursive helper that counts inversions in the subarray [left, right) and sorts it.
static int64_t mergeSortCount(std::vector<int>& arr, int left, int right) {
    if (right - left <= 1) return 0;
    int mid = left + (right - left) / 2;
    int64_t inv = mergeSortCount(arr, left, mid) + mergeSortCount(arr, mid, right);
    
    // Merge two sorted halves and count cross inversions.
    std::vector<int> temp(right - left);
    int i = left, j = mid, k = 0;
    while (i < mid && j < right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            // All remaining elements in left half are greater than arr[j].
            inv += (mid - i);
            temp[k++] = arr[j++];
        }
    }
    while (i < mid) temp[k++] = arr[i++];
    while (j < right) temp[k++] = arr[j++];
    for (int p = 0; p < k; ++p) arr[left + p] = temp[p];
    return inv;
}

// Returns the minimum number of adjacent swaps to sort the input array.
int minAdjacentSwapsToSort(const std::vector<int>& arr) {
    std::vector<int> copy = arr; // work on a copy to avoid modifying input
    return static_cast<int>(mergeSortCount(copy, 0, static_cast<int>(copy.size())));
}
// The minimum number of adjacent swaps needed to sort an array equals the number of inversions in the array. An inversion is a pair of indices (i, j) with i < j and arr[i] > arr[j]. Each adjacent swap reduces the inversion count by exactly one, and the array is sorted when zero inversions remain. The most efficient way to count inversions is via a modified merge sort: while merging two sorted halves, count how many elements from the right half are smaller than elements from the left half. This runs in O(n log n) time and uses O(n) auxiliary space for the temporary merge buffer. Edge cases: empty array returns 0; already sorted array returns 0; descending array returns n*(n-1)/2 (maximum inversions). The recursive merge-sort approach handles all cases uniformly. Since the input is not modified, we make a copy in the counting function. The solution uses `const` reference for the input and an internal recursive helper that works on a mutable vector. Complexity: O(n log n) time, O(n) auxiliary space (for the merge buffer and call stack).
