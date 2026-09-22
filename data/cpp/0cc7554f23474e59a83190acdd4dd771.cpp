Write a C++ function that takes two integer vectors `first` and `second` as inputs and returns a new vector where all elements of `second` are inserted into `first` starting immediately after the first element (i.e., at index 1). The original order of elements in both vectors must be preserved. The function must not modify the input vectors and must handle edge cases such as an empty `first` vector (in which case the result should simply be a copy of `second`), an empty `second` vector (in which case the result should be a copy of `first`), and both empty (return an empty vector). For example, given `first = {1, 2, 3}` and `second = {10, 20}`, the result should be `{1, 10, 20, 2, 3}`.
// The solution creates a result vector by reserving enough space for `first.size() + second.size()`. Then we copy the first element of `first` (if it exists) into the result, followed by all elements of `second`, and finally the remaining elements of `first` starting from index 1. Edge cases: if `first` is empty, we simply return a copy of `second`; if `second` is empty or `first` has only one element, we return a copy of `first`. The algorithm runs in O(n + m) time where n and m are the sizes of the input vectors, and uses O(n + m) auxiliary space for the result (which is unavoidable for the return value). No input modification occurs because we use `const` references and copy into a fresh vector.
#include <vector>

// Return a new vector with all elements of 'second' inserted after the first element of 'first'.
std::vector<int> insertAfterFirst(const std::vector<int>& first, const std::vector<int>& second) {
    std::vector<int> result;
    result.reserve(first.size() + second.size());

    // If first is empty, just return a copy of second (or empty if second also empty).
    if (first.empty()) {
        result.insert(result.end(), second.begin(), second.end());
        return result;
    }

    // Insert first element of first.
    result.push_back(first[0]);

    // Insert all elements of second.
    result.insert(result.end(), second.begin(), second.end());

    // Insert the rest of first (starting from index 1).
    result.insert(result.end(), first.begin() + 1, first.end());

    return result;
}
#include <cassert>
#include <vector>

// Free function prototype (provided by solution)
std::vector<int> insertAfterFirst(const std::vector<int>& first, const std::vector<int>& second);

int main() {
    // Normal case
    assert(insertAfterFirst({1, 2, 3}, {10, 20}) == std::vector<int>({1, 10, 20, 2, 3}));

    // Empty second vector
    assert(insertAfterFirst({5, 6, 7}, {}) == std::vector<int>({5, 6, 7}));

    // Empty first vector
    assert(insertAfterFirst({}, {9, 8}) == std::vector<int>({9, 8}));

    // Both empty
    assert(insertAfterFirst({}, {}).empty());

    // First has only one element
    assert(insertAfterFirst({42}, {1, 2, 3}) == std::vector<int>({42, 1, 2, 3}));

    // Larger vectors with duplicates
    assert(insertAfterFirst({0, 0, 0}, {5, 5}) == std::vector<int>({0, 5, 5, 0, 0}));

    // Check that inputs are not modified
    std::vector<int> a = {1, 2};
    std::vector<int> b = {3, 4};
    insertAfterFirst(a, b);
    assert(a == std::vector<int>({1, 2}));
    assert(b == std::vector<int>({3, 4}));

    return 0;
}
