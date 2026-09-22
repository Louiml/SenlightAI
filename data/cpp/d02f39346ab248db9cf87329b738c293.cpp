Write a C++ function that takes a raw integer array (pointer to its first element) and its length as arguments, and returns a `std::vector<int>` containing the elements at even indices (0, 2, 4, ...) in the same order they appear. The function must handle arrays of any length, including zero-length arrays, and must properly consider that the source array may not be modifiable. The function signature should use `const int*` for the array parameter.
#include <cassert>
#include <vector>
#include "solution.h" // assume the solution is in this header or included directly

int main() {
    // Test 1: Normal array
    int arr1[] = {0, 1, 2, 3, 4, 5};
    std::vector<int> res1 = extractEvenIndices(arr1, 6);
    assert(res1 == std::vector<int>({0, 2, 4}));

    // Test 2: Odd length array
    int arr2[] = {10, 20, 30, 40, 50};
    std::vector<int> res2 = extractEvenIndices(arr2, 5);
    assert(res2 == std::vector<int>({10, 30, 50}));

    // Test 3: Single element
    int arr3[] = {42};
    std::vector<int> res3 = extractEvenIndices(arr3, 1);
    assert(res3 == std::vector<int>({42}));

    // Test 4: Empty array
    int* arr4 = nullptr;
    std::vector<int> res4 = extractEvenIndices(arr4, 0);
    assert(res4.empty());

    // Test 5: Two elements
    int arr5[] = {7, 8};
    std::vector<int> res5 = extractEvenIndices(arr5, 2);
    assert(res5 == std::vector<int>({7}));

    // Test 6: Larger array with negative values
    int arr6[] = {-5, 100, -3, 200, -1, 300};
    std::vector<int> res6 = extractEvenIndices(arr6, 6);
    assert(res6 == std::vector<int>({-5, -3, -1}));

    // Test 7: Array with zeros at even positions
    int arr7[] = {0, 1, 0, 3, 0, 5};
    std::vector<int> res7 = extractEvenIndices(arr7, 6);
    assert(res7 == std::vector<int>({0, 0, 0}));
}
#include <vector>

// Extract elements at even indices (0, 2, 4, ...) from an integer array.
// Parameters:
//   arr - pointer to the first element of a const integer array
//   length - number of elements in the array
// Returns:
//   A std::vector<int> containing the elements at even indices in order.
std::vector<int> extractEvenIndices(const int* arr, int length) {
    std::vector<int> result;
    for (int i = 0; i < length; ++i) {
        if (i % 2 == 0) {
            result.push_back(arr[i]);
        }
    }
    return result;
}
// The task is a straightforward filtering operation: iterate through the input array and select elements at positions where the index modulo 2 equals zero. The core algorithm involves a single loop from index 0 up to (but not including) the length, checking `i % 2 == 0` for each step. For each such index, we push the element into a result vector. Edge cases include an empty array (length 0) where the result should be an empty vector, and an array of length 1 where only the first element is selected. Since we only read from the input and never modify it, a `const int*` parameter is appropriate. Time complexity is O(n) where n is the length of the array, as we visit each element exactly once. Space complexity is O(k) where k is the number of even-indexed elements (approximately n/2), which is the size of the returned vector. No additional auxiliary space is used beyond the result vector.
