Write a C++ function that takes a vector of integers and returns a new vector containing the elements in non-decreasing order but with all duplicate values removed, using a min-heap priority queue as the only data structure for sorting. The function should preserve the relative order of first occurrences of distinct values as they appear in the original sorted order (i.e., sorted ascending, then deduplicated). If the input vector is empty, return an empty vector. The function must not use any standard sort algorithm, set, or map; only priority_queue, vector, and other basic utilities are allowed. For example, given `{5, 3, 8, 3, 1, 5, 2}`, the output should be `{1, 2, 3, 5, 8}`.
The solution uses a min-heap (`priority_queue<int, vector<int>, greater<int>>`) to extract the smallest element repeatedly. We push all elements into the heap in O(n) time. Then we repeatedly pop the top, which gives the current minimum. To remove duplicates, we keep track of the previously popped value; if the current popped value equals the previous one, we skip adding it to the result. Otherwise, we append it to the result and update the previous value. This works because popping from a min-heap yields elements in non-decreasing order, so duplicates appear consecutively. Edge cases: empty input returns empty output; all equal values produce a single element; negative numbers are handled naturally. Time complexity: pushing n elements is O(n log n) because each push is O(log n) in the worst case (heapify could be O(n) if we use a different construction, but standard `push` is O(log n)). Popping all n elements is also O(n log n). Thus overall O(n log n). Space complexity: O(n) for the heap plus O(n) for the result vector, so O(n) auxiliary.
#include <vector>
#include <queue>
#include <functional> // for std::greater

// Sorts and deduplicates the input vector using a min-heap.
// Returns a new vector with unique values in non-decreasing order.
std::vector<int> sortedUniqueFromMinHeap(const std::vector<int>& input) {
    // Min-heap: smallest element at top.
    std::priority_queue<int, std::vector<int>, std::greater<int>> heap;
    
    // Push all elements into the heap.
    for (int value : input) {
        heap.push(value);
    }
    
    std::vector<int> result;
    int previous = 0; // arbitrary initial value; will be overwritten on first pop
    bool first = true; // flag to skip duplicate check for the first element
    
    while (!heap.empty()) {
        int current = heap.top();
        heap.pop();
        
        // Skip if current equals previous (duplicate).
        if (!first && current == previous) {
            continue;
        }
        
        result.push_back(current);
        previous = current;
        first = false;
    }
    
    return result;
}
#include <cassert>
#include <vector>

// The solution function is defined above; here's the test.
int main() {
    // Basic example
    std::vector<int> input1 = {5, 3, 8, 3, 1, 5, 2};
    std::vector<int> expected1 = {1, 2, 3, 5, 8};
    assert(sortedUniqueFromMinHeap(input1) == expected1);

    // Empty input
    std::vector<int> empty;
    assert(sortedUniqueFromMinHeap(empty).empty());

    // All duplicates
    std::vector<int> input2 = {7, 7, 7};
    std::vector<int> expected2 = {7};
    assert(sortedUniqueFromMinHeap(input2) == expected2);

    // Already sorted unique
    std::vector<int> input3 = {1, 2, 3, 4};
    std::vector<int> expected3 = {1, 2, 3, 4};
    assert(sortedUniqueFromMinHeap(input3) == expected3);

    // Negative and zero
    std::vector<int> input4 = {-3, -10, 0, -3, 5, -10};
    std::vector<int> expected4 = {-10, -3, 0, 5};
    assert(sortedUniqueFromMinHeap(input4) == expected4);

    // Unsorted with many duplicates
    std::vector<int> input5 = {4, 2, 2, 4, 1, 3, 1, 2};
    std::vector<int> expected5 = {1, 2, 3, 4};
    assert(sortedUniqueFromMinHeap(input5) == expected5);

    // Single element
    std::vector<int> input6 = {42};
    std::vector<int> expected6 = {42};
    assert(sortedUniqueFromMinHeap(input6) == expected6);

    // Large numbers
    std::vector<int> input7 = {1000, -1000, 500, -1000, 1000};
    std::vector<int> expected7 = {-1000, 500, 1000};
    assert(sortedUniqueFromMinHeap(input7) == expected7);

    return 0;
}
