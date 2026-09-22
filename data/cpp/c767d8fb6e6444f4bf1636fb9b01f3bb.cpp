/*
Write a C++ function `bool areBracketsBalanced(const std::string& s)` that takes a string containing only the characters `'('`, `')'`, `'{'`, `'}'`, `'['`, and `']'` and returns `true` if the brackets are correctly matched and properly nested, and `false` otherwise. The matching rules are: each opening bracket must be closed by the same type of bracket, and brackets must close in the correct order (i.e., the most recently opened unmatched bracket must be closed first). The function should handle empty strings (returning `true`), strings with only one bracket, and strings with mixed types such as `"([)]"` (which is invalid because `'('` closes before `'['`). Your implementation must not use any external libraries beyond the C++ standard library, and must be free of memory leaks or undefined behavior.
*/

#include <string>
#include <stack>

// Returns true if the brackets in s are correctly matched and nested.
bool areBracketsBalanced(const std::string& s) {
    std::stack<char> st;
    for (char c : s) {
        if (c == '(' || c == '{' || c == '[') {
            st.push(c);
        } else {
            if (st.empty()) {
                return false;
            }
            char top = st.top();
            if ((c == ')' && top != '(') ||
                (c == '}' && top != '{') ||
                (c == ']' && top != '[')) {
                return false;
            }
            st.pop();
        }
    }
    return st.empty();
}

#include <cassert>

int main() {
    assert(areBracketsBalanced("") == true);
    assert(areBracketsBalanced("()") == true);
    assert(areBracketsBalanced("()[]{}") == true);
    assert(areBracketsBalanced("(]") == false);
    assert(areBracketsBalanced("([)]") == false);
    assert(areBracketsBalanced("{[]}") == true);
    assert(areBracketsBalanced("(") == false);
    assert(areBracketsBalanced(")") == false);
    assert(areBracketsBalanced("((()))") == true);
    assert(areBracketsBalanced("([{}])") == true);
}

// The solution uses a stack-based approach. We iterate character by character over the input string. When we encounter an opening bracket (`'('`, `'{'`, or `'['`), we push it onto the stack. When we encounter a closing bracket, we first check if the stack is empty—if it is, there is no matching opening bracket, so the string is invalid. If the stack is not empty, we compare the top of the stack with the closing bracket; if they do not match (e.g., `')'` with `'{'`), the string is invalid. Otherwise, we pop the matching opening bracket from the stack. After processing all characters, the string is valid only if the stack is empty—if any unmatched opening brackets remain, the string is invalid. Edge cases include an empty string (valid), a string with only closing brackets (invalid because stack empties early), a string with only opening brackets (invalid because stack non-empty at the end), and nested or interspersed valid sequences like `"{[]}"` (valid). The time complexity is O(n) where n is the string length since each character is processed once. The space complexity is O(n) in the worst case, when all characters are opening brackets and thus stored on the stack.
