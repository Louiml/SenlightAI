Write a C++ function `processQueueCommands` that takes a vector of strings representing queue commands (`push`, `pop`, `size`, `empty`, `front`, `back`) with optional integer arguments for `push`, and returns a vector of strings containing the output produced by executing those commands on a FIFO (First-In-First-Out) queue. The function must simulate the behavior of a standard queue: `push val` adds `val` to the back, `pop` removes and prints the front element (print `-1` if empty), `size` prints the current number of elements, `empty` prints `1` if empty else `0`, `front` prints the front element (print `-1` if empty), and `back` prints the rear element (print `-1` if empty). Each output should be a separate string in the returned vector, preserving the order of commands. The input commands are guaranteed to be valid (i.e., only the six listed command names, with `push` always followed by an integer). Ensure the implementation uses `std::queue<int>` internally and handles edge cases like popping from an empty queue or querying front/back of an empty queue.
The solution simulates a real queue using `std::queue<int>`. We iterate through the vector of command strings. For each command, we parse the command name (and the integer argument if it's a `push`). We maintain a `std::queue<int> Q` and produce a string output for each command:
- For `push val`: push `val` into the queue, produce no output (empty string or skip adding to result). However, the problem states "Returns a vector of strings containing the output produced by executing those commands"—typically `push` does not produce output, so we simply push and do not add anything to the result vector. For consistency, we can interpret that only commands that print something add to the result. But the specification says "Each output should be a separate string in the returned vector, preserving the order of commands." Since `push` produces no output, it's safe to not add any string for push commands. Alternatively, we could add an empty string, but that would break order comparisons. The natural interpretation: only commands that print (pop, size, empty, front, back) add to the result. The `push` command only modifies the queue. We'll follow that.
- For `pop`: if `Q.empty()` output `-1`, else output `Q.front()` and then `Q.pop()`.
- For `size`: output `Q.size()` as a string.
- For `empty`: output `1` if empty else `0`.
- For `front`: output `Q.front()` if not empty, else `-1`.
- For `back`: output `Q.back()` if not empty, else `-1`.

Edge cases: calling `pop`, `front`, `back` on an empty queue must result in `-1`, never a crash. The queue is initially empty. The commands vector may contain any mixture, including consecutive pushes and pops. Time complexity: each command is processed in O(1) on average, so for `n` commands the total is O(n). Space complexity: O(n) for the queue in the worst case (if all pushes) and O(n) for the result vector.
#include <bits/stdc++.h>

// Simulate queue commands and return the output strings.
// Each command is a string; "push" is followed by an integer in the next element.
std::vector<std::string> processQueueCommands(const std::vector<std::string>& commands) {
    std::queue<int> Q;
    std::vector<std::string> result;
    size_t i = 0;

    while (i < commands.size()) {
        const std::string& cmd = commands[i];

        if (cmd == "push") {
            // Next element is the integer argument
            int val = std::stoi(commands[i + 1]);
            Q.push(val);
            i += 2; // skip push and its argument
        } else {
            if (cmd == "pop") {
                if (Q.empty()) {
                    result.push_back("-1");
                } else {
                    result.push_back(std::to_string(Q.front()));
                    Q.pop();
                }
            } else if (cmd == "size") {
                result.push_back(std::to_string(Q.size()));
            } else if (cmd == "empty") {
                result.push_back(Q.empty() ? "1" : "0");
            } else if (cmd == "front") {
                if (Q.empty()) {
                    result.push_back("-1");
                } else {
                    result.push_back(std::to_string(Q.front()));
                }
            } else if (cmd == "back") {
                if (Q.empty()) {
                    result.push_back("-1");
                } else {
                    result.push_back(std::to_string(Q.back()));
                }
            }
            i++;
        }
    }

    return result;
}
#include <bits/stdc++.h>

// Include the solution function here (or include from a header).

int main() {
    // Test 1: Basic push, front, back, size, empty, pop
    {
        std::vector<std::string> cmds = {"push", "5", "front", "back", "size", "empty", "pop", "empty", "pop"};
        std::vector<std::string> result = processQueueCommands(cmds);
        std::vector<std::string> expected = {"5", "5", "1", "0", "5", "1", "-1"};
        assert(result == expected);
    }

    // Test 2: Pop from empty queue
    {
        std::vector<std::string> cmds = {"pop", "front", "back", "size", "empty"};
        std::vector<std::string> result = processQueueCommands(cmds);
        std::vector<std::string> expected = {"-1", "-1", "-1", "0", "1"};
        assert(result == expected);
    }

    // Test 3: Multiple pushes and pops in FIFO order
    {
        std::vector<std::string> cmds = {"push", "1", "push", "2", "push", "3", "front", "pop", "front", "pop", "back"};
        std::vector<std::string> result = processQueueCommands(cmds);
        std::vector<std::string> expected = {"1", "1", "2", "2", "3"};
        assert(result == expected);
    }

    // Test 4: Only pushes (no output)
    {
        std::vector<std::string> cmds = {"push", "10", "push", "20"};
        std::vector<std::string> result = processQueueCommands(cmds);
        std::vector<std::string> expected = {};
        assert(result == expected);
    }

    // Test 5: Negative numbers and size after pops
    {
        std::vector<std::string> cmds = {"push", "-3", "push", "-7", "front", "pop", "back", "size", "pop", "size"};
        std::vector<std::string> result = processQueueCommands(cmds);
        std::vector<std::string> expected = {"-3", "-3", "-7", "1", "-7", "0"};
        assert(result == expected);
    }

    // Test 6: Mixed operations with empty checks in between
    {
        std::vector<std::string> cmds = {"empty", "push", "42", "back", "front", "pop", "empty"};
        std::vector<std::string> result = processQueueCommands(cmds);
        std::vector<std::string> expected = {"1", "42", "42", "42", "1"};
        assert(result == expected);
    }

    return 0;
}
