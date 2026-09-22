// Given a binary heap stored as a `std::vector<int>` where the heap property holds (for a max-heap, parent ≥ children), write a C++ function `bool isMaxHeap(const std::vector<int>& heap)` that returns `true` if the given vector represents a valid max-heap and `false` otherwise. The function must handle empty vectors (treated as valid) and vectors of size 1 (valid). Additionally, write a second function `void heapifyDown(std::vector<int>& heap, int index)` that performs the standard "sift-down" operation on the element at the given index to restore the max-heap property, assuming all other elements already satisfy the heap property. The task combines both correctness checking and a mutating operation, requiring clear understanding of heap indexing (0-based, children at `2*i+1` and `2*i+2`).
#include <cassert>
#include <vector>

// (Assume the two functions are declared above or included. For test, we just call them.)

int main() {
    // isMaxHeap tests
    assert(isMaxHeap({}) == true);
    assert(isMaxHeap({5}) == true);
    assert(isMaxHeap({10, 8, 7, 5, 6, 3}) == true); // valid max-heap
    assert(isMaxHeap({10, 8, 7, 5, 6, 8}) == true); // duplicates allowed
    assert(isMaxHeap({10, 12, 7}) == false);        // left child violates
    assert(isMaxHeap({10, 5, 12}) == false);        // right child violates
    assert(isMaxHeap({10, 8, 7, 9, 6, 3}) == false); // deeper violation

    // heapifyDown tests
    std::vector<int> h1 = {3, 8, 7, 5, 6, 2};
    heapifyDown(h1, 0);
    assert(isMaxHeap(h1) == true);
    assert(h1[0] == 8); // largest promoted to root

    std::vector<int> h2 = {10, 1, 7, 5, 6, 2};
    heapifyDown(h2, 1);
    // After sifting down at index 1 (value 1), it swaps with 6.
    assert(isMaxHeap(h2) == true);

    std::vector<int> h3 = {10, 8, 7};
    heapifyDown(h3, 2); // leaf index, no change
    assert(h3 == (std::vector<int>{10, 8, 7}));

    std::vector<int> h4 = {10, 8, 7, 5, 6, 2};
    heapifyDown(h4, 0); // already correct, no change
    assert(h4 == (std::vector<int>{10, 8, 7, 5, 6, 2}));

    return 0;
}
#include <vector>
#include <algorithm> // for std::max

// Returns true if the vector represents a valid max-heap (parent >= children).
bool isMaxHeap(const std::vector<int>& heap) {
    int n = static_cast<int>(heap.size());
    // Empty or single-element heap is trivially valid.
    if (n <= 1) return true;
    // Check all internal nodes from last non-leaf up to root.
    for (int i = n / 2 - 1; i >= 0; --i) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < n && heap[i] < heap[left]) return false;
        if (right < n && heap[i] < heap[right]) return false;
    }
    return true;
}

// Restores max-heap property at the given index by sifting down.
void heapifyDown(std::vector<int>& heap, int index) {
    int n = static_cast<int>(heap.size());
    if (index < 0 || index >= n) return; // out-of-bounds guard

    while (true) {
        int largest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < n && heap[left] > heap[largest]) largest = left;
        if (right < n && heap[right] > heap[largest]) largest = right;

        if (largest == index) break; // already satisfies heap property

        std::swap(heap[index], heap[largest]);
        index = largest; // continue downward
    }
}
// For `isMaxHeap`, iterate from the last non-leaf node down to the root (index `size/2 - 1` to 0). For each index `i`, check that if a left child exists, `heap[i] >= heap[left]`; if a right child exists, also `heap[i] >= heap[right]`. If any violation occurs, return `false`. Empty and single-element vectors trivially satisfy the condition. Complexity: O(n) time, O(1) space. For `heapifyDown`, given an index, repeatedly compare the element with its children; if a child is larger, swap with the largest child and move down to that child’s index; otherwise stop. This assumes the subtree rooted at the children are valid max-heaps. Complexity: O(log n) time, O(1) space. Edge cases include index out of bounds (function does nothing), leaf nodes (immediate stop), and duplicate values (inequality is ≥, so duplicates are fine).
