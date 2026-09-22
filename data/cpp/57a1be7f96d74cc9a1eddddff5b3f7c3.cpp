// Write a standalone C++ function `longestValidParentheses` that takes a string consisting only of the characters `'('` and `')'` and returns the length of the longest valid (well-formed) parentheses substring. A valid substring must have matching pairs of parentheses in the correct order, e.g., `"()"`, `"()()"`, `"(())"` are valid, while `")("`, `"())"`, `"(()"` are not. The function should handle empty strings and strings with no valid substring (return 0). The input string may be up to 10^4 characters long, so the solution should run in linear time. The task is to implement this function from scratch; you may not use any standard library function that directly solves the problem (e.g., no regex, no stack-based matching from a library), but you may use containers and algorithms as needed.

// The problem of finding the longest valid parentheses substring can be solved efficiently in O(n) time using a stack-based approach or a two-pass counting method. Since the constraint is up to 10^4 characters, O(n) time is required; O(n) space is acceptable but not necessary.
//
// The stack approach: Iterate through the string, using a stack that stores indices of unmatched characters. Initialize the stack with a sentinel index `-1`. For each character at index `i`:
// - If it is `'('`, push `i` onto the stack.
// - If it is `')'`, pop the top of the stack. After popping:
//   - If the stack becomes empty, push `i` (this becomes the new base index for the next valid substring).
//   - If the stack is not empty, compute the length of the current valid substring as `i - stack.top()` and update the maximum.
//
// Edge cases: Empty string returns 0. Strings with no valid substring (e.g., `")("`, `"((("`, `")))"`) return 0. Strings with leading `')'` are handled by the sentinel approach because the stack never goes negative after the first pop. The time complexity is O(n) because we process each character once and each index is pushed and popped at most once. The space complexity is O(n) in the worst case (e.g., all `'('`).
//
// Alternative: A two-pass counting approach (left-to-right and right-to-left) uses O(1) space and is also O(n) time. It counts `left` and `right`; when `left == right`, update max; when `right > left`, reset both to 0. Then traverse from right to left similarly to handle cases where there are more `'('` than `')'`. This is more space-efficient. The solution below uses the stack method for clarity.

#include <string>
#include <stack>
#include <algorithm>

// Returns the length of the longest valid parentheses substring.
int longestValidParentheses(const std::string& s) {
    std::stack<int> indices;
    indices.push(-1); // sentinel base index

    int max_len = 0;

    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '(') {
            indices.push(static_cast<int>(i));
        } else { // s[i] == ')'
            indices.pop();
            if (indices.empty()) {
                // No matching '(' left; this ')' starts a new base.
                indices.push(static_cast<int>(i));
            } else {
                // Valid substring from indices.top()+1 to i has length i - indices.top()
                max_len = std::max(max_len, static_cast<int>(i) - indices.top());
            }
        }
    }

    return max_len;
}

#include <cassert>
#include <string>

// Function declaration from the solution
int longestValidParentheses(const std::string& s);

int main() {
    // Basic cases
    assert(longestValidParentheses("") == 0);
    assert(longestValidParentheses("(") == 0);
    assert(longestValidParentheses(")") == 0);
    assert(longestValidParentheses("()") == 2);
    assert(longestValidParentheses("())") == 2);
    assert(longestValidParentheses("(()") == 2);
    assert(longestValidParentheses(")()(") == 2);
    assert(longestValidParentheses("()()") == 4);
    assert(longestValidParentheses("(())") == 4);
    assert(longestValidParentheses("(()())") == 6);

    // Mixed and longer cases
    assert(longestValidParentheses("())())") == 4);
    assert(longestValidParentheses("()(())") == 6);
    assert(longestValidParentheses(")()())") == 4);
    assert(longestValidParentheses("((()))") == 6);
    assert(longestValidParentheses("())(()") == 2);
    assert(longestValidParentheses("(()))())(") == 4);

    // All same characters
    assert(longestValidParentheses("(((((") == 0);
    assert(longestValidParentheses(")))))") == 0);
    assert(longestValidParentheses("(((())))") == 8);

    return 0;
}
