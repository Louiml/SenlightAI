// Write a C++ function named `processStackOperations` that simulates a fixed-capacity integer stack (capacity 4) and processes a string of commands separated by spaces. The commands are `push <value>` to push an integer, `pop` to remove the top element, `show` to output the current stack contents from top to bottom, and `end` to stop processing. The function must return a string containing the results of all operations after each command, each result on its own line, in the order the commands are executed. For `push`, if the stack is full (4 elements), append the line `overflow`; otherwise, append nothing. For `pop`, if the stack is empty, append `underflow`; otherwise, append the popped value as a line. For `show`, if the stack is empty, append `underflow`; otherwise, append each element from top to bottom on its own line. For `end`, terminate processing and return the accumulated string (without any trailing newline). The input string is guaranteed to contain at least one command, and all tokens are valid.
#include <cassert>
#include <string>

// Assume processStackOperations is defined above in the same translation unit.

int main() {
    // Basic push, pop, show, end
    assert(processStackOperations("push 1 push 2 show pop show end") == "2\n1\n2\n1");
    // Empty pop and show
    assert(processStackOperations("pop show end") == "underflow\nunderflow");
    // Overflow on full stack (capacity 4)
    assert(processStackOperations("push 1 push 2 push 3 push 4 push 5 end") == "overflow");
    // After pop, can push again
    assert(processStackOperations("push 1 push 2 push 3 push 4 pop push 5 end") == "4");
    // Multiple shows and complete cycle
    assert(processStackOperations("push 10 show pop push 20 push 30 show end") == "10\n30\n20");
    // End without previous operations
    assert(processStackOperations("end") == "");
    // Push after underflow show
    assert(processStackOperations("show push 7 show end") == "underflow\n7");
    return 0;
}
#include <string>
#include <vector>
#include <sstream>

// Process stack commands and return the accumulated output as a multi-line string.
std::string processStackOperations(const std::string& input) {
    const int SIZE = 4;
    int stack[SIZE];
    int top = -1;
    std::string output;
    
    std::istringstream iss(input);
    std::string token;
    bool running = true;
    
    while (running && iss >> token) {
        if (token == "push") {
            int value;
            if (!(iss >> value)) {
                break; // Malformed input but we assume valid
            }
            if (top == SIZE - 1) {
                if (!output.empty()) output += "\n";
                output += "overflow";
            } else {
                top++;
                stack[top] = value;
            }
        } else if (token == "pop") {
            if (top == -1) {
                if (!output.empty()) output += "\n";
                output += "underflow";
            } else {
                if (!output.empty()) output += "\n";
                output += std::to_string(stack[top]);
                top--;
            }
        } else if (token == "show") {
            if (top == -1) {
                if (!output.empty()) output += "\n";
                output += "underflow";
            } else {
                for (int i = top; i >= 0; --i) {
                    if (!output.empty()) output += "\n";
                    output += std::to_string(stack[i]);
                }
            }
        } else if (token == "end") {
            running = false;
        }
    }
    
    return output;
}
// The solution simulates a classic fixed-size stack with an integer array and a top index. The algorithm processes the input string by splitting it into tokens. When a `push` command is encountered, the next token is read as the value; if `top` is at `SIZE-1`, output `overflow`; otherwise, increment `top` and store the value. For `pop`, if `top` is -1, output `underflow`; otherwise, output the value at `top` and decrement `top`. For `show`, if the stack is empty, output `underflow`; otherwise, iterate from `top` down to 0, outputting each element on a new line. For `end`, break out of the loop. Edge cases include handling an empty stack (for `pop` and `show`), a full stack (for `push`), and ensuring that after `push` we correctly skip the value token. Time complexity is O(n) where n is the total number of tokens (commands plus values), as each token is processed exactly once. Space complexity is O(1) for the stack array (fixed size 4) and the return string grows with the number of output lines, which is at most O(n), but the auxiliary space is O(1) besides the output.
