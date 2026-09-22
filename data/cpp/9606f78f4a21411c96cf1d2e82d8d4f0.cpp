Write a C++ function that merges two sorted integer arrays into the first array in non-decreasing order. The function should accept two arrays: `nums1` (which has enough space to hold the merged result) and `nums2`, along with their respective valid element counts `m` and `n`. After the merge, the elements of `nums1` must be sorted in ascending order. The function should operate in-place on `nums1` and should not allocate additional arrays. The inputs are assumed to be valid: `m + n` equals the capacity of `nums1`, and both `nums1` and `nums2` are already sorted individually.
The task is a classic merge of two sorted sequences, but the twist is that the output must be written into the first array starting from the end to avoid overwriting elements that have not yet been compared. The algorithm uses three indices: `pos` (the current write position, initially `m + n - 1`), `i` (the last valid element of `nums1`, initially `m - 1`), and `j` (the last valid element of `nums2`, initially `n - 1`). At each step, compare `nums1[i]` and `nums2[j]`, write the larger value to `nums1[pos]`, and decrement the corresponding index and `pos`. Continue until either `i` or `j` becomes negative. If `j` remains non-negative after the loop, copy the remaining elements from `nums2` into the front of `nums1`. If `i` remains non-negative, no action is needed because those elements are already in their correct positions. Edge cases: when `m == 0` (nums1 is effectively empty), the function simply copies `nums2` into `nums1`; when `n == 0`, no work is done. The algorithm runs in O(m + n) time and uses O(1) extra space, as it only modifies the input array in place.
#include <vector>

// Merge two sorted arrays into the first array in non-decreasing order.
// nums1 has enough capacity to hold m + n elements; only the first m are valid.
void mergeSortedArrays(std::vector<int>& nums1, int m, const std::vector<int>& nums2, int n) {
    int pos = m + n - 1;
    int i = m - 1;
    int j = n - 1;

    // Merge from the back to avoid overwriting unprocessed elements in nums1.
    while (i >= 0 && j >= 0) {
        if (nums1[i] > nums2[j]) {
            nums1[pos--] = nums1[i--];
        } else {
            nums1[pos--] = nums2[j--];
        }
    }

    // Copy any remaining elements from nums2 (if m was exhausted first).
    while (j >= 0) {
        nums1[pos--] = nums2[j--];
    }
    // Elements remaining in nums1 (i >= 0) are already in place.
}
#include <cassert>
#include <vector>

int main() {
    // Normal merge with distinct values.
    std::vector<int> nums1 = {1, 3, 5, 0, 0, 0};
    std::vector<int> nums2 = {2, 4, 6};
    mergeSortedArrays(nums1, 3, nums2, 3);
    assert((nums1 == std::vector<int>{1, 2, 3, 4, 5, 6}));

    // nums1 empty.
    std::vector<int> nums3 = {0, 0, 0};
    std::vector<int> nums4 = {1, 2, 3};
    mergeSortedArrays(nums3, 0, nums4, 3);
    assert((nums3 == std::vector<int>{1, 2, 3}));

    // nums2 empty.
    std::vector<int> nums5 = {1, 2, 3};
    std::vector<int> nums6 = {};
    mergeSortedArrays(nums5, 3, nums6, 0);
    assert((nums5 == std::vector<int>{1, 2, 3}));

    // Duplicate values.
    std::vector<int> nums7 = {1, 2, 2, 0, 0, 0};
    std::vector<int> nums8 = {2, 3, 3};
    mergeSortedArrays(nums7, 3, nums8, 3);
    assert((nums7 == std::vector<int>{1, 2, 2, 2, 3, 3}));

    // All elements from nums2 are smaller than nums1.
    std::vector<int> nums9 = {5, 6, 7, 0, 0, 0};
    std::vector<int> nums10 = {1, 2, 3};
    mergeSortedArrays(nums9, 3, nums10, 3);
    assert((nums9 == std::vector<int>{1, 2, 3, 5, 6, 7}));

    // All elements from nums2 are larger than nums1.
    std::vector<int> nums11 = {1, 2, 3, 0, 0, 0};
    std::vector<int> nums12 = {4, 5, 6};
    mergeSortedArrays(nums11, 3, nums12, 3);
    assert((nums11 == std::vector<int>{1, 2, 3, 4, 5, 6}));

    // Both arrays with one element each.
    std::vector<int> nums13 = {0, 0};
    std::vector<int> nums14 = {1};
    mergeSortedArrays(nums13, 0, nums14, 1);
    assert((nums13 == std::vector<int>{0, 1}));

    // Both arrays with one element each, but nums1 has the larger.
    std::vector<int> nums15 = {2, 0};
    std::vector<int> nums16 = {1};
    mergeSortedArrays(nums15, 1, nums16, 1);
    assert((nums15 == std::vector<int>{1, 2}));

    return 0;
}
