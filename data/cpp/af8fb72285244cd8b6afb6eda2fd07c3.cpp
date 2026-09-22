Write a C++ function that accepts a raw array of integers and its length, and returns an `std::vector<int>` containing every second element from the original array starting at index 0 (i.e., elements at even indices: 0, 2, 4, ...), preserving their relative order. The function must not use any external library beyond the C++ standard library, must be `const`-correct (the input array must not be modified), and must handle edge cases such as empty arrays and arrays with only one element. The function should be named `extractEvenIndexedElements`.
#include <cassert>
#include <vector>

// Include the solution function (assume it's in the same translation unit)
std::vector<int> extractEvenIndexedElements(const int* array, int length);

int main() {
    // Test with a regular array of 12 elements
    int arr1[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    std::vector<int> expected1 = {0, 2, 4, 6, 8, 10};
    assert(extractEvenIndexedElements(arr1, 12) == expected1);

    // Test with an odd-length array
    int arr2[5] = {10, 20, 30, 40, 50};
    std::vector<int> expected2 = {10, 30, 50};
    assert(extractEvenIndexedElements(arr2, 5) == expected2);

    // Test with a single-element array
    int arr3[1] = {42};
    assert(extractEvenIndexedElements(arr3, 1) == std::vector<int>{42});

    // Test with an empty array (length 0)
    int* arr4 = nullptr;
    assert(extractEvenIndexedElements(arr4, 0).empty());

    // Test with array of length 2
    int arr5[2] = {-1, -2};
    assert(extractEvenIndexedElements(arr5, 2) == std::vector<int>{-1});

    // Test with all zeros
    int arr6[4] = {0, 0, 0, 0};
    assert(extractEvenIndexedElements(arr6, 4) == std::vector<int>({0, 0}));

    // Test with large values
    int arr7[3] = {1000000, -1000000, 2147483647};
    assert(extractEvenIndexedElements(arr7, 3) == std::vector<int>({1000000, 2147483647}));

    // Test with negative length is not expected, but if passed, loop won't run; we test with length 0
    int arr8[3] = {1, 2, 3};
    assert(extractEvenIndexedElements(arr8, 0).empty());

    return 0;
}
#include <vector>

// Extract elements at even indices (0, 2, 4, ...) from a raw array.
// The input array is not modified. Returns a vector containing the selected elements.
std::vector<int> extractEvenIndexedElements(const int* array, int length) {
    std::vector<int> result;
    for (int i = 0; i < length; i += 2) {
        result.push_back(array[i]);
    }
    return result;
}
// The task is a straightforward linear traversal. Since we need every element at an even index, we iterate with a step of 2 from index 0 to `length-1`. The main algorithm is: initialize an empty `std::vector<int>`, then loop `for (int i = 0; i < length; i += 2)` and push back `array[i]` into the vector. Edge cases: if `length` is 0, the loop does not execute and we return an empty vector; if `length` is 1, the loop executes once and returns a vector with the single element. No special handling is required for negative lengths (assume non-negative input). Time complexity is O(n/2) = O(n) where n is the input length, and space complexity is O(n/2) = O(n) for the returned vector. The function should take the array as a const pointer or const reference to an array (we can use a pointer with a length parameter) to ensure we do not modify the input.
