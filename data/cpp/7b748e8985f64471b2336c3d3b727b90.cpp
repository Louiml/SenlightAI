/*
Write a C++ function named `stackOperationResult` that takes an integer array (`std::vector<int>`) simulating a fixed-capacity stack of size 5, and a string command ("push", "pop", "display", "isEmpty", "isFull") with an optional integer value for push (passed separately). The function should simulate the stack operations exactly as the given snippet intends (but corrected for logical errors) and return a string describing the outcome: for "push", return "Pushed: X" or "Stack is full" if full; for "pop", return "Popped: X" or "Stack is empty" if empty; for "display", return the elements from top to bottom separated by spaces, or "Stack is empty" if empty; for "isEmpty" return "true"/"false"; for "isFull" return "true"/"false". The function must not modify the input vector; it should use a local copy for simulation. Handle edge cases where the stack is empty/full, and ensure the push command validates that the value is added only when there is space.
*/
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>

// Simulate stack operations on a fixed-capacity stack (size 5)
std::string stackOperationResult(const std::vector<int>& initialStack, const std::string& command, int value = 0) {
    const int CAPACITY = 5;
    std::vector<int> stack(CAPACITY, 0);
    int top = -1;
    
    // Copy initial stack contents (assume they are already placed from bottom to top)
    for (int i = 0; i < static_cast<int>(initialStack.size()) && i < CAPACITY; ++i) {
        stack[i] = initialStack[i];
        top = i;
    }
    
    if (command == "push") {
        if (top == CAPACITY - 1) {
            return "Stack is full";
        }
        ++top;
        stack[top] = value;
        return "Pushed: " + std::to_string(value);
    }
    else if (command == "pop") {
        if (top == -1) {
            return "Stack is empty";
        }
        int popped = stack[top];
        --top;
        return "Popped: " + std::to_string(popped);
    }
    else if (command == "display") {
        if (top == -1) {
            return "Stack is empty";
        }
        std::ostringstream oss;
        for (int i = top; i >= 0; --i) {
            if (i != top) oss << " ";
            oss << stack[i];
        }
        return oss.str();
    }
    else if (command == "isEmpty") {
        return (top == -1) ? "true" : "false";
    }
    else if (command == "isFull") {
        return (top == CAPACITY - 1) ? "true" : "false";
    }
    else {
        return "Unknown command";
    }
}
#include <cassert>
#include <vector>
#include <string>

int main() {
    // Test push on empty stack
    assert(stackOperationResult({}, "push", 10) == "Pushed: 10");
    
    // Test pop on empty stack
    assert(stackOperationResult({}, "pop") == "Stack is empty");
    
    // Test push until full (capacity 5)
    std::vector<int> initial = {1, 2, 3, 4, 5};
    assert(stackOperationResult(initial, "push", 6) == "Stack is full");
    
    // Test pop from non-empty
    assert(stackOperationResult({1, 2, 3}, "pop") == "Popped: 3");
    
    // Test display
    assert(stackOperationResult({1, 2, 3}, "display") == "3 2 1");
    
    // Test display empty
    assert(stackOperationResult({}, "display") == "Stack is empty");
    
    // Test isEmpty
    assert(stackOperationResult({}, "isEmpty") == "true");
    assert(stackOperationResult({42}, "isEmpty") == "false");
    
    // Test isFull
    assert(stackOperationResult({1, 2, 3, 4, 5}, "isFull") == "true");
    assert(stackOperationResult({1, 2}, "isFull") == "false");
    
    // Test pushing onto partially filled stack
    assert(stackOperationResult({1, 2}, "push", 99) == "Pushed: 99");
    
    // Test pop until empty and then pop again
    assert(stackOperationResult({7}, "pop") == "Popped: 7");
    assert(stackOperationResult({}, "pop") == "Stack is empty");
    
    // Test unknown command
    assert(stackOperationResult({}, "unknown") == "Unknown command");
    
    return 0;
}
// The solution simulates a stack of fixed capacity 5 using a local `std::vector<int>` copy. We maintain a `top` index initialized to -1. For each command, we check conditions: for "push", if `top == 4` (full) return "Stack is full", else increment top, store the value, and return "Pushed: X". For "pop", if `top == -1` (empty) return "Stack is empty", else return "Popped: X" and decrement top. For "display", if empty return "Stack is empty", else build a string from top down to 0. For "isEmpty" and "isFull", compare `top` with -1 and 4 respectively. Edge cases: pushing when full, popping when empty, display on empty. Complexity: each operation is O(1) except display which is O(k) where k is number of elements, and total time O(n) for n commands; space O(1) auxiliary (only a fixed-size vector of 5).
