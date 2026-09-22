/*
Write a C++ function named `printMinimalGroupRanges` that takes an array of integers (containing only two distinct values, e.g., 0 and 1) and its size, and prints the starting and ending indices of every contiguous group of elements that differs from the element at index 0. The function should output each group's range in the format `from <start> to <end>` on its own line, where `<start>` is the first index of the group and `<end>` is the last index of that group. If the array has only one distinct value (all elements equal to the first element), the function should print nothing. The function must be efficient (single pass, no extra data structures), handle empty arrays gracefully (print nothing), and use `const int*` for the array parameter to ensure the input is not modified.
*/

#include <iostream>

// Prints the start and end indices of every contiguous group whose value
// differs from arr[0]. Assumes the array contains only two distinct values.
void printMinimalGroupRanges(const int* arr, int n) {
    if (n <= 0) return; // empty array, nothing to print

    int start = -1; // start index of current group opposite to arr[0]

    for (int i = 1; i < n; ++i) {
        if (arr[i] != arr[i - 1]) {
            if (arr[i] != arr[0]) {
                // Beginning of a group that differs from arr[0]
                start = i;
            } else {
                // End of such a group (previous element was the last of that group)
                if (start != -1) {
                    std::cout << "from " << start << " to " << (i - 1) << '\n';
                    start = -1;
                }
            }
        }
    }

    // If the last group goes to the end of the array
    if (start != -1) {
        std::cout << "from " << start << " to " << (n - 1) << '\n';
    }
}

#include <cassert>
#include <sstream>
#include <iostream>

// Forward declaration of the function to test
void printMinimalGroupRanges(const int* arr, int n);

// Helper to capture output
std::string captureOutput(const int* arr, int n) {
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    printMinimalGroupRanges(arr, n);
    std::cout.rdbuf(old);
    return oss.str();
}

int main() {
    // Test 1: Example from prompt
    int arr1[] = {1,1,0,0,0,1,1,1,0,0,0,1,1,0};
    assert(captureOutput(arr1, 14) == "from 2 to 4\nfrom 8 to 10\nfrom 13 to 13\n");

    // Test 2: All same values
    int arr2[] = {0,0,0,0};
    assert(captureOutput(arr2, 4) == "");

    // Test 3: Single element
    int arr3[] = {1};
    assert(captureOutput(arr3, 1) == "");

    // Test 4: Empty array
    int* arr4 = nullptr;
    assert(captureOutput(arr4, 0) == "");

    // Test 5: Alternating starting and ending with opposite
    int arr5[] = {0,1,0,1,0};
    assert(captureOutput(arr5, 5) == "from 1 to 1\nfrom 3 to 3\n");

    // Test 6: Opposite group at the very end
    int arr6[] = {1,1,1,0,0};
    assert(captureOutput(arr6, 5) == "from 3 to 4\n");

    // Test 7: Opposite group at the beginning (impossible, but ensure no output)
    int arr7[] = {0,0,0};  // all same as arr[0]
    assert(captureOutput(arr7, 3) == "");

    // Test 8: Only one opposite element in middle
    int arr8[] = {1,0,1};
    assert(captureOutput(arr8, 3) == "from 1 to 1\n");

    // Test 9: Two groups adjacent to each other? Not possible with only two values but test pattern 0 1 1 0 0 1
    int arr9[] = {0,1,1,0,0,1};
    assert(captureOutput(arr9, 6) == "from 1 to 2\nfrom 5 to 5\n");

    // Test 10: Large group
    int arr10[] = {1,0,0,0,0,0,1};
    assert(captureOutput(arr10, 7) == "from 1 to 5\n");

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

// The core idea is to detect transitions between different values in the array. Since the array contains only two values, and one of them is `arr[0]`, we only need to print groups that are the *opposite* of `arr[0]`. We walk through the array from index 1 to n-1. Whenever we see a change from `arr[i-1]` to `arr[i]`, we know we are at the start or end of a group. If `arr[i] != arr[0]`, then we are starting a group of the opposite value, so we record `i` as the start. If `arr[i] == arr[0]`, then we just ended a group of the opposite value, so we print `from <lastStart> to <i-1>` and reset. After the loop, if the last element is still opposite to `arr[0]`, we print the final group from `lastStart` to `n-1`. Edge cases: empty array (n==0) → print nothing; all elements same → no transitions, so nothing printed; array length 1 → no loop iterations, and last element equals `arr[0]` so nothing printed; array with only one group of opposite value starting at 0? The problem states groups that differ from `arr[0]`, so a group cannot start at index 0 because that would be equal. The algorithm runs in O(n) time and O(1) extra space.
