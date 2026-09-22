Write a C++ function that simulates a circular queue using a dynamic array, implementing `enqueue`, `dequeue`, and `display` operations. The function should accept a vector of integers and a maximum queue capacity, process the integers in order (enqueue each one; if the queue is full, dequeue the front element first then enqueue), and return a vector containing the queue's contents from front to back after all insertions. The queue must follow FIFO semantics with circular wrapping, where one slot is left empty to distinguish full from empty. Return an empty vector if no elements are present. The input vector may contain any integers (including negatives and duplicates), and the capacity is guaranteed to be at least 1.

#include <cassert>
#include <vector>

// Function declaration (or include the solution above)
std::vector<int> simulateCircularQueue(const std::vector<int>& input, int capacity);

int main() {
    // Basic FIFO with capacity enough
    assert(simulateCircularQueue({1,2,3}, 5) == std::vector<int>({1,2,3}));
    // Over capacity triggers dequeue
    assert(simulateCircularQueue({1,2,3}, 2) == std::vector<int>({2,3}));
    // Empty input
    assert(simulateCircularQueue({}, 3) == std::vector<int>());
    // Capacity 1: only one element can stay
    assert(simulateCircularQueue({10,20,30}, 1) == std::vector<int>({30}));
    // Large sequence and wrapping
    assert(simulateCircularQueue({1,2,3,4,5}, 3) == std::vector<int>({3,4,5}));
    // Negative values and duplicates
    assert(simulateCircularQueue({-1,-1,-2}, 5) == std::vector<int>({-1,-1,-2}));
    // Exact capacity fill
    assert(simulateCircularQueue({1,2,3,4}, 4) == std::vector<int>({1,2,3,4}));
    // Full then insert more
    assert(simulateCircularQueue({1,2,3,4,5}, 3) == std::vector<int>({3,4,5}));
    // Long sequence with small capacity
    assert(simulateCircularQueue({5,4,3,2,1}, 2) == std::vector<int>({2,1}));
    return 0;
}

#include <vector>

// Simulate a circular queue operations on a sequence of integers.
// Returns the final queue contents from front to back.
std::vector<int> simulateCircularQueue(const std::vector<int>& input, int capacity) {
    if (capacity <= 0) return {};
    
    int size = capacity + 1; // one extra slot to distinguish full from empty
    std::vector<int> q(size, 0);
    int front = 0, rear = 0;
    
    for (int value : input) {
        if ((rear + 1) % size == front) {
            // Queue is full: dequeue front
            front = (front + 1) % size;
        }
        // Enqueue
        rear = (rear + 1) % size;
        q[rear] = value;
    }
    
    // Collect elements from front+1 to rear
    std::vector<int> result;
    if (front != rear) {
        int i = (front + 1) % size;
        do {
            result.push_back(q[i]);
            i = (i + 1) % size;
        } while (i != (rear + 1) % size);
    }
    return result;
}

// The solution models a circular queue using an array of size `capacity+1` (since one slot is kept unused to differentiate empty from full). Maintain two indices: `front` (points to the slot before the first element) and `rear` (points to the last element). Initially both are 0. For each integer in the input vector:
// - If `(rear+1) % size == front`, the queue is full. In that case, dequeue the front element (move `front = (front+1)%size`) to make room.
// - Then enqueue by moving `rear = (rear+1)%size` and storing the value.
// After processing all integers, collect elements from `front+1` up to `rear` (wrapping around) into a result vector. Edge cases: capacity 1 (then the effective size is 2, one element max), empty input (returns empty), full queue with many insertions (each full triggers a dequeue). Time complexity: O(n) for n input integers, plus O(k) to collect the final queue contents, where k≤capacity. Space complexity: O(capacity) for the queue array, excluding the result vector.
