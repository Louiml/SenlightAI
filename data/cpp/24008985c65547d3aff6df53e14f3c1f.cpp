// Write a C++ function named `queueFromTwoStacks` that simulates a FIFO queue using only two LIFO stacks (via `std::stack<int>`). The function should accept a series of operations encoded as a vector of strings, where each string is either `"push X"` (where X is a positive integer), `"pop"`, `"top"` (return the front element without removing it), `"size"` (return the current number of elements), or `"print"` (output the current queue contents from front to back, space-separated, followed by a newline). After processing all operations, the function must return a vector of integers containing the results of all `"top"` and `"size"` operations, in the order they appear. Assume the queue may be empty; for `"top"` on an empty queue, push `-1` to the result vector, and for `"pop"` on an empty queue, simply do nothing. All pushes are valid. Example: for input `{"push 1","push 2","print","pop","top","push 3","size","print"}`, the expected output vector is `{2, 3}` (from top=2 and size=3) and the printed lines would be `1 2`, then after pop `2`, then `2 3`, but the function itself only returns the vector.
#include <cassert>
#include <vector>
#include <string>

int main() {
    // Test 1: Basic operations
    std::vector<std::string> ops1 = {"push 1", "push 2", "push 3", "top", "size", "pop", "top", "size"};
    std::vector<int> res1 = queueFromTwoStacks(ops1);
    assert((res1 == std::vector<int>{1, 3, 2, 2}));

    // Test 2: Empty queue top returns -1
    std::vector<std::string> ops2 = {"top", "size", "pop"};
    std::vector<int> res2 = queueFromTwoStacks(ops2);
    assert((res2 == std::vector<int>{-1, 0}));

    // Test 3: Pushes after pops (outStack has elements, inStack also)
    std::vector<std::string> ops3 = {"push 10", "push 20", "pop", "push 30", "top", "size"};
    std::vector<int> res3 = queueFromTwoStacks(ops3);
    assert((res3 == std::vector<int>{20, 2}));

    // Test 4: All pops after transfer
    std::vector<std::string> ops4 = {"push 5", "push 6", "top", "pop", "pop", "top"};
    std::vector<int> res4 = queueFromTwoStacks(ops4);
    assert((res4 == std::vector<int>{5, -1}));

    // Test 5: Many pushes and top then size
    std::vector<std::string> ops5 = {"push 7", "push 8", "top", "size", "pop", "top", "size"};
    std::vector<int> res5 = queueFromTwoStacks(ops5);
    assert((res5 == std::vector<int>{7, 2, 8, 1}));

    return 0;
}
#include <vector>
#include <string>
#include <stack>
#include <sstream>
#include <iostream>

// Simulate a FIFO queue using two LIFO stacks and process operations.
std::vector<int> queueFromTwoStacks(const std::vector<std::string>& operations) {
    std::stack<int> inStack;   // newest elements on top
    std::stack<int> outStack;  // oldest elements on top (front of queue)

    std::vector<int> results;

    auto moveToOut = [&]() {
        while (!inStack.empty()) {
            outStack.push(inStack.top());
            inStack.pop();
        }
    };

    for (const std::string& op : operations) {
        if (op.rfind("push", 0) == 0) {
            std::istringstream iss(op.substr(5));
            int x;
            iss >> x;
            inStack.push(x);
        } else if (op == "pop") {
            if (outStack.empty()) {
                if (inStack.empty()) continue;  // nothing to pop
                moveToOut();
            }
            outStack.pop();
        } else if (op == "top") {
            if (outStack.empty() && inStack.empty()) {
                results.push_back(-1);
            } else {
                if (outStack.empty()) moveToOut();
                results.push_back(outStack.top());
            }
        } else if (op == "size") {
            results.push_back(static_cast<int>(inStack.size() + outStack.size()));
        } else if (op == "print") {
            // Print queue contents: outStack top->bottom, then inStack bottom->top
            std::stack<int> tempOut = outStack;
            std::stack<int> tempIn = inStack;
            
            // Copy outStack in correct order (front first)
            std::vector<int> frontPart;
            while (!tempOut.empty()) {
                frontPart.push_back(tempOut.top());
                tempOut.pop();
            }
            
            // Reverse inStack to get from bottom to top
            std::stack<int> reversedIn;
            while (!tempIn.empty()) {
                reversedIn.push(tempIn.top());
                tempIn.pop();
            }
            // Now reversedIn has oldest at top; collect in order
            std::vector<int> backPart;
            while (!reversedIn.empty()) {
                backPart.push_back(reversedIn.top());
                reversedIn.pop();
            }
            
            // Combine and print
            for (int val : frontPart) std::cout << val << " ";
            for (int val : backPart) std::cout << val << " ";
            std::cout << std::endl;
        }
    }
    return results;
}
// The solution uses two stacks: `inStack` for enqueue operations and `outStack` for dequeue/front operations. When pushing, we always push onto `inStack` (O(1)). For `pop` or `top`, if `outStack` is empty, we transfer all elements from `inStack` to `outStack` by popping from `inStack` and pushing onto `outStack`, which reverses their order to achieve FIFO. This transfer happens at most once per element, giving amortized O(1) per operation. For `size`, simply return `inStack.size() + outStack.size()`. For `print`, we need to output elements from front to back: first output all elements of `outStack` from top to bottom (they are already in correct order), then output the elements of `inStack` in reverse order (i.e., from bottom to top), because the oldest elements in `inStack` are at the bottom. We do this without modifying the stacks by copying them or using temporary stacks. Time complexity: each push is O(1), each pop/top is amortized O(1), each size is O(1), each print is O(n) where n is the current size. Space complexity: O(n) for the two stacks plus O(n) for the result vector and temporary copies during printing. Edge cases include empty queue for top/pop/print (print on empty outputs an empty line), and when `outStack` is empty but `inStack` has elements during top/pop.
