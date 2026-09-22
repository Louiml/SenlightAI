/*
Write a C++ function named `isBalancedWorld` that takes a single non-empty string as input and returns a boolean value indicating whether the parentheses and brackets in the string are correctly balanced. The string may contain any printable ASCII characters, but only the round parentheses `()` and square brackets `[]` are considered for balance. The function must ensure that every opening bracket has a corresponding closing bracket of the same type, that brackets are properly nested (e.g., `([)]` is invalid), and that no closing bracket appears without a matching unclosed opening bracket. The input will not contain a trailing newline or other whitespace beyond what is naturally part of a sentence, and you may assume the string is terminated by a period `.` if it is meant as a complete sentence, but the function must handle any string that does not contain that sentinel as well. The function should return `true` if the entire string is balanced (including the case of zero brackets), and `false` otherwise.
*/
#include <string>
#include <stack>

// Returns true if all parentheses and brackets in the input string are correctly balanced.
bool isBalancedWorld(const std::string& text) {
    std::stack<char> brackets;

    for (char ch : text) {
        if (ch == '(' || ch == '[') {
            brackets.push(ch);
        } else if (ch == ')' || ch == ']') {
            if (brackets.empty()) {
                return false;
            }
            char open = brackets.top();
            if ((ch == ')' && open != '(') || (ch == ']' && open != '[')) {
                return false;
            }
            brackets.pop();
        }
        // All other characters are ignored.
    }

    return brackets.empty();
}
int main() {
    // Basic cases
    assert(isBalancedWorld("()") == true);
    assert(isBalancedWorld("[]") == true);
    assert(isBalancedWorld("()[]") == true);
    assert(isBalancedWorld("([])") == true);
    assert(isBalancedWorld("([)]") == false);
    assert(isBalancedWorld("(]") == false);

    // Edge cases
    assert(isBalancedWorld("") == true);
    assert(isBalancedWorld("hello world.") == true);
    assert(isBalancedWorld("(") == false);
    assert(isBalancedWorld(")") == false);
    assert(isBalancedWorld("(()") == false);
    assert(isBalancedWorld("())") == false);
    assert(isBalancedWorld("[a(b)c]") == true);
    assert(isBalancedWorld("a[b(c)d]e") == true);
    assert(isBalancedWorld("a[b(c)d]e)") == false);
    assert(isBalancedWorld("(a[b]c)") == true);
    assert(isBalancedWorld("((()))") == true);
    assert(isBalancedWorld("((())") == false);
}
(Note: The test code includes `assert` which requires `#include <cassert>` and is implied to be present in the global environment; for a standalone file, add that header before `main`.)
// The core algorithm uses a stack (LIFO) to track unmatched opening brackets. When reading each character:
// - If it is `(` or `[`, push it onto the stack.
// - If it is `)` or `]`, first check that the stack is not empty; if empty, it is unbalanced. Then compare the top of the stack with the expected matching opening bracket: `)` must match `(`, and `]` must match `[`. If they match, pop the stack; if not, return `false`.
// - Ignore all other characters.
//
// After processing the entire string, if the stack is empty, all brackets were matched correctly; otherwise, some opening brackets were left unclosed, so return `false`.
//
// Important edge cases:
// - Empty string (if allowed) should return `true` because there are no brackets.
// - A string with brackets but no nesting (e.g., `()[]`) should be balanced.
// - A string with reversed order (e.g., `)(` or `[(])`) should be unbalanced.
// - A string ending with an unclosed bracket (e.g., `(a`) should be unbalanced.
// - A string with only non-bracket characters should be balanced.
//
// Time complexity: O(n) where n is the length of the string, because each character is processed once and stack operations are O(1). Space complexity: O(m) where m is the maximum depth of nested brackets, at most O(n) in the worst case.
