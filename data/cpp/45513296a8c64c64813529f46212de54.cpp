/*
Write a C++ function `simulateStackOperations(istream& input, ostream& output)` that reads a sequence of integers from a given input stream until the sentinel value `-1` is encountered. For each positive integer read, push it onto an integer stack (capacity at most 300). For each `0` read, if the stack is not empty, pop the top value and store it in a pop-result list. After reading `-1`, output the pop-result list in the order they were popped, each followed by a space (with a trailing space after the last popped value if any), then on a new line output `top: ` followed by the current top value of the stack if the stack is not empty, or `top: ` followed by `0` if the stack is empty. The function must not modify the `istream` or `ostream` beyond reading/writing; it must be `const`-correct where applicable. The function should handle edge cases like stack underflow (ignore pop attempts when empty) and no pops at all.
*/

#include <iostream>
#include <vector>
#include <istream>
#include <ostream>

// Simulate stack operations: push on positive, pop on 0, stop at -1.
// Outputs popped values in order, then the current top (0 if empty).
void simulateStackOperations(std::istream& input, std::ostream& output) {
    const int MAX_SIZE = 300;
    int stack[MAX_SIZE];
    int top = -1;
    std::vector<int> popped;

    int value;
    while (input >> value) {
        if (value == -1) {
            break;
        }
        if (value == 0) {
            if (top != -1) {
                popped.push_back(stack[top]);
                --top;
            }
        } else {
            if (top < MAX_SIZE - 1) {
                ++top;
                stack[top] = value;
            }
        }
    }

    for (int v : popped) {
        output << v << " ";
    }
    output << "\n";
    output << "top: ";
    if (top != -1) {
        output << stack[top];
    } else {
        output << 0;
    }
}

#include <sstream>
#include <cassert>

int main() {
    // Test 1: Basic push, pop, and top
    {
        std::istringstream input("5 10 0 7 -1");
        std::ostringstream output;
        simulateStackOperations(input, output);
        assert(output.str() == "10 \ntop: 7");
    }
    // Test 2: Empty stack, pop attempt ignored
    {
        std::istringstream input("0 0 -1");
        std::ostringstream output;
        simulateStackOperations(input, output);
        assert(output.str() == "\ntop: 0");
    }
    // Test 3: Only pushes, no pops
    {
        std::istringstream input("1 2 3 -1");
        std::ostringstream output;
        simulateStackOperations(input, output);
        assert(output.str() == "\ntop: 3");
    }
    // Test 4: Many pops and pushes, check order
    {
        std::istringstream input("5 6 0 0 9 -1");
        std::ostringstream output;
        simulateStackOperations(input, output);
        assert(output.str() == "6 5 \ntop: 9");
    }
    // Test 5: Negative numbers other than -1 are treated as pushes
    {
        std::istringstream input("-3 -1");
        std::ostringstream output;
        simulateStackOperations(input, output);
        assert(output.str() == "\ntop: -3");
    }
    // Test 6: Sentinel as first input
    {
        std::istringstream input("-1");
        std::ostringstream output;
        simulateStackOperations(input, output);
        assert(output.str() == "\ntop: 0");
    }
    // Test 7: Mixed with duplicate pops after empty
    {
        std::istringstream input("0 1 0 2 -1");
        std::ostringstream output;
        simulateStackOperations(input, output);
        assert(output.str() == "1 \ntop: 2");
    }
    // Test 8: Large sequence to ensure capacity not exceeded (but not full here)
    {
        std::istringstream input("10 20 30 40 50 0 0 -1");
        std::ostringstream output;
        simulateStackOperations(input, output);
        assert(output.str() == "50 40 \ntop: 30");
    }
    // Test 9: Pop after sentinel has no effect; sentinel stops reading
    {
        std::istringstream input("1 0 -1 99");
        std::ostringstream output;
        simulateStackOperations(input, output);
        assert(output.str() == "1 \ntop: 0");
    }
    // Test 10: Multiple zeros push and pop alternating
    {
        std::istringstream input("1 0 2 0 3 -1");
        std::ostringstream output;
        simulateStackOperations(input, output);
        assert(output.str() == "1 2 \ntop: 3");
    }
    return 0;
}

// The solution uses a fixed-size array stack with an integer top index. The core algorithm reads integer values from the input stream in a loop. For each read value:
// - If the value is `-1`, break the loop and produce the final output.
// - If the value is `0`, check if the stack is non-empty; if so, pop the top element and append it to a vector that records pop results in order. If the stack is empty, do nothing (ignore the pop attempt).
// - Otherwise (positive or any other value), push the value onto the stack only if the stack is not full (capacity 300). If full, ignore the push (though the problem does not explicitly test full stack, we handle it).
//
// After the loop, write each popped value from the vector to the output stream separated by a space, followed by a newline. Then, if the stack is non-empty, pop the top value (without removing it? Actually the problem as given pops the top and prints it, but does not say to remove it—the original code pops it and prints, which modifies the stack; but the task is to simulate, so we just read the top without popping). The reference solution will just read `s.a[s.t]` if non-empty, otherwise output `0`. The time complexity is O(n) for n input values, and auxiliary space is O(300) for the stack plus O(n) for the pop-result vector in the worst case (if all values are `0` and stack is non-empty). Edge cases include: no values before `-1`, only pushes, only pops when empty, mix of pops and pushes, and reading until `-1` with no trailing newline.
