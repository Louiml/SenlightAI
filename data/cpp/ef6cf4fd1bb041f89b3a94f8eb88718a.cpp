Write a C++ function `mergeKSortedArrays` that takes a vector of `k` sorted integer vectors (each individually non-decreasing) and returns a single sorted vector containing all elements from all input arrays in non-decreasing order. The function should handle empty input vectors, duplicate elements, and zero or more arrays. The solution must not assume any limit on the total number of elements; it should use an efficient approach for merging rather than simply collecting and sorting. Implement the function with proper `const` correctness where applicable: the input should be accepted as `const vector<vector<int>>& kArrays`, and the return type is `vector<int>`. The function signature should be `vector<int> mergeKSortedArrays(const vector<vector<int>>& kArrays)`.
// The straightforward approach is to push all elements into a min-heap (priority queue with `greater<int>`) and then pop them sequentially, which yields a sorted result. This approach handles duplicates naturally and works even if some arrays are empty. However, it does not exploit the fact that each array is already sorted, and its time complexity is O(N log N) where N is the total number of elements, with O(N) auxiliary space for the heap. A more optimal approach uses a min-heap of iterators (or indices and array references) to merge k sorted lists in O(N log k) time using O(k) extra space. But since the problem does not require optimality, the simple heap approach is acceptable, as it correctly handles all edge cases: empty arrays contribute nothing, duplicates remain, and an empty input vector returns an empty result. For a total of N elements across all arrays, the time complexity is O(N log N) due to heap operations, and auxiliary space is O(N) for the heap and answer vector.
#include <vector>
#include <queue>

// Merge k sorted arrays into a single sorted vector.
// All input arrays are each sorted in non-decreasing order.
std::vector<int> mergeKSortedArrays(const std::vector<std::vector<int>>& kArrays) {
    // Min-heap to hold all elements in sorted order.
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;
    
    // Push every element from every array into the heap.
    for (const auto& arr : kArrays) {
        for (int value : arr) {
            minHeap.push(value);
        }
    }
    
    // Extract elements in ascending order.
    std::vector<int> result;
    result.reserve(minHeap.size()); // optional optimization
    while (!minHeap.empty()) {
        result.push_back(minHeap.top());
        minHeap.pop();
    }
    
    return result;
}
#include <cassert>
#include <vector>

// The solution function is assumed to be declared above.
// Test cases for the mergeKSortedArrays function.
int main() {
    // Case 1: Multiple arrays with distinct values.
    std::vector<std::vector<int>> arrays1 = {{1, 3, 5}, {2, 4, 6}, {0, 7}};
    std::vector<int> expected1 = {0, 1, 2, 3, 4, 5, 6, 7};
    assert(mergeKSortedArrays(arrays1) == expected1);

    // Case 2: Duplicates across arrays.
    std::vector<std::vector<int>> arrays2 = {{1, 1, 2}, {1, 2, 3}};
    std::vector<int> expected2 = {1, 1, 1, 2, 2, 3};
    assert(mergeKSortedArrays(arrays2) == expected2);

    // Case 3: Single empty array.
    std::vector<std::vector<int>> arrays3 = {{}};
    std::vector<int> expected3 = {};
    assert(mergeKSortedArrays(arrays3) == expected3);

    // Case 4: All arrays empty.
    std::vector<std::vector<int>> arrays4 = {{}, {}, {}};
    std::vector<int> expected4 = {};
    assert(mergeKSortedArrays(arrays4) == expected4);

    // Case 5: Only one array.
    std::vector<std::vector<int>> arrays5 = {{-5, 0, 3, 9}};
    std::vector<int> expected5 = {-5, 0, 3, 9};
    assert(mergeKSortedArrays(arrays5) == expected5);

    // Case 6: Large values and negative numbers.
    std::vector<std::vector<int>> arrays6 = {{-1000000, 0}, {-1, 1000000}};
    std::vector<int> expected6 = {-1000000, -1, 0, 1000000};
    assert(mergeKSortedArrays(arrays6) == expected6);

    // Case 7: Unsorted individual arrays? The problem states they are sorted, but we test anyway.
    // (Not required, but shows the function still returns globally sorted if input is unsorted.)
    std::vector<std::vector<int>> arrays7 = {{3, 1}, {2}};
    std::vector<int> expected7 = {1, 2, 3};
    assert(mergeKSortedArrays(arrays7) == expected7);

    // Case 8: Many small arrays.
    std::vector<std::vector<int>> arrays8 = {{0}, {1}, {2}};
    std::vector<int> expected8 = {0, 1, 2};
    assert(mergeKSortedArrays(arrays8) == expected8);

    // Case 9: Empty input vector (k=0).
    std::vector<std::vector<int>> arrays9 = {};
    std::vector<int> expected9 = {};
    assert(mergeKSortedArrays(arrays9) == expected9);

    // Case 10: Random mix.
    std::vector<std::vector<int>> arrays10 = {{-3, 2}, {1, 4}, {0, 5}};
    std::vector<int> expected10 = {-3, 0, 1, 2, 4, 5};
    assert(mergeKSortedArrays(arrays10) == expected10);

    return 0;
}
