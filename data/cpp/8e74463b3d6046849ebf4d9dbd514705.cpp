/*
Write a C++ function named `findRange` that takes a fixed-size array of exactly 5 integers and returns a `std::pair<int, int>` containing the maximum value as the first element and the minimum value as the second element. The function should work correctly with any integer values, including negative numbers, zeros, and duplicates. The task requires reading exactly 5 integers from standard input (though the function itself should not handle I/O), computing the largest and smallest values, and returning them as a pair. The function must be `const`-correct with respect to the input array (i.e., it should not modify the input). The overall program (not part of the required function) should read one or more test cases from standard input, each with 5 integers, and output the maximum and minimum separated by a space for each case.
*/

#include <utility>   // for std::pair
#include <algorithm> // for std::max, std::min (optional, but used here)

// Returns a pair where first is the maximum and second is the minimum
// of the five integers in the given array. The input array is not modified.
std::pair<int, int> findRange(const int arr[5]) {
    int maxVal = arr[0];
    int minVal = arr[0];

    for (int i = 1; i < 5; ++i) {
        maxVal = std::max(maxVal, arr[i]);
        minVal = std::min(minVal, arr[i]);
    }

    return {maxVal, minVal};
}

#include <cassert>
#include <utility>

// Declaration of the function to test (provided in the solution)
std::pair<int, int> findRange(const int arr[5]);

int main() {
    // Test 1: Basic positive numbers
    int arr1[5] = {1, 2, 3, 4, 5};
    assert(findRange(arr1) == std::make_pair(5, 1));

    // Test 2: Negative numbers
    int arr2[5] = {-1, -5, -3, -10, -2};
    assert(findRange(arr2) == std::make_pair(-1, -10));

    // Test 3: All equal
    int arr3[5] = {7, 7, 7, 7, 7};
    assert(findRange(arr3) == std::make_pair(7, 7));

    // Test 4: Duplicates and zeros
    int arr4[5] = {0, 0, -2, 0, 4};
    assert(findRange(arr4) == std::make_pair(4, -2));

    // Test 5: Mixed, max at first, min at last
    int arr5[5] = {100, 2, 3, 4, -100};
    assert(findRange(arr5) == std::make_pair(100, -100));

    // Test 6: Max at last, min at first
    int arr6[5] = {-50, 2, 3, 4, 50};
    assert(findRange(arr6) == std::make_pair(50, -50));

    return 0;
}

// The solution is straightforward: initialize both `maxVal` and `minVal` to the first element of the array. Then iterate through the remaining 4 elements, comparing each with the current `maxVal` and `minVal`. If an element is greater than `maxVal`, update `maxVal`; if it is less than `minVal`, update `minVal`. After processing all 5 elements, return the pair `{maxVal, minVal}`. Edge cases include all elements being equal (then max and min are the same), negative numbers, and duplicates, all handled naturally by the comparisons. The time complexity is O(5) = O(1) because the array size is fixed, and space complexity is O(1) since only a few local variables are used. The function does not modify the input array, so the parameter is `const int arr[5]`.
