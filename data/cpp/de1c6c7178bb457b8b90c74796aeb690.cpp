Write a C++ function that takes a vector of integers as input and returns a new vector containing the elements sorted in ascending order using an incremental heap-insertion algorithm. The function must insert elements one by one into a min-heap represented internally as a vector, maintaining the heap property after each insertion. The input vector may be empty, may contain duplicate values, and may contain negative numbers. The function should return a sorted copy of the input vector without modifying the original vector. The algorithm must explicitly implement the "bubble-up" (or "sift-up") operation for each insertion, rather than using standard library heap functions.

The core idea is to simulate building a binary min-heap by inserting elements one at a time, and then extracting them in sorted order. For each element from the input, we push it to the end of the internal heap vector. Then we perform a bubble-up: compare the new element with its parent (at index `(i-1)/2`). If the child is smaller than the parent, swap them and continue upward; otherwise, stop because the heap property is satisfied. This ensures that after every insertion, the smallest element is at index 0. After processing all inputs, we repeatedly remove the root (the minimum) by swapping it with the last element, popping the last element, and then performing a bubble-down (or sift-down) to restore the heap property for the remaining elements. The removed roots, collected in a result vector, are naturally in ascending order. Edge cases: an empty input returns an empty result; a single element returns that same element; duplicates are handled correctly since swapping on equality is not needed—we only swap when the child is strictly smaller. Time complexity: building the heap has O(n log n) in the worst case (each insertion is O(log n)), and extracting n elements also costs O(n log n), so the total is O(n log n). Space complexity: we use O(n) auxiliary space for the heap and the result vector (in addition to the output itself, which is the returned vector).

#include <vector>
#include <algorithm>

// Returns a new vector containing the elements of input sorted ascending.
// Uses incremental min-heap insertion and extraction.
std::vector<int> sortWithHeapInsertion(const std::vector<int>& input) {
    std::vector<int> heap;
    heap.reserve(input.size());
    
    // Build min-heap by inserting each element with bubble-up.
    for (int x : input) {
        heap.push_back(x);
        int cur = static_cast<int>(heap.size()) - 1;
        // Bubble-up: move smaller child up until heap property holds.
        while (cur > 0) {
            int parent = (cur - 1) / 2;
            if (heap[cur] < heap[parent]) {
                std::swap(heap[cur], heap[parent]);
                cur = parent;
            } else {
                break;
            }
        }
    }
    
    // Extract all elements from the heap in sorted order.
    std::vector<int> result;
    result.reserve(input.size());
    while (!heap.empty()) {
        // The root is the minimum.
        result.push_back(heap[0]);
        // Replace root with the last element and shrink heap.
        heap[0] = heap.back();
        heap.pop_back();
        if (heap.empty()) break;
        // Bubble-down: restore heap property from the root.
        int cur = 0;
        int n = static_cast<int>(heap.size());
        while (true) {
            int left = 2 * cur + 1;
            int right = 2 * cur + 2;
            int smallest = cur;
            if (left < n && heap[left] < heap[smallest]) smallest = left;
            if (right < n && heap[right] < heap[smallest]) smallest = right;
            if (smallest == cur) break;
            std::swap(heap[cur], heap[smallest]);
            cur = smallest;
        }
    }
    
    return result;
}

#include <cassert>
#include <vector>

// (The solution function is assumed to be defined above this main.)

int main() {
    // Empty input.
    std::vector<int> empty;
    assert(sortWithHeapInsertion(empty) == std::vector<int>{});
    
    // Single element.
    std::vector<int> single = {42};
    assert(sortWithHeapInsertion(single) == std::vector<int>{42});
    
    // Already sorted.
    std::vector<int> sorted = {1, 2, 3, 4, 5};
    assert(sortWithHeapInsertion(sorted) == std::vector<int>({1, 2, 3, 4, 5}));
    
    // Reverse sorted.
    std::vector<int> reverse = {5, 4, 3, 2, 1};
    assert(sortWithHeapInsertion(reverse) == std::vector<int>({1, 2, 3, 4, 5}));
    
    // Duplicates and negatives.
    std::vector<int> dupNeg = {-3, 5, -3, 0, 5, -10};
    assert(sortWithHeapInsertion(dupNeg) == std::vector<int>({-10, -3, -3, 0, 5, 5}));
    
    // Larger random-like set.
    std::vector<int> random = {7, 2, 9, 1, 9, 3, 2, 8, 0};
    assert(sortWithHeapInsertion(random) == std::vector<int>({0, 1, 2, 2, 3, 7, 8, 9, 9}));
    
    // Ensure the original vector is not modified.
    std::vector<int> original = {3, 1, 2};
    std::vector<int> copy = original;
    sortWithHeapInsertion(original);
    assert(original == copy);
    
    return 0;
}
