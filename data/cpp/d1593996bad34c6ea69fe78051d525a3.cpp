// Write a C++ function `insertionSortAndPrint` that takes a plain C-style array of integers and its size, sorts the array in ascending order using the insertion sort algorithm, and prints the sorted elements to standard output, each followed by a space. The function must not return any value; it should perform the sorting in-place and print the final sorted sequence directly. The input array may contain duplicate values, negative numbers, and any number of elements from zero upward. If the size is zero, the function should print nothing (no output). The function must be declared as `void insertionSortAndPrint(int a[], int n)`.

The solution implements the classic insertion sort algorithm. The main idea is to iterate from the second element to the last element, treating the prefix before the current element as already sorted. For each current element `temp = a[i]`, we shift all larger elements in the sorted prefix one position to the right, starting from the end of the prefix, until we find the correct insertion point. Then we place `temp` there. This process is repeated for every element. Edge cases: if `n` is 0 or 1, the loop does nothing, and no output is produced or just one element printed (which is already sorted). Duplicate values are handled naturally because the while loop uses `>` instead of `>=`, so equal elements do not shift, preserving stability. Negative numbers are handled because comparisons are numeric. Complexity: time is \(O(n^2)\) in the worst and average case, and \(O(n)\) in the best case (already sorted). Space is \(O(1)\) extra because we only use a temporary variable. The output printing uses `cout << a[i] << " "` for each element, so the final output has a trailing space after the last element, but that is acceptable per the task specification.

#include <iostream>

// Sorts the array using insertion sort and prints the sorted elements.
void insertionSortAndPrint(int a[], int n) {
    for (int i = 1; i < n; ++i) {
        int temp = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > temp) {
            a[j + 1] = a[j];
            --j;
        }
        a[j + 1] = temp;
    }
    for (int i = 0; i < n; ++i) {
        std::cout << a[i] << " ";
    }
}

#include <cassert>
#include <iostream>
#include <sstream>

// Redirect cout to a buffer to capture the output for testing.
void testInsertionSortAndPrint(const std::string& input, const std::string& expected) {
    int arr[100];
    int n = 0;
    std::istringstream iss(input);
    int val;
    while (iss >> val) {
        arr[n++] = val;
    }
    std::ostringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());
    insertionSortAndPrint(arr, n);
    std::cout.rdbuf(oldCout);
    assert(buffer.str() == expected);
}

int main() {
    // Test with unsorted positive numbers
    testInsertionSortAndPrint("5 2 7 1 3", "1 2 3 5 7 ");
    // Test with duplicates and negatives
    testInsertionSortAndPrint("-3 5 5 -3 0", "-3 -3 0 5 5 ");
    // Test with single element
    testInsertionSortAndPrint("42", "42 ");
    // Test with empty input
    testInsertionSortAndPrint("", "");
    // Test with already sorted
    testInsertionSortAndPrint("1 2 3 4", "1 2 3 4 ");
    // Test with reverse sorted
    testInsertionSortAndPrint("9 7 5 3 1", "1 3 5 7 9 ");
    // Test with all same values
    testInsertionSortAndPrint("8 8 8", "8 8 8 ");
    // Test with negative and positive mix
    testInsertionSortAndPrint("10 -10 0 -5 5", "-10 -5 0 5 10 ");

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
