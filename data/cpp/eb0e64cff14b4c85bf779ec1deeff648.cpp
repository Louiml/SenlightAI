// Write a C++ function `kSmallestWithPriorityQueue` that takes a non-negative integer `k` and a vector of integers (which may be empty or contain duplicate values), and returns a new vector containing the `k` smallest elements from the input in ascending order. If `k` is greater than or equal to the number of elements in the input vector, return the entire input vector sorted in ascending order. If `k` is 0, return an empty vector. The function must use a priority queue internally to solve the problem, and it should not modify the input vector. The returned vector must be sorted in ascending order.
#include <cassert>
#include <vector>
#include <algorithm>

// The solution function is assumed to be included above.

int main() {
    // Basic case
    std::vector<int> v1 = {5, 1, 3, 2, 4};
    assert(kSmallestWithPriorityQueue(3, v1) == std::vector<int>({1, 2, 3}));

    // k = 0 returns empty
    assert(kSmallestWithPriorityQueue(0, v1).empty());

    // k larger than size returns sorted full vector
    assert(kSmallestWithPriorityQueue(10, v1) == std::vector<int>({1, 2, 3, 4, 5}));

    // Duplicate values
    std::vector<int> v2 = {7, 7, 1, 7, 3};
    assert(kSmallestWithPriorityQueue(2, v2) == std::vector<int>({1, 3}));

    // Empty input returns empty
    std::vector<int> v3;
    assert(kSmallestWithPriorityQueue(5, v3).empty());

    // Negative numbers
    std::vector<int> v4 = {-10, -20, 0, 5, -1};
    assert(kSmallestWithPriorityQueue(3, v4) == std::vector<int>({-20, -10, -1}));

    // k equals exact size
    assert(kSmallestWithPriorityQueue(5, v4) == std::vector<int>({-20, -10, -1, 0, 5}));

    // All identical values
    std::vector<int> v5 = {4, 4, 4, 4};
    assert(kSmallestWithPriorityQueue(2, v5) == std::vector<int>({4, 4}));

    // Single element
    std::vector<int> v6 = {42};
    assert(kSmallestWithPriorityQueue(1, v6) == std::vector<int>({42}));

    // Larger vector to ensure heap behavior
    std::vector<int> v7 = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    assert(kSmallestWithPriorityQueue(4, v7) == std::vector<int>({1, 2, 3, 4}));
}
#include <vector>
#include <queue>
#include <algorithm>

// Given k and a vector of integers, return the k smallest elements in ascending order.
// Uses a max-heap of size up to k to track the smallest elements seen.
std::vector<int> kSmallestWithPriorityQueue(int k, const std::vector<int>& input) {
    std::vector<int> result;
    if (k <= 0 || input.empty()) {
        return result;
    }

    // Max-heap to store the k smallest elements seen so far
    std::priority_queue<int> maxHeap;

    for (int value : input) {
        if (static_cast<int>(maxHeap.size()) < k) {
            maxHeap.push(value);
        } else if (value < maxHeap.top()) {
            maxHeap.pop();
            maxHeap.push(value);
        }
    }

    // Extract heap contents into result
    while (!maxHeap.empty()) {
        result.push_back(maxHeap.top());
        maxHeap.pop();
    }

    // Sort ascending as required
    std::sort(result.begin(), result.end());
    return result;
}
// The solution uses a max-heap (via `priority_queue<int>`) of size at most `k` to keep track of the smallest `k` elements seen so far. We iterate through the input vector: for each element, if the heap size is less than `k`, we push it; otherwise, if the current element is smaller than the heap's top (which is the largest among the current smallest `k`), we pop the top and push the current element. After processing all elements, the heap contains exactly the smallest `min(k, n)` elements (where `n` is the size of the input). We then extract all elements from the heap into a result vector and sort it ascending. Edge cases: empty input (return empty), `k=0` (return empty), `k>=n` (push all, then sort and return). Time complexity is O(n log k) for processing plus O(k log k) for the final sort, dominated by O(n log k) when `k` is small. Space complexity is O(k) for the heap plus O(k) for the result.
