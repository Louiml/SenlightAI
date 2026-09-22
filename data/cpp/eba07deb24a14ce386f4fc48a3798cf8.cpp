Write a C++ function named `isBalancedParentheses` that takes a `const std::string&` containing only characters from the set `()[]{}` and returns a `bool` indicating whether the brackets are correctly balanced and properly nested. A string is valid if every opening bracket has a corresponding closing bracket of the same type, in the correct order, and no unmatched brackets remain at the end. Handle edge cases such as empty input (valid), a single closing bracket (invalid), and deeply nested structures.

#include <cassert>
#include <string>

// Function under test is declared above.
int main() {
    assert(isBalancedParentheses("") == true);
    assert(isBalancedParentheses("()") == true);
    assert(isBalancedParentheses("()[]{}") == true);
    assert(isBalancedParentheses("([{}])") == true);
    assert(isBalancedParentheses("(]") == false);
    assert(isBalancedParentheses("([)]") == false);
    assert(isBalancedParentheses("(") == false);
    assert(isBalancedParentheses(")") == false);
    assert(isBalancedParentheses("{[()]}") == true);
    assert(isBalancedParentheses("((()))") == true);
}

#include <string>
#include <stack>

// Returns true if the input string has correctly matched and nested brackets.
bool isBalancedParentheses(const std::string& s) {
    std::stack<char> st;
    
    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') {
            st.push(c);
        } else {
            if (st.empty()) {
                return false;
            }
            char top = st.top();
            if ((c == ')' && top == '(') ||
                (c == ']' && top == '[') ||
                (c == '}' && top == '{')) {
                st.pop();
            } else {
                return false;
            }
        }
    }
    return st.empty();
}

// The solution uses a stack to track unmatched opening brackets. Iterate through each character: if it is `(`, `[`, or `{`, push it onto the stack. Otherwise (it must be a closing bracket), check if the stack is empty — if so, return `false` immediately because there is no matching opener. If the stack is non-empty, compare the top of the stack with the current closing bracket: if they match (`()` , `[]` , `{}`), pop the stack; otherwise, return `false` because the brackets are misaligned. After processing all characters, the string is valid iff the stack is empty. Important edge cases include empty input (valid), a single closing bracket (invalid at first check), and mismatched nesting like `(]` (invalid due to top mismatch). Time complexity is O(n) where n is the length of the string, and space complexity is O(n) in the worst case (all opening brackets) due to the stack.
