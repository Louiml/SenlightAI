// Write a C++ function named `describeStackOperations` that takes a vector of integers as input, simulates a stack using a singly linked list (you may reuse the exact `Node` and `Stack` class design from the given snippet but with a public interface), and returns a string that represents the stack's state after the following sequence: push all elements in the given order, then pop exactly two elements (if possible; if fewer than two elements are present, pop only as many as exist), and finally output the stack contents from top to bottom separated by " -> " (no trailing arrow). If the stack becomes empty after the pops, return the string "empty". Additionally, the function must handle edge cases: if the input vector is empty, return "empty"; if after pushing and popping the stack has exactly one element, return just that number as a string. The function must be `const`-correct in the sense that it should not modify the input vector and should use only the stack's public methods (`push`, `pop`, `peek`, `isEmpty`). Do not modify the provided class definitions; your function should create its own private `Stack` instance internally.

// The solution approach: Create a local `Stack` object. For each integer in the input vector, call `push` to insert it. After pushing all elements, determine how many pops to perform: the minimum of 2 and the current stack size. To find the size, we need to know how many elements are in the stack — but the `Stack` class from the snippet does not expose a size method. Since we are not allowed to modify the class, we can count pops safely by using `isEmpty()` before each pop attempt: if the stack is not empty, pop; do this at most twice. After performing the pops, if the stack is empty, return "empty". Otherwise, build the result by calling `peek` to get the top, then pop repeatedly (but we need to keep the original elements for display). Since we cannot traverse the stack without destroying it, we can pop all elements into a temporary vector, then rebuild the stack back (or directly construct the output string in reverse). The simplest is to pop all remaining elements into a vector, then reconstruct the string from that vector in reverse order (since popping gives top first). But to maintain the original stack state? The function only outputs; it does not need to preserve the stack. So: after the pops, if not empty, create an empty result string; while the stack is not empty, take `peek`, pop, store that integer into a vector. After the loop, the vector contains elements from top to bottom. Then `reverse` the vector to get bottom-to-top, then output from top to bottom (the reversed order) — actually easier: we don't need to reverse; we can concatenate the integers in the order they were popped (which is top to bottom) directly into the output string, because the display expects top to bottom. So just pop and append each value, with " -> " separators. That gives the correct order. Edge cases: empty input -> return "empty"; input of size 1 -> push one, pops: we try to pop twice but only one pop happens because after first pop stack becomes empty, second pop check fails; then after that stack is empty, return "empty". Wait, the specification says "pop exactly two elements (if possible; if fewer than two elements are present, pop only as many as exist)". So for a single element, we pop one, then stack is empty, so return "empty". For two elements, pop both, return "empty". For three or more, pop exactly two, leaving the rest. That matches. Time complexity: O(n) for pushing n elements, O(k) for popping where k ≤ 2 plus O(m) for output where m is remaining size, total O(n). Space: O(n) for the internal stack, plus O(m) for the temporary vector for output, so O(n) auxiliary space.

#include <string>
#include <vector>
#include <stdexcept>

// Node class for singly linked list (as given in the snippet)
class Node {
public:
    int data;
    Node* link;
    Node(int n) : data(n), link(nullptr) {}
};

// Stack class using singly linked list (public interface only)
class Stack {
    Node* top;
public:
    Stack() : top(nullptr) {}
    ~Stack() {
        while (!isEmpty()) pop();
    }
    void push(int data) {
        Node* temp = new Node(data);
        temp->link = top;
        top = temp;
    }
    bool isEmpty() const { return top == nullptr; }
    int peek() const {
        if (isEmpty()) throw std::runtime_error("Stack is empty");
        return top->data;
    }
    void pop() {
        if (isEmpty()) throw std::runtime_error("Stack is empty");
        Node* temp = top;
        top = top->link;
        delete temp;
    }
};

// Function to simulate stack operations and describe final state
std::string describeStackOperations(const std::vector<int>& values) {
    Stack s;
    // Push all elements
    for (int v : values) {
        s.push(v);
    }
    // Pop up to two elements
    for (int i = 0; i < 2 && !s.isEmpty(); ++i) {
        s.pop();
    }
    // If stack is empty, return "empty"
    if (s.isEmpty()) {
        return "empty";
    }
    // Build result from top to bottom
    std::string result;
    bool first = true;
    while (!s.isEmpty()) {
        if (!first) result += " -> ";
        result += std::to_string(s.peek());
        s.pop();
        first = false;
    }
    return result;
}

#include <cassert>
#include <string>
#include <vector>

// Assume the solution function is already defined above
int main() {
    // Empty input
    assert(describeStackOperations({}) == "empty");

    // Single element: push then pop one (since we pop up to 2), stack empty
    assert(describeStackOperations({5}) == "empty");

    // Two elements: push 1,2 -> stack top 2, then pop two -> empty
    assert(describeStackOperations({1, 2}) == "empty");

    // Three elements: 1,2,3 -> top 3, pop 3 and 2, left 1 -> output "1"
    assert(describeStackOperations({1, 2, 3}) == "1");

    // Four elements: 1,2,3,4 -> top 4, pop 4 and 3, left 2,1 -> output "2 -> 1"
    assert(describeStackOperations({1, 2, 3, 4}) == "2 -> 1");

    // Negative numbers and larger stack
    assert(describeStackOperations({-1, 0, 5, 10, 20}) == "10 -> 5 -> 0 -> -1");

    // Duplicates
    assert(describeStackOperations({7, 7, 7}) == "7");

    // Large stack, many elements, check first few after pops
    std::vector<int> big(100);
    for (int i = 0; i < 100; ++i) big[i] = i;
    // After pushing 0..99, top is 99, pop 99 and 98, left 97 down to 0
    // Expected top is 97, then 96, ... down to 0
    std::string expected;
    for (int i = 97; i >= 0; --i) {
        if (i != 97) expected += " -> ";
        expected += std::to_string(i);
    }
    assert(describeStackOperations(big) == expected);

    return 0;
}
