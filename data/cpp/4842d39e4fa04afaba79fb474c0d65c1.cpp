/*
Write a C++ function named `isBalancedBrackets` that takes a non-empty string `s` containing only the characters `(`, `)`, `[`, `]`, `{`, `}` (no spaces, no other characters) and returns a boolean indicating whether the brackets are correctly matched and nested. A string is balanced if every opening bracket has a corresponding closing bracket of the same type in the correct order, and the string is fully consumed. For example, `"([])"` is balanced, while `"([)]"` is not. The function should be `const`-correct and use a standard container efficiently. Provide a reference solution and test cases.
*/
#include <string>
#include <stack>

// Returns true if the brackets in s are properly matched and nested.
bool isBalancedBrackets(const std::string& s) {
    std::stack<char> openers;
    
    for (char ch : s) {
        if (ch == '(' || ch == '[' || ch == '{') {
            openers.push(ch);
        } else {
            // It's a closing bracket.
            if (openers.empty()) {
                return false;
            }
            char top = openers.top();
            openers.pop();
            
            // Check that the types match.
            if (ch == ')' && top != '(') return false;
            if (ch == ']' && top != '[') return false;
            if (ch == '}' && top != '{') return false;
        }
    }
    
    // All opened brackets must be closed.
    return openers.empty();
}
#include <cassert>

int main() {
    // Basic valid cases
    assert(isBalancedBrackets("()") == true);
    assert(isBalancedBrackets("[]") == true);
    assert(isBalancedBrackets("{}") == true);
    
    // Nested and mixed valid cases
    assert(isBalancedBrackets("({[]})") == true);
    assert(isBalancedBrackets("()[]{}") == true);
    assert(isBalancedBrackets("{[()()]}") == true);
    
    // Invalid: mismatched closer
    assert(isBalancedBrackets("(]") == false);
    assert(isBalancedBrackets("([)]") == false);
    
    // Invalid: unclosed openers or extra closers
    assert(isBalancedBrackets("(") == false);
    assert(isBalancedBrackets(")") == false);
    assert(isBalancedBrackets("())") == false);
    assert(isBalancedBrackets("(()") == false);
    
    // Invalid: empty input (but here we test with a single bracket)
    assert(isBalancedBrackets("{(})") == false);
    
    // Long valid nesting
    assert(isBalancedBrackets("(((((((((())))))))))") == true);
}
// The classic approach uses a stack (or vector as a stack) to keep track of unmatched opening brackets. Iterate through each character of the string:  
// - If it’s an opening bracket (`(`, `[`, `{`), push it onto the stack.  
// - If it’s a closing bracket (`)`, `]`, `}`):  
//   - If the stack is empty, there is no matching opener → return `false`.  
//   - Otherwise, pop the top character and check if it matches the corresponding opener for the current closing bracket. If not, return `false`.  
// After processing all characters, the string is only balanced if the stack is empty (i.e., all openers were matched).  
// Edge cases:  
// - Input like `")("` → closing bracket with empty stack → false.  
// - Input like `"("` → stack not empty at end → false.  
// - Input like `"()"` → matches, stack empty → true.  
// - Input like `"(()"` → stack has one leftover after processing → false.  
// - Nested and adjacent pairs like `"({[]})"` work correctly because the stack LIFO order enforces proper nesting.  
// Time complexity is O(n) where n is the string length, since each character is processed once with O(1) stack operations. Space complexity is O(n) in the worst case (e.g., all opening brackets), using the stack.
