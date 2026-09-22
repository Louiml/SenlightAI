// Write a C++ function `std::string evaluatePostfix(const std::string& expr)` that takes a string containing a postfix (reverse Polish) expression with single-digit integer operands (0-9), whitespace separators, and the binary operators `+`, `-`, `*`, and `/`. The function should evaluate the expression and return the result as a string (using `std::to_string` on a `float`). If the expression is malformed (e.g., insufficient operands, invalid character, empty result stack after processing), return `"Invalid"`. If a division by zero is attempted, return `"Zero division"`. All operations are performed on `float` values, and division is standard floating-point division (not integer). The input may contain leading/trailing/multiple whitespace characters, and operands are always single characters (e.g., `'3'` means the value 3, not 30).
// The algorithm processes the input string character by character, using a `std::stack<float>` to hold intermediate results. For each character:
// - If it is a digit, convert it to a float and push it onto the stack.
// - If it is a whitespace, skip it.
// - If it is an operator, first check that the stack contains at least two values; if not, return `"Invalid"`. Otherwise, pop the top two operands (`val1` = top, `val2` = second top), then apply the operation in the correct order: for `-` compute `val2 - val1`, for `/` compute `val2 / val1`, for `+` compute `val2 + val1`, for `*` compute `val2 * val1`. For division, if `val2 == 0`, return `"Zero division"`. Push the result back.
// - If it is any other character (not a digit, whitespace, or operator), return `"Invalid"`.
//
// After processing all characters, if the stack is empty or contains more than one element, return `"Invalid"` (the latter case is redundant if single-digit operands are used, but it’s a safe check). Otherwise, return `std::to_string(s.top())`. Edge cases include: empty string → stack empty → `"Invalid"`; expression with a single operand (e.g., `"5"`) → returns `"5.000000"` (since `std::to_string` on a float yields a decimal); expressions with too few operands before an operator → `"Invalid"`; division by zero → `"Zero division"`; and invalid characters like letters or punctuation → `"Invalid"`. Time complexity is O(n) where n is the length of the input string; space complexity is O(n) in the worst case (stack size proportional to number of operands).
#include <string>
#include <stack>
#include <cctype>

// Evaluate a postfix expression with single-digit operands and +,-,*,/.
// Returns the result as a string, or "Invalid" / "Zero division" on errors.
std::string evaluatePostfix(const std::string& expr) {
    std::stack<float> stack;

    for (const char c : expr) {
        if (std::isdigit(c)) {
            stack.push(static_cast<float>(c - '0'));
        } else if (std::isspace(c)) {
            continue;
        } else if (c == '+' || c == '-' || c == '*' || c == '/') {
            if (stack.size() < 2) {
                return "Invalid";
            }

            const float val1 = stack.top();
            stack.pop();
            const float val2 = stack.top();
            stack.pop();

            switch (c) {
                case '+':
                    stack.push(val2 + val1);
                    break;
                case '-':
                    stack.push(val2 - val1);
                    break;
                case '*':
                    stack.push(val2 * val1);
                    break;
                case '/':
                    if (val2 == 0.0f) {
                        return "Zero division";
                    }
                    stack.push(val2 / val1);
                    break;
            }
        } else {
            return "Invalid";
        }
    }

    if (stack.size() != 1) {
        return "Invalid";
    }

    return std::to_string(stack.top());
}
#include <cassert>
#include <string>

// Declaration of the function under test (matches solution).
std::string evaluatePostfix(const std::string& expr);

int main() {
    // Basic addition, multiplication, and division
    assert(evaluatePostfix("3 4 +") == "7.000000");
    assert(evaluatePostfix("5 2 *") == "10.000000");
    assert(evaluatePostfix("8 2 /") == "4.000000");

    // Subtraction with order (val2 - val1)
    assert(evaluatePostfix("5 9 -") == "-4.000000");

    // Combined expression: (2 + 3) * 4 -> 20
    assert(evaluatePostfix("2 3 + 4 *") == "20.000000");

    // Whitespace variations
    assert(evaluatePostfix("  6  2  /  ") == "3.000000");
    assert(evaluatePostfix("1 2+") == "3.000000");

    // Single operand
    assert(evaluatePostfix("7") == "7.000000");

    // Invalid: missing operand
    assert(evaluatePostfix("+") == "Invalid");
    assert(evaluatePostfix("3 +") == "Invalid");

    // Invalid: extra operands left
    assert(evaluatePostfix("1 2 3 +") == "Invalid");

    // Invalid: non-operator character
    assert(evaluatePostfix("1 a 2 +") == "Invalid");

    // Zero division
    assert(evaluatePostfix("4 0 /") == "Zero division");

    // Empty expression
    assert(evaluatePostfix("") == "Invalid");
}
