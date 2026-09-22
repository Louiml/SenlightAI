// Write a standalone C++ function `std::vector<int> heapSort(const std::vector<int>& input)` that sorts a vector of integers in **ascending order** using a binary heap-based priority queue. The function must implement the heap operations from scratch (without `std::priority_queue` or `std::make_heap`), following the logic of the provided code snippet: store elements into a min-heap, then repeatedly extract the root (minimum) and place it into the result array. The heap must be implemented with 1-based indexing, dynamic resizing via a simple `std::vector<int>` (with index 0 unused or used as a sentinel), and support the core heapify-up (for insertion) and heapify-down (for extraction) operations. Your solution should handle empty input, duplicate values, negative numbers, and large inputs efficiently. The function should return a new sorted vector without modifying the original input.
The solution implements a classic binary min-heap using an underlying `std::vector<int>` where index 0 is unused (or a dummy) to simplify parent/child arithmetic: for a node at index `i`, its left child is at `2*i` and right child at `2*i+1`; parent is at `i/2`. The algorithm proceeds in two phases:

1. **Build phase (heapify-up on insert):** For each element in the input vector, append it to the end of the heap (incrementing size), then "bubble up" the new element by swapping with its parent as long as the parent’s value is greater (for a min-heap). This ensures the smallest element ends up at the root. This phase runs in \(O(\log n)\) per insertion, so \(O(n \log n)\) total.

2. **Extract phase (heapify-down):** Repeatedly take the root (the minimum), store it in the result, then replace the root with the last element, decrease the heap size, and "bubble down" the new root by swapping with the smaller of its two children (if any) until the heap property is restored. This also runs in \(O(\log n)\) per extraction, giving \(O(n \log n)\) total.

Edge cases: empty input returns an empty vector; a single element returns that element; duplicates are handled naturally because the heap property allows equal values. Negative numbers work because comparison is based on `<`. Time complexity is \(O(n \log n)\), and auxiliary space is \(O(n)\) for the heap storage (plus the output vector of size `n`). The solution uses `const` correctness where appropriate, passing the input by const reference and returning by value. The heap is implemented with explicit functions `heapifyUp` and `heapifyDown` that take the heap vector and an index, and a `heapifyUp` helper for insertion. To avoid code bloat, the heap operations are local static functions inside the main solution function, but they could also be written as a separate class for clarity.
#include <vector>
#include <cstddef>
#include <algorithm>

// Helper: swap the values at two indices in a vector.
static void swapValues(std::vector<int>& heap, int i, int j) {
    int temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

// Restore min-heap property by moving element at index 'idx' upward.
static void heapifyUp(std::vector<int>& heap, int idx) {
    while (idx > 1) {
        int parentIdx = idx / 2;
        if (heap[parentIdx] > heap[idx]) {
            swapValues(heap, parentIdx, idx);
            idx = parentIdx;
        } else {
            break;
        }
    }
}

// Restore min-heap property by moving element at index 'idx' downward.
static void heapifyDown(std::vector<int>& heap, int size, int idx) {
    while (true) {
        int left = 2 * idx;
        int right = 2 * idx + 1;
        int smallest = idx;
        if (left <= size && heap[left] < heap[smallest]) {
            smallest = left;
        }
        if (right <= size && heap[right] < heap[smallest]) {
            smallest = right;
        }
        if (smallest != idx) {
            swapValues(heap, idx, smallest);
            idx = smallest;
        } else {
            break;
        }
    }
}

// Sort a vector of integers in ascending order using a min-heap.
std::vector<int> heapSort(const std::vector<int>& input) {
    std::vector<int> result;
    result.reserve(input.size());
    if (input.empty()) {
        return result;
    }

    // Build a 1-indexed min-heap: index 0 unused (dummy).
    std::vector<int> heap;
    heap.reserve(input.size() + 1);
    heap.push_back(0); // dummy at index 0
    int heapSize = 0;
    for (int value : input) {
        ++heapSize;
        heap.push_back(value);
        heapifyUp(heap, heapSize);
    }

    // Extract the minimum repeatedly.
    for (int i = 0; i < static_cast<int>(input.size()); ++i) {
        result.push_back(heap[1]); // root is the minimum
        // Replace root with last element and shrink the heap.
        heap[1] = heap[heapSize];
        --heapSize;
        if (heapSize > 1) {
            heapifyDown(heap, heapSize, 1);
        }
    }
    return result;
}
#include <cassert>
#include <vector>

// The solution function is declared above; no need to redefine here.

int main() {
    // Basic ascending sort.
    std::vector<int> v1 = {5, 3, 8, 1, 9, 2};
    std::vector<int> s1 = heapSort(v1);
    std::vector<int> expected1 = {1, 2, 3, 5, 8, 9};
    assert(s1 == expected1);

    // Already sorted.
    std::vector<int> v2 = {1, 2, 3, 4, 5};
    assert(heapSort(v2) == v2);

    // Reverse order.
    std::vector<int> v3 = {5, 4, 3, 2, 1};
    std::vector<int> expected3 = {1, 2, 3, 4, 5};
    assert(heapSort(v3) == expected3);

    // Duplicates.
    std::vector<int> v4 = {3, 1, 3, 2, 1};
    std::vector<int> expected4 = {1, 1, 2, 3, 3};
    assert(heapSort(v4) == expected4);

    // Negative numbers.
    std::vector<int> v5 = {-5, -1, -10, 0, 7};
    std::vector<int> expected5 = {-10, -5, -1, 0, 7};
    assert(heapSort(v5) == expected5);

    // Single element.
    std::vector<int> v6 = {42};
    assert(heapSort(v6) == v6);

    // Empty vector.
    std::vector<int> v7;
    assert(heapSort(v7) == v7);

    // Large input with many duplicates.
    std::vector<int> v8;
    for (int i = 0; i < 100; ++i) {
        v8.push_back(100 - i);
        v8.push_back(i);
    }
    std::vector<int> expected8 = v8;
    std::sort(expected8.begin(), expected8.end());
    assert(heapSort(v8) == expected8);

    return 0;
}
