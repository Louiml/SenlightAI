// Write a C++ function named `infixToPostfix` that takes a single string representing an infix arithmetic expression containing lowercase letters (`a`–`z`) as operands and the operators `+`, `-`, `*`, `/`, `^` (exponentiation, right-associative), along with parentheses `(` and `)`, and returns the equivalent postfix expression as a string. The input is guaranteed to be a valid infix expression (no unbalanced parentheses, no invalid tokens). The function must handle standard precedence: `^` has the highest precedence, followed by `/` and `*`, then `+` and `-`. Operators of equal precedence (except `^`, which is right-associative) are left-associative, meaning they are popped from the stack when a new operator of equal or lower precedence arrives. Parentheses override precedence but do not appear in the output. The function must not modify the input and must be efficient for strings of length up to 10,000.

#include <cassert>
#include <string>

// Forward declaration of the function under test
std::string infixToPostfix(const std::string& expr);

int main() {
    // Basic addition and subtraction
    assert(infixToPostfix("a+b") == "ab+");
    assert(infixToPostfix("a-b-c") == "ab-c-");
    // Multiplication and division have higher precedence
    assert(infixToPostfix("a+b*c") == "abc*+");
    assert(infixToPostfix("a*b/c") == "ab*c/");
    // Exponent is right-associative and has highest precedence
    assert(infixToPostfix("a^b^c") == "abc^^");
    assert(infixToPostfix("a+b^c") == "abc^+");
    // Parentheses override precedence
    assert(infixToPostfix("(a+b)*c") == "ab+c*");
    assert(infixToPostfix("a*(b+c)") == "abc+*");
    // Nested parentheses
    assert(infixToPostfix("((a+b)*c-d)") == "ab+c*d-");
    // Single operand
    assert(infixToPostfix("x") == "x");
    // Mixed operators
    assert(infixToPostfix("a-b*c+d^e") == "abc*-de^+");
    return 0;
}

#include <string>
#include <stack>

// Convert an infix expression to postfix using the shunting-yard algorithm.
// Only lowercase letters, +, -, *, /, ^, ( and ) are allowed.
// The input is assumed to be a valid infix expression.
std::string infixToPostfix(const std::string& expr) {
    std::string output;
    std::stack<char> operators;

    // Helper to get precedence of an operator (higher = higher precedence)
    auto precedence = [](char op) -> int {
        if (op == '^') return 3;
        if (op == '*' || op == '/') return 2;
        if (op == '+' || op == '-') return 1;
        return 0; // parentheses or invalid
    };

    for (char token : expr) {
        if (token >= 'a' && token <= 'z') {
            // Operand goes directly to output
            output.push_back(token);
        } else if (token == '(') {
            operators.push(token);
        } else if (token == ')') {
            // Pop until matching '('
            while (!operators.empty() && operators.top() != '(') {
                output.push_back(operators.top());
                operators.pop();
            }
            if (!operators.empty()) {
                operators.pop(); // discard '('
            }
        } else {
            // Operator token
            int currentPrec = precedence(token);
            // For right-associative '^', we do not pop equal precedence.
            // For others, we pop while top has greater or equal precedence.
            while (!operators.empty() && operators.top() != '(' &&
                   (precedence(operators.top()) > currentPrec ||
                    (precedence(operators.top()) == currentPrec && token != '^'))) {
                output.push_back(operators.top());
                operators.pop();
            }
            operators.push(token);
        }
    }

    // Pop remaining operators
    while (!operators.empty()) {
        output.push_back(operators.top());
        operators.pop();
    }

    return output;
}

// The solution uses the classic shunting-yard algorithm with an explicit operator stack and an output queue. Each character in the input is processed once:
// - If it is a lowercase letter, it is immediately appended to the output string.
// - If it is `(`, it is pushed onto the stack.
// - If it is `)`, operators are popped from the stack and appended to the output until a `(` is found, which is then discarded. If no `(` is found (invalid input), this would be an error, but the task guarantees valid input.
// - If it is an operator, the algorithm pops operators from the stack while the stack is non-empty, the top is not `(`, and the top has **greater** precedence than the current operator, or **equal** precedence and the current operator is **not** right-associative (`^`). After popping, the current operator is pushed.
//   - Precedence: `^` = 3, `/` and `*` = 2, `+` and `-` = 1.
//   - For `^`, because it is right-associative, we do not pop equal-precedence operators (e.g., `^` on top of `^` stays).
// - After processing all characters, any remaining operators on the stack are popped and appended to the output.
//
// The algorithm correctly handles parentheses, precedence, and associativity. Edge cases include expressions with only a single operand (output is that operand), nested parentheses, and sequences of operators with equal precedence like `a-b-c` → `ab-c-`. Time complexity is O(n) because each character is pushed and popped at most once. Space complexity is O(n) in the worst case for the stack and output string.
