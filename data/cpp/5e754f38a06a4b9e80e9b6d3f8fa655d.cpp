// Write a C++ function named `isBalancedParentheses` that takes a `const std::string&` containing only the characters `(){}[]` and returns a `std::string` `"YES"` if the parentheses, curly braces, and square brackets are correctly nested and matched, and `"NO"` otherwise. The function must handle empty strings (return `"YES"`), unbalanced closing brackets (e.g., `")("`, `"(]"`), mismatched types (e.g., `"({)}"`), and deeply nested valid sequences (e.g., `"(((({}))))"`). The input string will contain no whitespace or other characters, and the function should not modify the input.
The solution uses a stack to track opening brackets. Iterate through each character of the input string:
- If the character is an opening bracket (`(`, `{`, `[`), push it onto the stack.
- If it is a closing bracket (`)`, `}`, `]`): 
  - If the stack is empty, there is no matching opening bracket, so return `"NO"`.
  - Otherwise, pop the top element and check if it matches the current closing bracket. A match occurs when the pair is one of `()`, `{}`, or `[]`. If not, return `"NO"`.
After processing all characters, if the stack is empty, all brackets were matched; otherwise, there are unclosed opening brackets. Return `"YES"` if empty, else `"NO"`.
Edge cases: empty string → valid; an unmatched closing bracket → invalid; an unmatched opening bracket at the end → invalid; mismatched pairs like `"(]"` → invalid; nested mixed types like `"({[]})"` → valid.
Time complexity: \(O(n)\) for scanning the string, where \(n\) is its length. Space complexity: \(O(n)\) in the worst case when all characters are opening brackets (e.g., `"((((("`); otherwise \(O(1)\) for balanced input.
#include <string>
#include <stack>
#include <unordered_map>

// Check whether a string of brackets is correctly balanced.
// Valid characters: '(', ')', '{', '}', '[', ']'
// Returns "YES" if balanced, "NO" otherwise.
std::string isBalancedParentheses(const std::string& s) {
    std::stack<char> st;
    std::unordered_map<char, char> openClose = {{')', '('}, {'}', '{'}, {']', '['}};
    
    for (char c : s) {
        if (c == '(' || c == '{' || c == '[') {
            st.push(c);
        } else {
            // Closing bracket
            if (st.empty() || st.top() != openClose[c]) {
                return "NO";
            }
            st.pop();
        }
    }
    
    return st.empty() ? "YES" : "NO";
}
#include <cassert>
#include <string>

// Free function declared in solution
std::string isBalancedParentheses(const std::string& s);

int main() {
    assert(isBalancedParentheses("()") == "YES");
    assert(isBalancedParentheses("()[]{}") == "YES");
    assert(isBalancedParentheses("(]") == "NO");
    assert(isBalancedParentheses("([)]") == "NO");
    assert(isBalancedParentheses("{[]}") == "YES");
    assert(isBalancedParentheses("") == "YES");
    assert(isBalancedParentheses("((()))") == "YES");
    assert(isBalancedParentheses("(((") == "NO");
    assert(isBalancedParentheses("}}}") == "NO");
    assert(isBalancedParentheses("({[{}]})") == "YES");
}
