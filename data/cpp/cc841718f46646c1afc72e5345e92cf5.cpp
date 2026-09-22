Write a C++ function named `evaluatePostfix` that accepts a single string containing a valid postfix (reverse Polish) expression, where tokens are separated by single spaces and may include non-negative integers (fits in `long long`) and the binary operators `+`, `-`, and `*`. The function must return the numeric result as a `long long`. The input will always be a valid postfix expression with at least one operand, and division by zero or invalid tokens will not occur. The function must use a stack of `long long` values to process tokens left-to-right, pushing operands and, upon encountering an operator, popping the two most recent operands (right operand first, then left operand), applying the operator in the correct order (`left operator right`), and pushing the result back. The final stack top is the result.
The algorithm simulates a stack machine. Tokenize the input string by iterating through characters and extracting space-separated substrings, or use a string stream. For each token: if it is an operator (`+`, `-`, `*`), pop the top as the right operand, pop the next as the left operand, compute the result of `left op right`, and push that result. Otherwise, convert the token to `long long` using `std::stoll` and push it. At the end, the stack must contain exactly one element, which is the answer. Edge cases: operands may be large (up to `long long` bounds), but since multiplication of two large numbers might overflow, the problem assumes inputs are such that no overflow occurs. Operators must be applied in correct order: for subtraction and division (though division is not in operators here), the first popped is the right operand. Time complexity is O(n) where n is the number of tokens, because each token is processed once and stack operations are O(1). Space complexity is O(n) in the worst case (e.g., all operands before any operator), but typically O(depth of stack) which is O(n) worst-case.
#include <string>
#include <stack>
#include <sstream>

// Evaluates a valid postfix expression with integers and +, -, * operators.
// Assumes input is space-separated and valid; no overflow in intermediate results.
long long evaluatePostfix(const std::string& expression) {
    std::stack<long long> values;
    std::istringstream tokenStream(expression);
    std::string token;

    while (tokenStream >> token) {
        if (token == "+" || token == "-" || token == "*") {
            // Pop right operand first, then left operand.
            long long right = values.top();
            values.pop();
            long long left = values.top();
            values.pop();

            long long result = 0;
            if (token == "+") {
                result = left + right;
            } else if (token == "-") {
                result = left - right;
            } else { // "*"
                result = left * right;
            }
            values.push(result);
        } else {
            // Operand: convert and push.
            values.push(std::stoll(token));
        }
    }

    // The final result is the only value left.
    return values.top();
}
#include <cassert>

// Global main function to run assertions.
int main() {
    assert(evaluatePostfix("3 4 +") == 7);
    assert(evaluatePostfix("10 5 -") == 5);
    assert(evaluatePostfix("2 3 *") == 6);
    assert(evaluatePostfix("5 1 2 + 4 * + 3 -") == 14);  // (5 + ((1+2)*4)) - 3 = 14
    assert(evaluatePostfix("7") == 7);
    assert(evaluatePostfix("100 200 300 + +") == 600);
    assert(evaluatePostfix("8 3 2 * -") == 2);  // 8 - (3*2) = 2
    assert(evaluatePostfix("6 2 3 * +") == 12); // 6 + (2*3) = 12
    assert(evaluatePostfix("9 9 9 * *") == 729);
    assert(evaluatePostfix("1000000000 1000000000 *") == 1000000000000000000LL);
    return 0;
}
