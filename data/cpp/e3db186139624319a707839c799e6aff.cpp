// Write a C++ function named `findKthSmallest` that takes a non-empty `std::vector<int>` and an integer `k` (1 ≤ k ≤ vector size) and returns the k-th smallest element in the vector. The function must work in-place (allowed to modify the input vector) and must use a quickselect-based partitioning approach. The vector may contain duplicate values, and the function must correctly handle cases where the k-th smallest element appears multiple times (i.e., return the correct value for the given rank). Do not use `std::nth_element` or any other standard library sorting/selection algorithms.

#include <cassert>
#include <vector>
#include <iostream>

// Function declaration (must match the solution's signature)
int findKthSmallest(std::vector<int>& nums, int k);

int main() {
    // Basic tests
    {
        std::vector<int> v = {3, 2, 1, 5, 4};
        assert(findKthSmallest(v, 1) == 1);
        assert(findKthSmallest(v, 5) == 5);
    }
    {
        std::vector<int> v = {7, 7, 7, 7};
        assert(findKthSmallest(v, 1) == 7);
        assert(findKthSmallest(v, 4) == 7);
    }
    // Single element
    {
        std::vector<int> v = {42};
        assert(findKthSmallest(v, 1) == 42);
    }
    // Negative numbers and duplicates
    {
        std::vector<int> v = {-5, 10, -5, 3, 3, 8, -5};
        assert(findKthSmallest(v, 1) == -5);
        assert(findKthSmallest(v, 3) == -5);
        assert(findKthSmallest(v, 4) == 3);
        assert(findKthSmallest(v, 7) == 10);
    }
    // Already sorted ascending
    {
        std::vector<int> v = {1, 2, 3, 4, 5, 6};
        assert(findKthSmallest(v, 3) == 3);
        assert(findKthSmallest(v, 6) == 6);
    }
    // Already sorted descending
    {
        std::vector<int> v = {6, 5, 4, 3, 2, 1};
        assert(findKthSmallest(v, 2) == 2);
        assert(findKthSmallest(v, 5) == 5);
    }
    // Large k (largest element)
    {
        std::vector<int> v = {100, -200, 0, 50, -200, 100};
        assert(findKthSmallest(v, 6) == 100);
    }
    // Random-ish duplicates with many values
    {
        std::vector<int> v = {9, 9, 9, 8, 8, 7, 7, 6, 6, 5};
        assert(findKthSmallest(v, 1) == 5);
        assert(findKthSmallest(v, 2) == 6);
        assert(findKthSmallest(v, 3) == 6);
        assert(findKthSmallest(v, 10) == 9);
    }
    std::cout << "All tests passed\n";
    return 0;
}

#include <vector>
#include <algorithm>

// Returns the k-th smallest element (1-indexed) in the vector.
// Modifies the input vector in-place using quickselect.
int findKthSmallest(std::vector<int>& nums, int k) {
    // Recursive helper using Hoare partition.
    // 'kIndex' is the 0-based target index we are looking for.
    // The function returns the element at that index.
    std::function<int(int, int, int)> quickSelect = [&](int left, int right, int kIndex) -> int {
        if (left == right) return nums[left];  // Only one element in the range.

        // Hoare partition with middle element as pivot.
        int i = left - 1;
        int j = right + 1;
        int pivot = nums[(left + right) / 2];

        while (i < j) {
            do { ++i; } while (nums[i] < pivot);
            do { --j; } while (nums[j] > pivot);
            if (i < j) std::swap(nums[i], nums[j]);
        }

        // After partitioning, j is the boundary.
        // All elements in [left, j] are <= pivot, and all in [j+1, right] are >= pivot.
        // Recursively search in the subarray that contains the kIndex.
        if (kIndex <= j) {
            return quickSelect(left, j, kIndex);
        } else {
            return quickSelect(j + 1, right, kIndex);
        }
    };

    // Convert 1-based k to 0-based index.
    int target = k - 1;
    return quickSelect(0, static_cast<int>(nums.size()) - 1, target);
}

// The solution implements a quickselect algorithm, which is a variation of quicksort that only recurses into the partition containing the target index. The function keeps a working subarray defined by `left` and `right` indices. It selects a pivot as the middle element of the current range (using `nums[(left + right) / 2]`). Then it performs a Hoare partition: two pointers (`i` starting from `left - 1` and `j` from `right + 1`) move toward each other, swapping elements that are on the wrong side of the pivot. After the partitioning loop, the index `j` represents the boundary where all elements to the left of `j` are ≤ pivot and all to the right are ≥ pivot (the exact position depends on pivot placement). The target index for the k-th smallest is `target = k - 1`. If `j >= target`, the answer lies in the left subarray `[left, j]`, so we recurse there; otherwise we recurse into `[j + 1, right]`. When `left == right`, the subarray has one element, which is the answer.
//
// Edge cases: k = 1 (smallest) and k = n (largest) must work; duplicates are handled naturally because equal elements are placed arbitrarily on either side, but the rank still corresponds to the correct value. The pivot selection avoids the worst-case for already sorted arrays (since it uses the middle). The recursion depth is O(log n) on average, but the worst case is O(n) (e.g., if partitions are very unbalanced). However, average-case time complexity is O(n) because each level processes a smaller portion, and the total work across levels is linear (n + n/2 + n/4 + ... = 2n). Space complexity is O(log n) for the recursion stack in the average case (O(n) worst-case).
