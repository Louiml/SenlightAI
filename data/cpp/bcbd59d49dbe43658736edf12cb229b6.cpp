/*
Implement a C++ function named `buildHeapFromVector` that takes a `std::vector<int>` and a `bool` parameter `useMinHeap` (default `true`). The function should return a new `std::vector<int>` that represents the heap in array form (where for a node at index `i`, its left child is at `2*i+1` and right child at `2*i+2`) after applying the standard heapify-up (or heapify-down) operations to satisfy the heap property. If `useMinHeap` is true, the root must be the smallest element; otherwise, it must be the largest. The function must not modify the input vector, must not use any external heap library (like `std::priority_queue` or `std::make_heap`), and must return a copy of the input rearranged in heap order. The heap must be built using the "heapify-down" (sift-down) approach starting from the last non-leaf node, which is more efficient than repeated insertions.
*/
#include <vector>
#include <algorithm>

// Builds a binary heap in array form from the given vector.
// If useMinHeap is true, produces a min-heap; otherwise a max-heap.
std::vector<int> buildHeapFromVector(const std::vector<int>& input, bool useMinHeap = true) {
    if (input.empty()) {
        return {};
    }

    std::vector<int> heap = input; // copy input to modify
    int n = static_cast<int>(heap.size());

    // Comparison lambda: for min-heap, parent < child is desired; for max-heap, parent > child.
    auto shouldSwap = [&](int parent, int child) {
        if (useMinHeap) {
            return heap[parent] > heap[child]; // parent larger than child -> swap
        } else {
            return heap[parent] < heap[child]; // parent smaller than child -> swap
        }
    };

    // Sift-down starting from last non-leaf node down to root.
    for (int i = n / 2 - 1; i >= 0; --i) {
        int parent = i;
        while (true) {
            int left = 2 * parent + 1;
            int right = 2 * parent + 2;
            int swapWith = parent;

            if (left < n && shouldSwap(parent, left)) {
                swapWith = left;
            }
            if (right < n && shouldSwap(swapWith, right)) {  // careful: when comparing with right, we compare with the candidate
                // If right is better than swapWith, choose right.
                bool rightBetter = useMinHeap ? heap[swapWith] > heap[right] : heap[swapWith] < heap[right];
                if (rightBetter) {
                    swapWith = right;
                }
            }

            if (swapWith == parent) {
                break;
            }
            std::swap(heap[parent], heap[swapWith]);
            parent = swapWith;
        }
    }

    return heap;
}
#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Test 1: empty input
    std::vector<int> empty;
    assert(buildHeapFromVector(empty).empty());

    // Test 2: single element
    std::vector<int> single = {5};
    assert(buildHeapFromVector(single) == single);

    // Test 3: min-heap basic (root smallest)
    std::vector<int> input1 = {4, 10, 3, 5, 1};
    auto minHeap = buildHeapFromVector(input1, true);
    assert(minHeap[0] == 1);
    // Check heap property for all nodes
    for (size_t i = 0; i < minHeap.size(); ++i) {
        size_t left = 2*i + 1, right = 2*i + 2;
        if (left < minHeap.size()) assert(minHeap[i] <= minHeap[left]);
        if (right < minHeap.size()) assert(minHeap[i] <= minHeap[right]);
    }
    // Original input unchanged
    assert(input1 == std::vector<int>({4, 10, 3, 5, 1}));

    // Test 4: max-heap basic (root largest)
    auto maxHeap = buildHeapFromVector(input1, false);
    assert(maxHeap[0] == 10);
    for (size_t i = 0; i < maxHeap.size(); ++i) {
        size_t left = 2*i + 1, right = 2*i + 2;
        if (left < maxHeap.size()) assert(maxHeap[i] >= maxHeap[left]);
        if (right < maxHeap.size()) assert(maxHeap[i] >= maxHeap[right]);
    }

    // Test 5: duplicates
    std::vector<int> dup = {7, 7, 7, 7};
    auto dupHeap = buildHeapFromVector(dup, true);
    assert(dupHeap == dup); // all same, any order works

    // Test 6: already sorted ascending becomes valid min-heap? Not necessarily but must satisfy heap property.
    std::vector<int> sorted = {1, 2, 3, 4, 5};
    auto sortedMin = buildHeapFromVector(sorted, true);
    assert(sortedMin[0] == 1);
    for (size_t i = 0; i < sortedMin.size(); ++i) {
        size_t left = 2*i + 1, right = 2*i + 2;
        if (left < sortedMin.size()) assert(sortedMin[i] <= sortedMin[left]);
        if (right < sortedMin.size()) assert(sortedMin[i] <= sortedMin[right]);
    }

    // Test 7: large random check using std::is_heap? Not allowed but we do manual check for 100 elements.
    std::vector<int> big(100);
    for (int i = 0; i < 100; ++i) big[i] = (i * 37) % 101; // pseudo-random
    auto bigMin = buildHeapFromVector(big, true);
    for (size_t i = 0; i < bigMin.size(); ++i) {
        size_t left = 2*i + 1, right = 2*i + 2;
        if (left < bigMin.size()) assert(bigMin[i] <= bigMin[left]);
        if (right < bigMin.size()) assert(bigMin[i] <= bigMin[right]);
    }
    // Also verify that the multiset of elements is preserved
    auto sortedBig = big; std::sort(sortedBig.begin(), sortedBig.end());
    auto sortedHeap = bigMin; std::sort(sortedHeap.begin(), sortedHeap.end());
    assert(sortedBig == sortedHeap);

    return 0;
}
// The solution must implement a binary heap using a contiguous array. The key algorithm is the "sift-down" (also called heapify-down) operation: for a given index, compare the node with its children, and if the heap property is violated, swap with the appropriate child and recurse downward. To build the heap in O(n) time, we start from the last non-leaf node (at index `n/2 - 1` for 0-based indexing) and apply sift-down up to the root. This works because all leaves already satisfy the heap property trivially. Edge cases: empty vector (return empty vector), single element (no changes), duplicate values (comparison must use strict inequality to decide swaps, but duplicates are fine because any order among equal elements is acceptable). Time complexity is O(n) for the build, and in the worst case each sift-down is O(log n), but the sum of all operations is O(n). Auxiliary space is O(1) excluding the returned vector (we modify a local copy). The function must be `const`-correct? The input is passed by const reference, and the returned vector is a copy.
