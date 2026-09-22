// Write a C++ function `reverseWithStack(const std::string& input)` that returns a new string containing the characters of the input string in reverse order, implemented using an explicit stack (e.g., a fixed-size array or `std::vector`) to simulate a LIFO structure. The function must handle empty strings (returning an empty string), strings of any length up to 1000 characters, and must not modify the input. Assume the input consists of printable ASCII characters (including spaces). Provide constant-correctness by taking the input as `const std::string&`. The solution must use a stack-based reversal algorithm, not the STL `std::reverse` or string constructor iterators.

// The main algorithm is straightforward: push every character of the input string onto a stack in order (index 0 to length-1), then pop characters one by one and append them to a result string. Because the stack is LIFO, the last character pushed is the first one popped, yielding reversed order. For an empty input, the stack remains empty and the result is also empty—no special case needed beyond the loop conditions. Edge cases include spaces (treated as ordinary characters), single-character strings (result equals input), and maximum length (ensure stack capacity is at least the input length). The implementation uses a `std::vector<char>` as the stack to avoid fixed-size limits and to be memory-safe. Time complexity is O(n) for two passes (push and pop), space complexity is O(n) for the stack and result string. The solution avoids using `std::reverse` or string reverse iterators to satisfy the stack-based requirement.

#include <string>
#include <vector>

// Reverse a string using an explicit stack (LIFO).
std::string reverseWithStack(const std::string& input) {
    std::vector<char> stack;
    stack.reserve(input.size());

    // Push all characters onto the stack.
    for (char ch : input) {
        stack.push_back(ch);
    }

    // Pop and append to result.
    std::string reversed;
    reversed.reserve(input.size());
    while (!stack.empty()) {
        reversed.push_back(stack.back());
        stack.pop_back();
    }

    return reversed;
}

#include <cassert>
#include <string>

// Declaration of the function (assume it's in the same translation unit or included).
std::string reverseWithStack(const std::string& input);

int main() {
    assert(reverseWithStack("hello") == "olleh");
    assert(reverseWithStack("") == "");
    assert(reverseWithStack("a") == "a");
    assert(reverseWithStack("abc def") == "fed cba");
    assert(reverseWithStack("!@# $%^") == "^%$ #@!");
    assert(reverseWithStack("   ") == "   ");
    assert(reverseWithStack("12345") == "54321");
    assert(reverseWithStack("racecar") == "racecar");
    assert(reverseWithStack("A b C") == "C b A");
    assert(reverseWithStack(std::string(1000, 'x')) == std::string(1000, 'x'));
    return 0;
}
