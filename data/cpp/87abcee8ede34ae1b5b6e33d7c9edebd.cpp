/*
Write a C++ function that performs selection sort on a fixed-size array of `long` integers and produces a step-by-step record of the array state after each completed outer-loop pass (i.e., after each swap), formatted exactly as `"Buoc <step>: "` followed by the array elements separated by spaces, with a newline after each record. The function must take an array and its size, sort the array in ascending order in place, and return a `std::string` containing all the step records concatenated in order. The original array must be modified (sorted) as a side effect. Handle the case where `n` is 0 or 1 by returning an empty string (no steps printed). The array elements are given as non-negative or negative `long` values, and you must not use any standard sorting library—only manual selection sort. The function signature must be `std::string selection_sort_steps(long arr[], int n)`.
*/
#include <string>
#include <sstream>

/**
 * Performs selection sort on arr in ascending order.
 * Returns a string containing step records after each swap.
 * Each record is formatted: "Buoc <step>: <arr[0]> <arr[1]> ... <arr[n-1]>\n"
 * If n <= 1, returns an empty string.
 */
std::string selection_sort_steps(long arr[], int n) {
    std::ostringstream out;
    for (int i = 0; i < n - 1; ++i) {
        int minIdx = i;
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
            }
        }
        // Swap the found minimum with the current position
        long temp = arr[i];
        arr[i] = arr[minIdx];
        arr[minIdx] = temp;

        // Build the step output
        out << "Buoc " << (i + 1) << ": ";
        for (int k = 0; k < n; ++k) {
            if (k > 0) out << " ";
            out << arr[k];
        }
        out << "\n";
    }
    return out.str();
}
#include <cassert>
#include <string>
#include <iostream>

// Assume selection_sort_steps from solution is included above.

int main() {
    // Test 1: Basic 4-element array
    long arr1[] = {4, 3, 2, 1};
    std::string result1 = selection_sort_steps(arr1, 4);
    assert(result1 == "Buoc 1: 1 3 2 4\nBuoc 2: 1 2 3 4\nBuoc 3: 1 2 3 4\n");
    assert(arr1[0] == 1 && arr1[1] == 2 && arr1[2] == 3 && arr1[3] == 4);

    // Test 2: Already sorted array
    long arr2[] = {1, 2, 3};
    std::string result2 = selection_sort_steps(arr2, 3);
    assert(result2 == "Buoc 1: 1 2 3\nBuoc 2: 1 2 3\n");
    assert(arr2[0] == 1 && arr2[1] == 2 && arr2[2] == 3);

    // Test 3: Single element -> empty string, array unchanged
    long arr3[] = {42};
    std::string result3 = selection_sort_steps(arr3, 1);
    assert(result3 == "");
    assert(arr3[0] == 42);

    // Test 4: Empty array (size 0) -> empty string
    long arr4[] = {};
    std::string result4 = selection_sort_steps(arr4, 0);
    assert(result4 == "");

    // Test 5: Duplicates and negatives
    long arr5[] = {-3, 5, -3, 0, 2};
    std::string result5 = selection_sort_steps(arr5, 5);
    assert(result5 == "Buoc 1: -3 5 -3 0 2\nBuoc 2: -3 -3 5 0 2\nBuoc 3: -3 -3 0 5 2\nBuoc 4: -3 -3 0 2 5\n");
    assert(arr5[0] == -3 && arr5[1] == -3 && arr5[2] == 0 && arr5[3] == 2 && arr5[4] == 5);

    // Test 6: Large values (check with long)
    long arr6[] = {1000000000L, -1000000000L, 0};
    std::string result6 = selection_sort_steps(arr6, 3);
    assert(result6 == "Buoc 1: -1000000000 1000000000 0\nBuoc 2: -1000000000 0 1000000000\n");
    assert(arr6[0] == -1000000000L && arr6[1] == 0 && arr6[2] == 1000000000L);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The algorithm follows the classic selection sort: for each position `i` from 0 to `n-2`, find the index `minIdx` of the smallest element in the unsorted subarray `arr[i..n-1]`, then swap `arr[i]` with `arr[minIdx]`. After each swap, we format a step record. The step counter starts at 1 and increments after each outer-loop iteration. We build the output string by appending `"Buoc " + to_string(step) + ": "`, then each element of the array separated by spaces, followed by a newline. Edge cases: if `n <= 1`, the loop never runs, so the function returns an empty string—this covers empty and single-element arrays. For `n=2`, there is exactly one step. Duplicates require no special handling because `arr[j] < arr[minIdx]` ensures we pick the first occurrence of the minimum, but swapping duplicates still works correctly. Time complexity is O(n^2) due to the nested loops (sum of comparisons = n(n-1)/2). Space complexity is O(n) for the output string plus O(1) auxiliary (excluding the input array). The function modifies the input array in place, so the caller will see the sorted order after the call.
