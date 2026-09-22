// Write a C++ function named `heapSortAscending` that takes a non-empty array of `long long` integers (as a pointer) and its size `n`, and sorts the array in ascending order in-place using the heapsort algorithm. The function must be self-contained (no external sorting utilities), must handle duplicate values, must correctly sort arrays of any size from 1 to large values, and must not use any global variables or static arrays. The sorting must be stable only in the sense that duplicates remain present; heap sort is not stable, but that is acceptable. The function should be declared with appropriate `const` correctness for the parameters (e.g., the pointer is not `const`, but the size parameter can be `const`). Do not include a `main` function or any I/O; the function should be purely computational.

// The solution implements the classic heapsort algorithm. First, build a max-heap from the input array using the `MaxHeapify` procedure, which ensures that for every node `i`, the subtree rooted at `i` satisfies the max-heap property (parent is larger than or equal to its children). The build process iterates from the last non-leaf node down to the root, calling `MaxHeapify` on each. After the heap is built, the largest element is at index 0. Then, for each index from `n-1` down to 1, swap the root with the current last element, reduce the heap size by one, and call `MaxHeapify` on the new root to restore the heap property for the reduced heap. This places the maximum element at the end of the array, effectively sorting in ascending order. Edge cases: when `n == 1`, the array is already sorted, and the algorithm handles it correctly because the loop for building the heap runs with `i = 0` and the subsequent loop does not execute. When the array contains duplicates, the heap property comparisons use `>` only, so duplicates are treated correctly without infinite recursion. The time complexity is `O(n log n)` for all cases (best, average, worst), because building the heap is `O(n)` and each of the `n-1` `MaxHeapify` calls is `O(log n)`. The space complexity is `O(log n)` due to recursion depth in `MaxHeapify`, but that can be considered `O(log n)` auxiliary space; if rewritten iteratively, it would be `O(1)`. For this solution, we keep recursion for clarity, so the auxiliary space is `O(log n)` in the worst case (stack depth). The function modifies the input array in-place and does not allocate dynamic memory.

#include <cstddef>

// Sorts an array of long long integers in ascending order using the heapsort algorithm.
// arr: pointer to the first element of the array (non-null, size >= 1)
// n: number of elements in the array
void heapSortAscending(long long* arr, const std::size_t n) {
    // Helper: maintain max-heap property for subtree rooted at index i within heap of size heapSize.
    auto maxHeapify = [&](auto&& self, long long* a, std::size_t i, const std::size_t heapSize) -> void {
        std::size_t largest = i;
        const std::size_t left = 2 * i + 1;
        const std::size_t right = 2 * i + 2;

        if (left < heapSize && a[left] > a[largest]) {
            largest = left;
        }
        if (right < heapSize && a[right] > a[largest]) {
            largest = right;
        }

        if (largest != i) {
            // Swap a[i] and a[largest]
            const long long temp = a[i];
            a[i] = a[largest];
            a[largest] = temp;
            // Recurse on the affected subtree
            self(self, a, largest, heapSize);
        }
    };

    // Build max-heap: start from last non-leaf node down to root.
    for (std::size_t i = n / 2; i > 0; --i) {
        maxHeapify(maxHeapify, arr, i - 1, n);
    }
    if (n > 0) {
        maxHeapify(maxHeapify, arr, 0, n);
    }

    // Extract elements one by one from the heap.
    for (std::size_t end = n - 1; end > 0; --end) {
        // Swap root (max) with the current end element.
        const long long temp = arr[0];
        arr[0] = arr[end];
        arr[end] = temp;
        // Restore heap property on the reduced heap (size = end).
        maxHeapify(maxHeapify, arr, 0, end);
    }
}

#include <cassert>
#include <cstddef>
#include <vector>

// Declaration of the tested function
void heapSortAscending(long long* arr, const std::size_t n);

int main() {
    // Test 1: Already sorted array
    long long arr1[] = {1, 2, 3, 4, 5};
    heapSortAscending(arr1, 5);
    std::vector<long long> expected1 = {1, 2, 3, 4, 5};
    for (std::size_t i = 0; i < 5; ++i) assert(arr1[i] == expected1[i]);

    // Test 2: Reverse sorted array
    long long arr2[] = {9, 7, 5, 3, 1};
    heapSortAscending(arr2, 5);
    std::vector<long long> expected2 = {1, 3, 5, 7, 9};
    for (std::size_t i = 0; i < 5; ++i) assert(arr2[i] == expected2[i]);

    // Test 3: Array with duplicates and negatives
    long long arr3[] = {-5, 10, -5, 0, 10, 3};
    heapSortAscending(arr3, 6);
    std::vector<long long> expected3 = {-5, -5, 0, 3, 10, 10};
    for (std::size_t i = 0; i < 6; ++i) assert(arr3[i] == expected3[i]);

    // Test 4: Single element
    long long arr4[] = {42};
    heapSortAscending(arr4, 1);
    assert(arr4[0] == 42);

    // Test 5: Two elements, unsorted
    long long arr5[] = {2, 1};
    heapSortAscending(arr5, 2);
    std::vector<long long> expected5 = {1, 2};
    for (std::size_t i = 0; i < 2; ++i) assert(arr5[i] == expected5[i]);

    // Test 6: Large values
    long long arr6[] = {1000000000000LL, -1000000000000LL, 0};
    heapSortAscending(arr6, 3);
    std::vector<long long> expected6 = {-1000000000000LL, 0, 1000000000000LL};
    for (std::size_t i = 0; i < 3; ++i) assert(arr6[i] == expected6[i]);

    // Test 7: All identical elements
    long long arr7[] = {7, 7, 7, 7};
    heapSortAscending(arr7, 4);
    std::vector<long long> expected7 = {7, 7, 7, 7};
    for (std::size_t i = 0; i < 4; ++i) assert(arr7[i] == expected7[i]);

    // Test 8: Even count, already max-heap but not sorted
    long long arr8[] = {10, 5, 8, 1, 4, 3};
    heapSortAscending(arr8, 6);
    std::vector<long long> expected8 = {1, 3, 4, 5, 8, 10};
    for (std::size_t i = 0; i < 6; ++i) assert(arr8[i] == expected8[i]);

    // Test 9: All negative numbers
    long long arr9[] = {-1, -3, -2, -10};
    heapSortAscending(arr9, 4);
    std::vector<long long> expected9 = {-10, -3, -2, -1};
    for (std::size_t i = 0; i < 4; ++i) assert(arr9[i] == expected9[i]);

    // Test 10: Random-ish pattern
    long long arr10[] = {5, 1, 4, 2, 8, 0, 3, 6, 7, 9};
    heapSortAscending(arr10, 10);
    std::vector<long long> expected10 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (std::size_t i = 0; i < 10; ++i) assert(arr10[i] == expected10[i]);

    return 0;
}
