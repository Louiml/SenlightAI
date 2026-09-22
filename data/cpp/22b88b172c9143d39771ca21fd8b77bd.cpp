Implement a C++ function that simulates a fixed-size integer queue (maximum capacity 5) using only arrays and manual front/rear indices, without using `std::queue` or dynamic memory. The function must accept a single integer `operation` and an optional value `x`. The operations are encoded as: 1 = enqueue `x`, 2 = dequeue, 3 = peek (return front element without removing), 4 = display all elements as a space-separated string (or "Queue is empty" if empty). The function should return an integer for enqueue/dequeue success (1) or failure (0), an integer for peek (the front value, or a sentinel like INT_MIN if empty), and for display, return a string. To keep the task self-contained, write three separate free functions: `int enqueue(int x)`, `int dequeue()`, `int peek()` that modify a global queue state, and `std::string display()`. The queue must handle the following edge cases: attempts to enqueue when full return 0, attempts to dequeue/peek when empty return 0/INT_MIN respectively, and after the last element is dequeued, the queue resets to an empty state (front = rear = -1). The queue is not circular – it is a simple linear queue where once the rear reaches capacity, no more enqueues are allowed even if elements have been dequeued from the front. Demonstrate the behavior by simulating the exact sequence from the snippet: enqueue 2, enqueue 5, enqueue -1, display, peek, dequeue, peek, display. The solution must use `const` where appropriate and include necessary headers.
#include <cassert>
#include <string>
#include <climits>

// The solution functions are declared here (assume they are defined above).
int enqueue(int x);
int dequeue();
int peek();
std::string display();

int main() {
    // Reset queue state
    front = rear = -1;

    // Simulate the exact sequence from the snippet
    assert(enqueue(2) == 1);
    assert(enqueue(5) == 1);
    assert(enqueue(-1) == 1);
    assert(display() == "2 5 -1");
    assert(peek() == 2);
    assert(dequeue() == 2);
    assert(peek() == 5);
    assert(display() == "5 -1");

    // Test edge cases: queue after all dequeued
    assert(dequeue() == 5);
    assert(dequeue() == -1);
    assert(dequeue() == 0); // empty
    assert(peek() == INT_MIN); // empty
    assert(display() == "Queue is empty");

    // Test overflow: fill queue to capacity
    assert(enqueue(10) == 1);
    assert(enqueue(20) == 1);
    assert(enqueue(30) == 1);
    assert(enqueue(40) == 1);
    assert(enqueue(50) == 1);
    assert(enqueue(60) == 0); // overflow

    // Test dequeue after partial fill (non-circular)
    assert(dequeue() == 10);
    assert(enqueue(60) == 0); // still full because rear == N-1
    assert(display() == "20 30 40 50");

    // Clean up for next tests
    front = rear = -1;

    return 0;
}
#include <string>
#include <climits>
#include <sstream>

const int N = 5;

// Global queue state (kept in a single translation unit)
int queue1[N];
int front = -1;
int rear = -1;

// Enqueue an integer into the queue.
// Returns 1 on success, 0 on overflow (queue full).
int enqueue(int x) {
    if (rear == N - 1) {
        return 0; // Overflow
    }
    if (front == -1 && rear == -1) {
        front = rear = 0;
        queue1[rear] = x;
    } else {
        ++rear;
        queue1[rear] = x;
    }
    return 1;
}

// Dequeue the front element.
// Returns the dequeued value on success, 0 if queue is empty.
int dequeue() {
    if (front == -1 && rear == -1) {
        return 0; // Empty
    }
    int value = queue1[front];
    if (front == rear) {
        front = rear = -1; // Queue becomes empty
    } else {
        ++front;
    }
    return value;
}

// Peek at the front element without removing it.
// Returns the front value, or INT_MIN if queue is empty.
int peek() {
    if (front == -1 && rear == -1) {
        return INT_MIN;
    }
    return queue1[front];
}

// Return a string representation of the queue elements (space-separated),
// or "Queue is empty" if no elements exist.
std::string display() {
    if (front == -1 && rear == -1) {
        return "Queue is empty";
    }
    std::ostringstream oss;
    for (int i = front; i <= rear; ++i) {
        oss << queue1[i] << " ";
    }
    std::string result = oss.str();
    if (!result.empty()) {
        result.pop_back(); // Remove trailing space
    }
    return result;
}
// The solution maintains three global variables: a fixed array `queue1` of size 5, and two integer indices `front` and `rear`. Initially both are -1, indicating an empty queue. Enqueue checks if `rear` equals N-1 (full) – if so, return 0; otherwise if empty (front==-1), set both to 0 and store the value; else increment `rear` and store the value. Dequeue checks if empty (front==-1) – return 0; if only one element (front==rear), capture the value, reset both to -1, and return the value; else increment `front` and return the dequeued value. Peek checks empty – return INT_MIN; else return the element at `front`. Display constructs a string by iterating from `front` to `rear`, appending each element and a space, then trimming trailing space. Edge cases: when queue becomes empty after dequeue, indices reset properly; overflow is detected by `rear==N-1` regardless of `front` position (since it's not circular). Time complexity: enqueue/dequeue/peek are O(1); display is O(k) where k is number of elements currently in queue. Space complexity is O(N) for the fixed array plus O(1) for indices.
