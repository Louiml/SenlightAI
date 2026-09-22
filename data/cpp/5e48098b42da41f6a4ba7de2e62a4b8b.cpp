Write a C++ function named `prefixToPostfix` that takes a non-empty string representing a valid arithmetic expression in prefix notation, where operands are single digits (`'0'` to `'9'`) and operators are one of `'+'`, `'-'`, `'*'`, or `'/'`. The function must return a string representing the equivalent expression in postfix notation, with operands and operators arranged in the correct order, and no extra spaces. For example, the prefix expression `"-/*+79483"` (which corresponds to `-(/(*(+7 9) 4) 8) 3`? Actually the snippet processes operators before operands, so the valid prefix is a normal prefix string) should produce `"79+4*8/3-"`. The function must not use any external parsing libraries beyond `<string>`, `<stack>`, and `<cctype>`. You may assume the input is always valid (correct number of operands/operators), but it can be of any length greater than zero. Handle single-digit operands only, and do not modify the input string.

#include <cassert>
#include <string>

// Assume prefixToPostfix is defined above.

int main() {
    // Single operand
    assert(prefixToPostfix("5") == "5");
    // Simple binary operators
    assert(prefixToPostfix("+12") == "12+");
    assert(prefixToPostfix("-*+79483") == "79+4*8/3-"); // From original snippet
    // Nested expressions with all operators
    assert(prefixToPostfix("/*+23-45") == "23+45-*");
    // Left and right associativity handled by prefix structure
    assert(prefixToPostfix("-+1*23/45") == "123*+45/-");
    // Multiple digits in sequence, not as multi-digit numbers but separate operands
    assert(prefixToPostfix("++123") == "12+3+");
    // Deeper nesting
    assert(prefixToPostfix("+-*123/45") == "12*3+45/-");
    // All operators in one expression
    assert(prefixToPostfix("+*+123/4-56") == "12+3*45/6-+");
    // Longer chain of same operator
    assert(prefixToPostfix("--------12345678") == "12-3-4-5-6-7-8-");
    return 0;
}

#include <string>
#include <stack>
#include <cctype>

// Convert a valid prefix expression (single-digit operands, + - * /) to postfix.
// The input is assumed to be non-empty and syntactically valid.
std::string prefixToPostfix(const std::string& prefix) {
    std::stack<std::string> st;
    // Process characters from right to left
    for (int i = static_cast<int>(prefix.size()) - 1; i >= 0; --i) {
        char ch = prefix[i];
        if (std::isdigit(static_cast<unsigned char>(ch))) {
            // Operand: push as a single-character string
            st.push(std::string(1, ch));
        } else { // Operator: one of + - * /
            // Pop the two operands: first popped is left operand's postfix,
            // second popped is right operand's postfix
            std::string left = st.top();
            st.pop();
            std::string right = st.top();
            st.pop();
            // Combine as left + right + operator
            st.push(left + right + ch);
        }
    }
    // The final result is the only element in the stack
    return st.top();
}

// The standard approach for prefix-to-postfix conversion is to process the prefix expression from right to left using a stack. Since prefix notation places operators before their operands, scanning in reverse ensures that when we encounter an operator, the top two stack entries are the already-converted postfix sub-expressions of its two operands in correct order (the first stack item is the left operand's postfix, the second is the right operand's postfix). For each digit, push it directly as a string. For each operator, pop the top two strings, combine them as `left + right + operator`, and push the result back. At the end, the stack contains exactly one string, which is the postfix expression. This works without any special handling for operator precedence because prefix notation already defines evaluation order. Edge cases: only one digit (returns that digit), all operators with nested expressions (the stack depth never exceeds expression size), and no need to handle spaces or multi-digit numbers. Time complexity is O(n) where n is the length of the input string, as each character is processed once and string concatenation is linear in total length (amortized O(n) since each concatenation is proportional to the size of the growing postfix string). Space complexity is O(n) for the stack storing intermediate strings.
