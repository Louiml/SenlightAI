Write a C++ function that simulates a pizza parlor order management system using a circular queue implemented with a fixed-size array. The function should manage a queue of pizza order IDs with a maximum capacity of 5 and support three operations: adding a new order (which fails if the queue is full), serving (removing) the oldest order (which fails if the queue is empty), and displaying all current orders in order from oldest to newest. The function should take a sequence of commands and return a string containing the results of each operation. Commands are given as a single string with commands separated by semicolons: "A" followed by an integer adds that order ID (e.g., "A101" adds order 101), "S" serves the oldest order, and "D" displays the current queue contents. Each operation must produce output: when adding successfully output "Added: <id>", when adding fails output "Queue Full", when serving successfully output "Served: <id>", when serving fails output "Queue Empty", and when displaying output the current orders separated by spaces (or "Empty" if none). Different operation outputs must be separated by newline characters.

The solution uses a circular queue with fixed capacity 5, implemented via an array and two indices `front` and `rear`. Initially both are -1, indicating an empty queue. The queue is full when either `(front == 0 && rear == size-1)` or `front == rear+1` (wrapping). It is empty when both front and rear are -1.  
- **Add**: If full, output "Queue Full". If empty, set front=rear=0 and store the new ID. Otherwise increment rear modulo 5 and store the ID. Output "Added: <id>".  
- **Serve**: If empty, output "Queue Empty". If front == rear (only one element), store the value, reset both to -1, and output "Served: <id>". Otherwise store value at front, increment front modulo 5, and output "Served: <id>".  
- **Display**: If empty output "Empty". Otherwise traverse from front to rear using modulo arithmetic, printing each ID separated by a space.  
The main algorithm parses the command string by splitting on ';'. For each token, if it starts with 'A', parse the integer following it and call add; if it's 'S', call serve; if it's 'D', call display. Accumulate outputs with newlines.  
**Edge cases**: Commands may be out of order (e.g., serving from an empty queue), adding when full, single-element queue wrap-around, and displaying when empty. The capacity is fixed at 5.  
**Complexity**: Each add/serve/display operation runs in O(1) time (display iterates over at most 5 elements, but since the capacity is constant, it's O(1)). Parsing the command string takes O(n) where n is the length of the string. Space usage is O(1) beyond the output string.

#include <string>
#include <vector>
#include <sstream>

// Simulate a pizza parlor with a circular queue of fixed capacity 5.
// Commands separated by ';', where 'A<id>' adds, 'S' serves, 'D' displays.
std::string pizzaParlorSimulation(const std::string& commands) {
    const int size = 5;
    int q[size] = {0};
    int front = -1, rear = -1;
    std::string result;

    // Helper to check if queue is full
    auto isFull = [&]() -> bool {
        return (front == 0 && rear == size - 1) || (front == rear + 1);
    };
    // Helper to check if queue is empty
    auto isEmpty = [&]() -> bool {
        return front == -1 && rear == -1;
    };
    // Helper to add an order
    auto addOrder = [&](int id) {
        if (isFull()) {
            result += "Queue Full\n";
            return;
        }
        if (isEmpty()) {
            front = 0;
            rear = 0;
            q[rear] = id;
        } else {
            rear = (rear + 1) % size;
            q[rear] = id;
        }
        result += "Added: " + std::to_string(id) + "\n";
    };
    // Helper to serve an order
    auto serveOrder = [&]() {
        if (isEmpty()) {
            result += "Queue Empty\n";
            return;
        }
        int served = q[front];
        if (front == rear) {
            front = -1;
            rear = -1;
        } else {
            front = (front + 1) % size;
        }
        result += "Served: " + std::to_string(served) + "\n";
    };
    // Helper to display orders
    auto displayOrders = [&]() {
        if (isEmpty()) {
            result += "Empty\n";
            return;
        }
        std::string line;
        for (int i = front; i != rear; i = (i + 1) % size) {
            line += std::to_string(q[i]) + " ";
        }
        line += std::to_string(q[rear]);
        result += line + "\n";
    };

    // Parse commands
    std::stringstream ss(commands);
    std::string token;
    while (std::getline(ss, token, ';')) {
        if (token.empty()) continue;
        if (token[0] == 'A') {
            int id = std::stoi(token.substr(1));
            addOrder(id);
        } else if (token[0] == 'S') {
            serveOrder();
        } else if (token[0] == 'D') {
            displayOrders();
        }
    }
    // Remove trailing newline if any
    if (!result.empty() && result.back() == '\n') result.pop_back();
    return result;
}

#include <cassert>

int main() {
    // Basic add and serve
    assert(pizzaParlorSimulation("A101;A102;S;D") == "Added: 101\nAdded: 102\nServed: 101\n102");
    // Queue empty on serve
    assert(pizzaParlorSimulation("S") == "Queue Empty");
    // Queue full after 5 adds
    assert(pizzaParlorSimulation("A1;A2;A3;A4;A5;A6") ==
           "Added: 1\nAdded: 2\nAdded: 3\nAdded: 4\nAdded: 5\nQueue Full");
    // Display empty
    assert(pizzaParlorSimulation("D") == "Empty");
    // Wrap-around: add 3, serve 2, add 4
    assert(pizzaParlorSimulation("A1;A2;A3;S;S;A4;A5;A6;D") ==
           "Added: 1\nAdded: 2\nAdded: 3\nServed: 1\nServed: 2\nAdded: 4\nAdded: 5\nAdded: 6\n3 4 5 6");
    // Full after wrap-around
    assert(pizzaParlorSimulation("A1;A2;S;A3;S;A4;A5;A6;A7;D") ==
           "Added: 1\nAdded: 2\nServed: 1\nAdded: 3\nServed: 2\nAdded: 4\nAdded: 5\nAdded: 6\nAdded: 7\n4 5 6 7");
    // Mixed operations with empty and full
    assert(pizzaParlorSimulation("A10;A20;S;S;S;D") ==
           "Added: 10\nAdded: 20\nServed: 10\nServed: 20\nQueue Empty\nEmpty");
    // Single element then serve
    assert(pizzaParlorSimulation("A42;S;D") == "Added: 42\nServed: 42\nEmpty");
    return 0;
}
