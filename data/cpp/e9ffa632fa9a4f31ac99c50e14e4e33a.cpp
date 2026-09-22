/*
Design a C++ function `stackTopAfterEfficientPush` that, given a vector of integers where each integer is either a positive value to push or `-1` to pop (with `-2` to query the top), processes all operations and returns a vector of integers containing only the top values queried via `-2`. The stack is implemented using only two queues (`std::queue<int>`), and the push operation must be O(1) time. If a pop or top is attempted on an empty stack, the operation returns `-1` for that query and does not modify the stack. The function should mimic the provided snippet's behavior but be generalized to process a sequence of operations. Assume the input vector is non-empty and contains only positive integers, `-1`, or `-2`. The return vector must preserve the order of top queries as they appear in the input.
*/

#include <vector>
#include <queue>
#include <cstddef>

// Process stack operations using two queues. 
// Positive values are pushed, -1 pops, -2 queries top.
// Returns a vector of results for each -2 operation.
std::vector<int> stackTopAfterEfficientPush(const std::vector<int>& operations) {
    std::queue<int> q1;
    std::queue<int> q2;
    std::vector<int> results;

    for (int op : operations) {
        if (op >= 0) {
            // push operation
            q1.push(op);
        } else if (op == -1) {
            // pop operation
            if (q1.empty()) {
                results.push_back(-1); // pop on empty stack returns -1
            } else {
                // Move all but the last element from q1 to q2
                while (q1.size() > 1) {
                    q2.push(q1.front());
                    q1.pop();
                }
                // Remove the last element (the top)
                q1.pop();
                // Move elements back from q2 to q1
                while (!q2.empty()) {
                    q1.push(q2.front());
                    q2.pop();
                }
            }
        } else if (op == -2) {
            // top operation
            if (q1.empty()) {
                results.push_back(-1); // top on empty stack returns -1
            } else {
                // Move all elements to q2, tracking the last one (which is the top)
                int topValue = -1;
                while (!q1.empty()) {
                    topValue = q1.front();
                    q2.push(topValue);
                    q1.pop();
                }
                // Move elements back from q2 to q1
                while (!q2.empty()) {
                    q1.push(q2.front());
                    q2.pop();
                }
                results.push_back(topValue);
            }
        }
    }
    return results;
}

#include <cassert>
#include <vector>

int main() {
    // Basic push and top
    std::vector<int> ops1 = {5, -2};
    assert(stackTopAfterEfficientPush(ops1) == std::vector<int>({5}));

    // Push, pop, top on empty
    std::vector<int> ops2 = {1, -1, -2};
    assert(stackTopAfterEfficientPush(ops2) == std::vector<int>({-1}));

    // Multiple pushes, pops, tops
    std::vector<int> ops3 = {1, 2, 3, -2, -1, -2, -1, -2};
    assert(stackTopAfterEfficientPush(ops3) == std::vector<int>({3, 2, 1}));

    // Empty stack top and pop
    std::vector<int> ops4 = {-2, -1, -2};
    assert(stackTopAfterEfficientPush(ops4) == std::vector<int>({-1, -1}));

    // Single element stack
    std::vector<int> ops5 = {42, -2, -1, -2};
    assert(stackTopAfterEfficientPush(ops5) == std::vector<int>({42, -1}));

    // Alternating pushes and pops, then top
    std::vector<int> ops6 = {1, -1, 2, -1, 3, -2};
    assert(stackTopAfterEfficientPush(ops6) == std::vector<int>({3}));

    // Many pushes and multiple tops
    std::vector<int> ops7 = {9, 8, 7, -2, -2, -2};
    assert(stackTopAfterEfficientPush(ops7) == std::vector<int>({7, 7, 7}));

    // Pop until empty, then top
    std::vector<int> ops8 = {1, 2, -1, -1, -1, -2};
    assert(stackTopAfterEfficientPush(ops8) == std::vector<int>({-1}));

    // Mixed operations with no top queries
    std::vector<int> ops9 = {1, -1, 2, -1};
    assert(stackTopAfterEfficientPush(ops9) == std::vector<int>());

    // Large repeated pattern
    std::vector<int> ops10;
    for (int i = 0; i < 100; ++i) {
        ops10.push_back(i);
        ops10.push_back(-2);
        ops10.push_back(-1);
    }
    std::vector<int> result10 = stackTopAfterEfficientPush(ops10);
    assert(result10.size() == 100);
    for (size_t i = 0; i < 100; ++i) {
        assert(result10[i] == static_cast<int>(99 - i));
    }

    return 0;
}

// The core idea is to maintain a stack using two queues `q1` and `q2`, where the front of `q1` is the top of the stack. Pushing simply enqueues to `q1`, which is O(1). Popping requires moving all but the last element from `q1` to `q2`, then removing the last element (which was the front before the moves? Actually the last element in `q1` is the most recently pushed, so we move all elements except the last one to `q2`, pop that last element, and then move everything back from `q2` to `q1`, preserving order). Similarly, peeking the top transfers all elements to `q2` while tracking the last one (which is the front of `q1` originally? Actually the top is the back of `q1`, so we move all elements to `q2`, remembering the last one moved, then move back). Both `pop` and `top` are O(n) per operation, but `push` is O(1). Edge cases include empty stack: `pop` and `top` return `-1` without changing state. The overall time complexity for processing `n` operations is O(n^2) in the worst case because each `pop`/`top` could be O(size), but the problem statement in the snippet claims O(N) per operation? Actually the snippet says "Time Complexity->O(N)" for the entire class, but that seems misleading; likely they mean O(1) for push and O(N) for pop/top. For the task, we analyze that processing `m` operations where each may trigger a pop/top of a stack of size up to `s` leads to worst-case O(m * s) time. Space complexity is O(s) for the two queues. The key edge cases: empty stack operations, single-element stack, and many alternating push/pop/top operations.
