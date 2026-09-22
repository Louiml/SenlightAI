Write a C++ function named `bubbleSortAndPrintSteps` that accepts a plain C-style array of exactly 10 integers (e.g., `int arr[10]`) and sorts it in ascending order using the classic bubble sort algorithm. The function must print the initial array before any sorting pass, followed by the array state after each complete pass (including the final sorted state), with each printed line containing the 10 integers separated by a single space and ending with a newline. The function must modify the array in place (sort by reference), and it must not rely on any external sorting functions. The output format must match the original code snippet’s `myPrint` exactly: space-separated integers with no trailing space, followed by a newline.
#include <cassert>
#include <cstdio>

// The solution function declared here
void bubbleSortAndPrintSteps(int arr[10]);

int main() {
    // Test 1: Unsorted array, verify final sorted order and correct number of prints
    {
        int a[10] = {9,8,7,6,5,4,3,2,1,0};
        bubbleSortAndPrintSteps(a);
        int expected[10] = {0,1,2,3,4,5,6,7,8,9};
        for (int i = 0; i < 10; ++i) {
            assert(a[i] == expected[i]);
        }
    }

    // Test 2: Already sorted array, should remain unchanged
    {
        int b[10] = {0,1,2,3,4,5,6,7,8,9};
        bubbleSortAndPrintSteps(b);
        int expected[10] = {0,1,2,3,4,5,6,7,8,9};
        for (int i = 0; i < 10; ++i) {
            assert(b[i] == expected[i]);
        }
    }

    // Test 3: Reverse order with duplicates, should sort correctly
    {
        int c[10] = {5,5,4,4,3,3,2,2,1,1};
        bubbleSortAndPrintSteps(c);
        int expected[10] = {1,1,2,2,3,3,4,4,5,5};
        for (int i = 0; i < 10; ++i) {
            assert(c[i] == expected[i]);
        }
    }

    // Test 4: All identical elements, should remain identical
    {
        int d[10] = {7,7,7,7,7,7,7,7,7,7};
        bubbleSortAndPrintSteps(d);
        for (int i = 0; i < 10; ++i) {
            assert(d[i] == 7);
        }
    }

    // Test 5: Random-like array with negatives
    {
        int e[10] = {-3, 10, -8, 0, 5, -2, 7, 1, -9, 4};
        bubbleSortAndPrintSteps(e);
        int expected[10] = {-9,-8,-3,-2,0,1,4,5,7,10};
        for (int i = 0; i < 10; ++i) {
            assert(e[i] == expected[i]);
        }
    }

    return 0;
}
#include <cstdio>

// Sorts a 10-element array in ascending order using bubble sort,
// printing the array before the first pass and after each pass.
void bubbleSortAndPrintSteps(int arr[10]) {
    // Print initial array
    for (int i = 0; i < 10; ++i) {
        std::printf("%d ", arr[i]);
    }
    std::printf("\n");

    // Perform 9 passes (outer loop)
    for (int k = 0; k < 9; ++k) {
        // Inner loop for adjacent comparisons
        for (int i = 0; i < 9; ++i) {
            if (arr[i] > arr[i + 1]) {
                int temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
            }
        }
        // Print array after this pass
        for (int i = 0; i < 10; ++i) {
            std::printf("%d ", arr[i]);
        }
        std::printf("\n");
    }
}
// The core algorithm is a nested loop bubble sort. The outer loop runs `k` from 0 to 8 (since 10 elements need at most 9 passes). The inner loop compares adjacent elements `arr[i]` and `arr[i+1]` from `i=0` to `i=8`. If the left element is greater than the right, swap them using a temporary variable. After each complete inner loop, print the current state of the array. Before the outer loop begins, print the initial array. Edge cases: the array is always exactly size 10, so no bounds checking needed. Duplicate values do not affect the algorithm (comparisons use `>` so equal values never swap). If the array is already sorted, the algorithm still performs all 9 passes (though no swaps occur), which matches the given snippet’s behavior. Time complexity is O(n²) = O(100) since n is fixed at 10; space complexity is O(1) beyond the input array (only a few loop counters and a temp variable).
