Given a string `s` containing only the characters `'('`, `')'`, `'{'`, `'}'`, `'['`, and `']'`, write a C++ function `bool isValidParentheses(const std::string& s)` that determines whether the input string has valid parentheses ordering. A string is valid if every opening bracket has a corresponding closing bracket of the same type, and each pair closes in the correct nested order (e.g., `"()"` is valid, `"([)]"` is invalid, `"()[]{}"` is valid). Assume the input may be empty, and the function should return `true` for an empty string.

// The solution uses a stack to track unmatched opening brackets. Iterate through each character of the string:
// - If the character is an opening bracket `(`, `{`, or `[`, push it onto the stack.
// - If the character is a closing bracket `)`, `}`, or `]`, first check if the stack is empty. If empty, there is no matching opening bracket, so return `false`. Otherwise, compare the top of the stack with the expected matching opening bracket. If they match, pop the stack; if not, return `false`.
//
// After processing all characters, the stack must be empty for the string to be valid (i.e., every opening bracket was matched). Edge cases include: empty string (valid), single closing bracket (invalid), single opening bracket (invalid), and nested/overlapping types like `"([)]"` which fails at the `]` when the top is `'('`. Time complexity is O(n) where n is the length of the string; space complexity is O(n) in the worst case for the stack (e.g., all opening brackets).

#include <stack>
#include <string>

// Determine if the parentheses in the string are valid.
// Returns true for an empty string.
bool isValidParentheses(const std::string& s) {
    std::stack<char> st;
    for (char c : s) {
        if (c == '(' || c == '{' || c == '[') {
            st.push(c);
        } else {
            if (st.empty()) {
                return false;
            }
            char top = st.top();
            if ((c == ')' && top == '(') || 
                (c == '}' && top == '{') || 
                (c == ']' && top == '[')) {
                st.pop();
            } else {
                return false;
            }
        }
    }
    return st.empty();
}

#include <cassert>
#include <string>

// Declare the solution function (included from the solution above).
bool isValidParentheses(const std::string& s);

int main() {
    // Basic valid cases
    assert(isValidParentheses("()") == true);
    assert(isValidParentheses("()[]{}") == true);
    assert(isValidParentheses("{[()]}") == true);
    
    // Basic invalid cases
    assert(isValidParentheses("(]") == false);
    assert(isValidParentheses("([)]") == false);
    assert(isValidParentheses("]") == false);
    assert(isValidParentheses("(") == false);
    assert(isValidParentheses(")") == false);
    
    // Edge cases
    assert(isValidParentheses("") == true);
    assert(isValidParentheses("((()))") == true);
    assert(isValidParentheses("((())") == false);
    
    // Longer mixed valid
    assert(isValidParentheses("{[]}()[]") == true);
    assert(isValidParentheses("{[]}()[]}") == false);
    
    return 0;
}
