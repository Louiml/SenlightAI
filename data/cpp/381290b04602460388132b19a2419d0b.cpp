// Write a standalone C++ function `bool stackMatchesInitializer(const Stack<int>& s, std::initializer_list<int> seq)` that determines whether the given stack contains exactly the elements in `seq`, but in reverse order (i.e., the top of the stack corresponds to the first element of `seq`). The function must handle empty stacks and empty sequences, and must not modify the stack. Assume the `Stack` class provided in the snippet is available (copy it exactly, including all member functions and the `#include <iostream>` and `#include <stdexcept>` headers). Your solution must use the public interface of `Stack` only (no access to private members) and must not rely on any implementation details.
// The approach is to traverse the stack from top to bottom and compare each element with the corresponding element in the sequence. Since the stack is LIFO, the top is compared to `seq[0]`, the next to `seq[1]`, and so on. If sizes differ, return `false`. Otherwise, loop through the sequence: pop a temporary copy? No—we cannot copy the stack because copy constructor prints “Copy constructor called” and we cannot rely on it. Instead, we can access `Top()` to read the top value without popping, but we need to iterate through all elements. The public interface does not provide iteration except through repeated `Top()` and `Pop()`, which would modify the stack. Therefore, we must create a copy of the stack to iterate. However, the copy constructor is public, so we can use it. Alternatively, we can just use a local copy and pop from that copy. The task explicitly says the function must not modify the original stack, so making a local copy is acceptable. To compare without copying the entire stack multiple times, we make one copy: `Stack<int> temp = s;` (this works because copy constructor is public). Then for each element in `seq`, we check if `temp.IsEmpty()` or `temp.Top() != seq_elem`, return `false`; otherwise `temp.Pop()`. After processing all sequence elements, return `temp.IsEmpty()` (true if both exhausted). Edge cases: empty stack and empty sequence → true; empty stack and non-empty sequence → false; non-empty stack and empty sequence → false (since after loop, `temp` not empty). Time complexity: O(n) where n is the number of elements, space O(n) for the copy.  
// The reference solution includes a helper that copies the stack, and also ensures that we handle the initializer_list iteration properly.
#include <iostream>
#include <stdexcept>

// Assume the Stack class from the snippet is defined here exactly as given.

// Function: compare stack top-to-bottom with a sequence in forward order.
bool stackMatchesInitializer(const Stack<int>& s, std::initializer_list<int> seq) {
    // Create a copy so we can pop without modifying the original.
    Stack<int> temp = s; // copy constructor is public
    // Compare top with first element, then pop, etc.
    for (int value : seq) {
        if (temp.IsEmpty() || !(temp.Top() == value)) {
            return false;
        }
        temp.Pop();
    }
    // If we popped all sequence elements, the copy must be empty now.
    return temp.IsEmpty();
}
#include <cassert>

int main() {
    // Test 1: stack {1,2,3} with top=1? Actually push order matters.
    // Let's build a stack by pushing 3,2,1: top=1, then 2, then 3.
    Stack<int> s1;
    s1.Push(3);
    s1.Push(2);
    s1.Push(1);
    // top-to-bottom: 1,2,3
    assert(stackMatchesInitializer(s1, {1,2,3}) == true);

    // Test 2: same stack, wrong order
    assert(stackMatchesInitializer(s1, {1,3,2}) == false);

    // Test 3: different length
    assert(stackMatchesInitializer(s1, {1,2}) == false);
    assert(stackMatchesInitializer(s1, {1,2,3,4}) == false);

    // Test 4: empty stack vs empty sequence
    Stack<int> s2;
    assert(stackMatchesInitializer(s2, {}) == true);

    // Test 5: empty stack vs non-empty
    assert(stackMatchesInitializer(s2, {5}) == false);

    // Test 6: single element stack
    Stack<int> s3;
    s3.Push(42);
    assert(stackMatchesInitializer(s3, {42}) == true);
    assert(stackMatchesInitializer(s3, {43}) == false);

    // Test 7: duplicate values
    Stack<int> s4;
    s4.Push(7);
    s4.Push(7);
    assert(stackMatchesInitializer(s4, {7,7}) == true);
    assert(stackMatchesInitializer(s4, {7}) == false);

    // Test 8: ensure original stack unchanged after function calls
    assert(s1.Size() == 3 && !s1.IsEmpty() && s1.Top() == 1);
    assert(s4.Size() == 2 && s4.Top() == 7);

    // Test 9: move constructor? Not needed here, but test copy constructor works indirectly
    // (already used in function). Just ensure program runs.

    return 0;
}
