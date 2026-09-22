// Write a C++ function `insertionSortDescending` that sorts an array of integers in strictly descending order using the classic insertion sort algorithm, but adapted so that each element is inserted into its correct position among the previously sorted elements (which are larger, not smaller, before the current position). The function must accept a raw array and its size, modify the array in place, and return nothing. Your solution must handle arrays of any size, including size 0, 1, or arrays with duplicate values. Do not use any standard library sorting functions; implement the sorting logic manually.

The insertion sort algorithm builds the sorted portion of the array one element at a time. Starting from the second element (index 1), each element is considered as the "key". We compare it with elements to its left (which are already sorted) and shift those elements one position to the right as long as they are smaller than the key (for descending order). This is the opposite comparison from the original ascending version: in ascending sort, we shift while `arr[j] > key`, but for descending we shift while `arr[j] < key`. After the shifting stops, we place the key in the vacated position. For an empty array (n=0) or a single element, the loop does nothing and the function simply returns. Duplicates are handled naturally because shifting stops when `arr[j] >= key` (since we shift only when strictly smaller), so equal elements keep their relative order, but since we only care about descending sorting, duplicate positions are fine. Time complexity is O(n²) in worst and average case because of the nested loops, and O(n) in the best case if the array is already sorted in descending order (then no shifting occurs). Space complexity is O(1) because sorting is done in place with a few integer variables.

#include <cstddef>

// Sorts an integer array in descending order using insertion sort.
// Modifies the array in place. Handles n = 0 gracefully.
void insertionSortDescending(int arr[], std::size_t n) {
    for (std::size_t i = 1; i < n; ++i) {
        int key = arr[i];
        std::size_t j = i;
        // Shift elements that are smaller than key to the right.
        while (j > 0 && arr[j - 1] < key) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }
}

#include <cassert>
#include <cstddef>

void insertionSortDescending(int arr[], std::size_t n); // forward declaration

int main() {
    // Test 1: normal unsorted array
    int arr1[] = {3, 1, 4, 1, 5, 9, 2, 6};
    insertionSortDescending(arr1, 8);
    assert(arr1[0] == 9 && arr1[1] == 6 && arr1[2] == 5 && arr1[3] == 4);
    assert(arr1[4] == 3 && arr1[5] == 2 && arr1[6] == 1 && arr1[7] == 1);

    // Test 2: already descending
    int arr2[] = {10, 8, 5, 2, 1};
    insertionSortDescending(arr2, 5);
    assert(arr2[0] == 10 && arr2[1] == 8 && arr2[2] == 5 && arr2[3] == 2 && arr2[4] == 1);

    // Test 3: all duplicates
    int arr3[] = {7, 7, 7, 7};
    insertionSortDescending(arr3, 4);
    assert(arr3[0] == 7 && arr3[1] == 7 && arr3[2] == 7 && arr3[3] == 7);

    // Test 4: single element
    int arr4[] = {42};
    insertionSortDescending(arr4, 1);
    assert(arr4[0] == 42);

    // Test 5: empty array (should not crash)
    int arr5[] = {};
    insertionSortDescending(arr5, 0);
    // No assertion needed; just verify it runs.

    // Test 6: negative numbers mixed
    int arr6[] = {-5, 0, -10, 3, -1};
    insertionSortDescending(arr6, 5);
    assert(arr6[0] == 3 && arr6[1] == 0 && arr6[2] == -1 && arr6[3] == -5 && arr6[4] == -10);

    // Test 7: two elements in ascending order (should flip to descending)
    int arr7[] = {2, 9};
    insertionSortDescending(arr7, 2);
    assert(arr7[0] == 9 && arr7[1] == 2);

    // Test 8: two elements already descending
    int arr8[] = {11, -3};
    insertionSortDescending(arr8, 2);
    assert(arr8[0] == 11 && arr8[1] == -3);

    return 0;
}
