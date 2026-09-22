Write a standalone C++ function that takes an integer array and its size as parameters and finds both the second smallest and second largest distinct values in the array. If the array has fewer than two distinct values (i.e., all elements are the same, or size is 0 or 1), the function should indicate that the second smallest and second largest do not exist—for example, by outputting `-1` for both. The function should handle negative numbers, duplicates, and unsorted input. It must not modify the input array. The function should print the two results (or the `-1` sentinel) to standard output in a clear format. For instance, given `{1, 2, 4, 6, 7, 5}`, it should print `Second smallest is 2` and `Second largest is 6`. The function must work efficiently for large arrays (up to 10^6 elements) and use only constant extra space beyond the input.

The solution uses a two-pass approach. First, find the global minimum and maximum of the array in a single pass, initializing both to the first element (or handling empty/single‑element cases). This pass runs in O(n). In a second pass, track the second smallest as the smallest value that is strictly greater than the minimum, and the second largest as the largest value that is strictly less than the maximum. Initialize `second_small` to `INT_MAX` and `second_large` to `INT_MIN`. For each element, if it differs from the global min and is smaller than current `second_small`, update; if it differs from the global max and is larger than current `second_large`, update. After the loop, if `second_small` is still `INT_MAX`, that means no second distinct smallest exists (all elements equal or only one element) — set it to `-1`; similarly for `second_large`. Edge cases include empty array, size 1, all duplicates (e.g., `{5,5,5}`), and arrays with exactly two distinct values (e.g., `{1,1,2}` → second smallest is 2, second largest is 1). Time complexity is O(n) with two passes, and space complexity is O(1) excluding input. The function prints results to `stdout`.

#include <climits>
#include <iostream>

// Prints the second smallest and second largest distinct elements in arr.
// If either does not exist (fewer than two distinct values), prints -1 for that.
void printSecondSmallestAndLargest(const int arr[], int n) {
    if (n < 2) {
        std::cout << "Second smallest is -1\n";
        std::cout << "Second largest is -1\n";
        return;
    }

    int smallest = arr[0];
    int largest = arr[0];
    for (int i = 1; i < n; ++i) {
        if (arr[i] < smallest) smallest = arr[i];
        if (arr[i] > largest) largest = arr[i];
    }

    int secondSmallest = INT_MAX;
    int secondLargest = INT_MIN;
    for (int i = 0; i < n; ++i) {
        if (arr[i] != smallest && arr[i] < secondSmallest) {
            secondSmallest = arr[i];
        }
        if (arr[i] != largest && arr[i] > secondLargest) {
            secondLargest = arr[i];
        }
    }

    if (secondSmallest == INT_MAX) secondSmallest = -1;
    if (secondLargest == INT_MIN) secondLargest = -1;

    std::cout << "Second smallest is " << secondSmallest << "\n";
    std::cout << "Second largest is " << secondLargest << "\n";
}

#include <cassert>
#include <sstream>
#include <string>
#include <iostream>

// Helper to capture output from the function
std::string captureOutput(const int arr[], int n) {
    std::ostringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());
    printSecondSmallestAndLargest(arr, n);
    std::cout.rdbuf(oldCout);
    return buffer.str();
}

int main() {
    // Normal case with distinct values
    int arr1[] = {1, 2, 4, 6, 7, 5};
    assert(captureOutput(arr1, 6) == "Second smallest is 2\nSecond largest is 6\n");

    // Negative numbers and duplicates
    int arr2[] = {-5, -10, -5, -1, -10};
    assert(captureOutput(arr2, 5) == "Second smallest is -5\nSecond largest is -5\n");

    // All duplicate values
    int arr3[] = {7, 7, 7};
    assert(captureOutput(arr3, 3) == "Second smallest is -1\nSecond largest is -1\n");

    // Single element
    int arr4[] = {42};
    assert(captureOutput(arr4, 1) == "Second smallest is -1\nSecond largest is -1\n");

    // Exactly two distinct values
    int arr5[] = {3, 3, 5};
    assert(captureOutput(arr5, 3) == "Second smallest is 5\nSecond largest is 3\n");

    // Empty array (size 0) - should handle gracefully
    int arr6[] = {};
    assert(captureOutput(arr6, 0) == "Second smallest is -1\nSecond largest is -1\n");

    // Unsorted with large spread
    int arr7[] = {100, -100, 0, 50, -50};
    assert(captureOutput(arr7, 5) == "Second smallest is -50\nSecond largest is 50\n");

    // Two identical, one different
    int arr8[] = {4, 4, 4, 9};
    assert(captureOutput(arr8, 4) == "Second smallest is 9\nSecond largest is 4\n");

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
