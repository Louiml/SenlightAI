// Write a C++ function that takes a vector of integers and a target value, and returns a `std::optional<size_t>` containing the index of the **last** occurrence of the target in the vector. If the target is not found, return `std::nullopt`. The function must handle empty vectors gracefully (returning `nullopt`), preserve the original vector (use `const` reference), and work correctly for duplicate values. The signature should be: `std::optional<size_t> findLastIndex(const std::vector<int>& vec, int target)`. Example: `findLastIndex({1,2,3,2,4}, 2)` should return `3`, while `findLastIndex({1,2,3}, 5)` should return `std::nullopt`.
#include <cassert>
#include <optional>
#include <vector>

// Assume findLastIndex is defined above or included from header.

int main() {
    // Basic case: target at the end
    std::vector<int> v1 = {1, 2, 3, 4};
    assert(findLastIndex(v1, 4) == std::optional<size_t>(3));

    // Duplicate values: returns the last index
    std::vector<int> v2 = {5, 5, 5};
    assert(findLastIndex(v2, 5) == std::optional<size_t>(2));

    // Target not present
    std::vector<int> v3 = {1, 2, 3};
    assert(findLastIndex(v3, 9) == std::nullopt);

    // Empty vector
    std::vector<int> v4;
    assert(findLastIndex(v4, 1) == std::nullopt);

    // Single element that matches
    std::vector<int> v5 = {42};
    assert(findLastIndex(v5, 42) == std::optional<size_t>(0));

    // Single element that does not match
    std::vector<int> v6 = {7};
    assert(findLastIndex(v6, 8) == std::nullopt);

    // Multiple duplicates with matches in the middle
    std::vector<int> v7 = {2, 3, 2, 5, 2, 9};
    assert(findLastIndex(v7, 2) == std::optional<size_t>(4));

    // Negative numbers and matching a negative
    std::vector<int> v8 = {-1, -2, -3, -2};
    assert(findLastIndex(v8, -2) == std::optional<size_t>(3));

    return 0;
}
#include <optional>
#include <vector>
#include <cstddef>

// Return the index of the last occurrence of target in vec, or nullopt if not found.
std::optional<size_t> findLastIndex(const std::vector<int>& vec, int target) {
    // Iterate backwards from the last element down to the first.
    for (size_t i = vec.size(); i-- > 0; ) {
        if (vec[i] == target) {
            return i;
        }
    }
    return std::nullopt;
}
// The simplest approach is to iterate the vector from the end to the beginning, checking each element for equality with the target. Since we want the last occurrence, scanning backwards guarantees that the first match we encounter is the largest index. If the vector is empty or no match is found, return `std::nullopt`; otherwise, return the current index wrapped in `std::optional`. Complexity: the loop runs at most `n` times, where `n = vec.size()`, so time is `O(n)`. We only use a single index variable, so auxiliary space is `O(1)`. Edge cases include: empty vector → return `nullopt`; target appears once → return that index; target appears multiple times → return the highest index; target not present → return `nullopt`. Using `size_t` for the index avoids signed/unsigned comparisons and correctly handles indices from `0` upward, but be careful with the loop condition to avoid underflow; a common pattern is to use `for (size_t i = vec.size(); i-- > 0;)` or iterate with a signed index reversed.
