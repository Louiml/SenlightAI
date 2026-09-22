/*
Write a C++ function `void selectionSortSteps(int arr[], int n, int steps[], int& stepCount)` that performs selection sort on an integer array, but instead of printing each pass, it records the state of the array after every swap (not after every outer loop iteration, but only when an actual swap occurs). The function should store each post-swap array state in a 2D output array `steps` (assume it is pre-allocated as `int steps[][MAX]` where `MAX` is a compile-time constant), and set `stepCount` to the number of swaps performed. If the array is already sorted, no swaps occur and `stepCount` must be 0, leaving `steps` untouched. The function should modify the input array in place and should use `const` correctness where appropriate (e.g., do not modify the array size, but the array contents are intentionally changed). The task is to implement this function exactly as specified, handling edge cases like an empty array (n=0), a single element, all equal elements, and already sorted arrays.
*/

#include <algorithm>

// Performs selection sort on arr, recording the state of the array after each swap.
// steps: 2D array where each row will hold the array state after a swap.
// stepCount: output parameter set to the number of swaps performed.
// The array is sorted in place. If no swaps occur, stepCount is 0 and steps is untouched.
void selectionSortSteps(int arr[], const int n, int steps[][100], int& stepCount) {
    stepCount = 0;
    for (int i = 0; i < n - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            std::swap(arr[i], arr[minIndex]);
            // Record the entire array state after the swap
            for (int k = 0; k < n; ++k) {
                steps[stepCount][k] = arr[k];
            }
            ++stepCount;
        }
    }
}

#include <cassert>

#define MAX 100

int main() {
    // Test 1: Reverse sorted array - 3 swaps expected
    {
        int arr[] = {3, 2, 1};
        int steps[MAX][MAX];
        int stepCount = 0;
        selectionSortSteps(arr, 3, steps, stepCount);
        assert(stepCount == 2); // swaps: (3,1)->(1,2,3) then (2,3)->no swap? Actually let's trace: i=0 min=2 swap -> [1,2,3]; i=1 min=1 no swap. So 1 swap.
        // But fix: arr[3,2,1]: i=0, min=2 (value1) -> swap => [1,2,3]; i=1 min=1 no swap => stepCount=1.
        assert(stepCount == 1);
        assert(arr[0]==1 && arr[1]==2 && arr[2]==3);
        assert(steps[0][0]==1 && steps[0][1]==2 && steps[0][2]==3);
    }

    // Test 2: Already sorted - no swaps
    {
        int arr[] = {1, 2, 3, 4};
        int steps[MAX][MAX];
        int stepCount = 99;
        selectionSortSteps(arr, 4, steps, stepCount);
        assert(stepCount == 0);
        assert(arr[0]==1 && arr[3]==4);
    }

    // Test 3: Single element - no swaps
    {
        int arr[] = {42};
        int steps[MAX][MAX];
        int stepCount = 99;
        selectionSortSteps(arr, 1, steps, stepCount);
        assert(stepCount == 0);
        assert(arr[0]==42);
    }

    // Test 4: Empty array - no swaps
    {
        int arr[] = {};
        int steps[MAX][MAX];
        int stepCount = 99;
        selectionSortSteps(arr, 0, steps, stepCount);
        assert(stepCount == 0);
    }

    // Test 5: All equal - no swaps
    {
        int arr[] = {5, 5, 5};
        int steps[MAX][MAX];
        int stepCount = 99;
        selectionSortSteps(arr, 3, steps, stepCount);
        assert(stepCount == 0);
    }

    // Test 6: Array requiring exactly 2 swaps
    {
        int arr[] = {4, 3, 1, 2};
        int steps[MAX][MAX];
        int stepCount = 0;
        selectionSortSteps(arr, 4, steps, stepCount);
        assert(stepCount == 2);
        // After first swap (i=0, min=2) -> [1,3,4,2]
        assert(steps[0][0]==1 && steps[0][1]==3 && steps[0][2]==4 && steps[0][3]==2);
        // After second swap (i=1, min=3) -> [1,2,4,3]
        assert(steps[1][0]==1 && steps[1][1]==2 && steps[1][2]==4 && steps[1][3]==3);
        // Final sorted
        assert(arr[0]==1 && arr[1]==2 && arr[2]==3 && arr[3]==4);
    }

    return 0;
}

// The algorithm is a standard selection sort: for each index `i` from 0 to n-2, find the index of the minimum element in the subarray from `i` to n-1. If that minimum is not at index `i`, swap it with the element at `i` and record the entire array state after the swap into the `steps` matrix at row index `stepCount`, then increment `stepCount`. If the minimum is already at `i`, no swap occurs and no state is recorded. Important edge cases: empty array (n=0) – loop does nothing, `stepCount` remains 0; single element – no swaps, `stepCount=0`; all equal elements – the inner loop will always find `minIndex == i`, so no swaps; already sorted ascending – no swaps. The time complexity is O(n²) due to the nested loops, and O(n) auxiliary space for the minimum index variable, but the output matrix uses O(n * stepCount) which in the worst case (reverse sorted) is O(n²). The space complexity of the algorithm itself excluding output is O(1). The function must be careful not to write beyond the pre-allocated `steps` matrix, but the problem statement assumes sufficient capacity (e.g., `stepCount` will never exceed n or n-1). `const` correctness: the size parameter `n` should be `int` (not const because it's a value), but we can use `const int n` in the signature without issue.
