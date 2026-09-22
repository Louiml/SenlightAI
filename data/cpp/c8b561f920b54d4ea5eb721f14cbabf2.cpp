Write a C++ function `simulateTwoStacks` that takes an integer `capacity` and a vector of pairs of the form `{operation, value}`, where each operation is either `"push1"`, `"push2"`, `"pop1"`, or `"pop2"` (for each push, `value` is the integer to push; for each pop, ignore `value`). The function should simulate two stacks growing from opposite ends of a single shared array of size `capacity` (stack1 grows from index 0 upward, stack2 grows from size-1 downward). It must return a vector of strings: after every operation, append a string containing the current array contents separated by spaces (if an operation causes overflow or underflow, still append the array as it is, but do not modify the array; print the overflow/underflow message to `std::cerr`). The final returned vector should include one entry per operation, in order. Ensure proper checking for overflow (when the two stack tops would cross) and underflow (popping from an empty stack).
The core idea is to maintain two indices: `top1` starts at -1 and increments on `push1`, `top2` starts at `capacity` and decrements on `push2`. The shared array has size `capacity`. Overflow occurs on a push when `top2 == top1 + 1` — i.e., there is no gap left. Underflow occurs on a pop when `top1 == -1` for stack1 or `top2 == capacity` for stack2. For each operation, we update the array and then format the entire array as a space-separated string. The formatting is straightforward: iterate from index 0 to capacity-1 and concatenate integer values. Edge cases include: pushing when only one slot remains (should succeed but then next push fails), popping from an empty stack (do nothing, print message), and capacity=0 (all pushes overflow, all pops underflow). Time complexity is O(N * capacity) where N is the number of operations, because each operation formats the whole array. Space complexity is O(capacity + N) for the array and the result vector. The solution should be robust to invalid operations by still appending the current array state.
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

// Simulate two stacks sharing a single array and return the state after each operation.
std::vector<std::string> simulateTwoStacks(int capacity, const std::vector<std::pair<std::string, int>>& operations) {
    std::vector<int> arr(capacity, 0);
    int top1 = -1;
    int top2 = capacity;
    std::vector<std::string> result;

    auto formatArray = [&]() -> std::string {
        std::ostringstream oss;
        for (int i = 0; i < capacity; ++i) {
            if (i > 0) oss << " ";
            oss << arr[i];
        }
        return oss.str();
    };

    for (const auto& op : operations) {
        const std::string& type = op.first;
        int value = op.second;

        if (type == "push1") {
            if (top2 == top1 + 1) {
                std::cerr << "Stack 1 overflow" << std::endl;
            } else {
                ++top1;
                arr[top1] = value;
            }
        } else if (type == "push2") {
            if (top2 == top1 + 1) {
                std::cerr << "Stack 2 overflow" << std::endl;
            } else {
                --top2;
                arr[top2] = value;
            }
        } else if (type == "pop1") {
            if (top1 == -1) {
                std::cerr << "Stack 1 underflow" << std::endl;
            } else {
                arr[top1] = 0;
                --top1;
            }
        } else if (type == "pop2") {
            if (top2 == capacity) {
                std::cerr << "Stack 2 underflow" << std::endl;
            } else {
                arr[top2] = 0;
                ++top2;
            }
        }
        result.push_back(formatArray());
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>

// The solution function is assumed to be declared above.

int main() {
    // Basic push/pop, no errors
    std::vector<std::pair<std::string, int>> ops1 = {{"push1",10},{"push1",30},{"push2",20},{"push2",40},{"push1",50},{"push2",60},{"pop2",0},{"pop1",0}};
    std::vector<std::string> r1 = simulateTwoStacks(5, ops1);
    std::vector<std::string> expected1 = {"10 0 0 0 0", "10 30 0 0 0", "10 30 0 0 20", "10 30 0 40 20", "10 30 50 40 20", "10 30 50 40 20", "10 30 50 0 20", "10 0 50 0 20"};
    assert(r1 == expected1);

    // Overflow and underflow cases, capacity 3
    std::vector<std::pair<std::string, int>> ops2 = {{"push1",1},{"push2",2},{"push1",3},{"push1",4},{"pop1",0},{"pop1",0},{"pop1",0},{"pop2",0},{"pop2",0}};
    std::vector<std::string> r2 = simulateTwoStacks(3, ops2);
    std::vector<std::string> expected2 = {"1 0 0", "1 0 2", "1 3 2", "1 3 2", "1 0 2", "0 0 2", "0 0 2", "0 0 0", "0 0 0"};
    assert(r2 == expected2);

    // Capacity 0
    std::vector<std::pair<std::string, int>> ops3 = {{"push1",5},{"pop2",0}};
    std::vector<std::string> r3 = simulateTwoStacks(0, ops3);
    std::vector<std::string> expected3 = {"", ""};
    assert(r3 == expected3);

    // Empty operations
    std::vector<std::pair<std::string, int>> ops4;
    std::vector<std::string> r4 = simulateTwoStacks(2, ops4);
    assert(r4.empty());
}
