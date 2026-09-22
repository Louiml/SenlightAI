// Implement a C++ function `std::vector<int> heapToSortedDescending(const std::vector<int>& input)` that, given a non-empty vector of integers, builds a max-heap from the elements using the same insertion logic as the provided `MaxHeap` class (i.e., push each element then perform `up_heapify`), and then extracts all elements in descending order using `ExtractMax`, finally returning the sorted list. The function must not modify the input vector and must handle duplicate values correctly. The output should be the original elements sorted from largest to smallest.
The solution simulates a max-heap explicitly to match the given code's behavior. First, we create a `MaxHeap` object (or replicate its logic inside the function) and insert each value from the input vector using `insert`, which appends to the internal vector and calls `up_heapify` to restore the heap property. After all insertions, we repeatedly call `ExtractMax` (which removes the root and fixes the heap with `down_heapify`) and collect the returned values into a result vector. Since `ExtractMax` returns the largest remaining element each time, the result is sorted descending. Edge cases: empty input is not allowed by the task specification; but if it were, the function should return an empty vector. Duplicates are handled naturally because heap operations preserve all occurrences. Time complexity: inserting `n` elements takes O(n log n) and extracting `n` elements also takes O(n log n), so total O(n log n). Space complexity: O(n) for the heap storage plus O(n) for the result vector, so O(n) auxiliary space.
#include <vector>
#include <algorithm> // for swap

// Build a max-heap from the input and return elements in descending order.
// Uses the same heap insertion and extraction logic as the provided MaxHeap.
std::vector<int> heapToSortedDescending(const std::vector<int>& input) {
    // Internal heap stored as a vector, mirroring MaxHeap's nodes.
    std::vector<int> heap;
    
    // Helper to maintain max-heap property upward from index idx.
    auto up_heapify = [&heap](int idx) {
        while (idx > 0 && heap[idx] > heap[(idx - 1) / 2]) {
            std::swap(heap[idx], heap[(idx - 1) / 2]);
            idx = (idx - 1) / 2;
        }
    };
    
    // Insert all elements from input into the heap.
    for (int x : input) {
        heap.push_back(x);
        up_heapify(static_cast<int>(heap.size()) - 1);
    }
    
    // Helper to maintain max-heap property downward from index idx.
    auto down_heapify = [&heap](int idx) {
        while (true) {
            int largest = idx;
            int l = 2 * idx + 1;
            int r = 2 * idx + 2;
            if (l < static_cast<int>(heap.size()) && heap[largest] < heap[l]) {
                largest = l;
            }
            if (r < static_cast<int>(heap.size()) && heap[largest] < heap[r]) {
                largest = r;
            }
            if (largest == idx) break;
            std::swap(heap[idx], heap[largest]);
            idx = largest;
        }
    };
    
    // Extract max repeatedly to produce descending order.
    std::vector<int> result;
    while (!heap.empty()) {
        result.push_back(heap[0]);
        // Replacement and removal of root.
        std::swap(heap[0], heap[heap.size() - 1]);
        heap.pop_back();
        if (!heap.empty()) {
            down_heapify(0);
        }
    }
    
    return result;
}
#include <cassert>
#include <vector>
#include <iostream>

// Forward declaration for testing.
std::vector<int> heapToSortedDescending(const std::vector<int>& input);

int main() {
    // Basic test.
    std::vector<int> input1 = {4, 7, 9, 1, 10, 20, 30};
    std::vector<int> expected1 = {30, 20, 10, 9, 7, 4, 1};
    assert(heapToSortedDescending(input1) == expected1);

    // Already descending input.
    std::vector<int> input2 = {9, 5, 2, 1};
    std::vector<int> expected2 = {9, 5, 2, 1};
    assert(heapToSortedDescending(input2) == expected2);

    // All duplicates.
    std::vector<int> input3 = {3, 3, 3, 3};
    std::vector<int> expected3 = {3, 3, 3, 3};
    assert(heapToSortedDescending(input3) == expected3);

    // Single element.
    std::vector<int> input4 = {42};
    std::vector<int> expected4 = {42};
    assert(heapToSortedDescending(input4) == expected4);

    // Negative numbers and mixed signs.
    std::vector<int> input5 = {-1, -5, 0, 8, -3, 2};
    std::vector<int> expected5 = {8, 2, 0, -1, -3, -5};
    assert(heapToSortedDescending(input5) == expected5);

    // Large input with duplicates.
    std::vector<int> input6 = {100, 99, 100, 98, 1, 1, 50};
    std::vector<int> expected6 = {100, 100, 99, 98, 50, 1, 1};
    assert(heapToSortedDescending(input6) == expected6);

    // Input that is ascending order.
    std::vector<int> input7 = {1, 2, 3, 4, 5};
    std::vector<int> expected7 = {5, 4, 3, 2, 1};
    assert(heapToSortedDescending(input7) == expected7);

    // Check that original input is not modified.
    std::vector<int> original = {5, 3, 8};
    std::vector<int> copy = original;
    (void)heapToSortedDescending(original);
    assert(original == copy);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
