// Write a C++ function `bubbleSort(int arr[], int n)` that sorts an integer array in ascending order using the bubble sort algorithm. The function must modify the array in place, meaning it should not create a separate sorted copy. The array may contain duplicate values and unsorted elements in any order, including already sorted or reverse-sorted input. In addition to the sorting logic, ensure the implementation is optimized by stopping early if the array becomes sorted before all passes are complete (i.e., use a flag to detect whether any swaps occurred during a pass). The function must be declared in the `Solution` class as a public member, and it should return `void`.
#include <cassert>
#include <iostream>
#include <algorithm>

// The Solution class definition (as above) would be included here.
// For brevity, the test assumes the Solution class is already defined.

int main() {
    Solution sol;

    // Test 1: Basic unsorted array
    int arr1[] = {64, 34, 25, 12, 22, 11, 90};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    sol.bubbleSort(arr1, n1);
    for (int i = 0; i < n1 - 1; ++i) assert(arr1[i] <= arr1[i + 1]);

    // Test 2: Already sorted array (best case, should exit early)
    int arr2[] = {1, 2, 3, 4, 5};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    sol.bubbleSort(arr2, n2);
    for (int i = 0; i < n2 - 1; ++i) assert(arr2[i] <= arr2[i + 1]);

    // Test 3: Reverse-sorted array (worst case)
    int arr3[] = {9, 7, 5, 3, 1};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
    sol.bubbleSort(arr3, n3);
    for (int i = 0; i < n3 - 1; ++i) assert(arr3[i] <= arr3[i + 1]);

    // Test 4: Duplicate values
    int arr4[] = {4, 2, 4, 1, 2};
    int n4 = sizeof(arr4) / sizeof(arr4[0]);
    sol.bubbleSort(arr4, n4);
    for (int i = 0; i < n4 - 1; ++i) assert(arr4[i] <= arr4[i + 1]);

    // Test 5: Single element
    int arr5[] = {42};
    int n5 = sizeof(arr5) / sizeof(arr5[0]);
    sol.bubbleSort(arr5, n5);
    assert(arr5[0] == 42);

    // Test 6: Empty array (n=0)
    // Note: Calling with n=0 is valid, but we must avoid dereferencing arr.
    // The function handles n<=1 gracefully. We'll just call with n=0.
    int* arr6 = nullptr;
    sol.bubbleSort(arr6, 0); // Should not crash

    // Test 7: Large sorted array to ensure early exit works
    const int n7 = 1000;
    int* arr7 = new int[n7];
    for (int i = 0; i < n7; ++i) arr7[i] = i;
    sol.bubbleSort(arr7, n7);
    for (int i = 0; i < n7 - 1; ++i) assert(arr7[i] <= arr7[i + 1]);
    delete[] arr7;

    std::cout << "All tests passed!\n";
    return 0;
}
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    // Sorts an integer array in ascending order using bubble sort.
    // Modifies the array in place. Early exit if array becomes sorted.
    void bubbleSort(int arr[], int n) {
        if (n <= 1) return; // No sorting needed for empty or single-element arrays

        for (int i = n - 1; i > 0; --i) {
            bool didSwap = false; // Track if any swap occurred in this pass

            for (int j = 0; j < i; ++j) {
                if (arr[j] > arr[j + 1]) {
                    // Swap adjacent elements using a temporary variable
                    int temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                    didSwap = true;
                }
            }

            // If no swaps occurred, the array is already sorted
            if (!didSwap) break;
        }
    }
};
// The bubble sort algorithm repeatedly steps through the array, compares adjacent elements, and swaps them if they are in the wrong order. Each pass places the largest remaining element at its final position at the end of the unsorted portion. The outer loop runs from the last index down to 1, representing the unsorted boundary. The inner loop compares adjacent pairs from index 0 to the boundary minus 1, swapping when `arr[j] > arr[j+1]`. To optimize, maintain a boolean flag (`didSwap`) initialized to `false` at the start of each outer pass; set it to `true` whenever a swap occurs. After the inner loop, if the flag remains `false`, no swaps happened, meaning the array is already sorted, and the algorithm can break early. Edge cases include an empty array or `n <= 1`, where no sorting is needed, and arrays with duplicates, where comparisons using `>` (not `>=`) ensure stability (equal elements are not swapped). Time complexity is O(n^2) in the worst and average cases (reverse-sorted or random) and O(n) in the best case (already sorted, due to early exit). Space complexity is O(1) because sorting is in-place and only a few temporary variables are used.
