// Write a C++ function that merges two sorted integer vectors into a single sorted vector, but with a twist: the first vector may have extra capacity beyond its logical size, and the function must only use the first `m` elements of the first vector as valid input while ignoring any trailing values beyond `m` (these trailing slots are considered "padding" and must not influence the result). The function should return a new vector containing all `m + n` elements in sorted order (where `m` is the number of valid elements in the first vector, and `n` is the size of the second vector). The function must not modify the input vectors, and it must handle cases where either vector is empty, where all elements of one vector come before the other, and where duplicate values exist across the two vectors.
// The solution uses a standard two-pointer merge algorithm. We create an output vector of size `m + n` (though we could also use a dynamic approach with `push_back`). We maintain three indices: `i` for the first input vector (only iterating up to `m`), `j` for the second input vector (up to `n`), and `k` for the output. In each step, we compare `nums1[i]` and `nums2[j]` and copy the smaller value into the output, advancing the corresponding pointer. When one vector is exhausted, we copy the remaining elements from the other vector. Edge cases: if `m == 0`, we simply copy all of `nums2`; if `n == 0`, we copy the first `m` of `nums1`; if duplicates occur, the comparison `<=` or `<` determines which is copied first (both are correct as long as we do not skip values). Time complexity is O(m + n) because each element is visited exactly once. Space complexity is O(m + n) for the output vector, plus O(1) auxiliary space for the indices. The original code snippet had a bug where it used `while(i < m)` and `while(j < n)` but `push_back` into a pre-sized vector, which could lead to wrong size; our solution avoids this by properly initializing the output size.
#include <vector>

// Merge two sorted vectors (first only considering first m elements) into a new sorted vector.
std::vector<int> mergeSorted(const std::vector<int>& nums1, int m,
                             const std::vector<int>& nums2, int n) {
    std::vector<int> result(m + n);
    int i = 0, j = 0, k = 0;

    // Standard two-pointer merge
    while (i < m && j < n) {
        if (nums1[i] <= nums2[j]) {
            result[k++] = nums1[i++];
        } else {
            result[k++] = nums2[j++];
        }
    }

    // Copy remaining elements from nums1 (only first m)
    while (i < m) {
        result[k++] = nums1[i++];
    }

    // Copy remaining elements from nums2
    while (j < n) {
        result[k++] = nums2[j++];
    }

    return result;
}
#include <cassert>
#include <vector>

// Declaration of the function to test (include the solution content here if not already included)
// For this test, we assume the solution is in the same translation unit.

int main() {
    // Normal case with both arrays having elements
    std::vector<int> nums1 = {1, 2, 3, 0, 0, 0};
    std::vector<int> nums2 = {2, 5, 6};
    std::vector<int> expected = {1, 2, 2, 3, 5, 6};
    assert(mergeSorted(nums1, 3, nums2, 3) == expected);

    // First vector empty (m=0)
    std::vector<int> nums3 = {0, 0};
    std::vector<int> nums4 = {1, 4, 7};
    expected = {1, 4, 7};
    assert(mergeSorted(nums3, 0, nums4, 3) == expected);

    // Second vector empty (n=0)
    std::vector<int> nums5 = {5, 6, 7};
    std::vector<int> nums6 = {};
    expected = {5, 6, 7};
    assert(mergeSorted(nums5, 3, nums6, 0) == expected);

    // Both empty
    assert(mergeSorted({0}, 0, {}, 0).empty());

    // Duplicate values across arrays
    std::vector<int> nums7 = {1, 2, 2, 3};
    std::vector<int> nums8 = {2, 2, 4};
    expected = {1, 2, 2, 2, 2, 3, 4};
    assert(mergeSorted(nums7, 4, nums8, 3) == expected);

    // All of first before second
    std::vector<int> nums9 = {1, 2, 3, 0};
    std::vector<int> nums10 = {4, 5};
    expected = {1, 2, 3, 4, 5};
    assert(mergeSorted(nums9, 3, nums10, 2) == expected);

    // All of second before first
    std::vector<int> nums11 = {10, 20, 30};
    std::vector<int> nums12 = {1, 2, 3};
    expected = {1, 2, 3, 10, 20, 30};
    assert(mergeSorted(nums11, 3, nums12, 3) == expected);

    // Single elements each
    assert(mergeSorted({5}, 1, {3}, 1) == std::vector<int>({3, 5}));

    return 0;
}
