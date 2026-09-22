/*
Write a C++ function `count_unique_values` that takes a `std::vector<int>` and returns the number of distinct integer values in it. The function must treat the vector as a sorted doubly linked list conceptually, but you should implement it using standard C++ containers and algorithms. The function should handle an empty vector (returning 0), contain only unique values, contain duplicates, and handle negative numbers. Do not modify the input vector.
*/

#include <vector>
#include <algorithm>

// Returns the number of distinct integer values in the input vector.
// Does not modify the input; sorts a copy and uses std::unique to collapse duplicates.
int count_unique_values(const std::vector<int>& values) {
    if (values.empty()) {
        return 0;
    }

    std::vector<int> sorted = values;               // copy to avoid modifying input
    std::sort(sorted.begin(), sorted.end());        // O(n log n)

    // Eliminate consecutive duplicates; returns iterator to new logical end.
    auto new_end = std::unique(sorted.begin(), sorted.end());

    // Distance from begin to new_end gives the number of unique elements.
    return static_cast<int>(std::distance(sorted.begin(), new_end));
}

#include <cassert>
#include <vector>

// Forward declaration (the function is defined in the solution section).
int count_unique_values(const std::vector<int>& values);

int main() {
    // Empty vector
    assert(count_unique_values({}) == 0);

    // Single element
    assert(count_unique_values({5}) == 1);

    // All unique
    assert(count_unique_values({3, 1, 4, 2}) == 4);

    // All duplicates
    assert(count_unique_values({7, 7, 7, 7}) == 1);

    // Mixed with negative numbers and duplicates
    assert(count_unique_values({-2, 5, -2, 0, 5, 3}) == 4);

    // Unsorted input with many duplicates
    assert(count_unique_values({1, 1, 2, 2, 3, 3, 4, 4, 5, 5}) == 5);

    // Large numbers
    assert(count_unique_values({1000000, -1000000, 1000000}) == 2);

    // Input already sorted with duplicates
    assert(count_unique_values({1, 1, 2, 2, 3}) == 3);

    // Ensure input is not modified
    std::vector<int> input = {3, 1, 3, 2};
    count_unique_values(input);
    assert((input == std::vector<int>{3, 1, 3, 2}));

    return 0;
}

// The simplest approach is to sort a copy of the input vector, then iterate through it once, counting every time the current element differs from the previous element. Alternatively, use a `std::set` or `std::unordered_set` to insert all values and return its size, but that has extra overhead. The sorting approach requires `O(n log n)` time due to the sort and `O(n)` auxiliary space for the copy. Edge cases: empty vector returns 0; single element returns 1; all duplicates returns 1. Using `std::unique` on a sorted copy and then taking the distance from begin to the new end gives the count cleanly. Time complexity is `O(n log n)` for sort, and space is `O(n)` for the copy. For a linear-time approach, use an unordered set, but the sorting method is deterministic and avoids hash collisions.
