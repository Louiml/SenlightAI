Write a C++ function named `isBalanced` that takes a string `s` containing only the characters `'('`, `')'`, `'{'`, `'}'`, `'['`, and `']'` and returns `true` if the brackets are correctly matched and nested, and `false` otherwise. The function must handle empty strings (returning `true`), correctly process interleaved and nested brackets, and reject any sequence where a closing bracket does not match the most recent unmatched opening bracket or where there are unmatched opening brackets at the end.
The solution uses a stack to track unmatched opening brackets. Iterate through each character: if it is an opening bracket (`(`, `{`, `[`), push it onto the stack. If it is a closing bracket (`)`, `}`, `]`), first check if the stack is empty—if so, there is no matching opener, so return `false`. Otherwise, pop the top of the stack and verify it matches the expected opener for the closing bracket. If it does not match, return `false`. After processing all characters, the stack must be empty (all openers matched), otherwise return `false`. Edge cases include an empty string (valid), a single closing bracket (invalid), and sequences like `"([)]"` (invalid because nesting is broken) versus `"([])"` (valid). Time complexity is O(n) for n characters; space complexity is O(n) in the worst case when the string is all opening brackets, but for typical balanced cases it is O(n/2) worst case.
#include <stack>
#include <string>

// Returns true if the bracket sequence in s is valid (correctly matched and nested).
bool isBalanced(const std::string& s) {
    std::stack<char> st;
    for (char ch : s) {
        if (ch == '(' || ch == '{' || ch == '[') {
            st.push(ch);
        } else {
            if (st.empty()) return false;
            char top = st.top();
            st.pop();
            if ((ch == ')' && top == '(') ||
                (ch == '}' && top == '{') ||
                (ch == ']' && top == '[')) {
                continue;
            }
            return false;
        }
    }
    return st.empty();
}
#include <cassert>
#include <string>

// The solution function is declared above (or included from a header).
bool isBalanced(const std::string& s);

int main() {
    assert(isBalanced("") == true);
    assert(isBalanced("()") == true);
    assert(isBalanced("()[]{}") == true);
    assert(isBalanced("(]") == false);
    assert(isBalanced("([)]") == false);
    assert(isBalanced("{[]}") == true);
    assert(isBalanced("((((") == false);
    assert(isBalanced("))))") == false);
    assert(isBalanced("({[") == false);
    assert(isBalanced("({[]})") == true);
    return 0;
}
