/*
Write a C++ function `bool isSortedAfterSort(int arr[], int n, int sortChoice)` that takes an array of integers, its length `n`, and an integer `sortChoice` (where `1` selects Shell Sort, `2` selects Merge Sort, and `3` selects Heap Sort). The function must perform the corresponding sorting algorithm on the array **in-place**, then return `true` if the resulting array is non‑decreasing (sorted in ascending order) for all valid inputs, and `false` otherwise (e.g., if `n <= 0` or `sortChoice` is not 1, 2, or 3). The function must handle duplicate values, negative numbers, and arrays of length 1 correctly. The sorting algorithms must be implemented exactly as given in the snippet, but you are free to reorganize helper functions. Do **not** include any `main` function; only provide the function(s) and necessary headers.
*/

#include <algorithm>
#include <cstddef>

// Helper for Heap Sort
void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;
    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

// Shell Sort
void shellSort(int arr[], int n) {
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap)
                arr[j] = arr[j - gap];
            arr[j] = temp;
        }
    }
}

// Merge helper
void merge(int arr[], int left, int middle, int right) {
    int n1 = middle - left + 1;
    int n2 = right - middle;
    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[middle + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

// Merge Sort
void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;
        mergeSort(arr, left, middle);
        mergeSort(arr, middle + 1, right);
        merge(arr, left, middle, right);
    }
}

// Heap Sort
void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
    for (int i = n - 1; i >= 0; i--) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// Main solution function
bool isSortedAfterSort(int arr[], int n, int sortChoice) {
    if (n <= 0 || (sortChoice < 1 || sortChoice > 3))
        return false;

    if (sortChoice == 1)
        shellSort(arr, n);
    else if (sortChoice == 2)
        mergeSort(arr, 0, n - 1);
    else // sortChoice == 3
        heapSort(arr, n);

    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1])
            return false;
    }
    return true;
}

#include <cassert>

int main() {
    // Test 1: Shell Sort basic
    int arr1[] = {12, 11, 13, 5, 6, 7};
    assert(isSortedAfterSort(arr1, 6, 1) == true);
    assert(arr1[0] <= arr1[1] && arr1[1] <= arr1[2] && arr1[2] <= arr1[3] && arr1[3] <= arr1[4] && arr1[4] <= arr1[5]);

    // Test 2: Merge Sort with duplicates and negatives
    int arr2[] = {-5, -1, -10, 0, 3, 3, -1};
    assert(isSortedAfterSort(arr2, 7, 2) == true);
    for (int i = 0; i < 6; i++) assert(arr2[i] <= arr2[i+1]);

    // Test 3: Heap Sort with single element
    int arr3[] = {42};
    assert(isSortedAfterSort(arr3, 1, 3) == true);

    // Test 4: Invalid n
    int arr4[] = {1, 2};
    assert(isSortedAfterSort(arr4, 0, 1) == false);

    // Test 5: Invalid sortChoice
    int arr5[] = {3, 1, 2};
    assert(isSortedAfterSort(arr5, 3, 4) == false);
    // Ensure array unchanged for invalid selection
    assert(arr5[0] == 3 && arr5[1] == 1 && arr5[2] == 2);

    // Test 6: Already sorted array for all three algorithms
    int arr6[] = {1, 2, 3, 4, 5};
    assert(isSortedAfterSort(arr6, 5, 1) == true);
    int arr7[] = {1, 2, 3, 4, 5};
    assert(isSortedAfterSort(arr7, 5, 2) == true);
    int arr8[] = {1, 2, 3, 4, 5};
    assert(isSortedAfterSort(arr8, 5, 3) == true);

    // Test 7: Reverse sorted array with heap sort
    int arr9[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    assert(isSortedAfterSort(arr9, 9, 3) == true);

    // Test 8: Large duplicate values with shell sort
    int arr10[] = {7, 7, 7, 7};
    assert(isSortedAfterSort(arr10, 4, 1) == true);
}

// The solution must implement three classic comparison‑based sorting algorithms. **Shell Sort** uses a gap sequence starting at `n/2` and halves each iteration; it performs insertion‑sort‑like passes on subarrays separated by the current gap. **Merge Sort** divides the array recursively into left and right halves, sorts each half, then merges them using temporary arrays. **Heap Sort** first builds a max‑heap via `heapify` from the last non‑leaf node upward, then repeatedly swaps the root (maximum) with the last unsorted element and restores the heap property on the reduced heap. All three must sort in‑place (except temporary arrays inside merge). The function chooses the algorithm based on `sortChoice` and calls it. Edge cases: `n <= 0` is invalid and must return `false`; `n == 1` is already sorted, but the selected algorithm must still be called (it should handle it without errors). Duplicate and negative values are naturally handled by comparison operators. After sorting, verify that `arr[i] <= arr[i+1]` for all `i` from 0 to `n-2`. Time complexity: Shell Sort worst‑case is `O(n^2)` but often better; Merge Sort is `O(n log n)` with `O(n)` auxiliary space; Heap Sort is `O(n log n)` with `O(1)` auxiliary space. Space for helpers is constant except merge’s temporary arrays.
