// Write a C++ function `longestValidParenthesesLength` that takes a `std::string` containing only the characters `'('` and `')'` and returns the length of the longest well-formed parentheses substring. A substring is well-formed if every opening parenthesis has a matching closing parenthesis in the correct order, and no closing parenthesis appears before its matching opening. The function must handle empty strings, strings with only one type of parenthesis, and strings with multiple valid substrings separated by invalid characters. For example, for input `")()())"`, the longest valid substring is `"()()"` with length 4; for `"(()"`, the longest is `"()"` with length 2. The solution must use a stack-based approach.
The classic solution uses a stack to track indices of unmatched parentheses. We push a sentinel index `-1` onto the stack initially, representing the position just before the string. We iterate over each character by its index `i`:
- If the character is `'('`, push its index onto the stack.
- If the character is `')'`, pop the top element from the stack. After popping, if the stack becomes empty, it means the current `')'` has no matching `'('` and closes a valid segment prematurely; we push the current index `i` as a new sentinel (the new "virtual start" before the next possible valid substring). If the stack is not empty after popping, then the substring from `stack.top() + 1` to `i` is valid, and its length is `i - stack.top()`. We update the maximum length accordingly.

Edge cases:  
- Empty string → returns 0.  
- All `'('` → no valid substring → 0.  
- All `')'` → each close pops the sentinel, pushes itself, stack never has a valid segment → 0.  
- Valid substring at the beginning, middle, or end is handled because the sentinel updates appropriately when an unmatched `')'` appears.  

Time complexity: O(n) for a single pass over the string, where n is the length. Space complexity: O(n) in the worst case when all characters are `'('`, because the stack holds all indices.
#include <string>
#include <stack>
#include <algorithm>

// Returns the length of the longest valid (well-formed) parentheses substring.
int longestValidParenthesesLength(const std::string& s) {
    std::stack<int> indices;
    indices.push(-1);  // sentinel before the start
    int max_length = 0;

    for (int i = 0; i < static_cast<int>(s.size()); ++i) {
        if (s[i] == '(') {
            indices.push(i);
        } else {  // s[i] == ')'
            indices.pop();
            if (indices.empty()) {
                // No matching '(' for this ')'; this ')' cannot be part of a valid substring.
                // Push it as a new sentinel.
                indices.push(i);
            } else {
                max_length = std::max(max_length, i - indices.top());
            }
        }
    }
    return max_length;
}
#include <cassert>
#include <string>

// Assume the function is declared above.
int main() {
    assert(longestValidParenthesesLength("") == 0);
    assert(longestValidParenthesesLength("(") == 0);
    assert(longestValidParenthesesLength(")") == 0);
    assert(longestValidParenthesesLength("()") == 2);
    assert(longestValidParenthesesLength("(()") == 2);
    assert(longestValidParenthesesLength(")()())") == 4);
    assert(longestValidParenthesesLength("((()))") == 6);
    assert(longestValidParenthesesLength("()(()") == 2);
    assert(longestValidParenthesesLength("(()))(") == 4);
    assert(longestValidParenthesesLength(")()(())") == 6);
    return 0;
}
