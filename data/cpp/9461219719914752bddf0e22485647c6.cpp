Write a C++ function `int evaluateLeftToRight(const std::string& expr)` that takes a string representing an arithmetic expression containing only non-negative integers and the operators `+`, `-`, `*`, `/`, with no spaces, and terminated by a single `=`. The evaluation must be performed strictly left-to-right with no operator precedence (i.e., each operation is applied immediately to the previous result and the next number). Division is integer division and truncates toward zero; you may assume no division by zero and the expression always valid. The function must return the final result before the `=` sign. For example, `"2+3*4="` evaluates as `(2+3)*4 = 20`, and `"10-2/4="` evaluates as `(10-2)/4 = 2`.

The solution mimics a simple calculator without precedence. Start by reading the first integer from the string, then loop while the next character is not `=`. For each iteration, read an operator, then the next integer, and apply the operation to the current result. Update the current result and continue. The main edge cases include: input may be a single integer followed by `=` (e.g., `"5="` returns 5); division by zero should not occur per constraints; integer division truncates toward zero in C++ (for non-negative operands, it’s floor, but since inputs are non-negative, this is fine). The algorithm runs in O(n) time where n is the length of the string, and uses O(1) auxiliary space (only a few variables). The implementation uses `std::stoi` to parse numbers but must handle multi-digit numbers carefully by finding the next operator or `=`. A simpler approach is to iterate with an index and build numbers using `std::stoi` with a starting position, but we must ensure we skip the operator after reading it.

#include <string>
#include <cctype>

// Evaluate an expression like "2+3*4=" left-to-right with no precedence.
// Assumes non-negative integers, valid operators + - * /, and no division by zero.
int evaluateLeftToRight(const std::string& expr) {
    int result = 0;
    std::size_t pos = 0;

    // Parse the first number (up to the first operator or '=')
    std::size_t next_pos = pos;
    while (next_pos < expr.size() && expr[next_pos] != '+' && expr[next_pos] != '-' &&
           expr[next_pos] != '*' && expr[next_pos] != '/' && expr[next_pos] != '=') {
        ++next_pos;
    }
    result = std::stoi(expr.substr(pos, next_pos - pos));
    pos = next_pos;

    // Process remaining operators and numbers until '='
    while (pos < expr.size() && expr[pos] != '=') {
        char op = expr[pos];
        ++pos; // move past operator

        // Find end of next number
        std::size_t num_start = pos;
        while (pos < expr.size() && expr[pos] != '+' && expr[pos] != '-' &&
               expr[pos] != '*' && expr[pos] != '/' && expr[pos] != '=') {
            ++pos;
        }
        int operand = std::stoi(expr.substr(num_start, pos - num_start));

        // Apply operation
        if (op == '+') {
            result += operand;
        } else if (op == '-') {
            result -= operand;
        } else if (op == '*') {
            result *= operand;
        } else if (op == '/') {
            result /= operand;
        }
    }
    return result;
}

#include <cassert>

int main() {
    assert(evaluateLeftToRight("5=") == 5);
    assert(evaluateLeftToRight("2+3=") == 5);
    assert(evaluateLeftToRight("2+3*4=") == 20);
    assert(evaluateLeftToRight("10-2/4=") == 2);
    assert(evaluateLeftToRight("100/10/5=") == 2);
    assert(evaluateLeftToRight("1+2+3+4=") == 10);
    assert(evaluateLeftToRight("7*0+9=") == 9);
    assert(evaluateLeftToRight("12/3*2=") == 8);
    assert(evaluateLeftToRight("0-5-3=") == -8); // note: result may become negative
    assert(evaluateLeftToRight("8/2/2=") == 2);
    return 0;
}
