// Write a C++ function named `isBalancedParentheses` that takes a `std::string` as input and returns a `bool` indicating whether the parentheses in the string are balanced. The string may contain only lowercase letters, digits, spaces, and three types of parentheses: round `()`, square `[]`, and curly `{}`. A string is considered balanced if every opening parenthesis has a matching closing parenthesis of the same type in the correct order, and no parentheses are left unclosed or unmatched. Return `true` if the string is balanced, `false` otherwise. The string may be empty, and any non-parenthesis characters should be ignored.
#include <cassert>

int main() {
    assert(isBalancedParentheses("") == true);
    assert(isBalancedParentheses("abc 123 !@#") == true);
    assert(isBalancedParentheses("()") == true);
    assert(isBalancedParentheses("[]{}()") == true);
    assert(isBalancedParentheses("([{}])") == true);
    assert(isBalancedParentheses("(a[ b{c} ]d)") == true);
    assert(isBalancedParentheses("(") == false);
    assert(isBalancedParentheses(")") == false);
    assert(isBalancedParentheses("(]") == false);
    assert(isBalancedParentheses("([)]") == false);
    assert(isBalancedParentheses("{[]}") == true);
    assert(isBalancedParentheses("{[}]") == false);
    assert(isBalancedParentheses("((()))") == true);
    assert(isBalancedParentheses("(()") == false);
    return 0;
}
#include <string>
#include <stack>

// Returns true if the parentheses in the input string are balanced, false otherwise.
bool isBalancedParentheses(const std::string& input) {
    std::stack<char> stack;

    for (char ch : input) {
        if (ch == '(' || ch == '[' || ch == '{') {
            stack.push(ch);
        } else if (ch == ')' || ch == ']' || ch == '}') {
            if (stack.empty()) {
                return false;
            }
            char top = stack.top();
            stack.pop();
            if ((ch == ')' && top != '(') ||
                (ch == ']' && top != '[') ||
                (ch == '}' && top != '{')) {
                return false;
            }
        }
        // Non-parenthesis characters are ignored.
    }

    return stack.empty();
}
// The solution uses a stack to track unmatched opening parentheses. Iterate through each character in the string: if it is an opening parenthesis (`(`, `[`, `{`), push it onto the stack. If it is a closing parenthesis (`)`, `]`, `}`), check that the stack is not empty and that the top of the stack matches the corresponding opening type; if either condition fails, return `false`. If the character is neither opening nor closing, ignore it. After processing all characters, the string is balanced only if the stack is empty. Edge cases include an empty string (balanced, `true`), a string with only non-parenthesis characters (`true`), unmatched closing parentheses (`false`), and mismatched types like `(]` (`false`). Time complexity is `O(n)` where `n` is the string length, and space complexity is `O(n)` in the worst case (all opening parentheses), otherwise `O(1)` for typical strings.
