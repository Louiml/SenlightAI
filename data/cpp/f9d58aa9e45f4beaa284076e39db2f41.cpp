// Write a C++ function that takes an array of integers and its size, and returns the difference between the maximum and minimum values in the array. If the array is empty (size 0), return 0. The function should not modify the original array and should handle arrays with negative numbers, duplicates, and a single element correctly. The signature is `int rangeOfArray(const int arr[], int size);`
#include <cassert>

int main() {
    int a1[] = {1, 2, 3, 4, 5};
    assert(rangeOfArray(a1, 5) == 4);

    int a2[] = {-3, -1, -7, -2};
    assert(rangeOfArray(a2, 4) == 6); // -1 - (-7) = 6

    int a3[] = {42};
    assert(rangeOfArray(a3, 1) == 0);

    int a4[] = {10, 10, 10};
    assert(rangeOfArray(a4, 3) == 0);

    int a5[] = {100, -100, 0, 50};
    assert(rangeOfArray(a5, 4) == 200);

    int a6[] = {7};
    assert(rangeOfArray(a6, 0) == 0); // empty size returns 0

    int a7[] = { -5, 5, -2, 2 };
    assert(rangeOfArray(a7, 4) == 10);
}
#include <climits> // for INT_MAX / INT_MIN if needed, but we use first element instead

// Returns max(arr) - min(arr). If size <= 0, returns 0.
int rangeOfArray(const int arr[], int size) {
    if (size <= 0) {
        return 0;
    }
    int minVal = arr[0];
    int maxVal = arr[0];
    for (int i = 1; i < size; ++i) {
        if (arr[i] < minVal) {
            minVal = arr[i];
        }
        if (arr[i] > maxVal) {
            maxVal = arr[i];
        }
    }
    return maxVal - minVal;
}
// The solution approach is straightforward: iterate through the entire array once, tracking both the current minimum and current maximum. Initialize both to the first element if size > 0; otherwise handle the empty case by returning 0. For each subsequent element, update the minimum if the current element is smaller, and update the maximum if it is larger. After the loop, return the difference `max - min`.  
// Edge cases:  
// - Empty array (size 0): return 0 directly.  
// - Single element: min = max = that element, difference = 0.  
// - All negative numbers: min will be the most negative, max the least negative, subtraction works normally.  
// - Duplicates: no special handling needed.  
// Time complexity is O(n) for a single pass. Space complexity is O(1) auxiliary (only a few variables), ignoring the input array itself. The function uses `const int arr[]` to guarantee the caller's data is not modified.
