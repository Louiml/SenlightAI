/*
Write a C++ function `void printAllSubarrays(const int* arr, int size)` that prints every non-empty contiguous subarray of the given integer array, each subarray on its own line with its elements printed consecutively (no spaces) and separated by a comma, and each line terminated by a newline. The subarrays must be printed in the order of increasing starting index, and for each fixed starting index, in increasing ending index. The function must handle arrays of any positive length, including a single element. The function should not modify the input array and must work with `const int*` to enforce read-only access. You may assume the input array is valid and `size > 0`.
*/
#include <iostream>

// Print all non-empty subarrays of arr[0..size-1] in order of increasing
// start index, and for each start, increasing end index. Each subarray's
// elements are printed consecutively, followed by a comma, and each line
// (for a fixed start) is terminated by a newline.
void printAllSubarrays(const int* arr, int size) {
    for (int start = 0; start < size; ++start) {
        for (int end = start; end < size; ++end) {
            for (int i = start; i <= end; ++i) {
                std::cout << arr[i];
            }
            std::cout << ',';
        }
        std::cout << '\n';
    }
}
#include <cassert>
#include <sstream>
#include <iostream>

// Declare the function (in practice this would come from the header)
void printAllSubarrays(const int* arr, int size);

// Helper to capture stdout into a stringstream
void testWithArray(const int* arr, int size, const std::string& expected) {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    printAllSubarrays(arr, size);
    std::cout.rdbuf(old);
    assert(buffer.str() == expected);
}

int main() {
    // Single element: one subarray, one line with trailing comma and newline
    int a1[] = {7};
    testWithArray(a1, 1, "7,\n");

    // Two elements: subarrays: [0] [0,1] then [1]
    int a2[] = {1, 2};
    testWithArray(a2, 2, "1,12,\n2,\n");

    // Three elements: lines for start=0,1,2
    int a3[] = {1, 2, 3};
    testWithArray(a3, 3, "1,12,123,\n2,23,\n3,\n");

    // Array with negative numbers and zeros
    int a4[] = {-1, 0, 2};
    testWithArray(a4, 3, "-1,-10,-102,\n0,02,\n2,\n");

    // Five elements from the snippet
    int a5[] = {1,2,3,4,5};
    testWithArray(a5, 5, "1,12,123,1234,12345,\n2,23,234,2345,\n3,34,345,\n4,45,\n5,\n");

    return 0;
}
// The problem is a direct triple‑nested loop enumeration of all subarrays. For each start index from 0 to size-1, for each end index from start to size-1, we print the elements from arr[start] through arr[end] consecutively. After each subarray, print a comma (except perhaps after the last one per line? The task says "separated by a comma" — in the original snippet a comma is printed after every subarray including the last one on a line, then a newline is printed after the end loop. That means each line for a given start ends with a comma then a newline. We replicate exactly that format. Edge cases: size=1 produces one line "a," — that is acceptable as the task allows any positive size and says "separated by comma" (the trailing comma is part of the format as given). Time complexity: O(size^3) because the innermost loop runs size - end times, summing to O(size^3). Space complexity: O(1) auxiliary space (ignoring output buffering). Const correctness: use `const int*` for the parameter and index with `size_t` to avoid signed/unsigned warnings.
