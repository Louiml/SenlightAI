// Write a C++ function `pipe_sort_unique(std::vector<int>& input)` that mimics the behavior of the piped expression `sort(v1) | unique` from the provided snippet. The function must sort the vector in ascending order and then remove consecutive duplicate elements, returning a new `std::vector<int>` containing the resulting unique values in sorted order. The original vector should be left unmodified. Handle edge cases such as an empty vector, a vector with all identical elements, and a vector already containing unique elements. The function should be self-contained and not rely on any external expression template libraries.

// The solution follows a two-step approach: first, create a copy of the input vector to avoid modifying the original. Second, sort the copy using `std::sort` (which has average-case O(n log n) time complexity). Then, use `std::unique` to rearrange the sorted vector so that each unique value appears only once in the front portion, and obtain an iterator to the new logical end. Finally, construct the result vector from the beginning of the sorted copy up to that iterator. This approach correctly handles: empty vectors (sort and unique are no-ops, returning an empty vector), all-identical elements (unique collapses to a single element), and already-unique sorted input (unique does nothing but returns end iterator). Time complexity is O(n log n) due to sorting, space complexity is O(n) for the copy (though we could sort in place if modification of input were allowed, but the task requires returning a new vector while leaving input unchanged).

#include <algorithm>
#include <vector>

/**
 * Sorts the input vector in ascending order and removes consecutive duplicates,
 * returning a new vector with unique values. The original input is not modified.
 */
std::vector<int> pipe_sort_unique(const std::vector<int>& input) {
    // Create a copy to sort and deduplicate
    std::vector<int> copy = input;
    
    // Sort the copy in ascending order
    std::sort(copy.begin(), copy.end());
    
    // Remove consecutive duplicates and get the new end iterator
    auto new_end = std::unique(copy.begin(), copy.end());
    
    // Construct the result from the beginning up to the new end
    return std::vector<int>(copy.begin(), new_end);
}

#include <cassert>
#include <vector>

// Declaration of the function under test (should match the solution)
std::vector<int> pipe_sort_unique(const std::vector<int>& input);

int main() {
    // Basic case with duplicates
    std::vector<int> v1 = {0, 2, 2, 7, 1, 3, 8};
    std::vector<int> result1 = pipe_sort_unique(v1);
    assert(result1 == std::vector<int>({0, 1, 2, 3, 7, 8}));
    assert(v1 == std::vector<int>({0, 2, 2, 7, 1, 3, 8})); // original unchanged

    // Empty vector
    std::vector<int> v2;
    assert(pipe_sort_unique(v2).empty());

    // All identical elements
    std::vector<int> v3 = {5, 5, 5, 5};
    assert(pipe_sort_unique(v3) == std::vector<int>({5}));

    // Already unique but unsorted
    std::vector<int> v4 = {9, 1, 5};
    assert(pipe_sort_unique(v4) == std::vector<int>({1, 5, 9}));

    // Already sorted and unique
    std::vector<int> v5 = {-3, 0, 2, 7};
    assert(pipe_sort_unique(v5) == std::vector<int>({-3, 0, 2, 7}));

    // Large duplicate run and multiple duplicates
    std::vector<int> v6 = {4, 4, 4, 4, 4, 1, 1, 3, 3, 3, 2, 2};
    assert(pipe_sort_unique(v6) == std::vector<int>({1, 2, 3, 4}));

    // Single element
    std::vector<int> v7 = {42};
    assert(pipe_sort_unique(v7) == std::vector<int>({42}));

    // Negatives and zeros
    std::vector<int> v8 = {-1, 0, -1, 0, -1, 0};
    assert(pipe_sort_unique(v8) == std::vector<int>({-1, 0}));

    return 0;
}
