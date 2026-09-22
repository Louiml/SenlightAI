// Write a C++ function named `bubbleSort` that takes an array of integers and its size as parameters and sorts the array in ascending order using the bubble sort algorithm. The function must modify the original array in place and return nothing (`void`). The input array may contain duplicate values, negative numbers, and any number of elements from 0 to a large size. The algorithm should perform one complete pass through the array for each element, comparing adjacent pairs and swapping them if they are out of order, with the inner loop shrinking each pass because the largest remaining element "bubbles" to the end. The sorting must be stable (equal elements retain their relative order) and should work correctly for an empty array or an array of size 1.

The solution uses nested loops over the array. For each outer iteration `i` from 0 to `n-1`, the inner loop runs from index 0 to `n-i-2` (or `j < n-i-1`), comparing `arr[j]` and `arr[j+1]`. If `arr[j] > arr[j+1]`, they are swapped. This ensures that after the first full pass, the largest element is placed at the last index; after the second pass, the second largest is placed at the second-to-last index, and so on. The algorithm terminates after `n` outer iterations, at which point the array is sorted. Edge cases: for an empty array or `n == 0`, the loops do not execute and the function returns immediately; for `n == 1`, no swaps are needed. The algorithm is stable because we only swap when `>` (strictly greater), so equal elements remain in their original order. Time complexity is O(n^2) in the worst and average cases (every pass fully compared), and O(n) in the best case if the array is already sorted (though the given implementation does not include an early-exit optimization, so it still runs O(n^2) even when sorted). Space complexity is O(1) because only a temporary variable for swapping is used.

#include <vector>

// Sorts an integer array in ascending order using the bubble sort algorithm.
// Modifies the array in place; returns nothing.
// Stable: equal elements retain their original relative order.
void bubbleSort(int arr[], int n) {
    if (n <= 1) {
        return; // Already sorted or empty
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                // Swap adjacent elements if out of order
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

#include <cassert>
#include <vector>

// (Declare the function prototype if not already visible)
void bubbleSort(int arr[], int n);

// Helper to compare array with a vector of expected values
bool arrayEquals(const int arr[], const std::vector<int>& expected, int n) {
    if (n != static_cast<int>(expected.size())) return false;
    for (int i = 0; i < n; ++i) {
        if (arr[i] != expected[i]) return false;
    }
    return true;
}

int main() {
    // Test 1: Basic sorting
    int arr1[] = {5, 1, 4, 2, 8};
    bubbleSort(arr1, 5);
    assert(arrayEquals(arr1, {1, 2, 4, 5, 8}, 5));

    // Test 2: Already sorted array
    int arr2[] = {1, 2, 3, 4};
    bubbleSort(arr2, 4);
    assert(arrayEquals(arr2, {1, 2, 3, 4}, 4));

    // Test 3: Reverse sorted array
    int arr3[] = {9, 7, 5, 3, 1};
    bubbleSort(arr3, 5);
    assert(arrayEquals(arr3, {1, 3, 5, 7, 9}, 5));

    // Test 4: Duplicate values
    int arr4[] = {4, 2, 4, 2, 4};
    bubbleSort(arr4, 5);
    assert(arrayEquals(arr4, {2, 2, 4, 4, 4}, 5));

    // Test 5: Negative numbers
    int arr5[] = {-3, 0, -10, 7, -1};
    bubbleSort(arr5, 5);
    assert(arrayEquals(arr5, {-10, -3, -1, 0, 7}, 5));

    // Test 6: Single element
    int arr6[] = {42};
    bubbleSort(arr6, 1);
    assert(arrayEquals(arr6, {42}, 1));

    // Test 7: Empty array (size 0)
    int arr7[] = {};
    bubbleSort(arr7, 0);
    assert(arrayEquals(arr7, {}, 0));

    // Test 8: Large mixed array
    int arr8[] = {100, 23, 45, -100, 0, 99, 3, 77, -45, 12};
    bubbleSort(arr8, 10);
    assert(arrayEquals(arr8, {-100, -45, 0, 3, 12, 23, 45, 77, 99, 100}, 10));

    return 0;
}
