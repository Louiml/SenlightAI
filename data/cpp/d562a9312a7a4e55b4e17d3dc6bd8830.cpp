/*
Write a C++ function `std::vector<int> mergeKHeaps(const std::vector<std::vector<int>>& heaps)` that takes a vector of unsorted integer vectors (each representing a collection of elements that conceptually form a max-heap, but the internal array order is not guaranteed to satisfy the heap property), and returns a single vector containing all elements sorted in non-increasing (descending) order. The function must not rely on `std::priority_queue` or any heap-related standard library containers; instead, it should implement a custom max-heap using a vector and provide `push`, `pop`, and `top` operations internally. The input may contain empty heaps, duplicate values, and negative numbers. The function should handle the case where the total number of elements is zero by returning an empty vector. Your solution must be self-contained and not use any external libraries beyond the C++ standard headers.
*/
#include <vector>
#include <algorithm>
#include <cstddef>

// Helper function to sift down a node in a max-heap represented by a vector.
// The heap occupies indices [0, heapSize). The node at index 'i' may violate
// the max-heap property; this function restores it by swapping with larger children.
void siftDown(std::vector<int>& heap, std::size_t i, std::size_t heapSize) {
    while (true) {
        std::size_t largest = i;
        std::size_t left = 2 * i + 1;
        std::size_t right = 2 * i + 2;

        if (left < heapSize && heap[left] > heap[largest])
            largest = left;
        if (right < heapSize && heap[right] > heap[largest])
            largest = right;

        if (largest == i)
            break;

        std::swap(heap[i], heap[largest]);
        i = largest;
    }
}

// Merge all elements from the given vectors and return them sorted in
// non-increasing (descending) order using a custom max-heap.
std::vector<int> mergeKHeaps(const std::vector<std::vector<int>>& heaps) {
    // Step 1: Concatenate all elements into a single vector.
    std::vector<int> merged;
    for (const auto& heap : heaps) {
        merged.insert(merged.end(), heap.begin(), heap.end());
    }

    if (merged.empty())
        return {};

    // Step 2: Build a max-heap in-place using bottom-up heapify.
    std::size_t n = merged.size();
    for (std::size_t i = n / 2; i-- > 0; ) {
        siftDown(merged, i, n);
    }

    // Step 3: Repeatedly extract the maximum and place it at the end.
    std::vector<int> result;
    result.reserve(n);
    for (std::size_t heapSize = n; heapSize > 0; --heapSize) {
        result.push_back(merged[0]);       // current maximum
        std::swap(merged[0], merged[heapSize - 1]);  // move max to end
        siftDown(merged, 0, heapSize - 1); // restore heap for remaining part
    }

    return result;
}
#include <cassert>
#include <vector>

// Declaration of the solution function (as above)
std::vector<int> mergeKHeaps(const std::vector<std::vector<int>>& heaps);

int main() {
    // Test 1: Basic multiple heaps with duplicates and negatives.
    std::vector<std::vector<int>> input1 = {{3, 1, 4}, {1, 5, 9}, {2, 6}};
    std::vector<int> result1 = mergeKHeaps(input1);
    assert((result1 == std::vector<int>{9, 6, 5, 4, 3, 2, 1, 1}));

    // Test 2: Empty heaps are ignored, single element.
    std::vector<std::vector<int>> input2 = {{}, {7}, {}, {-3}};
    std::vector<int> result2 = mergeKHeaps(input2);
    assert((result2 == std::vector<int>{7, -3}));

    // Test 3: All negative numbers.
    std::vector<std::vector<int>> input3 = {{-5, -2}, {-10, -1}};
    std::vector<int> result3 = mergeKHeaps(input3);
    assert((result3 == std::vector<int>{-1, -2, -5, -10}));

    // Test 4: All equal values.
    std::vector<std::vector<int>> input4 = {{4, 4}, {4}};
    std::vector<int> result4 = mergeKHeaps(input4);
    assert((result4 == std::vector<int>{4, 4, 4}));

    // Test 5: Single element in entire input.
    std::vector<std::vector<int>> input5 = {{42}};
    std::vector<int> result5 = mergeKHeaps(input5);
    assert((result5 == std::vector<int>{42}));

    // Test 6: Fully empty input.
    std::vector<std::vector<int>> input6 = {{}, {}, {}};
    std::vector<int> result6 = mergeKHeaps(input6);
    assert(result6.empty());

    // Test 7: Already sorted descending input should remain unchanged.
    std::vector<std::vector<int>> input7 = {{10, 5, 3}, {2, 1}};
    std::vector<int> result7 = mergeKHeaps(input7);
    assert((result7 == std::vector<int>{10, 5, 3, 2, 1}));

    // Test 8: Large values and multiple duplicates.
    std::vector<std::vector<int>> input8 = {{100, 100, 50}, {200, 100}, {75, 0}};
    std::vector<int> result8 = mergeKHeaps(input8);
    assert((result8 == std::vector<int>{200, 100, 100, 100, 75, 50, 0}));

    return 0;
}
// The core approach is to merge all elements from all provided vectors into a single uncompressed vector, then use a custom max-heap (implemented via a vector with sift-down heapify) to sort them in descending order. First, concatenate all elements from all input vectors into one temporary vector. If this temporary vector is empty, return an empty result immediately. Then, build a max-heap from this vector in-place using the standard bottom-up heapify algorithm: for each index starting from the parent of the last element down to 0, call a sift-down function that swaps a node with its larger child if needed, recursively adjusting until the heap property is restored. After the heap is built, repeatedly extract the maximum element: swap the root (index 0) with the last element of the current heap, shrink the heap size by one, and sift down the new root. Collect these extracted values into a result vector, which will be in descending order. Edge cases include empty input vectors (ignored), a single element (trivially sorted), duplicate values (handled naturally by the heap, as they just compare equal and stop sifting), and negative numbers (no special treatment needed). Time complexity is O(N + N log N) = O(N log N) for N total elements, dominated by the extraction loop. Space complexity is O(N) for the temporary merged vector and the result vector, plus O(1) auxiliary for the heap operations (since we modify the temporary vector in place). The custom heap implementation avoids using `std::priority_queue` to match the task constraint.
