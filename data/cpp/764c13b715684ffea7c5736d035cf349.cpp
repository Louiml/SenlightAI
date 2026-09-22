// Write a C++ function `int evaluateBracketExpression(const std::string& input)` that computes the mathematical value of a balanced bracket expression using the following rules: each pair of `()` contributes 2 to the value, each pair of `[]` contributes 3 to the value, and nested brackets multiply their contributions. Specifically, the value of an expression is the sum of the products of the multipliers along each complete bracket pair. For example, `()` evaluates to 2, `[]` to 3, `(())` to 4 (2*2), `(()[()])` to 2*2 + 2*3 = 10 (since the outer `()` contains two inner pairs `()` and `[]`, each multiplied by the outer’s 2). The input will contain only the characters `(`, `)`, `[`, and `]`, and will be guaranteed to be a non-empty, well-formed (balanced) expression. If the input is malformed (e.g., unbalanced or with mismatched closing brackets), the function should return 0. Your implementation must use a stack and a running multiplication factor, processing the string left to right exactly as the reference approach, and handle edge cases such as empty string, mismatched brackets, and unbalanced expressions correctly.
The solution uses a stack to track opening brackets and a running `temp` factor that represents the multiplier contributed by all currently open brackets. As we scan each character:
- For an opening `(`: push it, multiply `temp` by 2.
- For an opening `[`: push it, multiply `temp` by 3.
- For a closing `)`: if the previous character is `(` (meaning this is an immediate pair), add the current `temp` to `result`. Then, if the stack top is `(`, pop it; divide `temp` by 2. If the stack top is not `(` or the stack is empty, this is a mismatch, so the expression is invalid.
- For a closing `]`: similarly, if previous character is `[`, add `temp` to `result`; then if stack top is `[`, pop and divide `temp` by 3; otherwise invalid.

After processing all characters, if the stack is not empty, the expression was unbalanced, so return 0; otherwise return `result`. Edge cases: empty input (should return 0 because it’s not a valid non-empty expression), any mismatched or unbalanced sequence (e.g., `(]`, `([)]`, `(()`), and expressions with adjacent pairs like `()[]` which sums 2+3=5. Time complexity is O(n) where n is the length of the string, and space complexity is O(n) in the worst case for the stack.
#include <string>
#include <stack>

// Evaluate a bracket expression: () = 2, [] = 3, nested pairs multiply.
// Returns 0 for malformed input (unbalanced/mismatched).
int evaluateBracketExpression(const std::string& input) {
    std::stack<char> s;
    int result = 0;
    int temp = 1;

    for (size_t i = 0; i < input.length(); ++i) {
        char ch = input[i];
        if (ch == '(') {
            s.push(ch);
            temp *= 2;
        } else if (ch == '[') {
            s.push(ch);
            temp *= 3;
        } else if (ch == ')') {
            if (i > 0 && input[i - 1] == '(') {
                result += temp;
            }
            if (!s.empty() && s.top() == '(') {
                s.pop();
                temp /= 2;
            } else {
                return 0; // Mismatched or extra closing bracket
            }
        } else if (ch == ']') {
            if (i > 0 && input[i - 1] == '[') {
                result += temp;
            }
            if (!s.empty() && s.top() == '[') {
                s.pop();
                temp /= 3;
            } else {
                return 0; // Mismatched or extra closing bracket
            }
        } else {
            return 0; // Invalid character
        }
    }

    return s.empty() ? result : 0; // Unbalanced if stack not empty
}
#include <cassert>

int main() {
    // Basic single pairs
    assert(evaluateBracketExpression("()") == 2);
    assert(evaluateBracketExpression("[]") == 3);
    
    // Nested same type
    assert(evaluateBracketExpression("(())") == 4);
    assert(evaluateBracketExpression("[[[]]]") == 27); // 3*3*3
    
    // Mixed nesting
    assert(evaluateBracketExpression("(()[()])") == 10); // 2*2 + 2*3
    assert(evaluateBracketExpression("([])") == 6); // 2*3
    
    // Adjacent pairs sum
    assert(evaluateBracketExpression("()[]") == 5);
    assert(evaluateBracketExpression("()()") == 4);
    
    // Complex expression
    assert(evaluateBracketExpression("([][()])") == 3*3 + 3*2); // 15
    
    // Malformed inputs return 0
    assert(evaluateBracketExpression("") == 0);
    assert(evaluateBracketExpression("(") == 0);
    assert(evaluateBracketExpression(")") == 0);
    assert(evaluateBracketExpression("(]") == 0);
    assert(evaluateBracketExpression("([)]") == 0);
    assert(evaluateBracketExpression("(()") == 0);
    
    return 0;
}
