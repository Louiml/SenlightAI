/*
Write a standalone C++ function `buildMaxHeap` that takes a `std::vector<int>&` and transforms it in-place into a valid max-heap using the bottom-up approach (starting from the last non-leaf node and applying a `siftDown` operation). The function must also include a helper `siftDown` (either as a private free function or a lambda within `buildMaxHeap`) that restores the heap property for a given index by repeatedly swapping the node with its larger child until both children are smaller or the node is a leaf. The original order of the input is not preserved; the only requirement is that the resulting array satisfies the max-heap property (i.e., for every index `i`, `heap[i] >= heap[2*i+1]` and `heap[i] >= heap[2*i+2]`, where children exist). The function should handle edge cases such as an empty vector, a vector with a single element, and vectors with duplicate values. Do not use any standard heap functions (e.g., `std::make_heap`); implement the logic manually.
*/

#include <vector>
#include <algorithm> // for std::swap

// Helper function: restores max-heap property at index i, assuming children subtrees are valid heaps.
void siftDown(std::vector<int>& heap, int i, int size) {
    while (true) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int largest = i;

        if (left < size && heap[left] > heap[largest]) {
            largest = left;
        }
        if (right < size && heap[right] > heap[largest]) {
            largest = right;
        }

        if (largest == i) {
            break;
        }

        std::swap(heap[i], heap[largest]);
        i = largest;
    }
}

// Transforms the vector into a max-heap in-place using bottom-up approach.
void buildMaxHeap(std::vector<int>& heap) {
    int size = static_cast<int>(heap.size());
    // Start from last non-leaf node and move towards root.
    for (int i = size / 2 - 1; i >= 0; --i) {
        siftDown(heap, i, size);
    }
}

#include <cassert>
#include <vector>

// The buildMaxHeap and siftDown functions are assumed to be declared above.

bool isMaxHeap(const std::vector<int>& heap) {
    int n = static_cast<int>(heap.size());
    for (int i = 0; i < n; ++i) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < n && heap[i] < heap[left]) return false;
        if (right < n && heap[i] < heap[right]) return false;
    }
    return true;
}

int main() {
    // Test 1: Empty vector
    std::vector<int> empty;
    buildMaxHeap(empty);
    assert(empty.empty());

    // Test 2: Single element
    std::vector<int> single = {42};
    buildMaxHeap(single);
    assert(single[0] == 42 && isMaxHeap(single));

    // Test 3: Already a max-heap
    std::vector<int> already = {10, 5, 7, 3, 1, 6, 2};
    buildMaxHeap(already);
    assert(isMaxHeap(already));

    // Test 4: Random permutation
    std::vector<int> random = {3, 1, 4, 1, 5, 9, 2, 6};
    buildMaxHeap(random);
    assert(isMaxHeap(random));
    // Largest element must be at root
    assert(random[0] == 9);

    // Test 5: All equal elements
    std::vector<int> equal = {7, 7, 7, 7};
    buildMaxHeap(equal);
    assert(isMaxHeap(equal));

    // Test 6: Reverse sorted (worst case)
    std::vector<int> descending = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    buildMaxHeap(descending);
    assert(isMaxHeap(descending));

    // Test 7: Ascending order (another worst case)
    std::vector<int> ascending = {1, 2, 3, 4, 5, 6, 7, 8};
    buildMaxHeap(ascending);
    assert(isMaxHeap(ascending));
    assert(ascending[0] == 8);
    return 0;
}

// The solution uses the classic bottom-up heap construction: starting from the last index that has at least one child (`size/2 - 1` for zero-based indexing) and moving down to index 0, we call `siftDown` on each index. The `siftDown` operation compares the current node with its left and right children (if they exist), finds the largest among them, and if that largest child is greater than the current node, swaps them and repeats the process from the child's index. This continues until the node reaches a position where it is larger than both children or becomes a leaf. The key edge cases: an empty vector or a single-element vector require no changes (the loop condition `for (int i = size/2 - 1; i >= 0; --i)` naturally handles empty because `size/2 - 1` is negative and the loop doesn't execute; for size 1, `size/2 - 1` is -1, also no execution). Duplicate values are handled naturally because we use strict greater-than comparisons (`if (largest != i && heap[largest] > heap[i])`) — if children are equal to the parent, no swap occurs, preserving a valid heap. The time complexity is O(n) for the entire `buildMaxHeap` (amortized, because siftDown operations from lower levels are cheaper), and O(log n) for a single `siftDown`. Space complexity is O(1) auxiliary (excluding the input vector) since all operations are in-place.
