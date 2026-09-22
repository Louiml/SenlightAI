// Write a C++ function `kthLargestStream` that, given an integer `k` and a vector of initial integers, processes a sequence of additional integers one at a time and returns the k-th largest element after each insertion. The function should accept the initial array, the value `k`, and a list of new numbers to insert, and return a vector containing the k-th largest element after each insertion (including after processing all initial numbers, i.e., before any new insertions, the first result should be the k-th largest of the initial array). If the initial array has fewer than `k` elements, you may assume `k` is at most the initial array size (so no invalid state). Duplicates are allowed, and `k` is 1-indexed, meaning `k=1` returns the maximum.

// The core idea is to maintain a min-heap of size exactly `k` that always contains the `k` largest elements seen so far. Initially, push all elements from the input vector into the heap; if the heap size exceeds `k`, pop the smallest (top) element, which removes the smallest among the current candidates, leaving exactly the `k` largest. The top of the heap is then the k-th largest (since it is the smallest of the `k` largest). For each new number, push it into the heap, then if the size exceeds `k`, pop the top. This guarantees the heap size stays exactly `k` and the top is the k-th largest. Duplicates are handled naturally because the heap stores all occurrences. Edge cases: if the initial array size equals `k`, no popping happens initially; if duplicates exist, they are all stored. Time complexity: building the initial heap takes O(m log k) where m is the initial array size, and each insertion takes O(log k). Space complexity is O(k) for the heap. The function returns a vector of results, one per insertion.

#include <queue>
#include <vector>

// Given an initial array and a value k (1-indexed), return the k-th largest
// element after initial setup and after each insertion from the extra list.
std::vector<int> kthLargestStream(int k, const std::vector<int>& initial, const std::vector<int>& extra) {
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;
    for (int val : initial) {
        minHeap.push(val);
        if (static_cast<int>(minHeap.size()) > k) {
            minHeap.pop();
        }
    }
    std::vector<int> results;
    results.push_back(minHeap.top());
    for (int val : extra) {
        minHeap.push(val);
        if (static_cast<int>(minHeap.size()) > k) {
            minHeap.pop();
        }
        results.push_back(minHeap.top());
    }
    return results;
}

#include <cassert>
#include <vector>

int main() {
    // Basic test with distinct values
    std::vector<int> init1 = {3, 1, 4, 2};
    std::vector<int> extra1 = {5, 0};
    assert(kthLargestStream(2, init1, extra1) == std::vector<int>({3, 4, 4}));
    // k=1 returns maximum
    assert(kthLargestStream(1, init1, extra1) == std::vector<int>({4, 5, 5}));
    // Duplicates and k equals initial size
    std::vector<int> init2 = {7, 7, 2};
    std::vector<int> extra2 = {7, 9};
    assert(kthLargestStream(3, init2, extra2) == std::vector<int>({2, 7, 7}));
    // Empty extra list
    assert(kthLargestStream(1, {5, 3}, {}) == std::vector<int>({5}));
    // Larger k with many insertions
    std::vector<int> init3 = {10, 20, 30};
    std::vector<int> extra3 = {25, 15, 35, 5};
    assert(kthLargestStream(2, init3, extra3) == std::vector<int>({20, 25, 25, 30, 30}));
    // Negative numbers
    std::vector<int> init4 = {-5, -10};
    std::vector<int> extra4 = {-1, -3};
    assert(kthLargestStream(2, init4, extra4) == std::vector<int>({-10, -5, -3}));
    return 0;
}
