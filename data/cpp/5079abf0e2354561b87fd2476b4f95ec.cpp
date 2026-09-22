// Write a C++ free function named `isBalancedBrackets` that takes a `std::string` as its argument and returns a `bool` indicating whether the string contains a balanced sequence of parentheses `()`, curly braces `{}`, and square brackets `[]`. The string may contain only these six bracket characters, and they must be properly nested and matched in the correct order (e.g., `"([{}])"` is valid, `"([)]"` is invalid, `"((()))"` is valid, `")("` is invalid). Empty strings are considered balanced. The function must not use recursion.
The solution uses a stack (LIFO) data structure to track the most recently opened unmatched bracket. We iterate through each character of the input string:
- If the character is an opening bracket (`(`, `{`, `[`), push it onto the stack.
- If the character is a closing bracket (`)`, `}`, `]`), first check if the stack is empty — if so, there is no matching opener, so return `false`. Otherwise, compare the top of the stack with the expected matching opener for the current closing bracket. If they match, pop the top; if they do not match, return `false` immediately.
After processing all characters, the string is balanced if and only if the stack is empty (no unmatched open brackets remain).  
Edge cases: empty string (balanced), a string starting with a closing bracket, a string ending with an opening bracket, and mismatched types like `"(]"` must all be handled correctly.  
Time complexity: \(O(n)\) where \(n\) is the string length, because each character is processed once and stack operations are \(O(1)\). Space complexity: \(O(n)\) in the worst case (e.g., a string of all opening brackets, where the stack holds all characters).
#include <string>
#include <stack>

// Returns true if the given string contains balanced brackets of types (), {}, [].
bool isBalancedBrackets(const std::string& s) {
    std::stack<char> st;
    
    for (char ch : s) {
        if (ch == '(' || ch == '{' || ch == '[') {
            st.push(ch);
        } else {
            // ch is a closing bracket; stack must not be empty and top must match
            if (st.empty()) {
                return false;
            }
            char top = st.top();
            if ((ch == ')' && top == '(') ||
                (ch == '}' && top == '{') ||
                (ch == ']' && top == '[')) {
                st.pop();
            } else {
                return false;
            }
        }
    }
    
    return st.empty();
}
#include <cassert>

int main() {
    assert(isBalancedBrackets("") == true);
    assert(isBalancedBrackets("()") == true);
    assert(isBalancedBrackets("()[]{}") == true);
    assert(isBalancedBrackets("([{}])") == true);
    assert(isBalancedBrackets("((()))") == true);
    assert(isBalancedBrackets("({[]})") == true);
    
    assert(isBalancedBrackets("(") == false);
    assert(isBalancedBrackets(")") == false);
    assert(isBalancedBrackets("([)]") == false);
    assert(isBalancedBrackets("{(})") == false);
    assert(isBalancedBrackets("((())") == false);
    assert(isBalancedBrackets("())") == false);
    
    return 0;
}
