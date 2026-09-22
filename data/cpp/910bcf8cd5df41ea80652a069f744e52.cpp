// Write a standalone C++ function named `selectionSort` that takes a non-empty array of integers (passed as a pointer) and its size `n`, sorts the array in ascending order using the selection sort algorithm, and returns the number of swaps performed during the sorting process. The function must modify the array in place and be `const`-correct in the sense that it should not modify any data other than the array contents. The function must handle the edge case where `n` is 0 or 1 gracefully (returning 0 swaps without error). Additionally, implement a helper function `printArray` that prints the sorted array to standard output, but the main solution function must only return the swap count. The task is to implement both functions, with `selectionSort` calling `printArray` at the end to display the sorted result.

The selection sort algorithm works by repeatedly finding the minimum element from the unsorted part of the array and swapping it with the element at the current position. The outer loop runs from index 0 to `n-2` (since after placing `n-1` elements, the last one is automatically in place). For each pass, the inner loop scans from the current index `i` to `n-1` to find the index of the smallest element. If the minimum index differs from `i`, a swap occurs and the swap counter is incremented. The algorithm is stable in terms of not altering relative order of equal elements only if the comparison is strict (`<`), which it is here. Important edge cases: when `n` is 0 or 1, the loops do not execute, and the swap count remains 0. The time complexity is \(O(n^2)\) for all cases (best, average, worst) due to the nested loops; the space complexity is \(O(1)\) since we only use a few integer variables for swapping and indexing. The `printArray` helper simply iterates and prints each element followed by a newline, which matches the original snippet's behavior.

#include <iostream>

// Helper function to print the array elements, each on a new line.
void printArray(const int arr[], int n) {
    for (int i = 0; i < n; ++i) {
        std::cout << arr[i] << "\n";
    }
}

// Selection sort: sorts arr in ascending order and returns the number of swaps performed.
int selectionSort(int arr[], int n) {
    int swapCount = 0;
    for (int i = 0; i < n - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            int temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
            ++swapCount;
        }
    }
    printArray(arr, n);
    return swapCount;
}

#include <cassert>
#include <iostream>

// Assume the solution functions are defined above.

int main() {
    // Test 1: Typical array with one swap needed
    int arr1[] = {4, 2, 1, 3};
    int size1 = sizeof(arr1) / sizeof(arr1[0]);
    assert(selectionSort(arr1, size1) == 2); // Sorted: {1,2,3,4}, swaps: (4↔1), (4↔3) -> 2
    assert(arr1[0] == 1 && arr1[1] == 2 && arr1[2] == 3 && arr1[3] == 4);

    // Test 2: Already sorted array (no swaps)
    int arr2[] = {1, 2, 3, 4, 5};
    assert(selectionSort(arr2, 5) == 0);
    int expected2[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; ++i) assert(arr2[i] == expected2[i]);

    // Test 3: Reverse sorted array (max swaps for size 4: 3 swaps)
    int arr3[] = {5, 4, 3, 2};
    assert(selectionSort(arr3, 4) == 3);
    int expected3[] = {2, 3, 4, 5};
    for (int i = 0; i < 4; ++i) assert(arr3[i] == expected3[i]);

    // Test 4: Single element array (no swaps)
    int arr4[] = {42};
    assert(selectionSort(arr4, 1) == 0);
    assert(arr4[0] == 42);

    // Test 5: Two elements already in order
    int arr5[] = {1, 2};
    assert(selectionSort(arr5, 2) == 0);
    assert(arr5[0] == 1 && arr5[1] == 2);

    // Test 6: Two elements out of order
    int arr6[] = {2, 1};
    assert(selectionSort(arr6, 2) == 1);
    assert(arr6[0] == 1 && arr6[1] == 2);

    // Test 7: Duplicate values
    int arr7[] = {3, 1, 3, 2};
    assert(selectionSort(arr7, 4) == 2); // Swaps: (3↔1), (3↔2) -> 2
    int expected7[] = {1, 2, 3, 3};
    for (int i = 0; i < 4; ++i) assert(arr7[i] == expected7[i]);

    // Test 8: Negative numbers
    int arr8[] = {-1, -5, 0, -3};
    assert(selectionSort(arr8, 4) == 2); // Swaps: (-1↔-5), (-1↔-3) -> 2
    int expected8[] = {-5, -3, -1, 0};
    for (int i = 0; i < 4; ++i) assert(arr8[i] == expected8[i]);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
