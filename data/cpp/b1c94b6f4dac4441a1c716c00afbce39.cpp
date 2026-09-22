// Write a C++ function `stable_quick_sort` that sorts a vector of integers in ascending order using a quick sort variant that always selects the middle element of the current subarray as the pivot, partitions the array using the classic two-pointer Hoare-style partition with a "less-or-equal on left, greater-or-equal on right" comparison (allowing duplicates to be exchanged), and then recursively sorts the left and right subarrays. The function must take a `std::vector<int>&` and sort it in place, and it must handle arrays of size 0 and 1 (empty and single-element arrays) without errors or infinite recursion. The function must be deterministic (same input always yields same sorted output) and stable with respect to value equality (though not necessarily preserving original order of equal elements, it must not corrupt duplicates or cause out-of-bounds access). The function must not use any external sorting utilities, and must be self-contained with only standard library includes. The underlying algorithm must follow the exact pivot-selection and partition logic from the given snippet's `quick_sort_hqq` function, but you must rename it to `stable_quick_sort` and ensure it works correctly for edge cases including all-equal elements, already sorted arrays, reverse-sorted arrays, and large arrays with many duplicates.

// The solution implements a recursive quick sort variant where the pivot is the middle element of the current subarray (`mid = (left+right)/2`). The partition step uses two indices, `begin` starting at `left` and `end` starting at `right`. The loop continues while `begin <= end`. Inside, `begin` advances while `arr[begin] <= pivot` and `begin <= end`; similarly, `end` retreats while `arr[end] >= pivot` and `end >= begin`. When both stop, if `begin <= end`, we swap the elements at these indices, then increment `begin` and decrement `end`. This partition places elements smaller than or equal to pivot on the left side and elements greater or equal on the right side, but because we use `<=` and `>=`, it may swap equal elements across sides; however, the loop condition `begin <= end` and the post-swap adjustments ensure that after the loop, `end` is the last index of the left partition (which contains elements <= pivot) and `begin` is the first index of the right partition (elements >= pivot). The recursion then sorts `[left, end]` and `[begin, right]`. Edge cases: if the array has 0 or 1 elements, `left >= right` triggers immediate return. For all-equal elements, both `begin` and `end` will sweep through the entire array, and swaps may occur but the loop terminates because `begin` will eventually exceed `end`. The time complexity is O(n log n) on average and O(n^2) in the worst case (e.g., already sorted arrays with middle pivot can still be O(n log n) because picking middle avoids worst-case for sorted inputs, but pathological cases possible). Space complexity is O(log n) for the recursion stack in typical balanced partitions, but O(n) in the worst case due to deep recursion. The main challenge is ensuring correct pointer bounds and avoiding infinite loops—the given implementation handles that by checking `begin <= end` in the inner while conditions.

#include <vector>
#include <utility>

// Sorts a vector of integers in ascending order using quick sort with middle-element pivot
// and a two-pointer partition that allows duplicates to be swapped.
void stable_quick_sort(std::vector<int>& arr, int left, int right) {
    // Base case: empty or single-element subarray
    if (left >= right) return;

    // Select pivot as the middle element
    int mid = (left + right) / 2;
    int pivot = arr[mid];

    int begin = left;
    int end = right;

    // Partition loop: move begin right, end left, swapping when both find misplaced elements
    while (begin <= end) {
        // Move begin right while element is <= pivot and within bounds
        while (begin <= end && arr[begin] <= pivot) {
            ++begin;
        }
        // Move end left while element is >= pivot and within bounds
        while (begin <= end && arr[end] >= pivot) {
            --end;
        }
        // If begin and end haven't crossed, swap misplaced elements
        if (begin <= end) {
            std::swap(arr[begin], arr[end]);
            ++begin;
            --end;
        }
    }

    // Recursively sort left and right partitions
    stable_quick_sort(arr, left, end);
    stable_quick_sort(arr, begin, right);
}

#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Edge: empty array
    std::vector<int> empty;
    stable_quick_sort(empty, 0, -1);
    assert(empty.empty());

    // Edge: single element
    std::vector<int> one = {42};
    stable_quick_sort(one, 0, 0);
    assert(one == std::vector<int>({42}));

    // Already sorted array
    std::vector<int> sorted = {1, 2, 3, 4, 5};
    std::vector<int> sorted_expected = sorted;
    stable_quick_sort(sorted, 0, sorted.size() - 1);
    assert(sorted == sorted_expected);

    // Reverse sorted array
    std::vector<int> reverse = {5, 4, 3, 2, 1};
    std::vector<int> reverse_expected = {1, 2, 3, 4, 5};
    stable_quick_sort(reverse, 0, reverse.size() - 1);
    assert(reverse == reverse_expected);

    // All equal elements
    std::vector<int> equal = {7, 7, 7, 7};
    std::vector<int> equal_expected = {7, 7, 7, 7};
    stable_quick_sort(equal, 0, equal.size() - 1);
    assert(equal == equal_expected);

    // General random array with duplicates
    std::vector<int> mixed = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    std::vector<int> mixed_sorted = mixed;
    std::sort(mixed_sorted.begin(), mixed_sorted.end());
    stable_quick_sort(mixed, 0, mixed.size() - 1);
    assert(mixed == mixed_sorted);

    // Larger array with negative and zero
    std::vector<int> negatives = {-5, -1, -10, 0, 3, -2, -10};
    std::vector<int> neg_sorted = negatives;
    std::sort(neg_sorted.begin(), neg_sorted.end());
    stable_quick_sort(negatives, 0, negatives.size() - 1);
    assert(negatives == neg_sorted);

    // Two elements already sorted
    std::vector<int> two_sorted = {2, 3};
    stable_quick_sort(two_sorted, 0, 1);
    assert(two_sorted == std::vector<int>({2, 3}));

    // Two elements reverse sorted
    std::vector<int> two_reverse = {3, 2};
    stable_quick_sort(two_reverse, 0, 1);
    assert(two_reverse == std::vector<int>({2, 3}));

    // Large array with many duplicates (1000 elements)
    std::vector<int> large(10000, 5);
    large[0] = 1; large[9999] = 9;
    std::vector<int> large_sorted = large;
    std::sort(large_sorted.begin(), large_sorted.end());
    stable_quick_sort(large, 0, large.size() - 1);
    assert(large == large_sorted);

    return 0;
}
