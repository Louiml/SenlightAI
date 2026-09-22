Write a C++ function named `MinStackHandler` that simulates a stack supporting four operations: `push(int val)`, `pop()`, `top()`, and `getMin()`. The function should accept a vector of string commands and a vector of integer arguments (arguments only for `push`; for other commands, the argument can be ignored). The function must return a vector of integers representing the outputs of `top()` and `getMin()` commands (in order of execution), while ignoring outputs for `push` and `pop`. The stack must support `getMin()` in O(1) time using an auxiliary stack that tracks the minimum at each state. You may assume all operations are valid (never pop or read from an empty stack). The input vectors are guaranteed to be non-empty and of equal length, with commands among exactly "push", "pop", "top", "getMin". For `push`, the corresponding argument integer is used; for other commands, the argument is irrelevant. Return an empty vector if no `top` or `getMin` commands occur.
The core idea is to maintain two stacks simultaneously: a primary stack `st` that holds all elements, and a secondary minimum stack `minst` that stores the minimum value of the stack at each level. On `push(val)`, we push `val` onto `st`, and then push `min(val, minst.top())` onto `minst` (if `minst` is empty, we push `val`). This ensures that `minst.top()` always equals the current minimum of the entire stack. On `pop`, we pop from both stacks, which removes the corresponding minimum entry. `top()` simply returns `st.top()`, and `getMin()` returns `minst.top()`. Edge cases: (1) Empty stack operations are not given, but the implementation would fail if they occur; we rely on validity. (2) Duplicate minimums are handled correctly because we push the same minimum value repeatedly when needed. (3) Negative numbers work naturally since `std::min` handles comparison. Time complexity is O(1) for each operation because we only push/pop from stacks (amortized constant). Space complexity is O(n) for `n` elements currently in the stack, as each element has a corresponding minimum stored. The total auxiliary space over the entire simulation equals the maximum stack size at any point, not the total commands processed.
#include <vector>
#include <string>
#include <stack>
#include <algorithm>
#include <stdexcept>

// Simulates a stack with constant-time minimum retrieval.
// Commands: "push", "pop", "top", "getMin"; arguments are used only for push.
std::vector<int> MinStackHandler(const std::vector<std::string>& commands,
                                 const std::vector<int>& args) {
    std::stack<int> st;
    std::stack<int> minst;
    std::vector<int> results;

    for (size_t i = 0; i < commands.size(); ++i) {
        const std::string& cmd = commands[i];
        if (cmd == "push") {
            int val = args[i];
            st.push(val);
            if (minst.empty()) {
                minst.push(val);
            } else {
                minst.push(std::min(val, minst.top()));
            }
        } else if (cmd == "pop") {
            st.pop();
            minst.pop();
        } else if (cmd == "top") {
            results.push_back(st.top());
        } else if (cmd == "getMin") {
            results.push_back(minst.top());
        } else {
            throw std::invalid_argument("Unknown command");
        }
    }
    return results;
}
#include <cassert>
#include <vector>
#include <string>

// Function declaration (assumes the solution is included above)
std::vector<int> MinStackHandler(const std::vector<std::string>&, const std::vector<int>&);

int main() {
    // Example 1: Basic operations
    {
        std::vector<std::string> cmds = {"push", "push", "getMin", "top", "pop", "getMin"};
        std::vector<int> args = {5, 3, 0, 0, 0, 0}; // args ignored for non-push
        std::vector<int> result = MinStackHandler(cmds, args);
        std::vector<int> expected = {3, 3, 5};
        assert(result == expected);
    }

    // Example 2: Negative and duplicate values
    {
        std::vector<std::string> cmds = {"push", "push", "push", "getMin", "pop", "getMin"};
        std::vector<int> args = {-2, -2, -1, 0, 0, 0};
        std::vector<int> result = MinStackHandler(cmds, args);
        std::vector<int> expected = {-2, -2};
        assert(result == expected);
    }

    // Example 3: No output commands
    {
        std::vector<std::string> cmds = {"push", "push", "pop"};
        std::vector<int> args = {1, 2, 0};
        std::vector<int> result = MinStackHandler(cmds, args);
        assert(result.empty());
    }

    // Example 4: Only getMin after single push
    {
        std::vector<std::string> cmds = {"push", "getMin"};
        std::vector<int> args = {42, 0};
        std::vector<int> result = MinStackHandler(cmds, args);
        std::vector<int> expected = {42};
        assert(result == expected);
    }

    // Example 5: Interleaved operations with increasing values
    {
        std::vector<std::string> cmds = {"push", "push", "getMin", "push", "top", "getMin"};
        std::vector<int> args = {1, 2, 0, 0, 0, 0};
        std::vector<int> result = MinStackHandler(cmds, args);
        std::vector<int> expected = {1, 2, 1};
        assert(result == expected);
    }

    // Example 6: Large values and multiple pops
    {
        std::vector<std::string> cmds = {"push", "push", "push", "getMin", "pop", "getMin", "pop", "getMin"};
        std::vector<int> args = {1000, -1000, 2000, 0, 0, 0, 0, 0};
        std::vector<int> result = MinStackHandler(cmds, args);
        std::vector<int> expected = {-1000, -1000, 1000};
        assert(result == expected);
    }

    return 0;
}
