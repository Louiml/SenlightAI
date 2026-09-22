Write a C++ function that takes an array of integers and its size `n`, and returns the second largest distinct element in the array. If the array has fewer than two distinct values, return `-1`. The function should not modify the original array, and must handle arrays with negative numbers, duplicates, and all equal elements correctly. For example, given `[10, 10, 5, 8]`, the second largest distinct value is `8`; given `[5, 5, 5]`, it should return `-1`.
#include <cassert>

int main() {
    int arr1[] = {10, 10, 5, 8};
    assert(secondLargestDistinct(arr1, 4) == 8);

    int arr2[] = {5, 5, 5};
    assert(secondLargestDistinct(arr2, 3) == -1);

    int arr3[] = {7};
    assert(secondLargestDistinct(arr3, 1) == -1);

    int arr4[] = {-5, -1, -10};
    assert(secondLargestDistinct(arr4, 3) == -5);

    int arr5[] = {1, 2};
    assert(secondLargestDistinct(arr5, 2) == 1);

    int arr6[] = {2, 2, 3, 3, 1};
    assert(secondLargestDistinct(arr6, 5) == 2);

    int arr7[] = {-3, -3, -2};
    assert(secondLargestDistinct(arr7, 3) == -3);

    int arr8[] = {0, 0, 0, 0};
    assert(secondLargestDistinct(arr8, 4) == -1);

    int arr9[] = {100, 99, 98, 97};
    assert(secondLargestDistinct(arr9, 4) == 99);

    int arr10[] = {1, -1};
    assert(secondLargestDistinct(arr10, 2) == -1);

    return 0;
}
#include <climits> // for INT_MIN

// Return the second largest distinct element in arr of size n,
// or -1 if the array has fewer than two distinct values.
int secondLargestDistinct(const int* arr, int n) {
    if (n < 2) return -1;

    int largest = INT_MIN;
    int second = INT_MIN;

    for (int i = 0; i < n; ++i) {
        int current = arr[i];
        if (current > largest) {
            second = largest;          // old largest becomes second
            largest = current;
        } else if (current < largest && current > second) {
            second = current;          // new distinct candidate for second
        }
    }

    return (second == INT_MIN) ? -1 : second;
}
// The simplest robust approach is to find the largest distinct value and then the largest value strictly smaller than that. Initialize two variables: `largest` and `secondLargest` with the smallest possible integer value (e.g., `INT_MIN`). Iterate through the array once. For each element, update `largest` if it's greater than `largest`, and before updating `largest`, check if the current element is different from `largest` and greater than `secondLargest`, then update `secondLargest`. After the loop, if `secondLargest` is still `INT_MIN`, it means no distinct second-largest exists, so return `-1`; otherwise return `secondLargest`. This avoids sorting and preserves the original array. Time complexity is O(n) and auxiliary space is O(1). Edge cases: all elements equal, only one element, or two elements where both are equal—all yield `-1`. Negative numbers are handled naturally because `INT_MIN` is smaller than any valid input.
