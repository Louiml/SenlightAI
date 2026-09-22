Write a C++ function named `isBalancedBrackets` that takes a `const std::string&` as input and returns a `bool` indicating whether all parentheses `()`, square brackets `[]`, and curly braces `{}` in the string are correctly balanced and properly nested. Unbalanced or incorrectly nested brackets (e.g., `(]`, `{[}]`, or an extra closing bracket) must return `false`. Non-bracket characters (letters, digits, spaces, punctuation) should be ignored. The string may be empty (which should return `true`), may contain only non-bracket characters (also `true`), and may contain unterminated opening brackets or extra closing brackets (both `false`). The function must not modify the input string.
The solution uses a stack to track opening brackets. Iterate through each character of the input string. If the character is an opening bracket (`(`, `[`, or `{`), push it onto the stack. If it is a closing bracket (`)`, `]`, or `}`), first check if the stack is empty—if so, there is no matching opening bracket, so return `false`. Otherwise, pop the top of the stack and verify that it matches the closing bracket type. If the pair does not match (e.g., `(` matched with `]`), return `false`. After processing all characters, the string is balanced only if the stack is empty; if any opening brackets remain, return `false`. Edge cases: empty string or string with no brackets returns `true`; strings like `"((("` return `false`; strings like `"}{"` return `false` because the closing bracket appears before any opening. Time complexity is O(n) for a string of length n (one pass, each bracket pushed/popped at most once). Space complexity is O(n) in the worst case (e.g., all opening brackets), but typically O(1) for balanced short strings.
#include <stack>
#include <string>

// Check if all parentheses, square brackets, and curly braces are balanced and properly nested.
bool isBalancedBrackets(const std::string& input) {
    std::stack<char> brackets;

    for (char ch : input) {
        if (ch == '(' || ch == '[' || ch == '{') {
            brackets.push(ch);
        } else if (ch == ')' || ch == ']' || ch == '}') {
            if (brackets.empty()) {
                return false;
            }
            const char top = brackets.top();
            brackets.pop();

            if ((ch == ')' && top != '(') ||
                (ch == ']' && top != '[') ||
                (ch == '}' && top != '{')) {
                return false;
            }
        }
        // Ignore all other characters.
    }

    return brackets.empty();
}
#include <cassert>
#include <string>

// The solution function is declared here (for completeness)
bool isBalancedBrackets(const std::string& input);

int main() {
    assert(isBalancedBrackets("") == true);
    assert(isBalancedBrackets("abc123 !?") == true);
    assert(isBalancedBrackets("()") == true);
    assert(isBalancedBrackets("[]{}()") == true);
    assert(isBalancedBrackets("{[()]}") == true);
    assert(isBalancedBrackets("(a + b) * [c - d]") == true);
    assert(isBalancedBrackets("(") == false);
    assert(isBalancedBrackets(")") == false);
    assert(isBalancedBrackets("(]") == false);
    assert(isBalancedBrackets("{[]}(") == false);
    assert(isBalancedBrackets("([)]") == false);
    assert(isBalancedBrackets("}{") == false);
    return 0;
}
