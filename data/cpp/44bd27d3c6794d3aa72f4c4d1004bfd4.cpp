Write a C++ function `sortAscending` that takes a vector of integers as input and returns a new vector containing the same integers sorted in ascending order. The function must use a `std::priority_queue` internally (specifically by pushing the negation of each number so the default max-heap becomes a min-heap), and it must not modify the input vector (pass it by `const` reference). Handle empty vectors gracefully by returning an empty vector. The result must be a vector of integers placed in non-decreasing order. Include proper `#include` directives and use `const` correctness.
#include <cassert>
#include <vector>

// Forward declaration of the solution function (assumed defined in the same translation unit)
std::vector<int> sortAscending(const std::vector<int>& input);

int main() {
    // Basic ascending order
    assert(sortAscending({5, 2, 9, 1, 5, 6}) == std::vector<int>({1, 2, 5, 5, 6, 9}));
    // Already sorted
    assert(sortAscending({-3, -1, 0, 2, 10}) == std::vector<int>({-3, -1, 0, 2, 10}));
    // Reverse order
    assert(sortAscending({7, 4, 2, 1}) == std::vector<int>({1, 2, 4, 7}));
    // Single element
    assert(sortAscending({42}) == std::vector<int>({42}));
    // Empty input
    assert(sortAscending({}) == std::vector<int>({}));
    // All negative numbers
    assert(sortAscending({-2, -10, -5, -1}) == std::vector<int>({-10, -5, -2, -1}));
    // Duplicates and zeros
    assert(sortAscending({0, 0, -1, -1, 3}) == std::vector<int>({-1, -1, 0, 0, 3}));
    return 0;
}
#include <vector>
#include <queue>

// Return a new vector containing the input integers in ascending order.
// Uses a min-heap implemented via std::priority_queue by pushing negated values.
std::vector<int> sortAscending(const std::vector<int>& input) {
    std::priority_queue<int> minHeap; // default max-heap; we store negations to simulate min-heap

    for (int value : input) {
        minHeap.push(-value);
    }

    std::vector<int> sorted;
    sorted.reserve(input.size());

    while (!minHeap.empty()) {
        sorted.push_back(-minHeap.top());
        minHeap.pop();
    }

    return sorted;
}
// The task requires sorting a vector using a priority queue, which is a heap-based data structure. The default `std::priority_queue<int>` is a max-heap, meaning the largest element is at the top. To sort in ascending order, we can push `-value` into the heap; then the smallest original value becomes the largest negative number and thus the top. After pushing all elements, we repeatedly pop the top, negate it, and append it to the result vector. This yields the original numbers in ascending order. Edge cases: an empty input vector should result in an empty output; a vector with duplicate values is handled naturally since the heap preserves duplicates; negative numbers work correctly because negating them produces positive values. Time complexity is O(n log n) due to `n` push operations and `n` pop operations, each O(log n). Space complexity is O(n) for the heap plus O(n) for the output vector (which is expected for the result). The function does not modify the input because it takes a `const` reference.
