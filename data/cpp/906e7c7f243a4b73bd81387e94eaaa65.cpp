// Implement a C++ function named `kthSmallestInHeap` that takes a `std::vector<int>` representing the internal array of a min-heap (stored in level-order, where for any index `i`, its left child is at `2*i+1` and right child at `2*i+2`), and an integer `k`. The heap is guaranteed to satisfy the min-heap property (each parent ≤ its children). The function should return the `k`-th smallest element (1-indexed, so `k=1` returns the minimum). If `k` is less than 1 or greater than the heap size, return -1. The function must not modify the original heap and must be efficient even for large heaps, ideally using a secondary min-heap to avoid fully sorting the array.

#include <cassert>
#include <vector>

// Function under test is declared here (or included from solution).
int kthSmallestInHeap(const std::vector<int>& heapArray, int k);

int main() {
    // Simple heap: {1, 3, 2, 6, 5, 4} - valid min-heap.
    std::vector<int> heap1 = {1, 3, 2, 6, 5, 4};
    assert(kthSmallestInHeap(heap1, 1) == 1);
    assert(kthSmallestInHeap(heap1, 2) == 2);
    assert(kthSmallestInHeap(heap1, 3) == 3);
    assert(kthSmallestInHeap(heap1, 4) == 4);
    assert(kthSmallestInHeap(heap1, 5) == 5);
    assert(kthSmallestInHeap(heap1, 6) == 6);

    // Duplicate values in heap.
    std::vector<int> heap2 = {2, 2, 3, 2, 4, 3, 5};
    assert(kthSmallestInHeap(heap2, 1) == 2);
    assert(kthSmallestInHeap(heap2, 2) == 2);
    assert(kthSmallestInHeap(heap2, 3) == 2);
    assert(kthSmallestInHeap(heap2, 4) == 3);
    assert(kthSmallestInHeap(heap2, 5) == 3);
    assert(kthSmallestInHeap(heap2, 6) == 4);
    assert(kthSmallestInHeap(heap2, 7) == 5);

    // Single-element heap.
    std::vector<int> heap3 = {42};
    assert(kthSmallestInHeap(heap3, 1) == 42);

    // Invalid k values.
    std::vector<int> heap4 = {5, 9, 7};
    assert(kthSmallestInHeap(heap4, 0) == -1);
    assert(kthSmallestInHeap(heap4, -3) == -1);
    assert(kthSmallestInHeap(heap4, 4) == -1);
    assert(kthSmallestInHeap(heap4, 100) == -1);

    // Larger heap with negative values.
    std::vector<int> heap5 = {-10, -5, -8, -1, -2, -6, -7};
    assert(kthSmallestInHeap(heap5, 1) == -10);
    assert(kthSmallestInHeap(heap5, 2) == -8);
    assert(kthSmallestInHeap(heap5, 3) == -7);
    assert(kthSmallestInHeap(heap5, 4) == -6);
    assert(kthSmallestInHeap(heap5, 5) == -5);
    assert(kthSmallestInHeap(heap5, 6) == -2);
    assert(kthSmallestInHeap(heap5, 7) == -1);

    return 0;
}

#include <vector>
#include <queue>
#include <utility>

// Returns the k-th smallest element in a min-heap represented as a vector.
// k is 1-indexed. Returns -1 if k is out of range.
int kthSmallestInHeap(const std::vector<int>& heapArray, int k) {
    int n = static_cast<int>(heapArray.size());
    if (k < 1 || k > n) {
        return -1;
    }
    
    // Min-heap of {value, index} to traverse the heap efficiently.
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> minHeap;
    minHeap.push({heapArray[0], 0});
    
    int result = -1;
    for (int count = 0; count < k; ++count) {
        auto [value, index] = minHeap.top();
        minHeap.pop();
        result = value;
        
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        
        if (leftChild < n) {
            minHeap.push({heapArray[leftChild], leftChild});
        }
        if (rightChild < n) {
            minHeap.push({heapArray[rightChild], rightChild});
        }
    }
    
    return result;
}

// The key insight is that in a min-heap, the smallest element is always at index 0. To find the `k`-th smallest without sorting the entire array, we use a priority queue (min-heap) of pairs `(value, index)`. Starting with the root (index 0), we repeatedly extract the smallest element from this secondary heap. Each extraction gives the next smallest element in order. When we extract a node at index `i`, we push its left child (`2*i+1`) and right child (`2*i+2`) into the secondary heap if they exist (i.e., their indices are within the vector size). After `k` extractions, the value obtained is the `k`-th smallest. Edge cases: if `k` is invalid (≤0 or > heap size), return -1. Duplicate values are handled naturally since indices are unique. Time complexity is `O(k log k)` because we perform `k` extractions, each costing `O(log k)` for heap operations on the secondary heap of size at most `k`. Space complexity is `O(k)` for the secondary heap. This is significantly better than sorting the whole array, which would be `O(n log n)`.
