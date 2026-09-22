/*
Write a C++ function named `hasBalancedBrackets` that takes a `const std::string&` containing a sequence of characters, and returns a `bool` indicating whether all parentheses `()`, square brackets `[]`, and curly braces `{}` in the string are properly balanced and correctly nested. Any non-bracket characters (e.g., letters, digits, spaces, punctuation) must be ignored. The function should return `true` for an empty string or a string with no brackets, and `false` for any unbalanced sequence, including mismatched or incorrectly ordered closing brackets, or unclosed opening brackets at the end.
*/

#include <string>
#include <stack>

// Returns true if all parentheses, square brackets, and curly braces in the
// input string are properly balanced and correctly nested.
bool hasBalancedBrackets(const std::string& s) {
    std::stack<char> brackets;
    for (char ch : s) {
        if (ch == '(' || ch == '[' || ch == '{') {
            brackets.push(ch);
        } else if (ch == ')' || ch == ']' || ch == '}') {
            if (brackets.empty()) {
                return false;  // closing without matching opening
            }
            char top = brackets.top();
            if ((ch == ')' && top == '(') ||
                (ch == ']' && top == '[') ||
                (ch == '}' && top == '{')) {
                brackets.pop();
            } else {
                return false;  // mismatched pair
            }
        }
        // ignore all other characters
    }
    return brackets.empty();  // all openings must be closed
}

#include <cassert>
#include <string>

// assume hasBalancedBrackets is declared above (or included here)

int main() {
    // Basic valid cases
    assert(hasBalancedBrackets("()") == true);
    assert(hasBalancedBrackets("[]") == true);
    assert(hasBalancedBrackets("{}") == true);
    assert(hasBalancedBrackets("({[]})") == true);
    assert(hasBalancedBrackets("a(b)c[d]e{f}g") == true);

    // Empty and no-bracket strings
    assert(hasBalancedBrackets("") == true);
    assert(hasBalancedBrackets("hello world 123!") == true);

    // Invalid cases
    assert(hasBalancedBrackets("(") == false);
    assert(hasBalancedBrackets(")") == false);
    assert(hasBalancedBrackets("(]") == false);
    assert(hasBalancedBrackets("([)]") == false);
    assert(hasBalancedBrackets("({})(") == false);

    // Mixed with other characters
    assert(hasBalancedBrackets("{[()]}") == true);
    assert(hasBalancedBrackets("{[(])}") == false);

    return 0;
}

// The solution uses a stack to track opening brackets. Iterate through each character of the string; when an opening bracket (`(`, `[`, `{`) is encountered, push it onto the stack. When a closing bracket (`)`, `]`, `}`) is encountered, first check if the stack is empty—if so, return `false` because there is no matching opening bracket. Otherwise, pop the top of the stack and verify that it matches the closing bracket type; if not, return `false`. After processing all characters, the stack must be empty—if any opening brackets remain, return `false`. Important edge cases include: empty string (valid), strings with only non-bracket characters (valid), nested but correctly paired brackets (valid), and same-type brackets (e.g., `([)]`) that are incorrectly nested (invalid). The time complexity is O(n) where n is the string length, and space complexity is O(m) where m is the maximum nesting depth of brackets, which is at most O(n).
