Write a C++ function named `queueSimulation` that takes an integer `capacity` and a vector of integers representing a sequence of push operations, and returns a vector of integers representing the results of `front()` calls immediately after each push operation. The function must simulate a fixed-size queue that resets both front and rear indices to 0 when the queue becomes empty after a pop, and the queue's capacity is fixed (no dynamic growth). If a push would exceed capacity, it should be ignored (no output for that push). The function should return a vector where each element corresponds to the front element after each successful push, in order. For example, if `capacity` is 3 and the input pushes are `[1, 2, 3, 4]`, the first three pushes succeed and return `1, 1, 1`, but the fourth push is ignored because the queue is full, so the result is `[1, 1, 1]`. The function should handle empty input and capacity of 0 gracefully.
#include <cassert>
#include <vector>

// The solution function is provided above; this is the test harness.
// (The function is repeated here for completeness in a standalone test.)
std::vector<int> queueSimulation(int capacity, const std::vector<int>& pushes);

int main() {
    // Basic case: capacity 3, 4 pushes, last ignored.
    assert(queueSimulation(3, {1, 2, 3, 4}) == std::vector<int>({1, 1, 1}));
    
    // Empty input.
    assert(queueSimulation(5, {}).empty());
    
    // Capacity 0: no pushes succeed.
    assert(queueSimulation(0, {1, 2, 3}).empty());
    
    // Capacity larger than number of pushes.
    assert(queueSimulation(10, {5, 6, 7}) == std::vector<int>({5, 5, 5}));
    
    // Single push.
    assert(queueSimulation(1, {42}) == std::vector<int>({42}));
    
    // Multiple pushes exactly filling capacity.
    assert(queueSimulation(2, {10, 20}) == std::vector<int>({10, 10}));
    
    // Negative values.
    assert(queueSimulation(4, {-3, -2, -1}) == std::vector<int>({-3, -3, -3}));
    
    // Capacity 1 with multiple pushes: first succeeds, rest ignored.
    assert(queueSimulation(1, {1, 2, 3}) == std::vector<int>({1}));
    
    // Larger sequence.
    assert(queueSimulation(5, {7, 8, 9, 10, 11, 12}) == std::vector<int>({7, 7, 7, 7, 7}));
    
    // All pushes succeed (capacity exactly matches count).
    assert(queueSimulation(5, {1, 2, 3, 4, 5}) == std::vector<int>({1, 1, 1, 1, 1}));
    
    return 0;
}
#include <vector>

// Simulate a fixed-size queue and return the front element after each successful push.
std::vector<int> queueSimulation(int capacity, const std::vector<int>& pushes) {
    std::vector<int> results;
    if (capacity <= 0) {
        return results;
    }
    
    std::vector<int> queue(capacity);
    int front = 0;
    int rear = 0;
    
    for (int value : pushes) {
        if (rear == capacity) {
            // Queue is full; ignore this push.
            continue;
        }
        queue[rear] = value;
        rear++;
        results.push_back(queue[front]);
    }
    
    return results;
}
// The solution simulates a simple array-based queue with two indices: `front` and `rear`, both starting at 0. For each integer in the input vector, we check if `rear` is equal to `capacity` (meaning the queue is full). If it is full, we skip the push and continue. Otherwise, we place the value at index `rear`, increment `rear`, and then read the front element (which is `arr[front]`) and append it to the result vector. Since the queue never pops in this simulation, we do not need to implement the reset logic that appears in the original snippet; however, to remain faithful to the snippet's behavior, we could optionally implement a pop that resets both indices when they become equal, but for this task only pushes are considered. Edge cases include an empty input vector (return an empty vector) and capacity of 0 (no pushes succeed, return empty vector). The time complexity is O(n) where n is the number of push attempts, since each push is O(1). The space complexity is O(n) for the result vector and O(capacity) for the underlying queue array.
