/*
Write a C++ function `reverseArrayInPlace` that takes a non-empty `std::vector<int>` and returns a new vector containing the elements in strictly reverse order without modifying the original vector. The function must handle both positive and negative integers, and must work correctly for vectors of size 1. The function signature should be `std::vector<int> reverseArrayInPlace(const std::vector<int>& arr)`, and must not use any built-in reverse functions (e.g., `std::reverse`). The returned vector must contain the elements of the input in reverse order, preserving any duplicates.
*/

#include <vector>

// Return a new vector containing the elements of arr in reverse order.
// The input vector is not modified. Works for non-empty vectors.
std::vector<int> reverseArrayInPlace(const std::vector<int>& arr) {
    std::vector<int> reversed;
    reversed.reserve(arr.size());  // Avoid reallocations

    // Iterate from the last index to the first.
    for (int i = static_cast<int>(arr.size()) - 1; i >= 0; --i) {
        reversed.push_back(arr[i]);
    }

    return reversed;
}

#include <cassert>
#include <vector>

// Include the solution function here (or link against it).
std::vector<int> reverseArrayInPlace(const std::vector<int>& arr);

int main() {
    // Test 1: normal case
    std::vector<int> v1 = {1, 2, 3, 4};
    assert(reverseArrayInPlace(v1) == std::vector<int>({4, 3, 2, 1}));

    // Test 2: single element
    std::vector<int> v2 = {42};
    assert(reverseArrayInPlace(v2) == std::vector<int>({42}));

    // Test 3: negative numbers
    std::vector<int> v3 = {-5, 0, -3, 7};
    assert(reverseArrayInPlace(v3) == std::vector<int>({7, -3, 0, -5}));

    // Test 4: duplicates
    std::vector<int> v4 = {3, 3, 3};
    assert(reverseArrayInPlace(v4) == std::vector<int>({3, 3, 3}));

    // Test 5: two elements
    std::vector<int> v5 = {10, -2};
    assert(reverseArrayInPlace(v5) == std::vector<int>({-2, 10}));

    // Test 6: larger vector (10 elements)
    std::vector<int> v6 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    assert(reverseArrayInPlace(v6) == std::vector<int>({10, 9, 8, 7, 6, 5, 4, 3, 2, 1}));

    // Test 7: original vector not modified
    std::vector<int> v7 = {1, 2, 3};
    std::vector<int> original = v7;
    reverseArrayInPlace(v7);
    assert(v7 == original);  // ensure input unchanged

    return 0;
}

// The approach is straightforward: create a new empty vector, then iterate over the input vector from the last index down to the first index (i.e., `for (int i = arr.size() - 1; i >= 0; --i)`), pushing each element into the new vector. Since we start at the last index and go backward, the first element of the new vector will be the last element of the original, and so on. We must carefully handle the index type: use `std::size_t` or `int` but be cautious about underflow when using `int` with size 0—though the function guarantees a non-empty vector, so this is safe. Edge cases include a vector with a single element (reversing yields the same vector) and negative numbers (they are handled naturally). Time complexity is O(n) because we visit each element exactly once; space complexity is O(n) for the returned vector, excluding the input which we do not modify. The function must be `const`-correct by taking the input as a const reference to avoid copying.
