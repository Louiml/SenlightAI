// Write a C++ function `bool isBalanced(const std::string& expression)` that takes a string containing only the characters `(`, `)`, `{`, `}`, `[`, and `]` (no spaces or other characters) and returns `true` if the brackets are properly balanced and correctly nested, and `false` otherwise. The input string may be empty (which is considered balanced). The function must use a stack to process the string, and it should handle all edge cases such as closing brackets without a matching opening bracket, mismatched bracket types, and extra opening brackets at the end.

// The standard approach uses a stack of characters. Iterate through each character in the input string:
// - If the character is an opening bracket (`(`, `{`, `[`), push it onto the stack.
// - If the character is a closing bracket (`)`, `}`, `]`), first check if the stack is empty. If it is empty, that means there is no matching opening bracket, so return `false`. Otherwise, pop the top of the stack and check if it matches the expected opening bracket for the current closing bracket (i.e., for `)` the top must be `(`, for `}` the top must be `{`, for `]` the top must be `[`). If the popped character does not match, return `false`.
// After processing all characters, the stack should be empty if every opening bracket had a matching closing bracket. If the stack is not empty, return `false`. Edge cases include an empty string (balanced), a single closing bracket, a single opening bracket, mismatched pairs like `(]`, and complex nested structures like `{[()]}`. Time complexity is O(n) where n is the length of the string, and space complexity is O(n) in the worst case (when all characters are opening brackets), but typically O(depth of nesting).

#include <stack>
#include <string>

// Check if a string of brackets is balanced and correctly nested.
bool isBalanced(const std::string& expression) {
    std::stack<char> stk;

    for (const char ch : expression) {
        if (ch == '(' || ch == '{' || ch == '[') {
            stk.push(ch);
        } else {
            // Closing bracket encountered
            if (stk.empty()) return false;

            const char top = stk.top();
            stk.pop();

            // Check if the popped opening bracket matches the closing bracket
            if ((ch == ')' && top != '(') ||
                (ch == '}' && top != '{') ||
                (ch == ']' && top != '[')) {
                return false;
            }
        }
    }

    // If stack is not empty, there are unmatched opening brackets
    return stk.empty();
}

#include <cassert>

int main() {
    // Empty string is balanced
    assert(isBalanced("") == true);

    // Simple balanced cases
    assert(isBalanced("()") == true);
    assert(isBalanced("{}") == true);
    assert(isBalanced("[]") == true);

    // Nested balanced
    assert(isBalanced("{[()]}") == true);
    assert(isBalanced("({[]})") == true);

    // Unbalanced: missing closing
    assert(isBalanced("(") == false);
    assert(isBalanced("{[}") == false);

    // Unbalanced: missing opening
    assert(isBalanced(")") == false);
    assert(isBalanced("()]") == false);

    // Unbalanced: mismatched type
    assert(isBalanced("(]") == false);
    assert(isBalanced("[)") == false);

    // Balanced but with multiple groups
    assert(isBalanced("()[]{}") == true);
    assert(isBalanced("([{}])") == true);

    // Unbalanced: extra opening at end
    assert(isBalanced("({[]") == false);

    return 0;
}
