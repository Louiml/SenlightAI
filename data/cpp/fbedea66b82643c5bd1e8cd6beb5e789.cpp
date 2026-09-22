/*
Write a C++ function `int evaluateExpression(const std::string& expr)` that evaluates a basic arithmetic expression containing non-negative integers and the operators `+`, `-`, `*`, and `/` (integer division, truncating toward zero). The input string may contain spaces anywhere, but no parentheses, no negative numbers as literals, and no division by zero. The expression must be evaluated with standard operator precedence (multiplication and division before addition and subtraction), and the operators are left-associative for equal precedence. Return the integer result. The input is guaranteed to be non-empty and valid (e.g., not ending with an operator, no consecutive operators).
*/
#include <string>
#include <vector>
#include <numeric>
#include <cctype>

// Evaluates a basic arithmetic expression with +, -, *, / on non-negative integers.
// Handles arbitrary spaces, multi-digit numbers, and standard operator precedence.
int evaluateExpression(const std::string& expr) {
    std::vector<int> stack;
    char lastOperator = '+';
    int currentNumber = 0;
    const int len = expr.size();

    for (int i = 0; i < len; ++i) {
        const char ch = expr[i];
        if (std::isdigit(static_cast<unsigned char>(ch))) {
            currentNumber = currentNumber * 10 + (ch - '0');
        }

        // Process when we hit an operator (not space) OR the last character.
        if ((!std::isdigit(static_cast<unsigned char>(ch)) && ch != ' ') || (i == len - 1 && std::isdigit(static_cast<unsigned char>(ch)))) {
            switch (lastOperator) {
                case '+': stack.push_back(currentNumber); break;
                case '-': stack.push_back(-currentNumber); break;
                case '*': stack.back() *= currentNumber; break;
                case '/': stack.back() /= currentNumber; break;
            }
            lastOperator = ch;
            currentNumber = 0;
        }
    }

    return std::accumulate(stack.begin(), stack.end(), 0);
}
#include <cassert>
#include <string>

int main() {
    assert(evaluateExpression("3+2*2") == 7);
    assert(evaluateExpression(" 3/2 ") == 1);
    assert(evaluateExpression(" 3+5 / 2 ") == 5);
    assert(evaluateExpression("42") == 42);
    assert(evaluateExpression("1-1+1") == 1);
    assert(evaluateExpression("10*5/2-3+4") == 26);
    assert(evaluateExpression("2*3*4") == 24);
    assert(evaluateExpression("100/10/2") == 5);
    assert(evaluateExpression("  5 + 5 * 2 - 3 / 3  ") == 14);
    assert(evaluateExpression("0*0") == 0);
    return 0;
}
// The classic approach is a single pass with a stack. We maintain a running value `currentNumber` while scanning digits, and we remember the last seen operator (`lastOperator`), initialized to `'+'` before the first number. Whenever we encounter an operator or reach the end of the string, we process the `currentNumber` according to `lastOperator`: for `+` we push `currentNumber` onto the stack; for `-` we push `-currentNumber`; for `*` we multiply the top of the stack by `currentNumber`; for `/` we divide the top of the stack by `currentNumber` (integer division, truncating toward zero; division by zero is guaranteed not to occur). After processing, we update `lastOperator` to the current character (if it is an operator) and reset `currentNumber` to 0. At the end, the result is the sum of all stack values, which correctly respects precedence because multiplication/division are applied immediately to the top of the stack before addition/subtraction are summed later. Important edge cases: numbers may have multiple digits, leading/trailing spaces, spaces between operators and numbers, and the expression may be a single number (in which case the stack gets one value and the sum is that number). Complexities: time is O(n) where n is the string length, and space is O(n) in the worst case if many numbers are separated by addition/subtraction (stack size grows), but for typical expressions it is proportional to the number of number tokens.
