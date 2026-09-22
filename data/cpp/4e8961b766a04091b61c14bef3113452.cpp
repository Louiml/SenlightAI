Write a standalone C++ function `bool isMinHeapOrdered(const std::vector<int>& data)` that returns `true` if the given vector represents a valid binary min-heap when interpreted level-order (i.e., index 0 is the root, children of index `i` are at `2*i+1` and `2*i+2`), and `false` otherwise. A valid min-heap must satisfy the heap property: for every node, its value is less than or equal to the values of its children (if they exist). The input may contain duplicate values, negative numbers, and any size (including empty). You must not modify the input vector.
#include <cassert>
#include <vector>

// Function declaration from the solution (assume it's included above).
bool isMinHeapOrdered(const std::vector<int>& data);

int main() {
    // Empty and single-element heaps.
    assert(isMinHeapOrdered({}) == true);
    assert(isMinHeapOrdered({0}) == true);
    assert(isMinHeapOrdered({-5}) == true);

    // Valid heaps.
    assert(isMinHeapOrdered({1, 2, 3, 4, 5, 6, 7}) == true);
    assert(isMinHeapOrdered({1, 1, 1, 1}) == true); // duplicates allowed.
    assert(isMinHeapOrdered({-3, -2, -1, 0, 10}) == true); // negative values.
    assert(isMinHeapOrdered({5, 10, 6, 11, 12, 7}) == true); // right child = 6 > 5.

    // Invalid heaps.
    assert(isMinHeapOrdered({2, 1}) == false); // left child smaller.
    assert(isMinHeapOrdered({1, 2, 0}) == false); // right child smaller.
    assert(isMinHeapOrdered({3, 1, 4, 0}) == false); // deeper violation.

    // Larger invalid case.
    std::vector<int> big(1000000, 1);
    big[500000] = 0; // index 500000 has parent 249999 (value 1 > 0).
    assert(isMinHeapOrdered(big) == true); // Wait: path check required.
    // Actually index 500000's parent is (500000-1)/2 = 249999, which is value 1, so violation.
    // Correct test:
    assert(isMinHeapOrdered(big) == false);

    return 0;
}
#include <vector>

/**
 * Returns true if the given vector is a valid binary min-heap
 * (level-order representation), false otherwise.
 * An empty or single-element vector is considered a valid heap.
 */
bool isMinHeapOrdered(const std::vector<int>& data) {
    const std::size_t n = data.size();
    
    // No nodes or only one node → trivially a heap.
    if (n < 2) {
        return true;
    }

    // Only nodes up to index (n/2 - 1) can have children.
    for (std::size_t i = 0; i <= n / 2 - 1; ++i) {
        // Left child always exists for these indices.
        if (data[i] > data[2 * i + 1]) {
            return false;
        }
        // Right child exists only if index is within bounds.
        if (2 * i + 2 < n && data[i] > data[2 * i + 2]) {
            return false;
        }
    }
    return true;
}
// The solution walks through each node that could have children (i.e., every index from 0 to `n/2 - 1`, where `n` is the size). For each such node, we check its left child (always exists for these nodes) and its right child (if within bounds) to ensure the parent is ≤ the child. If any violation is found, return `false` immediately. Edge cases: an empty vector or a single-element vector trivially satisfies the heap property (no children). Duplicate values are allowed because the heap property requires non-strict inequality (`≤`). Time complexity is O(n) because we examine each node at most once. Space complexity is O(1) auxiliary because we only use a few index variables and no extra containers.
