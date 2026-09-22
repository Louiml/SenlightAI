// Write a C++ function `determineBalancedParentheses` that takes a `const std::string&` containing only characters `'('`, `')'`, `'{'`, `'}'`, `'['`, and `']'` (no other characters, but possibly an empty string). The function should return `true` if the string has balanced parentheses, meaning every opening bracket has a corresponding closing bracket of the same type, and they are properly nested. Use a stack-based approach with a linked-list stack (or any stack container). For an empty string, return `true`. The function must be `const`-correct and avoid modifying the input. Ensure you handle edge cases like `")("`, `"([)]"`, `"([])"`, and long nested sequences.
// The core idea is to iterate through each character in the input string. For every opening bracket (`'('`, `'{'`, `'['`), push it onto a stack. For every closing bracket, check the top of the stack: if the stack is empty or the top does not match the corresponding opening bracket, return `false`. Otherwise, pop the stack. After processing all characters, if the stack is empty, all brackets were properly matched and nested, so return `true`; otherwise, return `false`. Edge cases include empty input (`true`), a closing bracket with no preceding opening, mismatched types, and an opening bracket that never gets closed. The algorithm processes each character exactly once, giving O(n) time complexity and O(n) space in the worst case (when all characters are opening brackets).
#include <string>
#include <stack>

// Determine if the parentheses in the string are balanced.
bool determineBalancedParentheses(const std::string& expression) {
    std::stack<char> parenStack;
    
    for (char ch : expression) {
        if (ch == '(' || ch == '{' || ch == '[') {
            parenStack.push(ch);
        } else {
            if (parenStack.empty()) {
                return false;
            }
            char topChar = parenStack.top();
            bool matched = (ch == ')' && topChar == '(') ||
                           (ch == '}' && topChar == '{') ||
                           (ch == ']' && topChar == '[');
            if (!matched) {
                return false;
            }
            parenStack.pop();
        }
    }
    
    return parenStack.empty();
}
#include <cassert>

int main() {
    assert(determineBalancedParentheses("") == true);
    assert(determineBalancedParentheses("()") == true);
    assert(determineBalancedParentheses("()[]{}") == true);
    assert(determineBalancedParentheses("(]") == false);
    assert(determineBalancedParentheses("([)]") == false);
    assert(determineBalancedParentheses("{[]}") == true);
    assert(determineBalancedParentheses(")(") == false);
    assert(determineBalancedParentheses("((()))") == true);
    assert(determineBalancedParentheses("((())") == false);
    assert(determineBalancedParentheses("{[()()]}") == true);
}
