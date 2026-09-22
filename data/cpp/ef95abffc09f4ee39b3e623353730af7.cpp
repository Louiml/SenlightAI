// Write a C++ function that takes a sorted array of integers (non-decreasing order) and its size as parameters, and prints the index of the first occurrence of each value that appears more than once consecutively, along with the duplicate value itself. The output should be formatted exactly as `Duplicate 'X' Found at Index I` for each such value, where `X` is the duplicated integer and `I` is the index of its first occurrence in the run. The array is guaranteed to be sorted, but may contain negative numbers, zero, and duplicates. The function should not print anything if no duplicates exist. The signature should be `void findDuplicates(const int arr[], int size)`.

#include <cassert>
#include <sstream>
#include <string>

// Helper to capture output for testing
std::string captureOutput(const int arr[], int size) {
    std::ostringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());
    findDuplicates(arr, size);
    std::cout.rdbuf(oldCout);
    return buffer.str();
}

int main() {
    // Test 1: Example from the prompt
    int arr1[] = {3, 6, 8, 8, 10, 12, 15, 15, 15, 20};
    assert(captureOutput(arr1, 10) == "Duplicate '8' Found at Index 2\nDuplicate '15' Found at Index 6\n");

    // Test 2: All duplicates
    int arr2[] = {1, 1, 1, 1};
    assert(captureOutput(arr2, 4) == "Duplicate '1' Found at Index 0\n");

    // Test 3: No duplicates
    int arr3[] = {1, 2, 3, 4, 5};
    assert(captureOutput(arr3, 5) == "");

    // Test 4: Duplicate at the very end
    int arr4[] = {-5, -5, 0, 2, 3, 7, 7};
    assert(captureOutput(arr4, 7) == "Duplicate '-5' Found at Index 0\nDuplicate '7' Found at Index 5\n");

    // Test 5: Duplicate negative values and zero
    int arr5[] = {-2, -2, -2, 0, 0, 1};
    assert(captureOutput(arr5, 6) == "Duplicate '-2' Found at Index 0\nDuplicate '0' Found at Index 3\n");

    // Test 6: Single element (no duplicates)
    int arr6[] = {42};
    assert(captureOutput(arr6, 1) == "");

    // Test 7: Empty array (size 0)
    int arr7[] = {};
    assert(captureOutput(arr7, 0) == "");

    // Test 8: Duplicate of different values adjacent
    int arr8[] = {1, 2, 2, 3, 4, 4, 4, 5};
    assert(captureOutput(arr8, 8) == "Duplicate '2' Found at Index 1\nDuplicate '4' Found at Index 4\n");

    return 0;
}

#include <climits>   // for INT_MIN
#include <iostream>  // for std::cout

/**
 * Prints the index of the first occurrence of each value that appears
 * consecutively at least twice in a sorted integer array.
 * @param arr The sorted array (non-decreasing order).
 * @param size The number of elements in the array.
 */
void findDuplicates(const int arr[], int size) {
    int lastReported = INT_MIN;  // sentinel to avoid duplicate reports

    for (int i = 0; i < size - 1; ++i) {
        if (arr[i] == arr[i + 1] && arr[i] != lastReported) {
            std::cout << "Duplicate '" << arr[i] << "' Found at Index " << i << std::endl;
            lastReported = arr[i];
        }
    }
}

// The algorithm scans the array from left to right, comparing each element with the next one. Because the array is sorted, duplicates always appear in contiguous runs. We maintain a variable `lastReported` initialized to a sentinel value (e.g., `INT_MIN` from `<climits>`) to avoid reporting the same value multiple times if its run is longer than two. When we find `arr[i] == arr[i+1]`, we know a run of at least two starts at index `i`. If `arr[i]` is different from `lastReported`, we print the duplicate and update `lastReported`. This ensures that for a run like `[5,5,5,5]`, we only report once at the first index (0) and not at 1 or 2. Edge cases: empty array (size 0) does nothing; array of one element no duplicates; all elements unique no output; all elements identical prints once at index 0. Time complexity is O(n) for a single pass; space complexity is O(1) auxiliary.
