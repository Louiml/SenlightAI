/*
Write a C++ function named `infixToPostfix` that takes a constant reference to a string representing a valid infix mathematical expression containing single-digit numbers (0-9), lowercase and uppercase letters (as operands), and the operators `+`, `-`, `*`, `/`, and `^` (exponentiation), along with parentheses `(` and `)`. The function must return a string representing the equivalent postfix (Reverse Polish Notation) expression. Operators must be output in the correct order based on standard precedence (`^` highest, then `*` and `/`, then `+` and `-`), and left-to-right associativity for all operators (i.e., `^` is treated as left-associative for this task, matching the given snippet). Parentheses must be handled correctly, and there should be no spaces in the output. The input is guaranteed to be well-formed, but you should handle optional leading/trailing whitespace if present.
*/

#include <string>
#include <stack>
#include <cctype>

// Helper to return precedence of an operator (higher means higher precedence).
int priority(char op) {
    if (op == '^') return 3;
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0; // for '(' or any invalid character
}

// Convert a valid infix expression (letters/digits, + - * / ^, parentheses) to postfix.
std::string infixToPostfix(const std::string& expr) {
    std::stack<char> st;
    std::string result;
    
    for (char ch : expr) {
        // Skip whitespace if any (though input is typically without spaces)
        if (std::isspace(static_cast<unsigned char>(ch))) {
            continue;
        }
        
        // Operand (letter or digit)
        if (std::isalnum(static_cast<unsigned char>(ch))) {
            result += ch;
        }
        // Left parenthesis
        else if (ch == '(') {
            st.push(ch);
        }
        // Right parenthesis
        else if (ch == ')') {
            while (!st.empty() && st.top() != '(') {
                result += st.top();
                st.pop();
            }
            if (!st.empty()) { // pop the '('
                st.pop();
            }
        }
        // Operator (+, -, *, /, ^)
        else {
            while (!st.empty() && priority(st.top()) >= priority(ch)) {
                result += st.top();
                st.pop();
            }
            st.push(ch);
        }
    }
    
    // Pop any remaining operators
    while (!st.empty()) {
        result += st.top();
        st.pop();
    }
    
    return result;
}

#include <cassert>
#include <string>

// Declare the function from the solution
std::string infixToPostfix(const std::string& expr);

int main() {
    // Basic arithmetic
    assert(infixToPostfix("A+B*C") == "ABC*+");
    assert(infixToPostfix("A+B*C-D") == "ABC*+D-");
    assert(infixToPostfix("a+b*(c^d-e)^(f+g*h)-i") == "abcd^e-fgh*+^*+i-");
    
    // Parentheses
    assert(infixToPostfix("(A+B)*C") == "AB+C*");
    assert(infixToPostfix("((A+B))") == "AB+");
    assert(infixToPostfix("A*(B+C/(D-E))") == "ABCDE-/+*");
    
    // Precedence and left-associativity
    assert(infixToPostfix("A^B^C") == "AB^C^");
    assert(infixToPostfix("A-B-C") == "AB-C-");
    assert(infixToPostfix("A/B*C") == "AB/C*");
    
    // Digits and letters mixed
    assert(infixToPostfix("1+2*3") == "123*+");
    assert(infixToPostfix("x/(y-z)+w") == "xyz-/w+");
    
    // Single operand
    assert(infixToPostfix("A") == "A");
    assert(infixToPostfix("5") == "5");
    
    // With whitespace (though not required, we handle it)
    assert(infixToPostfix("  A + B * C  ") == "ABC*+");
    
    return 0;
}

// The solution uses a stack to temporarily hold operators and left parentheses. The algorithm scans the input left to right:  
// - If the current character is an operand (letter or digit), append it directly to the result.  
// - If it is a left parenthesis `(`, push it onto the stack.  
// - If it is a right parenthesis `)`, pop operators from the stack and append them to the result until a left parenthesis is encountered, then pop and discard that left parenthesis.  
// - If it is an operator, while the stack is non-empty and the top of the stack has priority greater than or equal to the current operator (since we treat all operators as left-associative), pop and append to result. Then push the current operator.  
// After processing all characters, pop any remaining operators from the stack and append them.  
//
// Edge cases:  
// - Empty input or only operands → return the same string.  
// - Nested parentheses → correctly discarded.  
// - Multiple operators of same precedence → left-associative handling means the earlier operator is popped before the later one is pushed, producing correct order.  
// - The function assumes input is valid, so no error handling is needed.  
//
// Time complexity: O(n) where n is the length of the input string, because each character is processed once and each operator is pushed and popped at most once. Space complexity: O(n) for the stack in the worst case (e.g., many operators or open parentheses). The output string also uses O(n) space.
