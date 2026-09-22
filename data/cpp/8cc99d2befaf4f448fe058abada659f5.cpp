Write a C++ function that takes a `std::vector<int>` of any length (possibly empty) and returns a `std::vector<int>` containing the same elements sorted in ascending order. The function must not modify the input vector; instead, it should return a new sorted copy. For an empty vector, return an empty vector.
The straightforward approach is to make a copy of the input vector and then apply the standard library's `std::sort` algorithm on the copy. This preserves the original vector and avoids any side effects. The algorithm used internally by `std::sort` is typically introsort, which has an average and worst-case time complexity of \(O(n \log n)\), where \(n\) is the number of elements. The space complexity is \(O(n)\) because we create a copy of the input. Edge cases include an empty input vector (returns empty) and vectors with duplicate values (handled naturally by sort). No special handling for negative numbers is needed since integers compare normally. Additionally, we should use `const` references for input to avoid copying the whole vector unnecessarily, and apply `const` correctness to variables that are not modified.
#include <vector>
#include <algorithm>

// Return a new vector containing the elements of `input` sorted in ascending order.
// The input vector is not modified.
std::vector<int> sortedCopy(const std::vector<int>& input) {
    std::vector<int> result = input;
    std::sort(result.begin(), result.end());
    return result;
}
#include <cassert>
#include <vector>

// (The solution function is already included above or in the same translation unit.)

int main() {
    // Basic sorting
    std::vector<int> v1 = {3, 1, 2};
    assert((sortedCopy(v1) == std::vector<int>{1, 2, 3}));
    // Original vector must remain unchanged
    assert((v1 == std::vector<int>{3, 1, 2}));

    // Already sorted
    std::vector<int> v2 = {1, 2, 3};
    assert((sortedCopy(v2) == std::vector<int>{1, 2, 3}));

    // Reverse sorted
    std::vector<int> v3 = {5, 4, 3, 2, 1};
    assert((sortedCopy(v3) == std::vector<int>{1, 2, 3, 4, 5}));

    // Duplicate values
    std::vector<int> v4 = {2, 2, 1, 2, 1};
    assert((sortedCopy(v4) == std::vector<int>{1, 1, 2, 2, 2}));

    // Single element
    std::vector<int> v5 = {42};
    assert((sortedCopy(v5) == std::vector<int>{42}));

    // Empty vector
    std::vector<int> v6;
    assert(sortedCopy(v6).empty());

    // Negative numbers
    std::vector<int> v7 = {-5, 0, -2, 3, -1};
    assert((sortedCopy(v7) == std::vector<int>{-5, -2, -1, 0, 3}));

    // Large values
    std::vector<int> v8 = {1000000, -1000000, 0};
    assert((sortedCopy(v8) == std::vector<int>{-1000000, 0, 1000000}));
}
