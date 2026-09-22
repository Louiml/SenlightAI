Write a C++ function named `mergeTwoArrays` that takes two integer arrays (as raw pointers) and their respective sizes, and returns a `std::vector<int>` containing all elements from the first array followed by all elements from the second array, preserving the original order. The function must handle the edge case where either array has size zero (returning the other array's elements) and must be `const`-correct, meaning the input arrays are not modified. Do not use any standard library functions for merging; rely only on a simple loop approach.

#include <cassert>
#include <vector>

// Declaration of the tested function (already defined above)
std::vector<int> mergeTwoArrays(const int* arr1, size_t size1, const int* arr2, size_t size2);

int main() {
    // Basic case: both arrays non-empty
    int a[] = {1, 2, 3};
    int b[] = {4, 5, 6};
    std::vector<int> expected1 = {1, 2, 3, 4, 5, 6};
    assert(mergeTwoArrays(a, 3, b, 3) == expected1);

    // First array empty
    int empty[] = {};
    int c[] = {7, 8};
    std::vector<int> expected2 = {7, 8};
    assert(mergeTwoArrays(empty, 0, c, 2) == expected2);

    // Second array empty
    int d[] = {9, 10};
    assert(mergeTwoArrays(d, 2, empty, 0) == std::vector<int>({9, 10}));

    // Both arrays empty
    assert(mergeTwoArrays(empty, 0, empty, 0) == std::vector<int>());

    // Single elements
    int e[] = {42};
    int f[] = {-1};
    assert(mergeTwoArrays(e, 1, f, 1) == std::vector<int>({42, -1}));

    // Negative and zero values
    int g[] = {-5, 0, -3};
    int h[] = {2, -8};
    std::vector<int> expected3 = {-5, 0, -3, 2, -8};
    assert(mergeTwoArrays(g, 3, h, 2) == expected3);

    // Larger sizes to test iteration correctness
    int big1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int big2[] = {11, 12, 13, 14, 15};
    std::vector<int> expected4;
    for (int i = 1; i <= 10; ++i) expected4.push_back(i);
    for (int i = 11; i <= 15; ++i) expected4.push_back(i);
    assert(mergeTwoArrays(big1, 10, big2, 5) == expected4);

    return 0;
}

#include <vector>

// Merges two arrays into a single vector preserving order.
// Takes const pointers to avoid modification and sizes for bounds.
std::vector<int> mergeTwoArrays(const int* arr1, size_t size1, const int* arr2, size_t size2) {
    std::vector<int> result;
    result.reserve(size1 + size2); // optional optimization to avoid reallocations

    for (size_t i = 0; i < size1; ++i) {
        result.push_back(arr1[i]);
    }
    for (size_t i = 0; i < size2; ++i) {
        result.push_back(arr2[i]);
    }
    return result;
}

// The solution involves creating an empty `std::vector<int>` and using the `push_back` method to append each element from the first array, then each element from the second array. The main algorithm is straightforward: iterate through both arrays sequentially. Edge cases: if one array is empty (size 0), the loop for that array will simply not execute, and the function will correctly return only the other array's elements; if both are empty, an empty vector is returned. No overflow concerns because vector grows dynamically. Time complexity is O(n + m) where n and m are the sizes of the two arrays, since each element is visited exactly once. Space complexity is O(n + m) for the returned vector, plus the constant overhead of the loop.
