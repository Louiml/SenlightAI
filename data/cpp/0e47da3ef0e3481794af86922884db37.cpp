Write a C++ class named `MyStack` that implements a stack data structure using only the operations provided by the C++ Standard Library `std::queue`. The class must provide the following public member functions: a default constructor `MyStack()`, `void push(int x)` to push an integer onto the stack, `int pop()` to remove and return the top element, `int top()` to return the top element without removing it, and `bool empty()` to check if the stack is empty. The implementation must rely exclusively on `std::queue` and cannot use other container adaptors like `std::stack`, `std::deque`, or `std::vector`. The functions must behave exactly like a standard LIFO stack: elements are removed in the reverse order of insertion. The task is to implement the class with correct semantics and efficient behavior.
#include <cassert>

int main() {
    MyStack stack;

    assert(stack.empty() == true);

    stack.push(1);
    stack.push(2);
    stack.push(3);

    assert(stack.empty() == false);
    assert(stack.top() == 3);
    assert(stack.pop() == 3);
    assert(stack.top() == 2);

    stack.push(4);
    assert(stack.top() == 4);
    assert(stack.pop() == 4);
    assert(stack.pop() == 2);
    assert(stack.pop() == 1);

    assert(stack.empty() == true);

    // Additional edge test: push multiple, pop all
    MyStack stack2;
    for (int i = 0; i < 100; ++i) {
        stack2.push(i);
    }
    for (int i = 99; i >= 0; --i) {
        assert(stack2.pop() == i);
    }
    assert(stack2.empty() == true);

    return 0;
}
#include <queue>

class MyStack {
private:
    std::queue<int> q;

public:
    MyStack() {}

    void push(int x) {
        q.push(x);
        int n = q.size();
        // Rotate the queue so the last pushed element comes to the front
        for (int i = 0; i < n - 1; ++i) {
            q.push(q.front());
            q.pop();
        }
    }

    int pop() {
        int top = q.front();
        q.pop();
        return top;
    }

    int top() const {
        return q.front();
    }

    bool empty() const {
        return q.empty();
    }
};
// The classic approach to implement a stack using a single queue is to make the `push` operation expensive and `pop`/`top` operations O(1). For each `push(x)`, we enqueue `x` to the back, then rotate the queue by moving all existing elements (except the newly added one) from the front to the back, so the newly added element ends up at the front. This way, the front of the queue always represents the top of the stack. Specifically:
// - `push(x)`: enqueue `x`, then for `i` from 0 to `size-2` (or equivalently, repeat `size-1` times), pop the front and re-enqueue it. After this, `x` is at the front.
// - `pop()`: save the front, pop it, return it.
// - `top()`: return the front without popping.
// - `empty()`: return whether the queue is empty.
// Edge cases: pushing the first element requires no rotation (size is 1, rotation loop runs 0 times). Popping from an empty stack is undefined behavior per the typical LeetCode constraints, but we can assume the caller does not call `pop` or `top` on an empty stack. Time complexity: `push` is O(n) where n is the current number of elements (due to rotation), `pop`, `top`, `empty` are O(1). Space complexity is O(n) to store the elements, same as any container.
