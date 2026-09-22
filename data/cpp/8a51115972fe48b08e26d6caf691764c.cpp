// Implement a C++ function named `stable_sort_positive` that takes a `std::vector<int>` by value, removes all negative numbers, sorts the remaining non-negative numbers in **ascending order while preserving the relative order of equal elements** (i.e., a stable sort), and returns the resulting `std::vector<int>`. The function must not modify the original input vector (it works on a copy). If the input vector contains no non-negative numbers, return an empty vector. The sorting must be implemented explicitly using an insertion sort algorithm (no use of `std::sort` or `std::stable_sort`) to guarantee stability and demonstrate understanding of the sorting process. The function should handle duplicate values, edge cases like empty input, and all integers within the `int` range.
#include <cassert>
#include <vector>

// Assume the function declaration from the solution is available.

int main() {
    // Basic sorting with negatives removed
    {
        std::vector<int> input = {3, -1, 2, 0, -5, 4};
        std::vector<int> result = stable_sort_positive(input);
        assert((result == std::vector<int>{0, 2, 3, 4}));
    }
    // All negatives -> empty
    {
        std::vector<int> input = {-3, -1, -2};
        std::vector<int> result = stable_sort_positive(input);
        assert(result.empty());
    }
    // Empty input -> empty
    {
        std::vector<int> input = {};
        std::vector<int> result = stable_sort_positive(input);
        assert(result.empty());
    }
    // Stability with duplicates
    {
        std::vector<int> input = {5, 2, 5, 1, 2, 0};
        std::vector<int> result = stable_sort_positive(input);
        // Duplicate 5's maintain relative order, duplicate 2's maintain order.
        assert((result == std::vector<int>{0, 1, 2, 2, 5, 5}));
    }
    // Single element
    {
        std::vector<int> input = {7};
        std::vector<int> result = stable_sort_positive(input);
        assert((result == std::vector<int>{7}));
    }
    // Single negative
    {
        std::vector<int> input = {-7};
        std::vector<int> result = stable_sort_positive(input);
        assert(result.empty());
    }
    // Already sorted but with negatives interspersed
    {
        std::vector<int> input = {-10, 1, -20, 2, -30, 3};
        std::vector<int> result = stable_sort_positive(input);
        assert((result == std::vector<int>{1, 2, 3}));
    }
    // Large values and duplicates with negatives
    {
        std::vector<int> input = {100, -1, 100, 0, -2, 0, 50};
        std::vector<int> result = stable_sort_positive(input);
        assert((result == std::vector<int>{0, 0, 50, 100, 100}));
    }
    // Mixed positives and zeros with duplicates
    {
        std::vector<int> input = {4, 0, 4, 0, 1, 3};
        std::vector<int> result = stable_sort_positive(input);
        assert((result == std::vector<int>{0, 0, 1, 3, 4, 4}));
    }
    // Long unsorted with negatives
    {
        std::vector<int> input = {9, -1, 3, -2, 6, 1, -3, 8, 5, 0};
        std::vector<int> result = stable_sort_positive(input);
        assert((result == std::vector<int>{0, 1, 3, 5, 6, 8, 9}));
    }
    return 0;
}
#include <vector>

// Remove negative numbers, then stable-sort the remaining non-negatives in ascending order using insertion sort.
std::vector<int> stable_sort_positive(std::vector<int> input) {
    // Filter out negatives, preserving original order.
    std::vector<int> filtered;
    for (int value : input) {
        if (value >= 0) {
            filtered.push_back(value);
        }
    }

    // Stable insertion sort on the filtered vector.
    for (size_t i = 1; i < filtered.size(); ++i) {
        int key = filtered[i];
        size_t j = i;
        // Shift larger elements to the right. Use '>' (not '>=') to keep stability.
        while (j > 0 && filtered[j - 1] > key) {
            filtered[j] = filtered[j - 1];
            --j;
        }
        filtered[j] = key;
    }

    return filtered;
}
// The solution involves two main steps: filtering and stable sorting.
// 1. **Filtering**: Iterate through the input vector (or its copy) and collect all elements that are `>= 0` into a new vector. This step naturally preserves the relative order of non-negative elements as they appear in the input.
// 2. **Stable insertion sort**: Implement insertion sort manually. For each element from index 1 to n-1, compare it with previous elements moving backwards. Instead of swapping elements, shift larger elements to the right until the correct position is found. Crucially, when comparing, we use `>` (strictly greater) to determine when to shift. If the current element is equal to a previous element, we stop shifting (since `equal` is not `>`), which preserves the relative order of equal elements — this is what makes the sort stable. Insertion sort is O(n²) in the worst case and O(n) in the best case (already sorted). Space usage is O(n) for the output vector (excluding the copy if we choose to modify in place; but since we filter into a new vector, that's O(n) auxiliary). Edge cases: empty input -> empty output; all negatives -> empty output; duplicates -> stable via strict comparison; already sorted -> insertion sort does minimal work.
