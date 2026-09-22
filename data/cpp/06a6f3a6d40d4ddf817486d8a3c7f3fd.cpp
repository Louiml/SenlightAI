Design a C++ function that implements a special stack which supports standard `push`, `pop`, `top`, and `isEmpty` operations, but additionally retrieves the minimum element currently in the stack in O(1) time without using extra space beyond the stack itself (i.e., no auxiliary data structure for storing minima). The stack stores integers, and when empty, `pop` returns -1 and `top` returns -1. The function should accept a sequence of operations (as a vector of strings, each like "push 5", "pop", "top", "getMin", "isEmpty") and return a vector of results (for `pop`, `top`, and `getMin` return the integer value or -1 if invalid; for `isEmpty` return 1 if empty else 0). The push command always includes an integer. The function must correctly handle negative numbers, duplicates, and interleaved operations. Do not use any standard library container other than `std::stack` for the main storage (though you may use `std::vector` for the input/output). The solution must be self-contained and not depend on any global state.

// The core trick is the same as in the provided snippet: encode the previous minimum inside the stack when a new minimum is pushed. For each `push(data)`, if the stack is empty, store the data normally and set `mini = data`. Otherwise, if `data < mini`, push `2*data - mini` onto the stack (which will be less than `mini`, thus acting as a flag) and update `mini = data`. If `data >= mini`, push `data` normally. For `pop`, if the top value is greater than `mini`, then it is a normal element and we just pop it and return it. If the top value is less than or equal to `mini` (in practice, less than because a new minimum is always strictly less), then the top value is the encoded value; we recover the previous minimum as `2*mini - top`, set that as the new `mini`, pop the encoded value, and return the old `mini` (which is the actual popped element). For `top`, if the top value is less than `mini`, then it is an encoded minimum, so the actual top is `mini`; otherwise the top is the raw value. `isEmpty` simply checks the underlying stack. Edge cases: when the stack becomes empty, `mini` is irrelevant because the next push will reset it from data. The `top` function must also handle an empty stack (return -1). Time complexity: each operation is O(1). Space complexity: O(n) for the stack, but no extra space for minima tracking. The provided snippet has a bug in `top` (`if(s.top())` should be `if(s.empty())`), so the solution corrects that.

#include <vector>
#include <string>
#include <stack>
#include <sstream>

// Implements a special stack with O(1) getMin, using encoded values in the stack.
std::vector<int> specialStackOperations(const std::vector<std::string>& ops) {
    std::stack<int> s;
    int mini = 0; // placeholder, set on first push
    std::vector<int> results;

    for (const std::string& op : ops) {
        if (op.rfind("push", 0) == 0) {
            // Parse the integer after "push "
            int data = std::stoi(op.substr(5));
            if (s.empty()) {
                s.push(data);
                mini = data;
            } else {
                if (data < mini) {
                    s.push(2 * data - mini);
                    mini = data;
                } else {
                    s.push(data);
                }
            }
        } else if (op == "pop") {
            if (s.empty()) {
                results.push_back(-1);
                continue;
            }
            int curr = s.top();
            s.pop();
            if (curr > mini) {
                results.push_back(curr);
            } else {
                int prev = mini;
                mini = 2 * mini - curr; // recover previous minimum
                results.push_back(prev);
            }
        } else if (op == "top") {
            if (s.empty()) {
                results.push_back(-1);
                continue;
            }
            int curr = s.top();
            if (curr < mini) {
                results.push_back(mini);
            } else {
                results.push_back(curr);
            }
        } else if (op == "getMin") {
            if (s.empty()) {
                results.push_back(-1);
            } else {
                results.push_back(mini);
            }
        } else if (op == "isEmpty") {
            results.push_back(s.empty() ? 1 : 0);
        }
        // Ignore any unknown operation
    }
    return results;
}

#include <cassert>
#include <vector>
#include <string>

int main() {
    // Basic operations
    std::vector<std::string> ops1 = {"push 5", "push 3", "push 7", "getMin", "top", "pop", "getMin", "pop", "pop", "isEmpty"};
    std::vector<int> res1 = specialStackOperations(ops1);
    assert(res1 == std::vector<int>({5, 7, 3, 5, 3, 5, 1})); // getMin=5, top=7, pop=7, getMin=3, pop=3, pop=5, isEmpty=1

    // Negative numbers and duplicates
    std::vector<std::string> ops2 = {"push -2", "push 0", "push -2", "getMin", "pop", "getMin", "pop", "pop", "isEmpty"};
    std::vector<int> res2 = specialStackOperations(ops2);
    assert(res2 == std::vector<int>({-2, -2, -2, 0, -2, 1}));

    // Empty stack operations
    std::vector<std::string> ops3 = {"pop", "top", "getMin", "isEmpty"};
    std::vector<int> res3 = specialStackOperations(ops3);
    assert(res3 == std::vector<int>({-1, -1, -1, 1}));

    // Mixed sequence with pop when min is on top
    std::vector<std::string> ops4 = {"push 10", "push 1", "push 2", "pop", "getMin", "pop", "getMin", "pop", "getMin"};
    std::vector<int> res4 = specialStackOperations(ops4);
    assert(res4 == std::vector<int>({2, 1, 1, 10, 10}));

    // Large values and repeated operations
    std::vector<std::string> ops5 = {"push 1000000", "push -1000000", "getMin", "pop", "getMin", "pop", "isEmpty"};
    std::vector<int> res5 = specialStackOperations(ops5);
    assert(res5 == std::vector<int>({-1000000, -1000000, 1000000, 1}));

    return 0;
}
