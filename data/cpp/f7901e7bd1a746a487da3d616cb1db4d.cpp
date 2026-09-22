// Given an unsorted array of `N` distinct integers (where `N >= 1`), write a function that sorts the array in ascending order using the heap sort algorithm. The function must operate directly on the array (in-place), accept the array and its length as parameters, and return nothing. The original array should be rearranged so that after the call, `arr[0] <= arr[1] <= ... <= arr[N-1]`. The implementation must build a max-heap from the input and then repeatedly extract the maximum element to place it at the end of the array, following the classic heap sort procedure. Edge cases include `N = 1` (the array is already sorted) and arrays with duplicate values, though the problem guarantees distinctness for simplicity.

#include <cassert>
#include <vector>
#include <algorithm>

// Forward declaration for the function to test (assume it's included before).
void heap_sort(std::vector<int>& arr);

int main() {
    // Single element
    std::vector<int> a1 = {42};
    heap_sort(a1);
    assert(a1 == std::vector<int>({42}));

    // Already sorted
    std::vector<int> a2 = {1, 2, 3, 4, 5};
    heap_sort(a2);
    assert(a2 == std::vector<int>({1, 2, 3, 4, 5}));

    // Reverse sorted
    std::vector<int> a3 = {5, 4, 3, 2, 1};
    heap_sort(a3);
    assert(a3 == std::vector<int>({1, 2, 3, 4, 5}));

    // Random order with distinct values
    std::vector<int> a4 = {10, -3, 7, 0, 2, -8, 15};
    std::vector<int> expected4 = a4;
    std::sort(expected4.begin(), expected4.end());
    heap_sort(a4);
    assert(a4 == expected4);

    // Two elements
    std::vector<int> a5 = {9, 1};
    heap_sort(a5);
    assert(a5 == std::vector<int>({1, 9}));

    // Duplicate values (though not guaranteed, still works)
    std::vector<int> a6 = {3, 1, 3, 2, 1};
    std::vector<int> expected6 = a6;
    std::sort(expected6.begin(), expected6.end());
    heap_sort(a6);
    assert(a6 == expected6);

    // Large size to stress test
    std::vector<int> a7(1000);
    for (int i = 0; i < 1000; ++i) a7[i] = 1000 - i; // reverse
    std::vector<int> expected7 = a7;
    std::sort(expected7.begin(), expected7.end());
    heap_sort(a7);
    assert(a7 == expected7);

    return 0;
}

#include <vector>
#include <algorithm>

// Heapify the subtree rooted at index i in a max-heap stored in arr[0..heap_size-1].
void max_heapify(std::vector<int>& arr, int heap_size, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < heap_size && arr[left] > arr[largest]) {
        largest = left;
    }
    if (right < heap_size && arr[right] > arr[largest]) {
        largest = right;
    }

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        max_heapify(arr, heap_size, largest);
    }
}

// Sort the given array in ascending order using heap sort (in-place).
void heap_sort(std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    if (n < 2) return;

    // Build max-heap: start from the last internal node.
    for (int i = n / 2 - 1; i >= 0; --i) {
        max_heapify(arr, n, i);
    }

    // Extract maximum from the heap and place at the end.
    for (int i = n - 1; i > 0; --i) {
        std::swap(arr[0], arr[i]);       // Move current root to the end.
        max_heapify(arr, i, 0);          // Restore heap property for reduced heap.
    }
}

// The solution uses the standard heap sort algorithm with a max-heap. First, build the max-heap from the unsorted array using a bottom-up heapify approach: starting from the last non-leaf node (index `N/2 - 1`) down to index 0, call a `heapify` function that ensures the subtree rooted at that index satisfies the max-heap property (parent ≥ children). Because the heap is stored as an array with 0-based indexing, for a node at index `i`, its left child is at `2*i+1` and its right child at `2*i+2`. The `heapify` function recursively (or iteratively) swaps the node with its largest child if the node is smaller, and continues down that subtree. After building the heap, perform `N-1` extraction steps: swap the root (maximum) with the last element of the current heap, reduce the heap size by one, and call `heapify` on the root to restore the heap property for the reduced heap. This leaves the array sorted in ascending order at the end. Edge cases: when `N = 1`, the heap build and extraction loop are trivially handled (the single element is already sorted). Duplicate values are not present per spec, but if they were, the comparisons still work correctly. Time complexity: building the heap is O(N), and each of the N-1 extractions calls heapify which is O(log N), so total O(N log N). Space complexity: O(1) auxiliary space, since sorting is in-place and recursion depth is O(log N) if recursive heapify is used, but can be made O(1) with an iterative version.
