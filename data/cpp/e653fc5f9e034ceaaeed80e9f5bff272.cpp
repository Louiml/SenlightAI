// Write a C++ function named `restoreStackContents` that takes a `std::stack<int>` by reference and performs a single "peek-and-restore" pass: it must print all elements from top to bottom (each on its own line, in the format `Value: X`), and after printing, the original stack must be left completely unchanged (same size, same order, same top element). The function must not use any other container besides one temporary `std::stack<int>` for reversal, and must not modify the stack before printing. The task is to implement the function with proper `const` correctness where applicable, handle an empty stack gracefully (print nothing and leave it empty), and ensure the stack is restored even if no elements exist. The function should return `void`.
#include <cassert>
#include <stack>
#include <sstream>
#include <iostream>

// The function under test (include the solution code here or in a header)
void restoreStackContents(std::stack<int>& inputStack);

// Helper to create a stack from a vector (bottom to top order)
std::stack<int> makeStack(const std::vector<int>& values) {
    std::stack<int> s;
    for (auto it = values.rbegin(); it != values.rend(); ++it) {
        s.push(*it);
    }
    return s;
}

// Helper to compare two stacks element by element (top to bottom)
bool stacksEqual(const std::stack<int>& a, const std::stack<int>& b) {
    if (a.size() != b.size()) return false;
    std::stack<int> aCopy = a;
    std::stack<int> bCopy = b;
    while (!aCopy.empty()) {
        if (aCopy.top() != bCopy.top()) return false;
        aCopy.pop();
        bCopy.pop();
    }
    return true;
}

int main() {
    // Test 1: normal stack with multiple elements
    std::stack<int> s1 = makeStack({1,2,3,4,5});
    std::stack<int> original1 = s1;
    restoreStackContents(s1);
    assert(stacksEqual(s1, original1));
    assert(s1.top() == 5);

    // Test 2: empty stack
    std::stack<int> s2;
    restoreStackContents(s2);
    assert(s2.empty());

    // Test 3: single element
    std::stack<int> s3 = makeStack({42});
    std::stack<int> original3 = s3;
    restoreStackContents(s3);
    assert(stacksEqual(s3, original3));
    assert(s3.top() == 42);

    // Test 4: negative and duplicate values
    std::stack<int> s4 = makeStack({-5, -5, 0, 10, -5});
    std::stack<int> original4 = s4;
    restoreStackContents(s4);
    assert(stacksEqual(s4, original4));
    assert(s4.top() == -5);

    // Test 5: large stack (10 elements)
    std::stack<int> s5 = makeStack({10,20,30,40,50,60,70,80,90,100});
    std::stack<int> original5 = s5;
    restoreStackContents(s5);
    assert(stacksEqual(s5, original5));
    assert(s5.top() == 100);

    // Test 6: repeated calls on the same stack
    std::stack<int> s6 = makeStack({1,2,3});
    std::stack<int> original6 = s6;
    restoreStackContents(s6);
    restoreStackContents(s6);
    assert(stacksEqual(s6, original6));

    return 0;
}
#include <stack>
#include <iostream>

// Prints all elements of the stack from top to bottom, then restores the
// original stack content exactly. Uses only one temporary stack.
void restoreStackContents(std::stack<int>& inputStack) {
    std::stack<int> tempStack;  // Holds elements while we print

    // Pop all elements, print them, and push into tempStack.
    while (!inputStack.empty()) {
        int value = inputStack.top();
        std::cout << "Value: " << value << "\n";
        inputStack.pop();
        tempStack.push(value);
    }

    // Restore original stack by moving elements back.
    while (!tempStack.empty()) {
        inputStack.push(tempStack.top());
        tempStack.pop();
    }
}
// The core idea is to traverse the stack without destroying it. Since `std::stack` only exposes the top element, we must pop elements to inspect them. To preserve the original order, we use a temporary stack to hold the popped elements. We pop each element from the input stack, print it, and push it onto the temporary stack. After the input stack becomes empty, we transfer all elements back from the temporary stack to the input stack by popping from the temporary and pushing onto the input. This reverses the reversal, restoring the original stack exactly. The main edge case is an empty stack: the while loop simply doesn't execute, and the restoration loop also doesn't execute, so nothing happens. Another edge case is a stack with a single element—the process works identically. Since we only use one additional stack of size `n`, the space complexity is `O(n)`. Each element is popped once and pushed once into the temporary, then popped once and pushed once back, so total operations are `O(n)` time. The function should be `void` and take a non-`const` reference because we temporarily modify the stack (pop and push) even though the final state is unchanged.
