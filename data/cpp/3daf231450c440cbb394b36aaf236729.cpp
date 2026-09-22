// Write a C++ function `double findMedianSortedArrays(const std::vector<int>& nums1, const std::vector<int>& nums2)` that takes two sorted integer vectors (which may be empty) and returns the median of the combined sorted sequence. The median is the middle element when the total size is odd, and the average of the two middle elements when the total size is even. Your solution must achieve O(log(min(n, m))) time complexity, where n and m are the sizes of the two vectors. Do not merge the arrays; use a divide-and-conquer approach that eliminates a fixed fraction of elements in each recursive step.
// The key insight is to find the k-th smallest element in two sorted arrays without merging. The recursive function `findK(nums1, start1, nums2, start2, k)` handles this. At each step, we aim to discard `k/2` elements from one of the arrays. We compare the element at position `start1 + cnt1 - 1` in `nums1` and `start2 + cnt2 - 1` in `nums2`, where `cnt1` and `cnt2` are chosen so that `cnt1 + cnt2 = k` and neither exceeds the remaining length of its array. If the first array's element is smaller or equal, then all elements before that position in `nums1` can be safely discarded because they cannot be the k-th smallest. We then recurse with `k` reduced by `cnt1`. Otherwise, we discard `cnt2` elements from `nums2`.  
// Base cases: if `k == 1`, return the minimum of the next available elements (or the other array's element if one is exhausted). If one array is fully exhausted, return the `k`-th element from the other. Edge cases: empty arrays, arrays of different lengths, and very small k values. For the median, we compute the `(totalSize/2)`-th and `(totalSize/2 + 1)`-th smallest elements when total size is even, and just the middle element when odd.  
// Time complexity: each recursion reduces `k` by about half, so O(log(n+m)). Space complexity: O(log(n+m)) for the recursion stack (or O(1) if iterative). The algorithm handles all edge cases elegantly without merging.
#include <vector>
#include <algorithm>
#include <cstdint>

// Find the k-th smallest element in the combined sorted arrays nums1 and nums2.
// k is 1-indexed.
int findKth(const std::vector<int>& nums1, int start1,
            const std::vector<int>& nums2, int start2, int k) {
    int n = nums1.size() - start1;
    int m = nums2.size() - start2;

    // If one array is exhausted, take from the other.
    if (n == 0) return nums2[start2 + k - 1];
    if (m == 0) return nums1[start1 + k - 1];

    // Base case: we need the smallest element.
    if (k == 1) {
        return std::min(nums1[start1], nums2[start2]);
    }

    // Determine how many elements to consider from each array.
    int cnt1 = std::min(k / 2, n);
    int cnt2 = k - cnt1;
    // Ensure cnt2 does not exceed the remaining elements of nums2.
    cnt2 = std::min(cnt2, m);
    cnt1 = k - cnt2;

    // Compare the last considered elements.
    if (nums1[start1 + cnt1 - 1] <= nums2[start2 + cnt2 - 1]) {
        // Discard cnt1 elements from nums1.
        return findKth(nums1, start1 + cnt1, nums2, start2, k - cnt1);
    } else {
        // Discard cnt2 elements from nums2.
        return findKth(nums1, start1, nums2, start2 + cnt2, k - cnt2);
    }
}

// Return the median of two sorted arrays.
// Time: O(log(min(n, m))), Space: O(log(n+m)) due to recursion.
double findMedianSortedArrays(const std::vector<int>& nums1,
                              const std::vector<int>& nums2) {
    int total = nums1.size() + nums2.size();
    if (total == 0) return 0.0; // unspecified for empty input, but handle gracefully.
    if (total % 2 == 1) {
        return findKth(nums1, 0, nums2, 0, total / 2 + 1);
    } else {
        int left = findKth(nums1, 0, nums2, 0, total / 2);
        int right = findKth(nums1, 0, nums2, 0, total / 2 + 1);
        return (left + right) / 2.0;
    }
}
#include <cassert>
#include <vector>

int main() {
    // Both non-empty, odd total
    assert(findMedianSortedArrays({1, 3}, {2}) == 2.0);
    // Both non-empty, even total
    assert(findMedianSortedArrays({1, 2}, {3, 4}) == 2.5);
    // One empty array
    assert(findMedianSortedArrays({}, {1, 2, 3}) == 2.0);
    assert(findMedianSortedArrays({1, 2, 3}, {}) == 2.0);
    // Single elements
    assert(findMedianSortedArrays({0}, {0}) == 0.0);
    // Arrays of different lengths with negatives
    assert(findMedianSortedArrays({-5, -1, 3}, {-10, 0, 2, 4}) == 0.0);
    // Large disparity in sizes
    assert(findMedianSortedArrays({1, 5, 9}, {2, 3, 4, 6, 7, 8}) == 5.0);
    // Duplicates
    assert(findMedianSortedArrays({1, 1, 1}, {1, 1}) == 1.0);
    // Both empty (edge case)
    assert(findMedianSortedArrays({}, {}) == 0.0);
    return 0;
}
