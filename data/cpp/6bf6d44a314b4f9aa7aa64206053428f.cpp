// Write a standalone C++ function named `removeAllOccurrences` that accepts an `std::vector<int>` and an integer `target`, and returns a new `std::vector<int>` containing all elements from the input vector except every occurrence equal to `target`, preserving the relative order of the remaining elements. The function must not modify the input vector (use `const` reference) and must allocate only the necessary memory for the result. You may assume the input vector is non-empty, but it can contain duplicates, negative numbers, and the target may or may not be present. The function should be self-contained, include all required headers, and be safe for use in a multi-faceted testing environment. Do not use `std::remove` or `std::remove_if` — implement the removal logic manually.
The solution iterates through the input vector using a range-based `for` loop, checking each element against the `target`. For each element that does not equal the target, it appends that element to a result vector. This approach naturally preserves order and only includes non-matching elements. Edge cases: if the target is not present, the result is a copy of the input; if all elements match, the result is empty; duplicates are handled correctly because each element is tested independently. Time complexity is O(n) because we traverse the vector once and each push_back is amortized O(1). Space complexity is O(n) in the worst case (when nothing is removed) for the result vector, which is unavoidable given the function’s contract. No special handling is needed for negative numbers or empty input since the problem states input is non-empty.
#include <vector>

// Return a new vector containing all elements from `input` except those equal to `target`.
// Preserves relative order and does not modify the original vector.
std::vector<int> removeAllOccurrences(const std::vector<int>& input, int target) {
    std::vector<int> result;
    result.reserve(input.size()); // preallocate to avoid reallocations
    for (int value : input) {
        if (value != target) {
            result.push_back(value);
        }
    }
    return result;
}
#include <cassert>
#include <vector>

int main() {
    // basic removal
    std::vector<int> v1 = {1, 2, 4, 3, 4, 5};
    assert(removeAllOccurrences(v1, 4) == std::vector<int>({1, 2, 3, 5}));

    // target not present
    std::vector<int> v2 = {10, 20, 30};
    assert(removeAllOccurrences(v2, 99) == v2);

    // all elements match target
    std::vector<int> v3 = {7, 7, 7};
    assert(removeAllOccurrences(v3, 7).empty());

    // duplicates and negatives
    std::vector<int> v4 = {-3, 1, -3, 2, -3};
    assert(removeAllOccurrences(v4, -3) == std::vector<int>({1, 2}));

    // single non-matching element
    std::vector<int> v5 = {42};
    assert(removeAllOccurrences(v5, 0) == std::vector<int>({42}));

    // single matching element
    std::vector<int> v6 = {42};
    assert(removeAllOccurrences(v6, 42).empty());
}
