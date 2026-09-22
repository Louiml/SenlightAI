// Write a C++ function `int stackOperations(std::vector<int> operations)` that simulates a stack using a singly linked list. The input is a vector of integers where positive values mean `push(value)`, `0` means `pop()`, and `-1` means `top()` (peek). The function should return the sum of all values that are printed by the `top()` operation, but only when the stack is non-empty at that moment. If `top()` is called on an empty stack, ignore it (no value added). If `pop()` is called on an empty stack, ignore it. The function must use a custom `Node` and `Stack` class similar to the snippet, but with proper `const` correctness and no global variables. Handle edge cases like repeated empty operations, pushes after pops, and large sequences.
The solution uses a linked-list-based stack where the head of the list is the top of the stack. We maintain a `Node* top` pointer, initialized to `nullptr`. For each operation:  
- If value > 0, create a new node with that data and link it to the current top (so new node becomes top).  
- If value == 0, check if stack is empty; if not, remove the top node by moving top to the next and freeing the old node.  
- If value == -1, check if stack is empty; if not, add `top->data` to an accumulator and return the accumulator at the end.  

Edge cases:  
- Empty stack operations: `pop` and `top` do nothing if the stack is empty.  
- Many pushes and pops in sequence; memory must be freed properly to avoid leaks.  
- Use `const` on the operations vector reference to avoid copying.  

Time complexity: O(n) for n operations, since each push/pop/top is O(1). Space complexity: O(m) where m is the maximum stack depth (number of nodes allocated at any time).
#include <vector>
#include <cstddef>

class Node {
public:
    int data;
    Node* next;
    Node(int d) : data(d), next(nullptr) {}
};

class Stack {
private:
    Node* topNode;
public:
    Stack() : topNode(nullptr) {}
    
    ~Stack() {
        while (topNode != nullptr) {
            Node* temp = topNode;
            topNode = topNode->next;
            delete temp;
        }
    }
    
    bool isEmpty() const {
        return topNode == nullptr;
    }
    
    void push(int val) {
        Node* newNode = new Node(val);
        newNode->next = topNode;
        topNode = newNode;
    }
    
    void pop() {
        if (isEmpty()) return;
        Node* temp = topNode;
        topNode = topNode->next;
        delete temp;
    }
    
    int top() const {
        return topNode->data;
    }
};

// Simulate stack operations and return sum of all top() values when stack is non-empty.
int stackOperations(const std::vector<int>& operations) {
    Stack s;
    int sum = 0;
    for (int op : operations) {
        if (op > 0) {
            s.push(op);
        } else if (op == 0) {
            s.pop();
        } else if (op == -1) {
            if (!s.isEmpty()) {
                sum += s.top();
            }
        }
    }
    return sum;
}
#include <cassert>
#include <vector>

int main() {
    // Basic push and top
    assert(stackOperations({5, -1}) == 5);
    
    // Push, pop, then top (stack empty)
    assert(stackOperations({5, 0, -1}) == 0);
    
    // Push multiple, top, pop, top
    assert(stackOperations({1, 2, 3, -1, 0, -1}) == 5); // 3 + 2
    
    // Empty operations ignored
    assert(stackOperations({0, 0, -1, 0}) == 0);
    
    // Complex sequence
    assert(stackOperations({10, 20, -1, 30, -1, 0, -1, 0, 0, -1}) == 60); // 20+30+10
    
    // All pushes only
    assert(stackOperations({1, 2, 3}) == 0);
    
    // Push after pops
    assert(stackOperations({1, 2, 0, 3, -1}) == 3);
    
    // Large values
    assert(stackOperations({1000, -1, 2000, -1}) == 3000);
    
    // Mixed with empty top calls
    assert(stackOperations({0, 5, 0, 7, -1, 0, -1}) == 7);
    
    return 0;
}
