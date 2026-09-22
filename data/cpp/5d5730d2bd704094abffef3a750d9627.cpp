/*
Write a C++ function that simulates a simple linear queue with a fixed capacity of 5 integers using an array, providing three operations: `enqueue(int value)` to add an element at the rear, `dequeue()` to remove the front element, and `display()` to return a string representation of the current queue contents (elements from front to rear separated by single spaces, or `"Queue is empty!"` if empty). The function should take a reference to a queue state (implemented as a struct containing the array, front index, rear index, and capacity) and a command string (e.g., `"enqueue 10"`, `"dequeue"`, `"display"`) as input, and return the output that would be printed by the operation (e.g., `"Inserted 10"` for enqueue, `"Removed 10"` for dequeue, or `"Queue is full!"` when full). Handle edge cases such as attempts to enqueue when full (return `"Queue is full! Cannot enqueue <value>"`), dequeue when empty (return `"Queue is empty! Cannot dequeue"`), and resetting front and rear to -1 when the queue becomes empty after dequeue. The function must maintain the queue state across multiple calls, so it should modify the passed struct in-place.
*/

#include <string>
#include <sstream>

struct QueueState {
    int data[5];
    int front;
    int rear;
    int capacity;
};

// Process a queue command and return the resulting output string.
std::string processQueueCommand(QueueState& q, const std::string& command) {
    std::istringstream iss(command);
    std::string operation;
    iss >> operation;

    if (operation == "enqueue") {
        int value;
        iss >> value;
        if (q.rear == q.capacity - 1) {
            return "Queue is full! Cannot enqueue " + std::to_string(value);
        }
        if (q.front == -1) {
            q.front = 0;
        }
        q.rear++;
        q.data[q.rear] = value;
        return "Inserted " + std::to_string(value);
    } else if (operation == "dequeue") {
        if (q.front == -1) {
            return "Queue is empty! Cannot dequeue";
        }
        int removed = q.data[q.front];
        q.front++;
        if (q.front > q.rear) {
            q.front = -1;
            q.rear = -1;
        }
        return "Removed " + std::to_string(removed);
    } else if (operation == "display") {
        if (q.front == -1) {
            return "Queue is empty!";
        }
        std::string result = "Queue elements:";
        for (int i = q.front; i <= q.rear; ++i) {
            result += " " + std::to_string(q.data[i]);
        }
        return result;
    }
    return "Unknown command";
}

#include <cassert>
#include <string>

int main() {
    QueueState q = {{0}, -1, -1, 5};

    assert(processQueueCommand(q, "enqueue 10") == "Inserted 10");
    assert(processQueueCommand(q, "enqueue 20") == "Inserted 20");
    assert(processQueueCommand(q, "enqueue 30") == "Inserted 30");
    assert(processQueueCommand(q, "display") == "Queue elements: 10 20 30");
    assert(processQueueCommand(q, "dequeue") == "Removed 10");
    assert(processQueueCommand(q, "display") == "Queue elements: 20 30");
    assert(processQueueCommand(q, "dequeue") == "Removed 20");
    assert(processQueueCommand(q, "dequeue") == "Removed 30");
    assert(processQueueCommand(q, "dequeue") == "Queue is empty! Cannot dequeue");
    assert(processQueueCommand(q, "display") == "Queue is empty!");

    // Test full condition
    q = {{0}, -1, -1, 5};
    processQueueCommand(q, "enqueue 1");
    processQueueCommand(q, "enqueue 2");
    processQueueCommand(q, "enqueue 3");
    processQueueCommand(q, "enqueue 4");
    processQueueCommand(q, "enqueue 5");
    assert(processQueueCommand(q, "enqueue 6") == "Queue is full! Cannot enqueue 6");
    assert(processQueueCommand(q, "display") == "Queue elements: 1 2 3 4 5");

    return 0;
}

// The solution models a linear queue using a struct that holds a fixed-size array, the current front and rear indices, and the capacity. For each command, parse the input string: if it starts with `"enqueue"`, extract the integer value and check whether `rear == capacity - 1`; if full, return the full message; otherwise, if the queue is empty (front == -1), set front to 0, increment rear, store the value, and return `"Inserted <value>"`. For `"dequeue"`, if front == -1 (empty), return the empty message; otherwise, capture the value at front, increment front, and if front now exceeds rear, reset both to -1 (empty), then return `"Removed <value>"`. For `"display"`, if empty, return `"Queue is empty!"`; otherwise, build a string by iterating from front to rear and appending each element with spaces. Edge cases include enqueueing after a dequeue (the linear queue does not reuse space, so full condition is based solely on rear reaching capacity), dequeueing until empty (must reset indices), and displaying an empty queue. Time complexity is O(1) for enqueue and dequeue, O(n) for display where n is the number of elements. Space complexity is O(capacity) for the array plus O(n) for the returned display string.
