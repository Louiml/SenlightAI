Write a standalone C++ function named `reverseStringWithStack` that takes a non-empty `std::string` as input and returns a new `std::string` containing the characters of the input in reverse order, using a `std::stack<char>` as the primary reversal mechanism. The function must not modify the input string, must handle any printable ASCII characters (including spaces, digits, punctuation), and must preserve the exact character sequence reversed. The function signature is `std::string reverseStringWithStack(const std::string& input)`. Do not include a `main` function in your solution; instead, provide only the function implementation with necessary headers and comments. The function must be `const`-correct and avoid using any built-in reverse algorithms or direct indexing for the reversal logic—only push and pop operations from the stack are permitted.
The solution uses a stack to reverse the string by exploiting the Last-In-First-Out (LIFO) property: push every character of the input string onto the stack in order, then pop characters one by one and append them to a result string. Pop order naturally gives the reverse order. Edge cases include a string of length 1 (the reversed string equals the input), a string with spaces or special characters (they are treated as ordinary characters), and potentially very large strings (the stack grows linearly with input size). Time complexity is O(n) where n is the length of the input, because each character is pushed exactly once and popped exactly once. Space complexity is O(n) as well, due to the stack storage and the output string; no additional significant memory is used.
#include <string>
#include <stack>

// Reverses the input string using a std::stack<char>.
// The input is not modified; returns a new reversed string.
std::string reverseStringWithStack(const std::string& input) {
    std::stack<char> charStack;

    // Push all characters onto the stack
    for (char c : input) {
        charStack.push(c);
    }

    // Pop characters to build the reversed string
    std::string reversed;
    while (!charStack.empty()) {
        reversed.push_back(charStack.top());
        charStack.pop();
    }

    return reversed;
}
#include <cassert>
#include <string>

// Assume the solution function is declared above.

int main() {
    // Basic test
    assert(reverseStringWithStack("hello") == "olleh");

    // Single character
    assert(reverseStringWithStack("a") == "a");

    // Palindrome
    assert(reverseStringWithStack("racecar") == "racecar");

    // String with spaces
    assert(reverseStringWithStack("hello world") == "dlrow olleh");

    // String with punctuation and digits
    assert(reverseStringWithStack("A1! b2@") == "@2b !1A");

    // Empty string (though task says non-empty, handle gracefully)
    assert(reverseStringWithStack("") == "");

    // String with only special characters
    assert(reverseStringWithStack("!@#$%") == "%$#@!");

    // Longer string with mixed content
    assert(reverseStringWithStack("C++ is fun!") == "!nuf si ++C");

    // String with repeated characters
    assert(reverseStringWithStack("aaa") == "aaa");

    // String with numbers and letters
    assert(reverseStringWithStack("abc123") == "321cba");
}
