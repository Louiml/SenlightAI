Write a standalone C++ function `bool isBalanced(const std::string& expression)` that takes a string containing only the characters `'('`, `')'`, `'['`, `']'`, `'{'`, and `'}'` (no other characters) and returns `true` if the brackets are correctly balanced (i.e., every opening bracket has a matching closing bracket of the same type in the correct order), and `false` otherwise. The function must handle empty strings (return `true`), strings with multiple bracket types, and ensure that a closing bracket of one type cannot close an unmatched opening bracket of a different type. Do not use any external libraries beyond the standard C++ headers.

The solution uses a stack to track unmatched opening brackets. Iterate through each character of the input string: if it is an opening bracket (`(`, `[`, `{`), push it onto the stack. If it is a closing bracket (`)`, `]`, `}`), first check if the stack is empty—if so, return `false` because there is no matching opening bracket. Otherwise, compare the top of the stack with the current closing bracket: if they match as a pair (e.g., `(` with `)`, `[` with `]`, `{` with `}`), pop the stack; if they don't match, return `false` immediately. After processing all characters, the string is balanced if and only if the stack is empty (meaning every opening bracket was matched). Edge cases include an empty string (valid), a leading closing bracket (invalid), a trailing unmatched opening bracket (invalid), and interleaved mismatched brackets (invalid). Time complexity is O(n) where n is the length of the string, and space complexity is O(n) in the worst case (when all characters are opening brackets).

#include <stack>
#include <string>

// Returns true if the bracket sequence in `expression` is correctly balanced.
bool isBalanced(const std::string& expression) {
    std::stack<char> brackets;
    for (char ch : expression) {
        if (ch == '(' || ch == '[' || ch == '{') {
            brackets.push(ch);
        } else {
            if (brackets.empty()) {
                return false;
            }
            char top = brackets.top();
            if ((ch == ')' && top == '(') ||
                (ch == ']' && top == '[') ||
                (ch == '}' && top == '{')) {
                brackets.pop();
            } else {
                return false;
            }
        }
    }
    return brackets.empty();
}

#include <cassert>

int main() {
    assert(isBalanced("()") == true);
    assert(isBalanced("({[]})") == true);
    assert(isBalanced("({})[]{}") == true);
    assert(isBalanced("") == true);
    assert(isBalanced("(") == false);
    assert(isBalanced(")") == false);
    assert(isBalanced("([)]") == false);
    assert(isBalanced("{(})") == false);
    assert(isBalanced("({[}])") == false);
    assert(isBalanced("({[]}") == false);
    return 0;
}
