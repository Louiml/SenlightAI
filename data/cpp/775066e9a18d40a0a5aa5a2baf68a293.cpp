/*
Write a C++ function that processes a sequence of integers read from standard input until a `0` is encountered (the `0` itself is not processed). Maintain a dynamic vector of integers subject to special insertion/removal rules: when a new integer `x` is read, first check if `x` is already present; if so, remove that existing occurrence and do not insert `x`. Otherwise, check if `x+1` is present; if so, remove that occurrence and do not insert `x`. Otherwise, check if `x-1` is present; if so, remove that occurrence and do not insert `x`. If none of these conditions hold, append `x` to the vector. At the end, if the vector is empty, return the string `"0"`; otherwise, return a string containing all elements of the vector separated by single spaces (with no trailing space). The function should be named `processSequence` and take no arguments (read directly from `std::cin`). The input will contain only valid integers (possibly negative), and the sequence terminator `0` will always appear exactly once as the final input.
*/
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// Processes a sequence of integers ending with 0 and returns the final vector state as a string.
std::string processSequence() {
    std::vector<int> state;
    int value;
    while (std::cin >> value) {
        if (value == 0) break;

        auto it = std::find(state.begin(), state.end(), value);
        if (it != state.end()) {
            state.erase(it);
            continue;
        }

        it = std::find(state.begin(), state.end(), value + 1);
        if (it != state.end()) {
            state.erase(it);
            continue;
        }

        it = std::find(state.begin(), state.end(), value - 1);
        if (it != state.end()) {
            state.erase(it);
            continue;
        }

        state.push_back(value);
    }

    if (state.empty()) return "0";

    std::string result;
    for (size_t i = 0; i < state.size(); ++i) {
        if (i > 0) result += " ";
        result += std::to_string(state[i]);
    }
    return result;
}
#include <cassert>
#include <sstream>

// Redirect cin to test the function with predefined inputs.
void runTest(const std::string& input, const std::string& expected) {
    std::istringstream iss(input);
    std::cin.rdbuf(iss.rdbuf());
    std::string result = processSequence();
    assert(result == expected);
}

int main() {
    runTest("1 2 3 0\n", "1 2 3");
    runTest("1 1 0\n", "");
    runTest("1 2 1 0\n", "");
    runTest("5 4 3 0\n", "3");
    runTest("10 -1 9 0\n", "-1 9");
    runTest("7 8 6 0\n", "7");
    runTest("-2 -1 0\n", "");
    runTest("0\n", "0");
    runTest("3 3 4 3 0\n", "4");
    runTest("1 2 3 4 5 0\n", "1 2 3 4 5");
    return 0;
}
// The solution simulates the described behavior exactly. The main idea is to maintain a vector that stores the current "state" of active numbers. For each input integer `n` (skipping the terminating `0`), we perform three `find` operations in order: first `n`, then `n+1`, then `n-1`. As soon as one of these is found, we erase that single occurrence and move to the next input. Only if none are found do we push `n` to the back. This ensures that each input either removes an existing element or adds a new one. Important edge cases: negative numbers must be handled correctly by `stoi`, and the `find` operations must compare values, not indices. The terminating `0` is not processed, so any existing `0` in the vector (if it was added earlier, e.g., as a negative detection? Actually `0` is never added because it terminates the loop, but a negative value like `-1` could be present, and `n+1` could equal `0` if `n` is `-1`; that is allowed). When erasing, use the iterator returned by `find` to avoid double-search. After processing all inputs, build the output string by iterating through the vector and inserting spaces between elements; if empty, return `"0"`. Time complexity is O(k * m) where k is the number of inputs and m is the current vector size (due to linear `find`), but in practice with small inputs it is fine; worst-case O(k^2) if the vector grows to size k. Space complexity is O(k) for the vector and O(k) for the output string.
