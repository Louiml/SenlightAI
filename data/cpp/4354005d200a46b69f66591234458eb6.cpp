// Write a C++ function `bool isBalancedBrackets(const std::string& expression)` that determines whether a string consisting only of the characters `'('`, `')'`, `'['`, `']'`, `'{'`, and `'}'` is properly balanced. Balanced means every opening bracket has a corresponding closing bracket of the same type in the correct order, and no extra closing brackets appear without a matching opener. The input string may be empty (considered balanced). The function must return `true` if the string is balanced and `false` otherwise. You may not use any external libraries beyond the standard C++ ones.
// The standard approach is to use a stack of characters. Iterate through each character in the input string. If the character is an opening bracket (`(`, `[`, or `{`), push it onto the stack. If it is a closing bracket (`)`, `]`, or `}`), check that the stack is not empty and that the top of the stack corresponds to the matching opening bracket. If either condition fails, return `false`. If the top matches, pop it from the stack. After processing all characters, return `true` if the stack is empty (all openers were properly closed), otherwise return `false`. Edge cases include: an empty string (balanced), a closing bracket with an empty stack, a mismatched pair (e.g., `(]`), and intertwined pairs (e.g., `([)]`). The algorithm runs in O(n) time and uses O(n) auxiliary space in the worst case for the stack, where n is the length of the input. No special handling is needed for characters outside the six bracket types because the problem constraint guarantees only these characters occur.
#include <string>
#include <stack>

// Returns true if the input string contains balanced brackets of types (), [], and {}.
bool isBalancedBrackets(const std::string& expression) {
    std::stack<char> bracketStack;

    for (char ch : expression) {
        // Push all opening brackets onto the stack.
        if (ch == '(' || ch == '[' || ch == '{') {
            bracketStack.push(ch);
        } else {
            // For a closing bracket, the stack must not be empty and the top must match.
            if (bracketStack.empty()) {
                return false;
            }
            char top = bracketStack.top();
            if ((ch == ')' && top != '(') ||
                (ch == ']' && top != '[') ||
                (ch == '}' && top != '{')) {
                return false;
            }
            bracketStack.pop();
        }
    }

    // Balanced only if no unmatched opening brackets remain.
    return bracketStack.empty();
}
#include <cassert>
#include <string>

// Declare the function under test (assume it is defined elsewhere or above).
bool isBalancedBrackets(const std::string& expression);

int main() {
    // Basic balanced cases
    assert(isBalancedBrackets("()") == true);
    assert(isBalancedBrackets("[]") == true);
    assert(isBalancedBrackets("{}") == true);
    assert(isBalancedBrackets("({[]})") == true);

    // Nested and mixed balanced
    assert(isBalancedBrackets("{[()]()}") == true);
    assert(isBalancedBrackets("((()))") == true);

    // Unbalanced cases
    assert(isBalancedBrackets("(") == false);
    assert(isBalancedBrackets(")") == false);
    assert(isBalancedBrackets("([)]") == false);
    assert(isBalancedBrackets("{(})") == false);
    assert(isBalancedBrackets("()]") == false);
    assert(isBalancedBrackets("[{]}") == false);

    // Empty string is balanced
    assert(isBalancedBrackets("") == true);

    // Single pair and multiple pairs
    assert(isBalancedBrackets("()[]{}") == true);
    assert(isBalancedBrackets("(]") == false);

    return 0;
}
