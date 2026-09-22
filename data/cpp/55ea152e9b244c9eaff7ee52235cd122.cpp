Write a C++ function named `reverseStackInto` that takes two `std::stack<int>` references: the first is the source stack, and the second is an initially empty destination stack. The function must recursively pop the top element from the source stack and push it onto the destination stack, effectively reversing the order of the source stack's elements into the destination stack. The source stack should end up empty, and the destination stack should contain the elements of the original source stack in reversed order (i.e., the original bottom element becomes the new top, and the original top element becomes the new bottom). If the source stack is empty, the function should do nothing. The function must be recursive (no iterative loops allowed), and it must not use any additional containers or global variables. The signature is `void reverseStackInto(std::stack<int>& source, std::stack<int>& dest);`. The function should not print, read, or write to standard I/O, and should be safe for any size stack (no recursion-depth constraints other than the system stack limit). The problem is adapted from the snippet provided, which uses a similar recursive pattern but with a fixed hard-coded input, whereas the task requires a general, reusable function.

// The core algorithm is a standard recursive reversal of a stack using a second stack as the output. The base case is when the source stack is empty. In the recursive case, we pop the top element `temp` from the source, push it directly onto the destination, then recursively call the function on the remaining source stack. This works because each recursive call processes one element from the top, and by pushing each element onto the destination as we unwinds, the elements are placed in reverse order relative to the original source. For example, if source is `[bottom...top]` = `[1,2,3]`, then: call 1 pops 3, pushes onto dest (dest now `[3]`), recurses; call 2 pops 2, pushes onto dest (`[3,2]`), recurses; call 3 pops 1, pushes onto dest (`[3,2,1]`), recurses on empty stack -> returns. The result dest is `[3,2,1]` which is the reverse of source. Edge cases: empty source (nothing happens), single-element source (just move that element). Time complexity is O(n) where n is the size of the source stack, because each element is popped and pushed exactly once. Space complexity is O(n) due to the recursion call stack (each recursive call uses stack memory for the current activation record, though the actual auxiliary data is just the temp variable per frame, but the recursion depth is n).

#include <stack>

// Recursively reverse the order of elements from the source stack into the destination stack.
// After the call, source is empty, and dest contains the elements in reversed order.
// The function uses recursion only, no loops or extra containers.
void reverseStackInto(std::stack<int>& source, std::stack<int>& dest) {
    // Base case: if source is empty, do nothing.
    if (source.empty()) {
        return;
    }
    // Pop the top element from the source.
    int topElement = source.top();
    source.pop();
    // Push it directly onto the destination.
    dest.push(topElement);
    // Recursively process the remaining elements.
    reverseStackInto(source, dest);
}

#include <stack>
#include <cassert>

// Function declaration (assume the solution is included above).
void reverseStackInto(std::stack<int>& source, std::stack<int>& dest);

int main() {
    // Test 1: Empty source should remain empty, dest stays empty.
    {
        std::stack<int> src, dst;
        reverseStackInto(src, dst);
        assert(src.empty());
        assert(dst.empty());
    }

    // Test 2: Single element stack.
    {
        std::stack<int> src, dst;
        src.push(42);
        reverseStackInto(src, dst);
        assert(src.empty());
        assert(dst.size() == 1);
        assert(dst.top() == 42);
    }

    // Test 3: Reversal of 1..5 (top is 5, bottom is 1).
    {
        std::stack<int> src, dst;
        for (int i = 1; i <= 5; ++i) {
            src.push(i);
        }
        reverseStackInto(src, dst);
        assert(src.empty());
        // dst should have top = 1, bottom = 5.
        assert(dst.size() == 5);
        int expected[5] = {1, 2, 3, 4, 5}; // from top to bottom after reversal
        for (int i = 0; i < 5; ++i) {
            assert(dst.top() == expected[i]);
            dst.pop();
        }
        assert(dst.empty());
    }

    // Test 4: Reversal with duplicates.
    {
        std::stack<int> src, dst;
        src.push(7);
        src.push(7);
        src.push(9);
        src.push(7);
        reverseStackInto(src, dst);
        assert(src.empty());
        int expected[4] = {7, 9, 7, 7}; // top to bottom
        for (int i = 0; i < 4; ++i) {
            assert(dst.top() == expected[i]);
            dst.pop();
        }
        assert(dst.empty());
    }

    // Test 5: Large stack (100 elements) to verify correctness.
    {
        std::stack<int> src, dst;
        const int N = 100;
        for (int i = 0; i < N; ++i) {
            src.push(i); // top = 99, bottom = 0
        }
        reverseStackInto(src, dst);
        assert(src.empty());
        assert(dst.size() == N);
        for (int i = 0; i < N; ++i) {
            assert(dst.top() == i); // after reversal, top should be 0
            dst.pop();
        }
        assert(dst.empty());
    }

    return 0;
}
