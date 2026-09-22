// Write a C++ function named `kthSmallestInHeapArray` that takes a 1-indexed array representing a max-heap (i.e., `arr[1]` is the root, each parent is greater than or equal to its children), the heap size `n`, and an integer `k` (where `1 ≤ k ≤ n`), and returns the k-th smallest element in the heap efficiently. The function must not modify the original heap and must not build a new heap. For example, given the max-heap array `[-1, 60, 50, 55, 20, 30]` with `n=5`, the elements are `{60,50,55,20,30}` and sorted they are `{20,30,50,55,60}`, so `k=1` returns 20, `k=3` returns 50. Your solution must run in `O(k log k)` time and `O(k)` space, leveraging the heap property by exploring only the smallest candidates.

A standard max-heap stores the largest element at the root, so the smallest elements are not directly accessible. However, we can use a min-heap (priority queue) to simulate a breadth-first search over the heap structure, starting from the root. Since each node's children are larger than the node itself (in a max-heap), the smallest unvisited node is always among the currently “frontier” nodes. We push the root into a min-heap (with its index), then repeatedly extract the minimum node. Each extracted node corresponds to the next smallest element overall. After extraction, we push its left child (if it exists) and right child (if it exists) into the frontier min-heap, because those children are the only new candidates that could be the next smallest. Repeating this extraction exactly `k` times yields the k-th smallest. Edge cases: if `k` equals 1, we simply return the root; if `k` is larger than the heap size, the problem is invalid but we assume `k ≤ n`. The algorithm explores at most `k` nodes, each push/pop is `O(log k)`, so time is `O(k log k)` and space is `O(k)` for the frontier heap. The original array is never modified.

#include <queue>
#include <vector>
#include <utility>

// Given a 1-indexed array `arr` representing a max-heap of size `n`,
// return the k-th smallest element (1 ≤ k ≤ n) without modifying the heap.
// Uses a min-heap frontier to explore candidates in non-decreasing order.
long long kthSmallestInHeapArray(const std::vector<long long>& arr, int n, int k) {
    // Min-heap of pairs (value, index) ordered by value.
    std::priority_queue<std::pair<long long, int>,
                        std::vector<std::pair<long long, int>>,
                        std::greater<std::pair<long long, int>>> frontier;

    // Start with the root (index 1). arr is 1-indexed, so ensure size > 1.
    frontier.push({arr[1], 1});

    long long result = -1;

    // Extract the smallest frontier element exactly k times.
    for (int count = 0; count < k; ++count) {
        auto [value, index] = frontier.top();
        frontier.pop();
        result = value;

        int left = 2 * index;
        int right = 2 * index + 1;

        if (left <= n) {
            frontier.push({arr[left], left});
        }
        if (right <= n) {
            frontier.push({arr[right], right});
        }
    }

    return result;
}

#include <cassert>
#include <vector>

// Declaration of the solution function (already defined above).
long long kthSmallestInHeapArray(const std::vector<long long>& arr, int n, int k);

int main() {
    // Example from the snippet: arr = {-1,60,50,55,20,30}, n=5
    // Sorted elements: {20,30,50,55,60}
    std::vector<long long> heap1 = {-1, 60, 50, 55, 20, 30};
    assert(kthSmallestInHeapArray(heap1, 5, 1) == 20);
    assert(kthSmallestInHeapArray(heap1, 5, 2) == 30);
    assert(kthSmallestInHeapArray(heap1, 5, 3) == 50);
    assert(kthSmallestInHeapArray(heap1, 5, 4) == 55);
    assert(kthSmallestInHeapArray(heap1, 5, 5) == 60);

    // Single element heap
    std::vector<long long> heap2 = {-1, 42};
    assert(kthSmallestInHeapArray(heap2, 1, 1) == 42);

    // Larger heap, k=1 (smallest is the smallest leaf or internal node)
    // arr: {-1, 100, 90, 80, 70, 60, 50, 40, 30, 20, 10}
    // n=10, sorted: {10,20,30,40,50,60,70,80,90,100}
    std::vector<long long> heap3 = {-1, 100, 90, 80, 70, 60, 50, 40, 30, 20, 10};
    assert(kthSmallestInHeapArray(heap3, 10, 1) == 10);
    assert(kthSmallestInHeapArray(heap3, 10, 5) == 50);
    assert(kthSmallestInHeapArray(heap3, 10, 10) == 100);

    // Heap where the smallest is near the root
    // arr: {-1, 5, 4, 3, 2, 1} but this is NOT a valid max-heap, so use valid one:
    // For valid max-heap: {-1, 100, 50, 60, 20, 30} sorted: {20,30,50,60,100}
    std::vector<long long> heap4 = {-1, 100, 50, 60, 20, 30};
    assert(kthSmallestInHeapArray(heap4, 5, 1) == 20);
    assert(kthSmallestInHeapArray(heap4, 5, 3) == 50);

    // Test when k equals n
    std::vector<long long> heap5 = {-1, 10, 9, 8, 7, 6, 5}; // n=6
    // Sorted: {5,6,7,8,9,10}
    assert(kthSmallestInHeapArray(heap5, 6, 6) == 10);

    return 0;
}
