/*
Write a standalone C++ function that, given a string containing a line of text with parentheses `()` and square brackets `[]`, returns `"yes"` if every opening bracket has a matching closing bracket of the same type in the correct order, and `"no"` otherwise. The string may contain any other characters (letters, digits, spaces, punctuation) which should be ignored. The function should treat an empty or whitespace-only string as balanced (returning `"yes"`), and must not modify the input string.
*/
#include <string>
#include <stack>

// Check if parentheses and square brackets in a string are balanced.
// Returns "yes" if balanced, otherwise "no".
std::string isBalancedBrackets(const std::string& input) {
    std::stack<char> st;
    
    for (char ch : input) {
        if (ch == '(' || ch == '[') {
            st.push(ch);
        } else if (ch == ')') {
            if (st.empty() || st.top() != '(') {
                return "no";
            }
            st.pop();
        } else if (ch == ']') {
            if (st.empty() || st.top() != '[') {
                return "no";
            }
            st.pop();
        }
    }
    
    return st.empty() ? "yes" : "no";
}
#include <cassert>
#include <string>

// The solution function is assumed to be declared above.
std::string isBalancedBrackets(const std::string& input);

int main() {
    // Simple balanced cases
    assert(isBalancedBrackets("()") == "yes");
    assert(isBalancedBrackets("[]") == "yes");
    assert(isBalancedBrackets("([])") == "yes");
    assert(isBalancedBrackets("[()]") == "yes");
    
    // Unbalanced cases
    assert(isBalancedBrackets("(") == "no");
    assert(isBalancedBrackets(")") == "no");
    assert(isBalancedBrackets("([)]") == "no");
    assert(isBalancedBrackets("{[}]") == "no"); // '{}' are ignored but order is wrong
    assert(isBalancedBrackets("(([]))(") == "no");
    
    // Edge cases
    assert(isBalancedBrackets("") == "yes");
    assert(isBalancedBrackets("   ") == "yes");
    assert(isBalancedBrackets("abc(def)ghi") == "yes");
    assert(isBalancedBrackets("a[b(c)d]e") == "yes");
    
    return 0;
}
// The solution uses a stack to track unmatched opening brackets. Iterate through each character of the input string: when encountering `(`, push a marker for round brackets; when encountering `[`, push a marker for square brackets. For closing brackets `)` and `]`, check whether the stack is non-empty and the top matches the corresponding opening type. If the top does not match or the stack is empty, immediately mark the string as unbalanced. After processing all characters, the string is balanced only if no mismatch occurred and the stack is empty. Key edge cases: an empty string (balanced), a single unmatched closing bracket (unbalanced), nested/mixed brackets like `([])` (balanced), and crossing pairs like `([)]` (unbalanced). Time complexity is O(n) where n is the length of the input, and space complexity is O(n) in the worst case due to the stack.
