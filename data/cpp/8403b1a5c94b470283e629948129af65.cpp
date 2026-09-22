// Write a C++ function `std::string infixToPostfix(const std::string& infix)` that converts a valid infix arithmetic expression containing single-letter operands (uppercase or lowercase letters, or digits), parentheses `(` and `)`, and the four basic binary operators `+`, `-`, `*`, `/` into its postfix (Reverse Polish Notation) form. The input may include spaces or commas as separators, which should be ignored. The function must correctly respect operator precedence (`*` and `/` have higher precedence than `+` and `-`) and left associativity (e.g., `a-b+c` becomes `ab-c+`, not `abc-+`). Parentheses override normal precedence. Assume the input is always syntactically valid (balanced parentheses and no missing operands). The output should contain no spaces or commas.
#include <cassert>
#include <string>

int main() {
    assert(infixToPostfix("a+b*c") == "abc*+");
    assert(infixToPostfix("(a+b)*c") == "ab+c*");
    assert(infixToPostfix("a-b+c") == "ab-c+");
    assert(infixToPostfix("a*(b+c/d)-e") == "abcd/+*e-");
    assert(infixToPostfix("1+2*3") == "123*+");
    assert(infixToPostfix("(A-B)*(C+D)") == "AB-CD+*");
    assert(infixToPostfix("a/b/c") == "ab/c/");
    assert(infixToPostfix("x*(y-z)") == "xyz-*");
    assert(infixToPostfix("  a , b - c ") == "ab+c-"); // spaces and commas ignored
    assert(infixToPostfix("a") == "a");
    return 0;
}
#include <string>
#include <stack>
#include <cctype>

// Helper: check if a character is an operand (letter or digit)
bool isOperand(char c) {
    return std::isalnum(static_cast<unsigned char>(c));
}

// Helper: get precedence weight of an operator
int precedenceWeight(char c) {
    if (c == '*' || c == '/') return 2;
    if (c == '+' || c == '-') return 1;
    return -1; // for non-operators
}

// Helper: check if the first operator has higher or equal precedence than the second
bool hasHigherOrEqualPrecedence(char op1, char op2) {
    return precedenceWeight(op1) >= precedenceWeight(op2);
}

// Convert a valid infix expression to postfix, ignoring spaces and commas.
std::string infixToPostfix(const std::string& infix) {
    std::stack<char> opStack;
    std::string postfix;

    for (char c : infix) {
        if (c == ' ' || c == ',') {
            continue;
        } else if (isOperand(c)) {
            postfix += c;
        } else if (c == '(') {
            opStack.push(c);
        } else if (c == ')') {
            while (!opStack.empty() && opStack.top() != '(') {
                postfix += opStack.top();
                opStack.pop();
            }
            if (!opStack.empty()) {
                opStack.pop(); // discard '('
            }
        } else { // operator
            while (!opStack.empty() && opStack.top() != '(' && hasHigherOrEqualPrecedence(opStack.top(), c)) {
                postfix += opStack.top();
                opStack.pop();
            }
            opStack.push(c);
        }
    }

    while (!opStack.empty()) {
        postfix += opStack.top();
        opStack.pop();
    }

    return postfix;
}
// The solution uses the standard shunting-yard algorithm adapted for this task. We iterate over each character in the input string. We skip spaces and commas. If the character is an operand (letter or digit), we append it directly to the result string. If it is an operator, we first pop from the stack and append to the result all operators that are on top of the stack and have precedence greater than or equal to the current operator (this ensures left associativity), and then we push the current operator onto the stack. If the character is a left parenthesis `(`, we push it onto the stack. If it is a right parenthesis `)`, we pop and append operators from the stack until we encounter a left parenthesis, which we then pop and discard. After processing all characters, we pop any remaining operators from the stack and append them. Edge cases include: nested parentheses, repeated operators, and expressions where an operator appears at the end (though the input is guaranteed valid, the algorithm handles it naturally). Time complexity is O(n) where n is the length of the input string, since each character is processed once and each operator is pushed and popped at most once. Space complexity is O(n) in the worst case for the stack and the output string.
