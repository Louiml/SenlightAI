// Write a C++ function named `findMaximum` that takes a non-empty array of integers and its size as parameters, and returns the largest integer value present in that array. The function must handle arrays containing negative numbers, duplicate values, and must work correctly even if the array has only one element. Additionally, the function should be `const`-correct, meaning it must not modify the input array, and should be implemented without relying on any standard library algorithms (i.e., do not use `std::max_element`). In the provided test harness, call this function with multiple arrays of varying content and verify correctness using `assert` statements.
The solution approach is straightforward linear scanning. Initialize a variable `maxVal` with the first element of the array (since the array is guaranteed non-empty, it's safe to access `arr[0]`). Then iterate through the remaining elements from index 1 to `n-1`, comparing each element to `maxVal`. If the current element is larger, update `maxVal` to that value. After the loop, return `maxVal`. Edge cases include: arrays with all negative numbers (the first element initialization still works because we compare and update only when a larger value is found), duplicate values (they do not affect the result because we only update on strictly greater comparisons), and single-element arrays (the loop simply does not execute, and the first element is returned). Time complexity is O(n) since we visit each element once. Space complexity is O(1) because we only use a single temporary variable, not counting the input array itself.
#include <vector> // included for completeness; not required for the function itself

// Returns the largest integer in the array arr of size n.
// Precondition: n > 0
int findMaximum(const int arr[], int n) {
    int maxVal = arr[0]; // initialize with first element
    for (int i = 1; i < n; ++i) {
        if (arr[i] > maxVal) {
            maxVal = arr[i];
        }
    }
    return maxVal;
}
#include <cassert>

// Global main function to test findMaximum
int main() {
    // Basic test with positive numbers
    int arr1[] = {1, 99, 1000, 121, 2, 2, 3, 7};
    assert(findMaximum(arr1, 8) == 1000);

    // All negative numbers
    int arr2[] = {-5, -1, -10, -3};
    assert(findMaximum(arr2, 4) == -1);

    // Single element
    int arr3[] = {42};
    assert(findMaximum(arr3, 1) == 42);

    // All duplicates
    int arr4[] = {7, 7, 7, 7};
    assert(findMaximum(arr4, 4) == 7);

    // Mixed with duplicates and negatives
    int arr5[] = {-2, -2, 0, 0, 5, 5, -1};
    assert(findMaximum(arr5, 7) == 5);

    // Large values
    int arr6[] = {INT_MAX, INT_MIN, 0, 123456};
    assert(findMaximum(arr6, 4) == INT_MAX);

    // Only one negative
    int arr7[] = {-100};
    assert(findMaximum(arr7, 1) == -100);

    // Decreasing order
    int arr8[] = {9, 8, 7, 6, 5};
    assert(findMaximum(arr8, 5) == 9);

    // Increasing order with max at end
    int arr9[] = {1, 2, 3, 4, 5};
    assert(findMaximum(arr9, 5) == 5);

    // Random order with max in middle
    int arr10[] = {3, 8, 1, 9, 2};
    assert(findMaximum(arr10, 5) == 9);

    return 0;
}
