// Write a C++ function `bool isBalancedParentheses(const std::string& expression)` that, given a string containing only the characters `'('`, `')'`, `'{'`, `'}'`, `'['`, `']'`, and possibly other characters (which should be ignored), returns `true` if every opening bracket has a matching closing bracket of the same type in the correct order, and `false` otherwise. The function must use a stack-based approach, processing the string left to right: when an opening bracket is encountered, push it onto the stack; when a closing bracket is encountered, check that the stack is not empty and that the top of the stack is the corresponding opening bracket, then pop it. After processing all characters, the stack must be empty for the string to be balanced. Ignore all non-bracket characters. The function must be `const`-correct and self-contained (no `main`). Example: `"a(b)c[d]{e}"` → `true`, `"( [ ) ]"` → `false`, `"((()))"` → `true`, `"({[)]}"` → `false`, `""` → `true`.
// The solution uses a `std::stack<char>` to track unmatched opening brackets. Iterate over each character of the input string. If the character is one of `'('`, `'{'`, or `'['`, push it onto the stack. If it is a closing bracket `')'`, `'}'`, or `']'`, first check if the stack is empty—if so, return `false` immediately because there is no matching opener. Then compare the top of the stack with the expected matching opener; if they match, pop the stack; otherwise, return `false`. Non-bracket characters are simply skipped. After the loop, the string is balanced if and only if the stack is empty, so return `stack.empty()`. Edge cases: empty string is balanced; a closing bracket with no opener; mismatched bracket types; and multiple nested or sequential groups. Time complexity is O(n) where n is the length of the input, since each character is processed once and each stack operation is O(1). Space complexity is O(n) in the worst case (e.g., all opening brackets), but typically O(number of unmatched openers).
#include <string>
#include <stack>

// Returns true if the bracket characters in the input are properly balanced.
// Non-bracket characters are ignored. Only the pairs (), {}, [] are considered.
bool isBalancedParentheses(const std::string& expression) {
    std::stack<char> stack;
    for (char ch : expression) {
        if (ch == '(' || ch == '{' || ch == '[') {
            stack.push(ch);
        } else if (ch == ')' || ch == '}' || ch == ']') {
            if (stack.empty()) {
                return false; // no matching opener
            }
            char top = stack.top();
            if ((ch == ')' && top == '(') ||
                (ch == '}' && top == '{') ||
                (ch == ']' && top == '[')) {
                stack.pop();
            } else {
                return false; // mismatched type
            }
        }
        // ignore all other characters
    }
    return stack.empty();
}
#include <cassert>

int main() {
    assert(isBalancedParentheses("()") == true);
    assert(isBalancedParentheses("(a[b]{c})") == true);
    assert(isBalancedParentheses("({[)]}") == false);
    assert(isBalancedParentheses("") == true);
    assert(isBalancedParentheses(")(") == false);
    assert(isBalancedParentheses("((()))") == true);
    assert(isBalancedParentheses("([{}])") == true);
    assert(isBalancedParentheses("{[}]") == false);
    assert(isBalancedParentheses("a(b)c{d}e[f]") == true);
    assert(isBalancedParentheses("( [ ) ]") == false);
    return 0;
}
