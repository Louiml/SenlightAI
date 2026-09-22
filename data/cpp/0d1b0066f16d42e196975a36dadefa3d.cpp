/*
Write a C++ function that implements selection sort on a dynamically sized array of integers and returns the number of swaps performed during the sorting process. The function should accept a pointer to the first element of the array and its size, sort the array in ascending order in-place, and return an integer representing the total number of swaps executed. The input array may contain duplicate values, negative numbers, and any size from 1 to 1000. The function must not use any standard library sorting functions and must follow the exact selection sort logic: for each position from the start to the second-to-last element, find the index of the minimum element in the remaining unsorted portion, then swap the current element with that minimum if they differ.
*/

#include <algorithm>  // for std::swap
#include <cstddef>    // for std::size_t

// Sorts an array in ascending order using selection sort.
// Returns the number of swaps performed.
// Parameters:
//   arr - pointer to the first element of the array
//   n   - number of elements in the array (non-negative)
// Precondition: arr points to valid memory of at least n elements.
int selectionSortSwapCount(int* const arr, std::size_t n) {
    int swapCount = 0;
    for (std::size_t i = 0; i + 1 < n; ++i) {
        std::size_t minIndex = i;
        // Find the index of the minimum element in the unsorted subarray.
        for (std::size_t j = i + 1; j < n; ++j) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        // Only swap if the minimum is not already at the current position.
        if (minIndex != i) {
            std::swap(arr[i], arr[minIndex]);
            ++swapCount;
        }
    }
    return swapCount;
}

#include <cassert>
#include <vector>

// Forward declaration of the function to test.
int selectionSortSwapCount(int* const arr, std::size_t n);

int main() {
    // Test 1: simple array with unique values
    int arr1[] = {5, 3, 4, 1, 2};
    int swaps1 = selectionSortSwapCount(arr1, 5);
    assert(std::vector<int>(arr1, arr1 + 5) == std::vector<int>({1, 2, 3, 4, 5}));
    assert(swaps1 == 4);

    // Test 2: already sorted array -> zero swaps
    int arr2[] = {1, 2, 3, 4, 5};
    int swaps2 = selectionSortSwapCount(arr2, 5);
    assert(std::vector<int>(arr2, arr2 + 5) == std::vector<int>({1, 2, 3, 4, 5}));
    assert(swaps2 == 0);

    // Test 3: array with duplicates
    int arr3[] = {3, 1, 3, 2, 1};
    int swaps3 = selectionSortSwapCount(arr3, 5);
    assert(std::vector<int>(arr3, arr3 + 5) == std::vector<int>({1, 1, 2, 3, 3}));
    assert(swaps3 == 3);

    // Test 4: negative numbers
    int arr4[] = {-3, 0, -10, 5, -2};
    int swaps4 = selectionSortSwapCount(arr4, 5);
    assert(std::vector<int>(arr4, arr4 + 5) == std::vector<int>({-10, -3, -2, 0, 5}));
    assert(swaps4 == 3);

    // Test 5: single element -> zero swaps
    int arr5[] = {42};
    int swaps5 = selectionSortSwapCount(arr5, 1);
    assert(arr5[0] == 42);
    assert(swaps5 == 0);

    // Test 6: empty array (n=0) -> zero swaps
    int* arr6 = nullptr;
    int swaps6 = selectionSortSwapCount(arr6, 0);
    assert(swaps6 == 0);

    // Test 7: reverse sorted array
    int arr7[] = {10, 9, 8, 7, 6};
    int swaps7 = selectionSortSwapCount(arr7, 5);
    assert(std::vector<int>(arr7, arr7 + 5) == std::vector<int>({6, 7, 8, 9, 10}));
    assert(swaps7 == 2);  // each swap places the correct minimum at the front

    // Test 8: all equal elements -> zero swaps
    int arr8[] = {7, 7, 7, 7};
    int swaps8 = selectionSortSwapCount(arr8, 4);
    assert(std::vector<int>(arr8, arr8 + 4) == std::vector<int>({7, 7, 7, 7}));
    assert(swaps8 == 0);

    return 0;
}

// The solution uses the classic selection sort algorithm: for each index `i` from `0` to `n-2`, assume `i` is the position of the minimum, then scan from `i+1` to `n-1` to find the true minimum index. If the found minimum index differs from `i`, perform a swap and increment the swap counter. This guarantees ascending order after the outer loop completes. Edge cases: an array with 0 or 1 element requires zero swaps and is already sorted; duplicates are handled naturally because the algorithm compares values and the first occurrence of a minimum is selected. Time complexity is O(n²) for both best and worst cases due to the nested loops, and space complexity is O(1) extra space since only a few integer variables are used. The swap count is returned, which can be used for testing or instrumentation. The function modifies the original array in-place and uses `std::swap` for efficiency and correctness with fundamental types.
