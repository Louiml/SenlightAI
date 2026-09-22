// Write a C++ function `int printerQueueTime(int docCount, int targetIndex, const std::vector<int>& priorities)` that simulates a printer queue. The queue initially holds documents in the order given, where `priorities[i]` is the priority of document `i` (0-based index). The printer repeatedly takes the first document in the queue. If its priority is the highest among all documents currently in the queue (ties are broken by arrival order — i.e., the front document with that priority is printed), it is printed and removed. Otherwise, it is moved to the back of the queue. The function must return the 1-based position (print order) at which the document originally at `targetIndex` gets printed. You may assume `docCount` is at least 1, `targetIndex` is a valid index in `[0, docCount-1]`, and all priorities are positive integers. The function should not modify the input vector.

The problem is a classic simulation using a queue for the document order and a max-heap (priority queue) for tracking the current highest priority. The algorithm: initialize a `std::queue<std::pair<int,int>>` with each document’s (priority, originalIndex), and a `std::priority_queue<int>` with all priorities. Maintain a counter `time = 1`. While the queue is not empty, pop the front document `cur`. If `cur.first == priorityQueue.top()`, then this document has the highest priority and will be printed. Check if `cur.second == targetIndex`; if so, return `time`. Otherwise, increment `time`, remove the top from the priority queue, and continue. If `cur.first` is not the highest, push `cur` back to the back of the queue. The loop always terminates because each document gets printed eventually. **Edge cases**:  
- If the target document has the highest priority initially, it prints immediately (time = 1).  
- Duplicate priorities are handled because only the front document with the highest priority is printed; if there are multiple with the same max, they print in queue order.  
- If the target document has the lowest priority, it may be moved many times before being printed.  
**Time complexity**: Each document is popped from the queue at most once per full rotation; in the worst case, a document may be rotated `docCount` times, so O(n²) pops overall. Each pop/push on the queue is O(1), and each priority queue operation is O(log n), leading to worst-case O(n² log n). Space is O(n) for the queue and priority queue.

#include <queue>
#include <vector>
#include <utility>

// Simulate the printer queue and return the 1-based print order of targetIndex.
int printerQueueTime(int docCount, int targetIndex, const std::vector<int>& priorities) {
    std::queue<std::pair<int, int>> docs; // (priority, originalIndex)
    std::priority_queue<int> maxHeap;     // current highest priorities

    for (int i = 0; i < docCount; ++i) {
        docs.push({priorities[i], i});
        maxHeap.push(priorities[i]);
    }

    int time = 1;
    while (!docs.empty()) {
        auto cur = docs.front();
        docs.pop();

        if (cur.first == maxHeap.top()) {
            // This document is printed.
            if (cur.second == targetIndex) {
                return time;
            }
            ++time;
            maxHeap.pop();
        } else {
            // Move to back of queue.
            docs.push(cur);
        }
    }
    // Unreachable because targetIndex is always printed.
    return -1; // fallback (should never be reached)
}

#include <cassert>
#include <vector>

// Function declaration (prototype) for the test.
int printerQueueTime(int docCount, int targetIndex, const std::vector<int>& priorities);

int main() {
    // Test 1: Single document
    assert(printerQueueTime(1, 0, {5}) == 1);

    // Test 2: All same priority, print in order
    assert(printerQueueTime(3, 2, {1, 1, 1}) == 3);
    assert(printerQueueTime(3, 0, {1, 1, 1}) == 1);

    // Test 3: Target has highest priority initially
    assert(printerQueueTime(3, 1, {2, 5, 3}) == 1);

    // Test 4: Target has lowest priority, must wait
    // priorities: [1,2,3] target 0, highest is 3 printed first, then 2, then 1
    assert(printerQueueTime(3, 0, {1, 2, 3}) == 3);

    // Test 5: Classic example from queue simulation (higher priority moves back)
    // docs: (2,0),(1,1),(3,2),(2,3) => print 3 (idx2) first, then 2 (idx0), then 2 (idx3), then 1 (idx1)
    assert(printerQueueTime(4, 1, {2, 1, 3, 2}) == 4);
    assert(printerQueueTime(4, 0, {2, 1, 3, 2}) == 2);
    assert(printerQueueTime(4, 3, {2, 1, 3, 2}) == 3);

    // Test 6: Duplicate highest priorities, tie broken by queue order
    // docs: (5,0),(5,1),(3,2) => both 5's print in order, target 1 prints at time 2
    assert(printerQueueTime(3, 1, {5, 5, 3}) == 2);

    // Test 7: Larger random with descending priorities
    // priorities: [4,3,2,1] target 3 (lowest) prints last
    assert(printerQueueTime(4, 3, {4, 3, 2, 1}) == 4);

    // Test 8: Target in middle, one lower before it but higher after
    // priorities: [1,3,2] target 0 => 3 printed first, then 2, then 1
    assert(printerQueueTime(3, 0, {1, 3, 2}) == 3);

    return 0;
}
