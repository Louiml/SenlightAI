Write a C++ function that reverses the contents of a `std::stack<int>` in-place, meaning the original stack object is modified so that its top and bottom elements are swapped and the rest are arranged in reverse order. The function must handle any stack size, including empty stacks, and must not use recursion or any other data structure besides temporary local variables (i.e., the solution may use a helper stack but must ultimately restore the same stack object). The function signature should be `void reverseStack(std::stack<int>& st)`.

// The simplest approach is to copy all elements from the input stack into an auxiliary stack, which produces the reversed order because pushing into a stack reverses the sequence. Then assign the auxiliary stack back to the original stack (using a swap or direct assignment). This works for all cases: empty stack (loop body never runs, assignment copies an empty stack), single element (loop pushes one, assignment copies back, no net change), and multiple elements. Edge cases include negative numbers, duplicates, and large sizes (no overflow concerns within standard `int`). The time complexity is O(n) because each of the n elements is pushed and popped once from the auxiliary stack plus the assignment copies n elements (usually O(n) copying or swapping pointers). The auxiliary space is O(n) because the temporary stack holds all elements. This is considered acceptable since the problem explicitly allows a helper stack.

#include <stack>

// Reverse the contents of the given stack in-place.
// The original stack is modified; the bottom element becomes top and vice versa.
void reverseStack(std::stack<int>& st) {
    std::stack<int> helper;
    // Move all elements from st to helper, which reverses their order.
    while (!st.empty()) {
        helper.push(st.top());
        st.pop();
    }
    // Now helper holds the elements in reversed order; copy back to st.
    st = helper;
}

#include <cassert>
#include <stack>
#include <vector>

// Forward declaration (defined elsewhere) – for direct inclusion, place the solution code above.

int main() {
    // Test 1: Basic reversal with multiple elements
    std::stack<int> s1;
    s1.push(1); s1.push(2); s1.push(3); // bottom 1, top 3
    reverseStack(s1);
    std::vector<int> v1;
    while (!s1.empty()) { v1.push_back(s1.top()); s1.pop(); }
    assert((v1 == std::vector<int>{3, 2, 1}));

    // Test 2: Single element remains unchanged
    std::stack<int> s2;
    s2.push(42);
    reverseStack(s2);
    assert(s2.size() == 1 && s2.top() == 42);

    // Test 3: Empty stack stays empty
    std::stack<int> s3;
    reverseStack(s3);
    assert(s3.empty());

    // Test 4: Negative numbers and duplicates
    std::stack<int> s4;
    s4.push(-5); s4.push(-5); s4.push(0); s4.push(10); // bottom -5, top 10
    reverseStack(s4);
    std::vector<int> v4;
    while (!s4.empty()) { v4.push_back(s4.top()); s4.pop(); }
    assert((v4 == std::vector<int>{10, 0, -5, -5}));

    // Test 5: Large size (1000 elements) – only check first/last after reversal
    std::stack<int> s5;
    for (int i = 0; i < 1000; ++i) s5.push(i);
    reverseStack(s5);
    // After reversal, top should be 0, bottom should be 999.
    assert(s5.top() == 0);
    // Pop until only one left (the original bottom becomes top)
    while (s5.size() > 1) s5.pop();
    assert(s5.top() == 999);

    return 0;
}
