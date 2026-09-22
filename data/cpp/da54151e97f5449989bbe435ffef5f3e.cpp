Write a C++ function named `sortAndModify` that accepts a non-const integer array and its size. The function must sort the array in ascending order using the bubble sort algorithm, and then, after sorting, replace every element at an even index (0-based) with its square while leaving the elements at odd indices unchanged. The function should modify the array in place and return nothing (void). Assume the input array is non-empty and contains at least one integer; no error handling for null pointers or zero size is required.

#include <cassert>

// Assume sortAndModify is declared above

int main() {
    // Test 1: Basic unsorted array
    int arr1[] = {5, 3, 8, 1};
    sortAndModify(arr1, 4);
    assert(arr1[0] == 1 && arr1[1] == 3 && arr1[2] == 64 && arr1[3] == 8);

    // Test 2: Already sorted array
    int arr2[] = {1, 2, 3, 4};
    sortAndModify(arr2, 4);
    assert(arr2[0] == 1 && arr2[1] == 2 && arr2[2] == 9 && arr2[3] == 4);

    // Test 3: Single element
    int arr3[] = {7};
    sortAndModify(arr3, 1);
    assert(arr3[0] == 49);

    // Test 4: Duplicates
    int arr4[] = {4, 2, 2, 4};
    sortAndModify(arr4, 4);
    assert(arr4[0] == 4 && arr4[1] == 2 && arr4[2] == 16 && arr4[3] == 4);

    // Test 5: Negative numbers
    int arr5[] = {-3, -1, 2};
    sortAndModify(arr5, 3);
    assert(arr5[0] == 1 && arr5[1] == 2 && arr5[2] == 9);

    // Test 6: All same values except one
    int arr6[] = {5, 5, 3, 5};
    sortAndModify(arr6, 4);
    assert(arr6[0] == 9 && arr6[1] == 5 && arr6[2] == 25 && arr6[3] == 5);

    // Test 7: Two elements reversed
    int arr7[] = {9, 4};
    sortAndModify(arr7, 2);
    assert(arr7[0] == 16 && arr7[1] == 9);

    // Test 8: Large array with known sorted result (n=6)
    int arr8[] = {6, 1, 4, 3, 5, 2};
    sortAndModify(arr8, 6);
    int expected8[] = {1, 2, 9, 4, 25, 6};
    for (int i = 0; i < 6; ++i) assert(arr8[i] == expected8[i]);

    return 0;
}

#include <utility> // for std::swap

// Sorts the array in ascending order using bubble sort, then squares elements at even indices.
void sortAndModify(int arr[], int n) {
    // Bubble sort
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }

    // Square elements at even indices (0, 2, 4, ...)
    for (int i = 0; i < n; i += 2) {
        arr[i] = arr[i] * arr[i];
    }
}

// The solution applies a standard bubble sort to the input array, repeatedly swapping adjacent out-of-order elements until the entire array is sorted. Bubble sort has a worst-case and average time complexity of O(n²), where n is the size of the array, and uses O(1) auxiliary space because it sorts in place. After sorting, iterate through the array with an index loop `for (int i = 0; i < n; i += 2)` and update `arr[i] = arr[i] * arr[i]`. Edge cases to consider: when the array has size 1, bubble sort does nothing (the outer loop runs zero times), and the single element (which is at even index 0) gets squared. For an array with two elements, after sorting, only index 0 is squared, index 1 remains as is. Negative numbers become positive after squaring, which is expected. The function must correctly handle duplicate values and already sorted arrays (no swaps needed). The const correctness is not applicable for the array parameter because the function modifies its contents.
