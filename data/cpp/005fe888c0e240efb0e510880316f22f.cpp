// Write a C++ function that takes an infix arithmetic expression as a string containing single-letter variables/operands and the operators `+`, `-`, `*`, `/` along with parentheses, and returns two strings: the equivalent postfix (Reverse Polish Notation) expression and the equivalent prefix (Polish Notation) expression. The input is guaranteed to be a valid, non-empty infix expression with standard precedence (`*` and `/` higher than `+` and `-`) and left-to-right associativity for equal-precedence operators. Your function should correctly handle nested parentheses and ignore spaces if any are present. The output should contain no spaces, preserving the order of operands as they appear in the original infix string (except for operator repositioning). You must implement both conversions in a single function that returns a pair of strings, with the first being postfix and the second being prefix.
int main() {
    auto result1 = infixToPostfixAndPrefix("a+b-(c*d)/e");
    assert(result1.first == "abcd*e/-+");
    assert(result1.second == "+-+ab/cde");

    auto result2 = infixToPostfixAndPrefix("a*b+c");
    assert(result2.first == "ab*c+");
    assert(result2.second == "+*abc");

    auto result3 = infixToPostfixAndPrefix("(a+b)*(c-d)");
    assert(result3.first == "ab+cd-*");
    assert(result3.second == "*+ab-cd");

    auto result4 = infixToPostfixAndPrefix("a");
    assert(result4.first == "a");
    assert(result4.second == "a");

    auto result5 = infixToPostfixAndPrefix("a/b/c");
    assert(result5.first == "ab/c/");
    assert(result5.second == "//abc");

    auto result6 = infixToPostfixAndPrefix("a-(b-c)");
    assert(result6.first == "abc--");
    assert(result6.second == "-a-bc");

    auto result7 = infixToPostfixAndPrefix("((a+b)*c)");
    assert(result7.first == "ab+c*");
    assert(result7.second == "*+abc");

    auto result8 = infixToPostfixAndPrefix("a+b*(c-d)/e");
    assert(result8.first == "abcd-*e/+");
    assert(result8.second == "+a/*b-cde");

    auto result9 = infixToPostfixAndPrefix("(a-b/c)*d");
    assert(result9.first == "abc/-d*");
    assert(result9.second == "*-a/bcd");
}
#include <string>
#include <stack>
#include <cctype>
#include <utility>

// Helper: return precedence of operator; lower number = lower precedence
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0; // for '('
}

// Convert infix 'expr' (single-letter operands, valid operators, parentheses) to postfix.
// Then derive prefix by reversing, swapping parens, converting, and reversing back.
std::pair<std::string, std::string> infixToPostfixAndPrefix(const std::string& expr) {
    auto toPostfix = [](const std::string& infix) {
        std::string postfix;
        std::stack<char> operators;
        for (char ch : infix) {
            if (std::isalnum(static_cast<unsigned char>(ch))) {
                postfix += ch;
            } else if (ch == '(') {
                operators.push(ch);
            } else if (ch == ')') {
                while (!operators.empty() && operators.top() != '(') {
                    postfix += operators.top();
                    operators.pop();
                }
                if (!operators.empty()) operators.pop(); // remove '('
            } else { // operator
                while (!operators.empty() && precedence(operators.top()) >= precedence(ch) && operators.top() != '(') {
                    postfix += operators.top();
                    operators.pop();
                }
                operators.push(ch);
            }
        }
        while (!operators.empty()) {
            postfix += operators.top();
            operators.pop();
        }
        return postfix;
    };

    // Postfix directly from original
    std::string postfix = toPostfix(expr);

    // Build reversed infix with swapped parentheses for prefix conversion
    std::string reversed;
    for (auto it = expr.rbegin(); it != expr.rend(); ++it) {
        char ch = *it;
        if (ch == '(') reversed += ')';
        else if (ch == ')') reversed += '(';
        else reversed += ch;
    }
    std::string prefixReversed = toPostfix(reversed);
    std::string prefix(prefixReversed.rbegin(), prefixReversed.rend());

    return {postfix, prefix};
}
// The solution uses two classic stack‑based algorithms. For postfix conversion: scan the infix string from left to right. When an alphanumeric character is found, append it to the result. When an opening parenthesis `(` is found, push it onto the operator stack. When a closing parenthesis `)` is found, pop operators from the stack and append them until the matching `(` is popped (which is discarded). For any operator, while the stack is non‑empty and the top of the stack is an operator with precedence greater than or equal to the current operator’s precedence, pop and append that top operator; then push the current operator onto the stack. After processing all characters, pop and append any remaining operators. For prefix conversion: a common approach is to reverse the infix string, swap `(` and `)`, run the postfix algorithm on this reversed string, then reverse the resulting postfix string to obtain the prefix expression. Edge cases include expressions with only a single operand (no operators), deeply nested parentheses, and operators with equal precedence where left‑associativity must be respected in postfix (by using `>=` in the while condition). The overall time complexity is O(n) for both conversions, where n is the length of the input string, because each character is processed a constant number of times. Space complexity is O(n) in the worst case for the stack and the result strings.
