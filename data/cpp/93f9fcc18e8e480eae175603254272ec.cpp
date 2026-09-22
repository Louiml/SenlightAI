Write a C++ function `evaluatePostfix` that takes a null-terminated character array (C-string) representing a postfix expression containing single-digit operands (0-9) and the binary operators `+`, `-`, `*`, and `/`. The function must return the integer result of evaluating the expression. The input will always be a valid postfix expression with no spaces or other characters, and division is integer division (truncating toward zero). You must implement the evaluation using an integer stack, without using the C++ standard template library stack or any other container. The function should handle expressions of arbitrary length up to 1000 characters, and operands are single digits only.

#include <cassert>

// Forward declaration for the function under test (since the test is separate).
int evaluatePostfix(const char* expr);

int main() {
    assert(evaluatePostfix("23+") == 5);
    assert(evaluatePostfix("53-") == 2);
    assert(evaluatePostfix("63/") == 2);
    assert(evaluatePostfix("24*") == 8);
    assert(evaluatePostfix("23+4*") == 20);
    assert(evaluatePostfix("82/3-") == 1);
    assert(evaluatePostfix("93-5+") == 11);
    assert(evaluatePostfix("123*+") == 7);
    assert(evaluatePostfix("52/") == 2);
    assert(evaluatePostfix("72-3*") == 15);
    return 0;
}

#include <cstddef>

// Evaluate a postfix expression with single-digit operands and + - * / operators.
// The input is a null-terminated C-string. Returns the integer result.
int evaluatePostfix(const char* expr) {
    int stack[1000];
    int top = -1;

    for (int i = 0; expr[i] != '\0'; ++i) {
        char ch = expr[i];

        if (ch >= '0' && ch <= '9') {
            stack[++top] = ch - '0';
        } else {
            int right = stack[top--];
            int left = stack[top--];

            int result = 0;
            if (ch == '+') {
                result = left + right;
            } else if (ch == '-') {
                result = left - right;
            } else if (ch == '*') {
                result = left * right;
            } else if (ch == '/') {
                result = left / right;
            }
            stack[++top] = result;
        }
    }

    return stack[top];
}

// The core algorithm processes the postfix expression from left to right. Maintain an integer stack (implemented as a fixed-size array with a top index). For each character: if it is a digit (0-9), convert it to an integer (`ch - '0'`) and push it onto the stack. If it is an operator, pop the top two operands: the first popped is the right operand `a`, and the second popped is the left operand `b`. Apply the operator to `b` and `a` (in that order, important for subtraction and division), compute the result, and push the result back onto the stack. At the end, the stack should contain exactly one value, which is the final result. Edge cases include negative results from subtraction, integer division truncation (which C++ handles by truncating toward zero for negative dividends), and expressions with multiple operations. Time complexity is O(n) where n is the length of the string, since each character is processed once. Space complexity is O(n) in the worst case (e.g., all digits before any operator), but bounded by the fixed stack size of 1000.
