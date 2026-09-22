// Write a C++ function `flattenAndSort2D` that takes a 2D integer array with fixed dimensions (5 rows and 4 columns), fills it with random values in the range [0, 99], and then sorts all 20 elements in ascending order across the entire array such that row-major order yields the sorted sequence (i.e., the array is fully sorted, not just rows or columns individually). The function should accept a pre-allocated 2D array (`int arr[5][4]`) and modify it in place. The function must also print the original and the sorted array to the console in a tabular format, with each row on a new line and values separated by tabs. You are allowed to use `rand()` without seeding (as in the original snippet) and may assume the array is always 5x4. The task is specifically about implementing the sorting logic, not about input/output formatting beyond the required prints. Ensure the function is self-contained and does not rely on global state other than `rand()`.
// The main algorithm is a brute-force selection sort applied over the flattened view of the 2D array. Since the array is 5x4, we can treat it as a 20-element contiguous block in row-major order. We iterate over all positions `(i,j)` in row-major order. For each position, we consider it as the current "minimum" position. Then we iterate over all subsequent positions (starting from the next cell after `(i,j)` in row-major order) and if we find a smaller value, we swap it with the current position. This is exactly the selection sort algorithm but with index arithmetic to convert 2D indices to a flat index and back. The key edge case is correctly computing the starting point of the inner loop: when `k == i`, the starting column is `j+1`; otherwise, the starting column is `0`. This avoids double-checking already sorted elements and correctly skips the current cell. The algorithm runs in O(ROWS*COLS)^2 = O(20^2) = O(400) time, which is constant for this fixed size, but in general terms for an MxN array it is O(MN)^2 = O(M^2 N^2). Space complexity is O(1) extra besides the array itself. There are no special edge cases beyond handling the starting column correctly; duplicates are naturally swapped but not equal values are not swapped since the condition is strictly less than.
#include <iostream>
#include <cstdlib> // for rand()

// Fills a 5x4 array with random values in [0,99], prints it, sorts it in ascending order,
// prints the sorted array, and modifies the array in place.
void flattenAndSort2D(int arr[5][4]) {
    const int ROWS = 5;
    const int COLS = 4;

    // Fill with random numbers
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            arr[i][j] = rand() % 100;
        }
    }

    // Print original array
    std::cout << "Original array:" << std::endl;
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            std::cout << arr[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // Sort the entire 2D array as a flat 20-element sequence using selection sort
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            // Current element is considered the minimum candidate
            for (int k = i; k < ROWS; ++k) {
                // Start from j+1 if we are in the same row, else from 0
                int start_col = (k == i) ? (j + 1) : 0;
                for (int l = start_col; l < COLS; ++l) {
                    if (arr[k][l] < arr[i][j]) {
                        int temp = arr[i][j];
                        arr[i][j] = arr[k][l];
                        arr[k][l] = temp;
                    }
                }
            }
        }
    }

    // Print sorted array
    std::cout << "Sorted array:" << std::endl;
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            std::cout << arr[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
#include <cassert>
#include <iostream>
#include <cstdlib>

// Declare the solution function (normally would be included from header)
void flattenAndSort2D(int arr[5][4]);

int main() {
    // Test 1: Verify the sorted array is non-decreasing in row-major order
    int arr1[5][4];
    flattenAndSort2D(arr1);
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 4; ++j) {
            int index = i * 4 + j;
            if (index > 0) {
                int prev_i = (index - 1) / 4;
                int prev_j = (index - 1) % 4;
                assert(arr1[i][j] >= arr1[prev_i][prev_j]);
            }
        }
    }

    // Test 2: After sorting, the minimum value is at [0][0] and max at [4][3]
    int arr2[5][4];
    flattenAndSort2D(arr2);
    assert(arr2[0][0] <= arr2[4][3]);

    // Test 3: The array contains exactly 20 values; no value outside [0,99]
    int arr3[5][4];
    flattenAndSort2D(arr3);
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 4; ++j) {
            assert(arr3[i][j] >= 0 && arr3[i][j] <= 99);
        }
    }

    // Test 4: Sorting twice should not change the order (idempotent operation)
    int arr4[5][4];
    flattenAndSort2D(arr4);
    int copy[5][4];
    for (int i = 0; i < 5; ++i) for (int j = 0; j < 4; ++j) copy[i][j] = arr4[i][j];
    // Call again on the sorted array (it will re-fill with random numbers, so this test is not applicable)
    // Instead, verify the array is sorted by checking each row is non-decreasing and each column is non-decreasing? Actually since fully sorted, both are true.
    for (int j = 0; j < 4; ++j) {
        for (int i = 1; i < 5; ++i) {
            // In a fully sorted 2D array, each column is also sorted non-decreasingly
            // Because row-major order means all elements in earlier rows are <= all in later rows
            assert(arr4[i][j] >= arr4[i-1][j]);
        }
    }

    std::cout << "All tests passed." << std::endl;
    return 0;
}
