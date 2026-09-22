Write a C++ function `string doublePriorityQueueSimulator(const vector<string>& operations)` that simulates a double-ended priority queue supporting four operation types: `"I x"` inserts integer `x` into the queue, `"D 1"` removes the maximum element (if any), `"D -1"` removes the minimum element (if any), and `"C"` clears the queue of all elements. After processing all operations, if the queue is empty, return `"EMPTY"`; otherwise return the current maximum and minimum separated by a space (e.g., `"42 3"`). The queue may contain duplicate values, and operations are guaranteed to be well‑formed with valid integers. This task must be solved without using `std::multiset` or any sorted container; you must implement it using two heaps and a frequency map for lazy deletion, mirroring the provided snippet’s approach.

The core idea is to maintain two priority queues: a max‑heap for retrieving the largest element and a min‑heap for the smallest. Since heaps cannot directly delete arbitrary elements, we use an unordered_map (or map) `counts` that stores how many copies of each integer are currently "alive" in the logical queue. When inserting `x`, we push `x` into both heaps and increment `counts[x]`. For deletion, we repeatedly pop from the appropriate heap until we find a value whose `counts` entry is positive (i.e., the value is still logically present), then decrement that entry (removing the entry if it becomes zero). We also need a helper `cleanTop` that removes stale heap tops whose counts are zero. However, note that calling `cleanTop` on both heaps before checking emptiness is crucial to avoid returning stale values. The `Empty` function should clean both heaps and return true if either is empty after cleaning. Edge cases include attempting to delete from an empty queue (which should be ignored), duplicate values (handled by the count map), and clearing the queue (we can simply clear both heaps and the map). Time complexity per operation is amortized `O(log N)` for pushes and pops, with each element inserted being popped at most once from each heap, plus `O(1)` for map operations. Space complexity is `O(N)` for storing all elements in heaps and map.

#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <sstream>
using namespace std;

// Simulate a double-ended priority queue with insert, delete max/min, and clear.
// Returns "EMPTY" if empty after all operations, else "max min".
string doublePriorityQueueSimulator(const vector<string>& operations) {
    priority_queue<int> maxHeap;
    priority_queue<int, vector<int>, greater<int>> minHeap;
    unordered_map<int, int> counts;

    // Remove stale tops from both heaps where count is zero.
    auto clean = [&]() {
        while (!maxHeap.empty() && counts[maxHeap.top()] == 0) maxHeap.pop();
        while (!minHeap.empty() && counts[minHeap.top()] == 0) minHeap.pop();
    };

    for (const auto& op : operations) {
        if (op[0] == 'C') {
            // Clear all data structures.
            while (!maxHeap.empty()) maxHeap.pop();
            while (!minHeap.empty()) minHeap.pop();
            counts.clear();
        } else if (op[0] == 'I') {
            int val = stoi(op.substr(2));
            maxHeap.push(val);
            minHeap.push(val);
            counts[val]++;
        } else { // 'D'
            clean();
            if (maxHeap.empty()) continue; // Queue empty, ignore delete.
            int dir = stoi(op.substr(2));
            if (dir == 1) {
                int val = maxHeap.top();
                maxHeap.pop();
                counts[val]--;
                if (counts[val] == 0) counts.erase(val);
            } else { // dir == -1
                int val = minHeap.top();
                minHeap.pop();
                counts[val]--;
                if (counts[val] == 0) counts.erase(val);
            }
        }
    }

    clean();
    if (maxHeap.empty() || minHeap.empty()) return "EMPTY";
    return to_string(maxHeap.top()) + " " + to_string(minHeap.top());
}

#include <cassert>
#include <string>
#include <vector>
using namespace std;

// Declaration of the function under test (assumed implemented elsewhere).
string doublePriorityQueueSimulator(const vector<string>& operations);

int main() {
    // Basic insert and query max/min.
    assert(doublePriorityQueueSimulator({"I 5", "I 3", "I 8"}) == "8 3");
    // Delete max then min.
    assert(doublePriorityQueueSimulator({"I 5", "I 3", "I 8", "D 1", "D -1"}) == "5 5");
    // Empty after deleting all.
    assert(doublePriorityQueueSimulator({"I 1", "D 1"}) == "EMPTY");
    // Duplicate values handling.
    assert(doublePriorityQueueSimulator({"I 4", "I 4", "D 1", "D 1"}) == "EMPTY");
    // Delete on empty queue ignored.
    assert(doublePriorityQueueSimulator({"D 1", "I 7"}) == "7 7");
    // Clear operation resets state.
    assert(doublePriorityQueueSimulator({"I 2", "I 9", "C", "I 1"}) == "1 1");
    // Mixed operations with duplicates.
    assert(doublePriorityQueueSimulator({"I 10", "I -3", "I 10", "D -1", "D -1"}) == "10 10");
    // Single element after operations.
    assert(doublePriorityQueueSimulator({"I 42", "I 0", "D 1", "D -1"}) == "EMPTY");
    // Negative numbers and multiple deletes.
    assert(doublePriorityQueueSimulator({"I -5", "I -1", "I -5", "D 1", "D 1"}) == "-1 -1");
    // Sequential insert-delete pattern.
    assert(doublePriorityQueueSimulator({"I 3", "D 1", "I 2", "D 1", "I 1"}) == "1 1");
    return 0;
}
