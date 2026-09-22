// Write a C++ function named `sortedSubarray` that takes a constant reference to a `std::vector<int>` and returns a new `std::vector<int>` containing the elements of the input vector sorted in non-decreasing order. The input vector may be empty, contain duplicate values, negative numbers, and any number of elements. The function must not modify the input vector. Use the standard library’s `std::sort` algorithm with iterators to achieve the sorting. Ensure the function is efficient for large inputs and handles the empty vector case gracefully by returning an empty vector.

#include <cassert>
#include <vector>

// Declare the function (in actual code this would be in a header or above main)
std::vector<int> sortedSubarray(const std::vector<int>& input);

int main() {
    // Basic sorting with unique elements
    std::vector<int> v1 = {5, 2, 9, 1, 7};
    assert((sortedSubarray(v1) == std::vector<int>{1, 2, 5, 7, 9}));
    // Ensure original vector is not modified
    assert((v1 == std::vector<int>{5, 2, 9, 1, 7}));

    // Empty vector
    std::vector<int> v2;
    assert(sortedSubarray(v2).empty());

    // Duplicate values
    std::vector<int> v3 = {4, 2, 4, 1, 2};
    assert((sortedSubarray(v3) == std::vector<int>{1, 2, 2, 4, 4}));

    // Negative and positive numbers
    std::vector<int> v4 = {-3, 0, -7, 5, -2};
    assert((sortedSubarray(v4) == std::vector<int>{-7, -3, -2, 0, 5}));

    // Single element
    std::vector<int> v5 = {42};
    assert((sortedSubarray(v5) == std::vector<int>{42}));

    // Already sorted input
    std::vector<int> v6 = {1, 2, 3, 4};
    assert((sortedSubarray(v6) == std::vector<int>{1, 2, 3, 4}));

    // Reverse sorted input
    std::vector<int> v7 = {9, 8, 7, 6};
    assert((sortedSubarray(v7) == std::vector<int>{6, 7, 8, 9}));

    // Large vector with many duplicates
    std::vector<int> v8(1000, 3);
    assert(sortedSubarray(v8) == std::vector<int>(1000, 3));

    return 0;
}

#include <vector>
#include <algorithm>

// Returns a new vector containing the elements of `input` in non-decreasing order.
// The input vector is left unmodified.
std::vector<int> sortedSubarray(const std::vector<int>& input) {
    std::vector<int> result = input;  // Copy to preserve original
    std::sort(result.begin(), result.end());
    return result;
}

// The solution should create a copy of the input vector to preserve the original data, then apply `std::sort` on the copy’s iterators. Since `std::sort` requires random-access iterators and `std::vector` provides them, we can simply call `std::sort(begin, end)`. Edge cases include an empty vector (the copy is empty, sorting works trivially) and vectors with duplicate values (sort handles them naturally). Time complexity is \(O(n \log n)\) due to the sort, and space complexity is \(O(n)\) for the copy, which is optimal since we must return a new sorted vector without modifying the input. The function should be `const`-correct: the parameter is `const std::vector<int>&`, and the returned vector is by value (moved or copied efficiently).
