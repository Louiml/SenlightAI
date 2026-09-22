/*
Write a C++ function named `isBalanced` that takes a string `s` containing only the characters `'('`, `')'`, `'['`, `']'`, `'{'`, and `'}'` and returns a `bool` indicating whether the brackets in the string are correctly balanced and properly nested. The string may be empty, in which case the function should return `true`. The function must handle cases where closing brackets appear without a matching opening bracket (or before any opening bracket), mismatched bracket types (e.g., `(]`), and correctly nested but unbalanced input (e.g., `([)]` should return `false`, while `([{}])` should return `true`). You are expected to implement the logic using a stack data structure, treating each character sequentially, with no reliance on any external libraries beyond the standard C++ headers.
*/

#include <string>
#include <stack>

// Returns true if the brackets in s are balanced and properly nested.
bool isBalanced(const std::string& s) {
    std::stack<char> st;
    for (char ch : s) {
        if (ch == '(' || ch == '[' || ch == '{') {
            st.push(ch);
        } else {
            // If the stack is empty or the top does not match the closing bracket.
            if (st.empty()) return false;
            char top = st.top();
            if ((top == '(' && ch != ')') ||
                (top == '[' && ch != ']') ||
                (top == '{' && ch != '}')) {
                return false;
            }
            st.pop();
        }
    }
    return st.empty();
}

#include <cassert>

int main() {
    assert(isBalanced("") == true);
    assert(isBalanced("()") == true);
    assert(isBalanced("[]") == true);
    assert(isBalanced("{}") == true);
    assert(isBalanced("()[]{}") == true);
    assert(isBalanced("([{}])") == true);
    assert(isBalanced("(]") == false);
    assert(isBalanced("([)]") == false);
    assert(isBalanced("{[]}") == true);
    assert(isBalanced("((((") == false);
    assert(isBalanced(")") == false);
    return 0;
}

// The solution uses a stack to track unmatched opening brackets. We iterate through each character in the input string. If the character is an opening bracket (`(`, `[`, or `{`), we push it onto the stack. If the character is a closing bracket (`)`, `]`, or `}`), we first check if the stack is empty—if so, there is no corresponding opening bracket, and the string is invalid. If the stack is not empty, we check whether the top of the stack matches the current closing bracket by comparing the expected closing counterpart (e.g., if top is `'('`, the expected closing is `')'`). If there is a mismatch, the string is invalid. If the match is correct, we pop the top element from the stack. After processing all characters, the string is valid only if the stack is empty—this ensures every opening bracket has been properly closed. Edge cases include an empty string (returns `true`), a string starting with a closing bracket (immediately returns `false`), mismatched pair like `(]` (returns `false`), and non‑nested but closed like `()[]{}` (returns `true`). Time complexity is \(O(n)\) for a string of length `n`, as each character is processed once. Space complexity is \(O(n)\) in the worst case (e.g., all opening brackets with no closures, or a deeply nested structure) because the stack may hold up to `n` elements.
