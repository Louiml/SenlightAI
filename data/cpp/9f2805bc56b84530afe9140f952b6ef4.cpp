// Write a C++ function named `processQueue` that takes a vector of non-negative integers `priorities` and an integer `location` (where `0 <= location < priorities.size()`), and returns the order (1-based) in which the item at index `location` would be printed according to the following printer rule: the queue holds pairs of (original index, priority); repeatedly take the front item, and if its priority is strictly less than the highest priority currently remaining, move it to the back; otherwise, print it immediately (removing it from contention); the printed sequence increments a counter, and the function returns the counter value when the item with the given `location` is printed. You may assume all priorities are non-negative and the vector is non-empty. The solution must not modify the input vector and should rely only on standard containers and algorithms.
#include <cassert>
#include <vector>

// Forward declaration of the solution function.
int processQueue(const std::vector<int>& priorities, int location);

int main() {
    // Simple case: target is the highest priority first.
    assert(processQueue({3, 1, 2}, 0) == 1);
    // Target is last in original order but highest priority.
    assert(processQueue({1, 1, 9, 1, 1}, 2) == 1);
    // Classic example from the original problem.
    assert(processQueue({2, 1, 3, 2}, 2) == 1);
    assert(processQueue({2, 1, 3, 2}, 0) == 3);
    // All equal priorities: printed in original order.
    assert(processQueue({5, 5, 5, 5}, 2) == 3);
    // Single element.
    assert(processQueue({7}, 0) == 1);
    // Decreasing priorities cause many rotations.
    assert(processQueue({5, 4, 3, 2, 1}, 4) == 5);
    // Target with middle priority.
    assert(processQueue({1, 2, 3, 2, 1}, 3) == 3);
    // Larger vector with duplicates.
    assert(processQueue({4, 2, 4, 1, 4, 0}, 4) == 2);
    return 0;
}
#include <vector>
#include <queue>
#include <utility>

// Simulate a printer queue and return the 1-based print order of the item
// originally at index 'location'.
// The function does not modify the input vector.
int processQueue(const std::vector<int>& priorities, int location) {
    // Max-heap of current remaining priorities.
    std::priority_queue<int> maxHeap;
    // FIFO queue of (original index, priority).
    std::queue<std::pair<int, int>> q;

    for (std::size_t i = 0; i < priorities.size(); ++i) {
        maxHeap.push(priorities[i]);
        q.push({static_cast<int>(i), priorities[i]});
    }

    int printedCount = 1;  // 1-based counter for the next printed item

    while (true) {
        auto front = q.front();
        q.pop();

        // If this item is not the current maximum, move it to the back.
        if (front.second != maxHeap.top()) {
            q.push(front);
        } else {
            // This item is printed.
            if (front.first == location) {
                return printedCount;
            }
            ++printedCount;
            maxHeap.pop();  // Remove the printed priority from the heap.
        }
    }
}
// The core idea mirrors the classic “printer queue” scheduling problem. We need to simulate the process while always knowing the maximum priority among items still in the queue. A `priority_queue` (max-heap) provides constant-time access to the current maximum, and a FIFO `queue` simulates the rotation. At each step, we pop the front of the queue; if its priority is less than the top of the heap, we push it back to the rear. If it equals the current maximum (since the heap stores priorities, not indices), then we treat it as printed: we increment a printed-count, and if its original index matches `location`, we return that count; otherwise we pop the max from the heap and continue. Edge cases: when the target item has the highest priority from the start, it will be printed first (count=1). Duplicate priorities are handled naturally: the heap’s top reflects the maximum value, and if multiple items share that max, any one of them that reaches the front is printed, but only the one with matching index triggers the return. Since every item eventually gets printed, the loop terminates. Time complexity is O(n) for initialization plus O(n log n) in the worst case because each item may be rotated multiple times, but each rotation and print is O(log n) due to heap operations; formally the worst-case is O(n²) if all priorities are equal and we rotate many times? Actually with equal priorities, every front matches the max, so each is printed immediately, giving O(n log n). For strictly decreasing priorities (like 5,4,3,2,1), we rotate items many times: the item with priority 5 prints first, then we must rotate the remaining 4,3,2,1 many times until 4 reaches front, etc., leading to about n(n+1)/2 queue operations, each with O(log n) heap pop, so total O(n² log n). In practice, for a teaching task we state O(n²) rotations and O(n² log n) overall worst-case, with O(n) auxiliary space for the heap and queue.
