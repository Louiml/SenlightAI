// Write a C++ function named `selectionSort` that takes an array of integers (as a pointer) and its length, and sorts the array in ascending order using the selection sort algorithm. The function must modify the array in place and return void. The input array may contain duplicate values, negative numbers, and any length from 1 upward. The function should not use any standard library sorting functions (e.g., `std::sort`), only basic operations and a helper `swap` if needed. Ensure the function is `const`-correct where applicable (i.e., the length parameter should be `const int`, but the array itself is modifiable).

The selection sort algorithm works by dividing the array into a sorted prefix and an unsorted suffix, initially the sorted prefix is empty and the whole array is unsorted. For each position `i` from 0 to `n-1`, find the index of the minimum element in the unsorted portion `[i, n-1]`, then swap that minimum with the element at position `i`. This grows the sorted prefix by one element each iteration. Duplicate values do not require special handling—swapping with an equal value is harmless. Negative numbers are handled naturally since comparisons use `<`. Edge cases include an array of length 1 (no swaps needed) and arrays already sorted (the inner loop still runs but no swaps occur because `min == i`). Time complexity is \(O(n^2)\) for all cases (best, average, worst) due to the nested loops, and space complexity is \(O(1)\) auxiliary (only a few integer variables). The `swap` helper uses pointers to exchange values, and the main function uses a simple inde tracking.

#include <utility> // for std::swap (optional, but we define our own for clarity)

// Helper function to swap two integers via pointers.
void swapInts(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Sort an array of integers in ascending order using selection sort.
// The array is modified in place; length is the number of elements.
void selectionSort(int ar[], const int length) {
    for (int i = 0; i < length; ++i) {
        int minIndex = i;
        // Find the smallest element in the unsorted suffix.
        for (int j = i + 1; j < length; ++j) {
            if (ar[j] < ar[minIndex]) {
                minIndex = j;
            }
        }
        // Swap only if a smaller element was found.
        if (minIndex != i) {
            swapInts(&ar[minIndex], &ar[i]);
        }
    }
}

#include <cassert>

int main() {
    // Test 1: Basic unsorted array
    int arr1[] = {5, 2, 9, 1, 5};
    selectionSort(arr1, 5);
    assert(arr1[0] == 1 && arr1[1] == 2 && arr1[2] == 5 && arr1[3] == 5 && arr1[4] == 9);

    // Test 2: Negative numbers and duplicates
    int arr2[] = {-3, -1, -7, -1, 0};
    selectionSort(arr2, 5);
    assert(arr2[0] == -7 && arr2[1] == -3 && arr2[2] == -1 && arr2[3] == -1 && arr2[4] == 0);

    // Test 3: Already sorted
    int arr3[] = {1, 2, 3, 4};
    selectionSort(arr3, 4);
    assert(arr3[0] == 1 && arr3[1] == 2 && arr3[2] == 3 && arr3[3] == 4);

    // Test 4: Reverse sorted
    int arr4[] = {10, 8, 6, 4, 2};
    selectionSort(arr4, 5);
    assert(arr4[0] == 2 && arr4[1] == 4 && arr4[2] == 6 && arr4[3] == 8 && arr4[4] == 10);

    // Test 5: Single element
    int arr5[] = {42};
    selectionSort(arr5, 1);
    assert(arr5[0] == 42);

    // Test 6: All elements equal
    int arr6[] = {7, 7, 7};
    selectionSort(arr6, 3);
    assert(arr6[0] == 7 && arr6[1] == 7 && arr6[2] == 7);

    return 0;
}
