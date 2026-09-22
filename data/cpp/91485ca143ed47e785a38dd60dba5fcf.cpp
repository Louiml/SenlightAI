/*
Write a C++ function `Vector2DIterator` that takes a `std::vector<std::vector<int>>` as input and returns a `std::vector<int>` containing the flattened sequence of all integers in row-major order (i.e., all elements of the first inner vector, then all elements of the second, and so on). The function must not modify the input vector, must handle empty inner vectors gracefully (simply skipping them), and must return the flattened vector. The returned vector should contain all integers exactly as they appear, preserving duplicates and order. For example, given `{{1,2},{},{3},{4,5}}`, the output should be `{1,2,3,4,5}`. The function should be `const`-correct and use `const` references for the parameter to avoid copying.
*/

#include <vector>

// Flatten a 2D vector of integers into a 1D vector in row-major order.
// Empty inner vectors are skipped. The input is not modified.
std::vector<int> flattenVector2D(const std::vector<std::vector<int>>& vec2d) {
    std::vector<int> result;
    // Reserve for better performance (optional)
    size_t total_elems = 0;
    for (const auto& inner : vec2d) {
        total_elems += inner.size();
    }
    result.reserve(total_elems);

    for (const auto& inner : vec2d) {
        // Append all elements of the current inner vector
        result.insert(result.end(), inner.begin(), inner.end());
    }
    return result;
}

#include <cassert>
#include <vector>

// Declaration of the solution function
std::vector<int> flattenVector2D(const std::vector<std::vector<int>>& vec2d);

int main() {
    // Basic case from the problem statement
    std::vector<std::vector<int>> v1 = {{1,2},{3},{4}};
    assert(flattenVector2D(v1) == std::vector<int>({1,2,3,4}));

    // Case with empty inner vectors
    std::vector<std::vector<int>> v2 = {{1,2},{},{3},{4,5}};
    assert(flattenVector2D(v2) == std::vector<int>({1,2,3,4,5}));

    // Case with only empty inner vectors
    std::vector<std::vector<int>> v3 = {{},{},{}};
    assert(flattenVector2D(v3).empty());

    // Empty outer vector
    std::vector<std::vector<int>> v4 = {};
    assert(flattenVector2D(v4).empty());

    // Single element
    std::vector<std::vector<int>> v5 = {{42}};
    assert(flattenVector2D(v5) == std::vector<int>({42}));

    // Duplicates and negative numbers
    std::vector<std::vector<int>> v6 = {{-1,0,-1},{2,2},{3}};
    assert(flattenVector2D(v6) == std::vector<int>({-1,0,-1,2,2,3}));

    // Larger test: many inner vectors with varying sizes
    std::vector<std::vector<int>> v7 = {{1},{2,3},{},{4,5,6},{}};
    assert(flattenVector2D(v7) == std::vector<int>({1,2,3,4,5,6}));

    // Verify input is not modified (const reference ensures this, but we check logically)
    std::vector<std::vector<int>> v8 = {{1,2},{3}};
    auto copy_input = v8;
    (void)flattenVector2D(v8);
    assert(v8 == copy_input); // Unchanged

    return 0;
}

// The solution is straightforward: iterate over each inner vector in the outer vector. For each inner vector, iterate over its elements and append each element to a result vector. This naturally handles empty inner vectors by simply not adding anything for them. The order is preserved because we traverse outer indices in increasing order and inner elements in their natural order. Edge cases include an empty outer vector (returns an empty result) and multiple empty inner vectors (skipped). Duplicate values are preserved. Time complexity is \(O(N)\), where \(N\) is the total number of integers across all inner vectors, because each integer is visited exactly once. Space complexity is \(O(N)\) for the result vector, plus \(O(1)\) auxiliary space for loop counters (ignoring the input's own storage). No special algorithm is needed beyond basic nested loops.
