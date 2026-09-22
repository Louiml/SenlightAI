// Write a C++ function named `processQueueOperations` that takes a vector of pairs where each pair contains an operation string (`"enqueue"`, `"dequeue"`, `"front"`, `"isEmpty"`) and an integer value (ignored for non-enqueue operations). The function must simulate a queue using a vector-based implementation similar to the provided `Queue` class, but as a free function. For each operation in order, it should: enqueue the given value, dequeue and return -1 if empty, return the front value or -1 if empty, or return 1 if empty and 0 if not. Collect all results (including return values of -1 or 0/1 from isEmpty) into a single vector of integers and return it. The queue must handle wrapping/reuse efficiently by using a front index that advances, but you may choose to simply erase from the vector front (O(n)) or use an index-based approach for O(1) amortized. Ensure the function works correctly with an initially empty queue, duplicate values, and sequences that empty and refill the queue.
The solution simulates a queue using a `std::vector<int>` as the underlying storage with two indices: `frontIdx` and `backIdx` (or a count). Enqueue appends to the vector and increments the back count; dequeue checks emptiness (frontIdx == backIdx) and returns -1 if empty, otherwise returns the element at `frontIdx` and increments `frontIdx`. Front similarly checks emptiness and returns `arr[frontIdx]` or -1. `isEmpty` returns 1 if `frontIdx == backIdx` else 0. A crucial edge case is that after many dequeues, `frontIdx` grows, but we never reclaim memory; to avoid unbounded growth, we can periodically compact the vector when frontIdx becomes large (e.g., if frontIdx > 0 and frontIdx == backIdx, reset both to 0 and clear). Alternatively, use a circular buffer with a fixed maximum size—but since the input size is known, we can preallocate and use modulo indexing. The simplest robust approach: use a `std::deque` or `std::queue` internally, but the task asks for a vector-based implementation similar to the snippet, so we use a vector with two indices and never shrink (or shrink when empty). Time complexity per operation is O(1) amortized for enqueue/dequeue/front, and O(n) overall for the whole sequence. Space complexity is O(number of enqueues) in the worst case if we never compact, but if we clear when empty, it's O(max simultaneous elements).
#include <vector>
#include <string>

// Simulate queue operations on a vector-based queue.
// Operations: "enqueue" (value used), "dequeue" (returns -1 if empty), "front" (returns -1 if empty), "isEmpty" (returns 1/0).
// Returns all results in order.
std::vector<int> processQueueOperations(const std::vector<std::pair<std::string, int>>& operations) {
    std::vector<int> storage;
    int frontIdx = 0;
    std::vector<int> results;

    for (const auto& op : operations) {
        const std::string& cmd = op.first;
        int val = op.second;

        if (cmd == "enqueue") {
            storage.push_back(val);
        } else if (cmd == "dequeue") {
            if (frontIdx == static_cast<int>(storage.size())) {
                results.push_back(-1);
            } else {
                results.push_back(storage[frontIdx++]);
                // Optional compaction: if frontIdx becomes large and queue is empty, reset.
                if (frontIdx == static_cast<int>(storage.size())) {
                    storage.clear();
                    frontIdx = 0;
                }
            }
        } else if (cmd == "front") {
            if (frontIdx == static_cast<int>(storage.size())) {
                results.push_back(-1);
            } else {
                results.push_back(storage[frontIdx]);
            }
        } else if (cmd == "isEmpty") {
            results.push_back(frontIdx == static_cast<int>(storage.size()) ? 1 : 0);
        }
    }

    return results;
}
#include <cassert>
#include <vector>
#include <string>

// Assuming the solution function is declared above.

int main() {
    // Test basic enqueue and dequeue order.
    std::vector<std::pair<std::string, int>> ops1 = {
        {"enqueue", 1}, {"enqueue", 2}, {"dequeue", 0}, {"dequeue", 0}, {"dequeue", 0}
    };
    std::vector<int> res1 = processQueueOperations(ops1);
    assert(res1 == std::vector<int>({1, 2, -1}));

    // Test front and isEmpty.
    std::vector<std::pair<std::string, int>> ops2 = {
        {"isEmpty", 0}, {"enqueue", 5}, {"front", 0}, {"isEmpty", 0}, {"dequeue", 0}, {"isEmpty", 0}, {"front", 0}
    };
    std::vector<int> res2 = processQueueOperations(ops2);
    assert(res2 == std::vector<int>({1, 5, 0, 5, 1, -1}));

    // Test duplicates and refill after empty.
    std::vector<std::pair<std::string, int>> ops3 = {
        {"enqueue", 3}, {"enqueue", 3}, {"dequeue", 0}, {"dequeue", 0}, {"enqueue", 4}, {"front", 0}, {"dequeue", 0}
    };
    std::vector<int> res3 = processQueueOperations(ops3);
    assert(res3 == std::vector<int>({3, 3, 4, 4}));

    // Test many operations and compaction (empty then reuse).
    std::vector<std::pair<std::string, int>> ops4 = {
        {"enqueue", 10}, {"dequeue", 0}, {"enqueue", 20}, {"enqueue", 30}, {"dequeue", 0}, {"dequeue", 0}, {"dequeue", 0}
    };
    std::vector<int> res4 = processQueueOperations(ops4);
    assert(res4 == std::vector<int>({10, 20, 30, -1}));

    // Test operations on empty queue only.
    std::vector<std::pair<std::string, int>> ops5 = {
        {"dequeue", 0}, {"front", 0}, {"isEmpty", 0}
    };
    std::vector<int> res5 = processQueueOperations(ops5);
    assert(res5 == std::vector<int>({-1, -1, 1}));

    return 0;
}
