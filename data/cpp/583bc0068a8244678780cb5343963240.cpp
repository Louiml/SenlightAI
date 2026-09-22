// Write a C++ function `buildHeapFromVector` that takes a `std::vector<int>` (with possible duplicate values) and an integer `target`, and returns the index of the first occurrence of `target` after converting the vector into a max-heap using the same insertion technique as the provided snippet (insert elements one by one into an initially empty heap). The heap should be stored in a flat `std::vector<int>` where the root is at index 0, and the standard parent/child indexing (`parent = (i-1)/2`, `left = 2*i+1`, `right = 2*i+2`) applies. After building the heap, scan it in order from index 0 to the end and return the first index where the value equals `target`. If `target` does not exist in the heap, return `-1`. The function must not modify the original vector. Assume the input vector may be empty, in which case return `-1`. Also, if `target` appears multiple times, the smallest index among all duplicates must be returned.

The solution simulates a max-heap built by sequential insertion. For each element from the input vector, we append it to the back of a result vector, then perform the "bubble up" operation: while the current node has a parent (index > 0) and the current node's value is greater than its parent's value, swap them and move the current index to the parent. This maintains the max-heap property after each insertion. Since the heap is stored as a flat vector, we can directly use arithmetic indexing. After processing all elements, we perform a linear scan from index 0 to `size-1`, returning the first index where the value equals `target`. Edge cases: empty input returns `-1`; duplicates are handled naturally because the scan checks from the beginning; for a vector with one element, no swaps occur. Time complexity is O(n log n) for building (each insertion is O(log n) in the worst case, and there are n insertions), plus O(n) for the scan, so overall O(n log n). Space complexity is O(n) for the returned heap vector, plus O(1) auxiliary space (excluding the output vector).

#include <vector>
#include <algorithm>

// Build a max-heap from a vector using sequential insertion, then find the
// first index of 'target'. Return -1 if not found or input is empty.
int buildHeapFromVector(const std::vector<int>& values, int target) {
    if (values.empty()) {
        return -1;
    }

    std::vector<int> heap;
    heap.reserve(values.size());

    for (int value : values) {
        // Insert at the end
        heap.push_back(value);
        int current = static_cast<int>(heap.size()) - 1;

        // Bubble up to maintain max-heap property
        while (current > 0) {
            int parent = (current - 1) / 2;
            if (heap[current] > heap[parent]) {
                std::swap(heap[current], heap[parent]);
                current = parent;
            } else {
                break;
            }
        }
    }

    // Find first occurrence of target
    for (std::size_t i = 0; i < heap.size(); ++i) {
        if (heap[i] == target) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

#include <cassert>
#include <vector>

// Forward declaration (solution function is defined elsewhere)
int buildHeapFromVector(const std::vector<int>& values, int target);

int main() {
    // Basic insertion order example from the snippet (0..9)
    std::vector<int> v1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    // After building max-heap: root 9, first occurrence of 9 is index 0
    assert(buildHeapFromVector(v1, 9) == 0);
    // 8 should be somewhere; let's verify it is present (not -1)
    assert(buildHeapFromVector(v1, 8) != -1);

    // Empty input
    std::vector<int> v2;
    assert(buildHeapFromVector(v2, 5) == -1);

    // Single element
    std::vector<int> v3 = {42};
    assert(buildHeapFromVector(v3, 42) == 0);
    assert(buildHeapFromVector(v3, 10) == -1);

    // Duplicates: input [7, 7, 7] -> heap remains [7,7,7], first index 0
    std::vector<int> v4 = {7, 7, 7};
    assert(buildHeapFromVector(v4, 7) == 0);

    // Negative numbers and duplicates mixed
    std::vector<int> v5 = {-3, 5, -3, 2, 5, 10};
    // Heap built via insertion (10 is largest, so root at 0)
    assert(buildHeapFromVector(v5, 10) == 0);
    assert(buildHeapFromVector(v5, -3) != -1);

    // Large element moves to root; verify target largest at index 0
    std::vector<int> v6 = {1, 2, 3, 4, 5};
    assert(buildHeapFromVector(v6, 5) == 0);

    // Value not in heap
    std::vector<int> v7 = {1, 2, 3};
    assert(buildHeapFromVector(v7, 0) == -1);

    return 0;
}
