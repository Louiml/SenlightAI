// Design a class `MinStack` that supports push, pop, and retrieving the minimum element in constant time. The push operation inserts an integer value onto the stack. The pop operation removes and returns the top element; if the stack is empty, return `INT_MIN` as a sentinel. The `getMin` operation returns the minimum element currently in the stack. Your implementation must achieve `O(1)` time for all three operations, and you must not use any additional container other than the built-in `std::stack`. Provide a free function `simulateMinStack` that takes a vector of strings (commands) and a vector of integer arguments (where irrelevant arguments are 0), executes the commands sequentially, and returns a vector of integers containing the results of `getMin` and `pop` operations (in the order they appear). Commands are exactly one of: `"push"`, `"pop"`, `"getMin"`. For `"push"`, the corresponding argument is the value; for others, it is ignored. The function must use your `MinStack` class internally.

#include <cassert>
#include <vector>
#include <string>

int main() {
    // Basic push, pop, getMin
    std::vector<std::string> c1 = {"push", "push", "getMin", "pop", "getMin"};
    std::vector<int> a1 = {5, 3, 0, 0, 0};
    assert((simulateMinStack(c1, a1) == std::vector<int>{3, 3}));

    // Duplicates and equal values
    std::vector<std::string> c2 = {"push", "push", "push", "getMin", "pop", "getMin", "pop", "getMin"};
    std::vector<int> a2 = {2, 2, 2, 0, 0, 0, 0, 0};
    assert((simulateMinStack(c2, a2) == std::vector<int>{2, 2, 2, 2}));

    // Pop on empty stack returns INT_MIN
    std::vector<std::string> c3 = {"pop", "getMin"};
    std::vector<int> a3 = {0, 0};
    assert((simulateMinStack(c3, a3) == std::vector<int>{INT_MIN, INT_MAX}));

    // Negative numbers
    std::vector<std::string> c4 = {"push", "push", "push", "getMin", "pop", "getMin"};
    std::vector<int> a4 = {-5, -1, -10, 0, 0, 0};
    assert((simulateMinStack(c4, a4) == std::vector<int>{-10, -10}));

    // Push increasing then pop back to min
    std::vector<std::string> c5 = {"push", "push", "push", "getMin", "pop", "pop", "getMin"};
    std::vector<int> a5 = {1, 2, 3, 0, 0, 0, 0};
    assert((simulateMinStack(c5, a5) == std::vector<int>{1, 1, 1}));

    // Empty commands
    std::vector<std::string> c6;
    std::vector<int> a6;
    assert(simulateMinStack(c6, a6).empty());

    // Complex scenario with multiple pushes and pops
    std::vector<std::string> c7 = {"push", "push", "push", "push", "getMin", "pop", "getMin", "pop", "pop", "getMin"};
    std::vector<int> a7 = {4, 5, 1, 2, 0, 0, 0, 0, 0, 0};
    assert((simulateMinStack(c7, a7) == std::vector<int>{1, 1, 2, 4}));

    return 0;
}

#include <vector>
#include <stack>
#include <string>
#include <climits>

class MinStack {
    std::stack<int> stk;
    int minVal = INT_MAX;

public:
    MinStack() {}

    void push(int v) {
        if (v < minVal) {
            stk.push(minVal);
        }
        stk.push(v);
        minVal = std::min(v, minVal);
    }

    int pop() {
        if (stk.empty()) return INT_MIN;
        int elem = stk.top();
        if (elem == minVal) {
            stk.pop();
            minVal = stk.top();
            stk.pop();
        }
        return elem;
    }

    int getMin() const {
        return minVal;
    }
};

// Simulate commands and return results of pop and getMin.
std::vector<int> simulateMinStack(const std::vector<std::string>& commands, const std::vector<int>& args) {
    MinStack ms;
    std::vector<int> results;
    for (size_t i = 0; i < commands.size(); ++i) {
        if (commands[i] == "push") {
            ms.push(args[i]);
        } else if (commands[i] == "pop") {
            results.push_back(ms.pop());
        } else if (commands[i] == "getMin") {
            results.push_back(ms.getMin());
        }
    }
    return results;
}

// The classic approach to support constant-time minimum retrieval uses an auxiliary stack that stores previous minimum values. When pushing a value `v`, if `v` is less than or equal to the current minimum, we first push the current minimum onto the auxiliary stack (or push `v` onto a min-track stack), then update the minimum. When popping, if the popped value equals the current minimum, we restore the previous minimum from the auxiliary stack. In this specific design (mirroring the snippet), we store values in `stk`. On `push(v)`: if `v < currentMin`, push the old minimum onto `stk` before pushing `v`. Then update `currentMin = min(currentMin, v)`. On `pop`: if the stack is empty, return `INT_MIN`. The top element is the value to return. If that top equals `currentMin`, we pop it, then set `currentMin` to the new top (which is the previous minimum), and pop that as well. This ensures that `stk` always contains the actual sequence of elements (with extra sentinel copies of old minima) and `currentMin` is correct after each operation. Edge cases: empty stack pop returns `INT_MIN`; pushing duplicates; pushing values equal to current minimum (use `<=` or `<` consistently—here we use `<` to avoid redundant sentinels but still correct because if equal, min doesn't change). Time complexity per operation is `O(1)`, space complexity is `O(n)` for the stack itself.
