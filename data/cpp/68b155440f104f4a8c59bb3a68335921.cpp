// Write a C++ function `heapSortDescending(int arr[], int n)` that sorts an array of integers in **descending** order using an in-place heap sort algorithm. The function must modify the input array directly and return `void`. You may implement helper functions such as `heapify` but must ensure the overall sort is efficient and handles edge cases like empty arrays, single-element arrays, and arrays with duplicate or negative values. The function should not use any standard sorting library functions.

// The solution adapts the standard max-heap-based heap sort to produce descending order by instead building a **min-heap**. In a min-heap, the smallest element is at the root. The algorithm works in two phases:  
// 1. **Build heap**: Starting from the last non-leaf node (`n/2 - 1`) down to the root, call a `heapify` function that ensures the subtree rooted at index `i` satisfies the min-heap property (parent ≤ children).  
// 2. **Extract repeatedly**: For `i` from `n-1` down to 1, swap the root (current minimum) with the last element of the current heap, then reduce the heap size by 1 and call `heapify` on the new root. After all iterations, the array is sorted in descending order (largest at the end, smallest at the front).  
//
// Key edge cases:  
// - Empty array (`n == 0`): no operation needed, return immediately.  
// - Single element (`n == 1`): already sorted, return immediately.  
// - Duplicate values: min-heap property handles them naturally; swaps may reorder equal elements but the sort remains correct.  
// - Negative numbers: work identically since comparisons use `<` and `>`.  
//
// Time complexity: `heapify` runs in O(log n), and it is called O(n) times during build and O(n) times during extraction, so total time is O(n log n). Space complexity is O(log n) auxiliary stack space due to recursion in `heapify`, or O(1) if implemented iteratively.

#include <algorithm> // for std::swap

// Heapify a subtree rooted at index i in arr[0..n-1] to maintain min-heap property.
// Assumes left and right subtrees of i are already valid min-heaps.
void minHeapify(int arr[], int n, int i) {
    int smallest = i; // smallest holds index of smallest among root, left, and right
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    // Compare left child with current smallest
    if (left < n && arr[left] < arr[smallest]) {
        smallest = left;
    }

    // Compare right child with current smallest
    if (right < n && arr[right] < arr[smallest]) {
        smallest = right;
    }

    // If smallest is not root, swap and continue heapifying down
    if (smallest != i) {
        std::swap(arr[i], arr[smallest]);
        minHeapify(arr, n, smallest);
    }
}

// Sort arr[0..n-1] in descending order using in-place heap sort (min-heap variant).
void heapSortDescending(int arr[], int n) {
    if (n <= 1) {
        return; // Already sorted or nothing to sort
    }

    // Build a min-heap from the array
    for (int i = n / 2 - 1; i >= 0; --i) {
        minHeapify(arr, n, i);
    }

    // Extract elements from heap one by one
    for (int i = n - 1; i > 0; --i) {
        // Move current root (minimum) to the end
        std::swap(arr[0], arr[i]);

        // Call minHeapify on the reduced heap
        minHeapify(arr, i, 0);
    }
}

#include <cassert>

int main() {
    // Test 1: Standard case
    int arr1[] = {12, 11, 13, 5, 6, 7};
    heapSortDescending(arr1, 6);
    assert(arr1[0] == 13 && arr1[1] == 12 && arr1[2] == 11 && arr1[3] == 7 && arr1[4] == 6 && arr1[5] == 5);

    // Test 2: Already descending
    int arr2[] = {5, 4, 3, 2, 1};
    heapSortDescending(arr2, 5);
    assert(arr2[0] == 5 && arr2[1] == 4 && arr2[2] == 3 && arr2[3] == 2 && arr2[4] == 1);

    // Test 3: Already ascending (should reverse)
    int arr3[] = {1, 2, 3, 4, 5};
    heapSortDescending(arr3, 5);
    assert(arr3[0] == 5 && arr3[1] == 4 && arr3[2] == 3 && arr3[3] == 2 && arr3[4] == 1);

    // Test 4: Single element
    int arr4[] = {42};
    heapSortDescending(arr4, 1);
    assert(arr4[0] == 42);

    // Test 5: Empty array (should not crash)
    int arr5[] = {};
    heapSortDescending(arr5, 0);

    // Test 6: Negative numbers and duplicates
    int arr6[] = {-3, -1, -2, 0, -1, 5, 5};
    heapSortDescending(arr6, 7);
    assert(arr6[0] == 5 && arr6[1] == 5 && arr6[2] == 0 && arr6[3] == -1 && arr6[4] == -1 && arr6[5] == -2 && arr6[6] == -3);

    // Test 7: Large sorted descending
    int arr7[] = {100, 99, 98, 97};
    heapSortDescending(arr7, 4);
    assert(arr7[0] == 100 && arr7[1] == 99 && arr7[2] == 98 && arr7[3] == 97);

    // Test 8: All identical
    int arr8[] = {7, 7, 7, 7};
    heapSortDescending(arr8, 4);
    assert(arr8[0] == 7 && arr8[1] == 7 && arr8[2] == 7 && arr8[3] == 7);

    return 0;
}
