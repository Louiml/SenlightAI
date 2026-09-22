/*
Write a C++ function named `findKthSmallestInHeap` that takes a `std::vector<int>` representing an array-based min-heap (where the minimum element is at index 0, and for any node at index `i`, its left child is at `2*i+1` and its right child is at `2*i+2`) and an integer `k` (1 ≤ k ≤ heap size), and returns the k-th smallest element in the heap without modifying the heap. You must implement the function by simulating the extraction process: repeatedly remove the smallest element from the heap using the standard heap-delete logic (swap last, sift down, and track the removed element) but without physically altering the original heap — instead, maintain a copy or perform the deletions on a local copy. Clarity, correct handling of edge cases (e.g., k = 1, k = heap size), and efficient time complexity (O(k log n)) are expected.
*/

#include <vector>
#include <algorithm>

// Finds the k-th smallest element in a min-heap stored as a vector (0-indexed).
// k is assumed to be between 1 and heap.size() inclusive.
int findKthSmallestInHeap(const std::vector<int>& heap, int k) {
    std::vector<int> h = heap; // working copy
    int size = static_cast<int>(h.size());

    // Helper lambda for sift-down on the copy, given current size.
    auto siftDown = [&](int index, int currentSize) {
        while (true) {
            int leftChild = 2 * index + 1;
            int rightChild = 2 * index + 2;
            int smallest = index;

            if (leftChild < currentSize && h[leftChild] < h[smallest])
                smallest = leftChild;
            if (rightChild < currentSize && h[rightChild] < h[smallest])
                smallest = rightChild;

            if (smallest == index)
                break;

            std::swap(h[index], h[smallest]);
            index = smallest;
        }
    };

    // Perform k-1 deletions (remove the root each time and reheapify).
    for (int removed = 0; removed < k - 1; ++removed) {
        // Place last element at root
        h[0] = h[size - 1];
        --size; // effectively removing the last element from logical heap
        if (size > 0) {
            siftDown(0, size);
        }
    }

    // After k-1 deletions, the smallest remaining element is the k-th smallest.
    return h[0];
}

#include <cassert>
#include <vector>

int main() {
    // Basic heap: {1, 2, 3, 4, 5, 6, 7}
    std::vector<int> heap1 = {1, 2, 3, 4, 5, 6, 7};
    assert(findKthSmallestInHeap(heap1, 1) == 1);
    assert(findKthSmallestInHeap(heap1, 2) == 2);
    assert(findKthSmallestInHeap(heap1, 4) == 4);
    assert(findKthSmallestInHeap(heap1, 7) == 7);

    // Heap with duplicates: {2, 2, 2, 5, 7}
    std::vector<int> heap2 = {2, 2, 2, 5, 7};
    assert(findKthSmallestInHeap(heap2, 1) == 2);
    assert(findKthSmallestInHeap(heap2, 3) == 2);
    assert(findKthSmallestInHeap(heap2, 4) == 5);
    assert(findKthSmallestInHeap(heap2, 5) == 7);

    // Larger heap: {10, 20, 15, 30, 40, 25, 35}
    std::vector<int> heap3 = {10, 20, 15, 30, 40, 25, 35};
    assert(findKthSmallestInHeap(heap3, 1) == 10);
    assert(findKthSmallestInHeap(heap3, 2) == 15);
    assert(findKthSmallestInHeap(heap3, 3) == 20);
    assert(findKthSmallestInHeap(heap3, 6) == 35);
    assert(findKthSmallestInHeap(heap3, 7) == 40);

    // Heap with negative numbers: {-5, -1, 0, 3, 9}
    std::vector<int> heap4 = {-5, -1, 0, 3, 9};
    assert(findKthSmallestInHeap(heap4, 1) == -5);
    assert(findKthSmallestInHeap(heap4, 3) == 0);
    assert(findKthSmallestInHeap(heap4, 5) == 9);

    // Single-element heap
    std::vector<int> heap5 = {42};
    assert(findKthSmallestInHeap(heap5, 1) == 42);

    return 0;
}

// The goal is to find the k-th smallest element in a min-heap array without changing the original heap. The straightforward approach is to copy the heap into a local vector and then perform k-1 deletions. Each deletion in a min-heap involves: 1) taking the root (the smallest), 2) moving the last element to the root, 3) sifting it down by comparing with children and swapping with the smaller child when necessary. After k-1 deletions, the new root is the k-th smallest (since each deletion removes the current smallest). Edge cases: if k == 1, return the root immediately (no deletions); if k equals heap size, we effectively remove all but one element, and the remaining single element is the maximum, but since we only need the k-th smallest, after k-1 deletions the root is the correct element. The time complexity is O(k log n) because each deletion requires sifting down along the height O(log n). Space complexity is O(n) for the copy. The function must handle indexing correctly: for a node at index `i`, parent is `(i-1)/2`, left child `2*i+1`, right child `2*i+2`. In the heap-delete sift-down, choose the smaller child (if both exist), and stop if the element is smaller than or equal to both children.
