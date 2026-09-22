// Implement a C++ function `int popElement(int stack[], int& top, int capacity)` that removes the top element from a fixed-capacity integer stack represented by a raw array and an integer reference for the top index. The function should return the popped value on success, or return `-1` and leave the stack unchanged when the stack is empty (underflow). It must also handle the edge case where `top` is already `-1` without modifying the array or `top`. The function must work correctly for a stack of any capacity greater than zero, and you may assume the stack array is pre-allocated by the caller.

#include <cassert>

int main() {
    // Test 1: Basic pop from a stack with multiple elements
    int stack1[] = {10, 20, 30, 40};
    int top1 = 3;  // top index points to value 40
    int capacity1 = 4;
    assert(popElement(stack1, top1, capacity1) == 40);
    assert(top1 == 2);
    assert(popElement(stack1, top1, capacity1) == 30);
    assert(top1 == 1);
    assert(popElement(stack1, top1, capacity1) == 20);
    assert(top1 == 0);
    assert(popElement(stack1, top1, capacity1) == 10);
    assert(top1 == -1);

    // Test 2: Pop from an empty stack returns -1 and top unchanged
    int stack2[] = {5, 6};
    int top2 = -1;
    int capacity2 = 2;
    assert(popElement(stack2, top2, capacity2) == -1);
    assert(top2 == -1);

    // Test 3: Pop from a stack with one element
    int stack3[] = {42};
    int top3 = 0;
    int capacity3 = 1;
    assert(popElement(stack3, top3, capacity3) == 42);
    assert(top3 == -1);
    assert(popElement(stack3, top3, capacity3) == -1);

    // Test 4: Multiple pops until underflow with duplicate values
    int stack4[] = {7, 7, 7};
    int top4 = 2;
    int capacity4 = 3;
    assert(popElement(stack4, top4, capacity4) == 7);
    assert(top4 == 1);
    assert(popElement(stack4, top4, capacity4) == 7);
    assert(top4 == 0);
    assert(popElement(stack4, top4, capacity4) == 7);
    assert(top4 == -1);
    assert(popElement(stack4, top4, capacity4) == -1);

    return 0;
}

#include <cstddef>  // for size_t (optional, not strictly needed)

// Removes and returns the top element of a stack represented by an array.
// Returns -1 and does nothing if the stack is empty.
// Parameters:
//   stack    - array holding stack elements
//   top      - reference to the current top index (modifies caller's variable)
//   capacity - maximum number of elements the stack can hold (unused in pop but kept for clarity)
int popElement(int stack[], int& top, int capacity) {
    // Underflow check: stack is empty
    if (top == -1) {
        return -1;
    }
    // Retrieve top element and decrement top
    int poppedValue = stack[top];
    top = top - 1;
    return poppedValue;
}

// The solution must check the underflow condition first: if `top == -1`, the stack is empty, so return `-1` without modifying any state. If the stack is not empty, retrieve the value at `stack[top]`, decrement `top` by 1, and return the retrieved value. The function signature uses `int& top` to allow modification of the caller's top index. No other stack operations are needed. The time complexity is O(1) because only constant-time operations are performed. Space complexity is O(1) since no extra storage is used. Edge cases include an empty stack (returns `-1`), a stack with exactly one element (after pop, `top` becomes `-1`), and repeated pops until underflow. The function does not need to handle capacity overflow because only pops are performed.
