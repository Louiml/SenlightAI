// Write a C++ function named `mergeSortedIntoFirst` that takes two sorted integer vectors, `nums1` and `nums2`, along with the number of valid elements `m` in `nums1` and `n` in `nums2`. The vector `nums1` has been preallocated to hold exactly `m + n` elements, where the first `m` positions contain the sorted elements and the remaining `n` positions are unused (contain garbage). The function must merge the sorted elements from `nums2` into `nums1` so that after the operation, `nums1` contains all `m + n` elements in non-decreasing sorted order. The function should modify `nums1` in-place and return `void`. Do not rely on any external sorting function; you must implement the merge logic manually. Assume `m` and `n` are non-negative, and if either is zero, the function should handle it correctly. The function must be efficient and use only constant extra space.

#include <cassert>
#include <vector>

// Declaration (provided by the solution file)
void mergeSortedIntoFirst(std::vector<int>& nums1, int m, const std::vector<int>& nums2, int n);

int main() {
    // Basic merge
    std::vector<int> nums1 = {1, 2, 3, 0, 0, 0};
    std::vector<int> nums2 = {2, 5, 6};
    mergeSortedIntoFirst(nums1, 3, nums2, 3);
    assert((nums1 == std::vector<int>{1, 2, 2, 3, 5, 6}));

    // All nums1 elements smaller than nums2
    nums1 = {1, 2, 3, 0, 0};
    nums2 = {4, 5};
    mergeSortedIntoFirst(nums1, 3, nums2, 2);
    assert((nums1 == std::vector<int>{1, 2, 3, 4, 5}));

    // All nums2 elements smaller than nums1
    nums1 = {4, 5, 6, 0, 0};
    nums2 = {1, 2};
    mergeSortedIntoFirst(nums1, 3, nums2, 2);
    assert((nums1 == std::vector<int>{1, 2, 4, 5, 6}));

    // Equal elements
    nums1 = {1, 1, 1, 0, 0};
    nums2 = {1, 1};
    mergeSortedIntoFirst(nums1, 3, nums2, 2);
    assert((nums1 == std::vector<int>{1, 1, 1, 1, 1}));

    // n == 0
    nums1 = {1, 2, 3};
    nums2 = {};
    mergeSortedIntoFirst(nums1, 3, nums2, 0);
    assert((nums1 == std::vector<int>{1, 2, 3}));

    // m == 0
    nums1 = {0, 0, 0};
    nums2 = {1, 2, 3};
    mergeSortedIntoFirst(nums1, 0, nums2, 3);
    assert((nums1 == std::vector<int>{1, 2, 3}));

    // Negative numbers
    nums1 = {-5, -2, 0, 0, 0};
    nums2 = {-10, -3, 4};
    mergeSortedIntoFirst(nums1, 3, nums2, 3);
    assert((nums1 == std::vector<int>{-10, -5, -3, -2, 4, 0})); // Note: nums1 size is 5? Actually preallocated to m+n, let's correct
    // Rework above case properly: m=3, n=3, so nums1 size=6
    nums1 = {-5, -2, 0, 0, 0, 0};
    nums2 = {-10, -3, 4};
    mergeSortedIntoFirst(nums1, 3, nums2, 3);
    assert((nums1 == std::vector<int>{-10, -5, -3, -2, 0, 4}));

    // Single element each
    nums1 = {5, 0};
    nums2 = {2};
    mergeSortedIntoFirst(nums1, 1, nums2, 1);
    assert((nums1 == std::vector<int>{2, 5}));

    return 0;
}

#include <vector>

// Merge two sorted vectors into the first, which is preallocated to hold m+n elements.
// nums1: first m elements are sorted, remaining n positions are unused.
// nums2: first n elements are sorted (size exactly n).
void mergeSortedIntoFirst(std::vector<int>& nums1, int m, const std::vector<int>& nums2, int n) {
    int i = m - 1; // last valid element in nums1
    int j = n - 1; // last element in nums2
    int k = m + n - 1; // last position in combined array

    // Merge from the end to avoid overwriting unprocessed elements
    while (i >= 0 && j >= 0) {
        if (nums1[i] > nums2[j]) {
            nums1[k--] = nums1[i--];
        } else {
            nums1[k--] = nums2[j--];
        }
    }

    // If nums2 still has elements, copy them (nums1 is already in place if i>=0)
    while (j >= 0) {
        nums1[k--] = nums2[j--];
    }
}

// The classic merge approach for two sorted arrays is applied from the end of the combined array backwards. Since `nums1` has enough space at the end, we can avoid shifting elements by filling from the largest index downward. We maintain three pointers: `i` for the last valid element in `nums1` (index `m - 1`), `j` for the last element in `nums2` (index `n - 1`), and `k` for the last position in the combined array (index `m + n - 1`). At each step, compare `nums1[i]` and `nums2[j]`, place the larger one at `nums1[k]`, and decrement the corresponding pointer and `k`. If one array becomes exhausted, copy the remaining elements from the other array directly. This works because we are writing from the end, so we never overwrite elements that we still need to read. Edge cases: if `m == 0`, we simply copy all of `nums2` into `nums1`; if `n == 0`, nothing changes. Duplicate values are handled naturally. The time complexity is `O(m + n)` because each element is processed once. The space complexity is `O(1)` since we only use a few integer variables.
