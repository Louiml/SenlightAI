Write a C++ function that takes a vector of integers and an additional integer value, and returns a new vector containing all elements from the original vector followed by all elements from a second vector provided as a parameter (i.e., concatenate two vectors). The original vectors must remain unchanged, and the returned vector must be sorted in ascending order. The function should handle empty vectors gracefully, and duplicates should be preserved (i.e., no removal of duplicates). The input vectors may contain negative numbers, zeros, and duplicates.

The task requires concatenating two integer vectors and then sorting the result in ascending order. The main algorithm is straightforward: create a new vector, copy all elements from the first vector into it, then copy all elements from the second vector into it, and finally sort the new vector using `std::sort`. The function should accept both vectors by `const` reference to ensure they are not modified. Edge cases include one or both vectors being empty, which should still produce a valid sorted output (possibly empty if both are empty, or just the sorted non-empty vector). Duplicates are preserved because `std::sort` does not remove them. Time complexity is \(O(n \log n)\) where \(n\) is the total number of elements (dominated by sorting). Space complexity is \(O(n)\) for the new vector we create, plus the input vectors themselves (which are not copied). We must ensure the function returns a value (the sorted concatenated vector) and applies `const` correctness to avoid unintended modifications.

#include <vector>
#include <algorithm>

// Concatenate two vectors of integers and return the sorted result.
// The original vectors are not modified.
std::vector<int> sortedConcatenate(const std::vector<int>& first, const std::vector<int>& second) {
    std::vector<int> result;
    result.reserve(first.size() + second.size());
    result.insert(result.end(), first.begin(), first.end());
    result.insert(result.end(), second.begin(), second.end());
    std::sort(result.begin(), result.end());
    return result;
}

#include <cassert>
#include <vector>

// Assume the solution function is defined above.
int main() {
    // Basic concatenation and sorting
    std::vector<int> v1 = {3, 1, 2};
    std::vector<int> v2 = {4, 0, 5};
    std::vector<int> result = sortedConcatenate(v1, v2);
    assert(result == std::vector<int>({0, 1, 2, 3, 4, 5}));

    // One empty vector
    std::vector<int> v3;
    std::vector<int> v4 = {2, -1, 0};
    assert(sortedConcatenate(v3, v4) == std::vector<int>({-1, 0, 2}));
    assert(sortedConcatenate(v4, v3) == std::vector<int>({-1, 0, 2}));

    // Both empty
    std::vector<int> v5;
    std::vector<int> v6;
    assert(sortedConcatenate(v5, v6).empty());

    // Duplicates preserved
    std::vector<int> v7 = {2, 2, 1};
    std::vector<int> v8 = {1, 3};
    assert(sortedConcatenate(v7, v8) == std::vector<int>({1, 1, 2, 2, 3}));

    // Negative numbers, zeros, and single-element vectors
    assert(sortedConcatenate({-5}, {0, -2}) == std::vector<int>({-5, -2, 0}));

    // Original vectors unchanged
    std::vector<int> orig1 = {5, 4};
    std::vector<int> orig2 = {3, 2};
    sortedConcatenate(orig1, orig2);
    assert(orig1 == std::vector<int>({5, 4}));
    assert(orig2 == std::vector<int>({3, 2}));
}
