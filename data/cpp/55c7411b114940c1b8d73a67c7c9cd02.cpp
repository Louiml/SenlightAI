// Write a C++ function `processCommands(const std::vector<int>& commands)` that simulates a simple stack-based command processor. The input is a vector of integers where each integer encodes a command: `2` means "push the next value from the input onto the stack", `1` means "pop and return the top value", and any other number means "pop and return the top value". However, due to the linear format, the commands are given as: for each element, if it equals 2, then the next element is the value to push (and that next element is consumed as data, not as a command). If it equals 1 or any other integer, it is a pop command. If a pop command is issued when the stack is empty, the function should output/record the phrase `"No Code"` for that operation instead of returning a value. The function should return a `std::vector<std::string>` containing the results of each pop operation in order (including `"No Code"` when appropriate). The vector may contain any integers, but the first command is always a valid command (i.e., the vector is non-empty and for every `2`, there is a valid next element). For simplicity, ignore invalid indices. The function must be pure and not use any global state.

The solution processes the input vector linearly with an index pointer. Maintain a `std::stack<int>` to store pushed values. Iterate through the vector using an index `i` from 0 to size-1. For each element `commands[i]`:
- If it equals 2, push the next element `commands[i+1]` onto the stack and increment `i` by an extra step (skip the data value). 
- Otherwise (any other integer including 1), treat it as a pop command. If the stack is empty, append `"No Code"` to the result vector. Else, append `std::to_string(s.top())` and pop.
Edge cases: repeated pops on empty stack produce `"No Code"` each time. Consecutive push commands are handled correctly by skipping data. A `2` at the end of the vector is not allowed by the problem statement (guaranteed valid), but code can still handle gracefully by checking bounds (though not required). Time complexity is O(n) where n is the size of the input vector, because each element is visited at most once (skipping data elements). Space complexity is O(n) for the stack in the worst case (all pushes) and O(p) for the result vector where p is number of pops.

#include <vector>
#include <stack>
#include <string>

// Process a sequence of stack commands.
// 2 means push next value; any other number means pop (return top or "No Code" if empty).
// Returns a vector of strings with each pop result in order.
std::vector<std::string> processCommands(const std::vector<int>& commands) {
    std::vector<std::string> results;
    std::stack<int> s;
    size_t i = 0;
    const size_t n = commands.size();

    while (i < n) {
        if (commands[i] == 2) {
            // Push the next value
            if (i + 1 < n) {
                s.push(commands[i + 1]);
                i += 2; // skip the data element
            } else {
                break; // invalid but safe
            }
        } else {
            // Pop command (1 or any other number)
            if (s.empty()) {
                results.push_back("No Code");
            } else {
                results.push_back(std::to_string(s.top()));
                s.pop();
            }
            ++i;
        }
    }
    return results;
}

#include <cassert>
#include <vector>
#include <string>

// The solution function is defined above. Test it below.

int main() {
    // Basic push and pop
    assert(processCommands({2, 10, 1}) == std::vector<std::string>{"10"});
    // Empty pop
    assert(processCommands({1}) == std::vector<std::string>{"No Code"});
    // Multiple empties
    assert(processCommands({1, 1}) == std::vector<std::string>{"No Code", "No Code"});
    // Push multiple, pop in LIFO order
    assert(processCommands({2, 1, 2, 2, 3, 1, 1}) == std::vector<std::string>{"1", "3", "2"});
    // Pop with empty after a push but not enough pops
    assert(processCommands({2, 5, 1, 1}) == std::vector<std::string>{"5", "No Code"});
    // Only push commands -> no pops, empty result
    assert(processCommands({2, 1, 2, 2}) == std::vector<std::string>{});
    // Mixed with non-2 pop-like numbers (e.g., 0 treated as pop)
    assert(processCommands({2, 7, 0, 1}) == std::vector<std::string>{"7", "No Code"});
    // Large sequence stress
    std::vector<int> input = {2, 100, 1, 2, 200, 1, 2, 300, 1, 1};
    assert(processCommands(input) == std::vector<std::string>{"100", "200", "300", "No Code"});
    return 0;
}
