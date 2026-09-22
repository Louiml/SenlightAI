/*
Write a C++ function that takes a vector of positive integers representing the weights of stones and simulates the process of smashing them together. At each step, select the two heaviest stones. If they have equal weights, both are destroyed; otherwise, the heavier stone is reduced to the absolute difference of their weights and placed back with the remaining stones. Continue this process until at most one stone remains. Return the weight of the last remaining stone, or 0 if no stones remain. The input vector may contain duplicate weights, and the order of elements is unspecified. The function must handle an empty vector and a single-element vector correctly.
*/

#include <vector>
#include <queue>
#include <cstdlib> // for std::abs

// Given a vector of stone weights, repeatedly smash the two heaviest stones
// until at most one remains. Return the final stone weight, or 0 if none remain.
int lastStoneWeight(const std::vector<int>& stones) {
    // Max-heap to efficiently retrieve the largest stones.
    std::priority_queue<int> maxHeap(stones.begin(), stones.end());

    // Smash stones until fewer than two remain.
    while (maxHeap.size() > 1) {
        int heaviest = maxHeap.top();
        maxHeap.pop();
        int secondHeaviest = maxHeap.top();
        maxHeap.pop();

        // If weights differ, the remainder goes back into the heap.
        if (heaviest != secondHeaviest) {
            maxHeap.push(std::abs(heaviest - secondHeaviest));
        }
        // If equal, both are destroyed (nothing is pushed).
    }

    // Return the last stone weight, or 0 if the heap is empty.
    return maxHeap.empty() ? 0 : maxHeap.top();
}

#include <cassert>
#include <vector>

// Declaration of the function under test (provided above).
int lastStoneWeight(const std::vector<int>& stones);

int main() {
    // Basic case with distinct weights.
    assert(lastStoneWeight({2, 7, 4, 1, 8, 1}) == 1);

    // Equal stones cancel out completely.
    assert(lastStoneWeight({3, 3, 3, 3}) == 0);

    // Two equal stones leave nothing.
    assert(lastStoneWeight({5, 5}) == 0);

    // Two different stones leave the difference.
    assert(lastStoneWeight({10, 4}) == 6);

    // Single stone returns itself.
    assert(lastStoneWeight({42}) == 42);

    // Empty vector returns 0.
    assert(lastStoneWeight({}) == 0);

    // Larger set with duplicates.
    assert(lastStoneWeight({1, 1, 2, 3, 5, 8}) == 0);

    // All identical odd count leaves one stone.
    assert(lastStoneWeight({7, 7, 7}) == 7);

    // Random mixed values.
    assert(lastStoneWeight({9, 3, 1, 1}) == 4);

    // Example from typical usage.
    assert(lastStoneWeight({1, 2, 3, 4, 5}) == 1);

    return 0;
}

// The solution uses a max-heap (priority queue) to always access the two largest stones efficiently. Initialize a max-heap with all stone weights. While the heap contains more than one stone, pop the two largest elements (`a` and `b`). If they differ, push the absolute difference `abs(a-b)` back into the heap. If they are equal, nothing is pushed, effectively destroying both. After the loop, if the heap is empty return 0, otherwise return the single remaining weight (the top of the heap). This simulates the exact process. Key edge cases: an empty vector returns 0; a vector with one stone returns that stone; duplicate weights are handled naturally because equal stones cancel out. Time complexity is O(n log n) where n is the number of initial stones, since each heap operation is O(log n) and we perform up to n-1 operations. Space complexity is O(n) for the heap.
