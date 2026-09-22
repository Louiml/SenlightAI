/*
Write a C++ function that takes a string containing only the characters `'('`, `')'`, `'{'`, `'}'`, `'['`, and `']'` and returns a boolean indicating whether the parentheses are valid. A valid string must have every opening bracket closed by a matching closing bracket in the correct order, with no unmatched or incorrectly nested brackets. The input string may be empty (which is considered valid), and the function must handle strings of any length. Do not assume the string length is even; check validity for all cases. The function should be named `isValidParentheses` and accept a `const std::string&` parameter.
*/
#include <string>
#include <stack>
#include <unordered_map>

// Check if a string of parentheses is valid.
// Returns true for empty strings and properly nested/ordered brackets.
bool isValidParentheses(const std::string& s) {
    std::stack<char> stk;
    // Map each closing bracket to its corresponding opening bracket
    const std::unordered_map<char, char> pairs = {
        {')', '('},
        {']', '['},
        {'}', '{'}
    };
    
    for (char c : s) {
        // If it's a closing bracket
        if (pairs.count(c)) {
            // Stack must not be empty and top must match the expected opening
            if (stk.empty() || stk.top() != pairs.at(c)) {
                return false;
            }
            stk.pop();
        } else {
            // It's an opening bracket (guaranteed by problem constraints)
            stk.push(c);
        }
    }
    // Valid if all brackets were properly closed
    return stk.empty();
}
#include <cassert>
#include <string>

int main() {
    // Basic valid cases
    assert(isValidParentheses("()") == true);
    assert(isValidParentheses("()[]{}") == true);
    assert(isValidParentheses("{[()]}") == true);
    
    // Basic invalid cases
    assert(isValidParentheses("(]") == false);
    assert(isValidParentheses("([)]") == false);
    assert(isValidParentheses("{") == false);
    assert(isValidParentheses(")") == false);
    
    // Edge cases
    assert(isValidParentheses("") == true);
    assert(isValidParentheses("(((((((((") == false);
    assert(isValidParentheses("))))))") == false);
    assert(isValidParentheses("[](){}") == true);
    
    // Longer mixed valid string
    assert(isValidParentheses("(([]){})") == true);
    
    // Mismatched close at end of valid prefix
    assert(isValidParentheses("()]") == false);
    
    return 0;
}
// The most efficient and robust solution uses a stack to track opening brackets. Iterate through each character in the input string. If the character is an opening bracket (`'('`, `'{'`, or `'['`), push it onto the stack. If it is a closing bracket (`')'`, `'}'`, or `']'`), check the top of the stack: if the stack is empty, or if the top does not match the corresponding opening bracket for this closing bracket, the string is invalid. Otherwise, pop the top of the stack. After processing all characters, the string is valid only if the stack is empty (all opening brackets were properly closed). An empty string is valid because the stack remains empty. Time complexity is O(n) where n is the length of the string, as each character is processed once. Space complexity is O(n) in the worst case (e.g., all opening brackets with no closures yet), but typical cases use less. Edge cases include strings with only opening brackets, only closing brackets, mismatched pairs like `"(]"`, and nested but incorrect order like `"([)]"`.
