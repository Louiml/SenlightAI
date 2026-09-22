Write a C++ function named `simulateCircularQueue` that takes an integer `capacity` and a vector of operation strings (each either `"enQueue x"` where x is an integer, or `"deQueue"`) and returns a vector of integers representing the final state of a fixed-capacity circular queue from front to rear. Operations that would exceed capacity (enQueue when full) or underflow (deQueue when empty) are silently ignored. The function must process all operations sequentially, maintaining the circular nature of the queue (i.e., when the rear reaches the end of the underlying storage, it wraps around to the beginning). Return the final queue contents in front-to-rear order. Assume `capacity` is a positive integer and the operations vector is valid (no malformed commands, enQueue always has an integer argument). Do not modify the input vector.

#include <cassert>
#include <vector>
#include <string>

// The solution function is declared above.
int main() {
    // Test 1: Basic fill, dequeue, re-enqueue (example from prompt)
    {
        std::vector<std::string> ops = {"enQueue 1", "enQueue 2", "enQueue 3", "deQueue", "enQueue 4"};
        std::vector<int> result = simulateCircularQueue(3, ops);
        assert(result == std::vector<int>({2, 3, 4}));
    }

    // Test 2: Empty queue dequeue ignored, single element
    {
        std::vector<std::string> ops = {"deQueue", "enQueue 10"};
        std::vector<int> result = simulateCircularQueue(2, ops);
        assert(result == std::vector<int>({10}));
    }

    // Test 3: Capacity 1, fill, dequeue, fill again
    {
        std::vector<std::string> ops = {"enQueue 5", "enQueue 6", "deQueue", "enQueue 7"};
        std::vector<int> result = simulateCircularQueue(1, ops);
        assert(result == std::vector<int>({7}));
    }

    // Test 4: Full queue ignores extra enQueue, multiple deQueue
    {
        std::vector<std::string> ops = {"enQueue 1", "enQueue 2", "enQueue 3", "deQueue", "deQueue", "enQueue 4"};
        std::vector<int> result = simulateCircularQueue(3, ops);
        assert(result == std::vector<int>({3, 4}));
    }

    // Test 5: Wraparound with capacity 4: fill, dequeue twice, fill two more
    {
        std::vector<std::string> ops = {"enQueue 1", "enQueue 2", "deQueue", "enQueue 3", "deQueue", "enQueue 4", "enQueue 5"};
        std::vector<int> result = simulateCircularQueue(4, ops);
        assert(result == std::vector<int>({3, 4, 5}));
    }

    // Test 6: Empty operations return empty
    {
        std::vector<std::string> ops;
        std::vector<int> result = simulateCircularQueue(5, ops);
        assert(result.empty());
    }

    // Test 7: Negative numbers and multiple enqueues after full
    {
        std::vector<std::string> ops = {"enQueue -1", "enQueue -2", "enQueue -3", "enQueue 100", "deQueue", "enQueue -4"};
        std::vector<int> result = simulateCircularQueue(3, ops);
        assert(result == std::vector<int>({-2, -3, -4}));
    }

    // Test 8: Many operations with capacity 2
    {
        std::vector<std::string> ops = {"enQueue 1", "deQueue", "enQueue 2", "deQueue", "enQueue 3", "enQueue 4"};
        std::vector<int> result = simulateCircularQueue(2, ops);
        assert(result == std::vector<int>({3, 4}));
    }

    // Test 9: Exact fill and full ignore
    {
        std::vector<std::string> ops = {"enQueue 1", "enQueue 2", "enQueue 3", "enQueue 4", "deQueue", "enQueue 5"};
        std::vector<int> result = simulateCircularQueue(3, ops);
        assert(result == std::vector<int>({2, 3, 5}));
    }

    // Test 10: Dequeue until empty then enqueue
    {
        std::vector<std::string> ops = {"enQueue 1", "enQueue 2", "deQueue", "deQueue", "deQueue", "enQueue 9"};
        std::vector<int> result = simulateCircularQueue(2, ops);
        assert(result == std::vector<int>({9}));
    }

    return 0;
}

#include <vector>
#include <string>
#include <sstream>

// Simulates a circular queue with fixed capacity.
// Returns the final queue contents from front to rear.
std::vector<int> simulateCircularQueue(int capacity, const std::vector<std::string>& operations) {
    std::vector<int> buffer(capacity);
    int head = 0;
    int tail = 0;
    int count = 0;

    for (const auto& op : operations) {
        std::istringstream iss(op);
        std::string command;
        iss >> command;
        if (command == "enQueue") {
            int value;
            iss >> value;
            if (count < capacity) {
                buffer[tail] = value;
                tail = (tail + 1) % capacity;
                ++count;
            }
        } else if (command == "deQueue") {
            if (count > 0) {
                head = (head + 1) % capacity;
                --count;
            }
        }
    }

    std::vector<int> result;
    result.reserve(count);
    for (int i = 0; i < count; ++i) {
        result.push_back(buffer[(head + i) % capacity]);
    }
    return result;
}

// The task requires simulating a circular queue with O(1) time per operation and O(capacity) space. The optimal approach uses a fixed-size vector (or array) of length `capacity`, and two integer indices: `head` (index of front element) and `tail` (index where next element will be inserted). We also maintain `count` to track the number of elements currently in the queue. For each operation:
// - `enQueue x`: If `count < capacity`, place `x` at `buffer[tail]`, then advance `tail = (tail + 1) % capacity`, and increment `count`. If full, ignore.
// - `deQueue`: If `count > 0`, advance `head = (head + 1) % capacity`, and decrement `count`. If empty, ignore.
//
// After processing all operations, build the result vector by iterating `count` times starting at `head` and wrapping using modulo arithmetic: `result[i] = buffer[(head + i) % capacity]`.
//
// Edge cases: capacity of 1 (enQueue then deQueue works correctly); operations that fill and empty the queue multiple times; ensuring modulo arithmetic handles wraparound correctly when tail or head reaches the end; ignoring enQueue when full and deQueue when empty; empty final state returns an empty vector. Time complexity is O(number of operations + final count) = O(n + capacity) worst-case, but effectively O(n) for n operations since each operation is O(1). Space complexity is O(capacity) for the buffer plus O(count) for the result (which is at most capacity). This is more efficient than a naive vector-based approach that erases from the front (O(n) per deQueue) or a std::deque (which is also O(1) but uses more overhead).
