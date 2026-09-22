Write a C++ function `int circularQueueOperation(int capacity, const std::vector<int>& commands)` that simulates a circular queue of fixed capacity `capacity` (initialized empty) and processes a sequence of encoded commands. Each command is an integer: a non-negative value `x` represents enqueueing `x` (returns 1 if successful, 0 if the queue is full), and the value `-1` represents dequeueing (returns the dequeued element, or -1 if the queue is empty). The function should process all commands in order, accumulate the results of each command (each result being 1/0 for enqueue or the dequeued value for dequeue) into a single running sum, and return that final sum. The queue must operate with circular indexing, overwriting logical slots to maintain constant-time enqueue/dequeue, and must correctly handle edge cases: empty queue, full queue, wrapping around the end of the underlying array, and a single-element queue.

// The solution simulates a circular queue using a fixed-size integer array (or vector) of length `capacity`, plus two indices: `front` (index of the first element) and `rear` (index of the last element). Initially `front = rear = -1` to indicate emptiness. For enqueue: first check overflow — the queue is full when `(front == 0 && rear == capacity-1)` (normal full) or when `rear == (front-1) % (capacity-1)` (wrapped full, but note the modulo with `capacity-1` is tricky; correct condition is `(rear + 1) % capacity == front`). However, the given snippet uses a simpler but flawed check; for a robust solution we use the standard `(rear + 1) % capacity == front`. If full, return 0 (not 1). If empty, set both `front` and `rear` to 0. If `rear` is at last index but front not 0, wrap `rear` to 0; else increment `rear`. Then store value. For dequeue: if `front == -1`, return -1. Save the value at `front`, clear it (optional). If `front == rear` (single element), set both to -1. Else if `front == capacity-1`, wrap front to 0; else increment `front`. Return the saved value. Accumulate each result into a sum (enqueue returns 1 if success, 0 if fail; dequeue returns the popped value or -1 if empty). Time complexity is O(number of commands) since each operation is O(1), and space complexity is O(capacity) for the storage array. Edge cases: capacity 0 (immediately full/empty? typically capacity>0 for a queue; handle by returning 0 for any enqueue and -1 for any dequeue), wrap-around, alternating enqueue/dequeue, and filling fully.

#include <vector>
#include <cstddef>

// Simulate a circular queue and return the sum of all operation results.
// Commands: non-negative value = enqueue (result 1 on success, 0 on failure), -1 = dequeue (result popped value or -1 if empty).
int circularQueueOperation(int capacity, const std::vector<int>& commands) {
    if (capacity < 0) {
        return 0; // invalid capacity, per problem definition assume non-negative
    }
    
    std::vector<int> arr(capacity, 0); // underlying storage
    int front = -1;
    int rear = -1;
    int total = 0;
    
    for (int cmd : commands) {
        if (cmd >= 0) { // enqueue
            bool isFull;
            if (capacity == 0) {
                isFull = true;
            } else {
                isFull = (front == 0 && rear == capacity - 1) || (rear != -1 && (rear + 1) % capacity == front);
            }
            if (isFull) {
                total += 0; // failed push
                continue;
            }
            if (front == -1) { // empty
                front = rear = 0;
            } else if (rear == capacity - 1 && front != 0) {
                rear = 0;
            } else {
                rear++;
            }
            arr[rear] = cmd;
            total += 1; // successful push
        } else { // dequeue (cmd == -1)
            if (front == -1) { // empty
                total += -1;
                continue;
            }
            int value = arr[front];
            arr[front] = -1; // optional clear
            if (front == rear) { // single element
                front = rear = -1;
            } else if (front == capacity - 1) {
                front = 0;
            } else {
                front++;
            }
            total += value;
        }
    }
    return total;
}

#include <cassert>
#include <vector>

// Forward declaration of the function under test (or include the header)
int circularQueueOperation(int capacity, const std::vector<int>& commands);

int main() {
    // Basic single enqueue
    assert(circularQueueOperation(3, {5}) == 1);
    
    // Enqueue then dequeue
    assert(circularQueueOperation(3, {5, -1}) == 1 + 5); // 6
    
    // Empty dequeue returns -1
    assert(circularQueueOperation(3, {-1}) == -1);
    
    // Full queue rejects enqueue
    assert(circularQueueOperation(2, {1, 2, 3}) == 1 + 1 + 0);
    
    // Wrap-around: capacity 3, enqueue 1,2,3, dequeue, enqueue 4
    assert(circularQueueOperation(3, {1, 2, 3, -1, 4}) == 1 + 1 + 1 + 1 + 1); // first three pushes + pop returns 1 + push 4 returns 1 = 5? Wait pop returns 1, so sum = 1+1+1+1+1=5
    assert(circularQueueOperation(3, {1, 2, 3, -1, 4}) == 5);
    
    // Single element queue dequeue empties it, then enqueue works
    assert(circularQueueOperation(1, {10, -1, 20}) == 1 + 10 + 1);
    
    // Mixed operations with wrap-around and full state
    assert(circularQueueOperation(4, {1,2,-1,3,4,5,-1,-1}) == 1+1+1+1+1+0+2+3);
    
    // Capacity 0: enqueue fails, dequeue fails
    assert(circularQueueOperation(0, {1, -1, 2}) == 0 + (-1) + 0);
    
    // Empty after all dequeue operations
    assert(circularQueueOperation(2, {-1, -1}) == -1 + -1);
    
    // Only wrap-around full detection
    assert(circularQueueOperation(2, {1,2,3}) == 1+1+0);
    
    return 0;
}
