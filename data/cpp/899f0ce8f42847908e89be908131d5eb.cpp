// Given a single string representing a simple arithmetic expression of the form `"A op B"` where `A` and `B` are integers (which may include a leading minus sign) and `op` is one of `+`, `-`, `*`, or `/` (integer division, truncating toward zero), write a C++ function `std::string calculateExpression(const std::string& expression)` that extracts the two operands and the operator, parses them, performs the operation, and returns the result as a string. Handle negative operands correctly (e.g., `"-5 + 3"`, `"4 - -2"`), and assume the input is always well-formed with exactly one space before and after the operator (e.g., `"12 + 5"`). Division by zero can be assumed not to occur. The result must be an integer (for division, perform integer division, truncating toward zero, e.g., `-7 / 2` gives `-3`). Return the result as a string with no extra whitespace.

The main challenge is parsing a string where operands may be negative, making it ambiguous where the number ends and the operator begins if we rely on simple tokenization. The key insight from the snippet is to use the space character as a delimiter to split the expression into three tokens: the first operand, the operator (which is a single character surrounded by spaces), and the second operand. We first locate the first space using `find`, which separates the first operand from the rest. The character immediately after that space is the operator. The second operand starts two characters later (skipping the operator and the following space). Once we have the substrings for both operands, we handle leading minus signs: if an operand begins with `'-'`, we remove that sign and record its sign separately. After cleaning, we convert the numeric parts to integers via `std::stoi`. Then we apply the operator using a `switch` or `if` chain. For division, we use integer division (truncating toward zero in C++ for negative numbers). Edge cases include both operands being negative, only one being negative, or neither. The result is converted back to a string using `std::to_string`. The algorithm runs in O(n) time where n is the length of the input string (due to substring operations) and O(n) auxiliary space for the substrings and returned string.

#include <string>
#include <cstddef>

// Given an expression "A op B" with exactly one space around op,
// returns the result of the integer operation as a string.
std::string calculateExpression(const std::string& expression) {
    // Locate the first space separating the first operand from the operator.
    std::size_t first_space = expression.find(' ');
    // Operator is the character right after the first space.
    char operation = expression[first_space + 1];
    // Second operand starts after the operator and the following space.
    std::size_t second_start = first_space + 3;

    // Extract raw operand substrings.
    std::string first_raw = expression.substr(0, first_space);
    std::string second_raw = expression.substr(second_start);

    // Determine signs and strip leading minus signs.
    bool first_negative = false;
    if (!first_raw.empty() && first_raw[0] == '-') {
        first_negative = true;
        first_raw = first_raw.substr(1);
    }
    bool second_negative = false;
    if (!second_raw.empty() && second_raw[0] == '-') {
        second_negative = true;
        second_raw = second_raw.substr(1);
    }

    // Convert numeric parts to integers.
    int first = std::stoi(first_raw);
    int second = std::stoi(second_raw);
    if (first_negative) first = -first;
    if (second_negative) second = -second;

    // Perform the operation.
    int result = 0;
    switch (operation) {
        case '+': result = first + second; break;
        case '-': result = first - second; break;
        case '*': result = first * second; break;
        case '/': result = first / second; break; // Integer division, truncates toward zero.
    }

    return std::to_string(result);
}

#include <cassert>
#include <string>

// The solution function is declared here (for testing, we assume it is included above).
std::string calculateExpression(const std::string& expression);

int main() {
    assert(calculateExpression("5 + 3") == "8");
    assert(calculateExpression("10 - 4") == "6");
    assert(calculateExpression("6 * 7") == "42");
    assert(calculateExpression("20 / 4") == "5");
    assert(calculateExpression("-5 + 3") == "-2");
    assert(calculateExpression("4 - -2") == "6");
    assert(calculateExpression("-8 * -3") == "24");
    assert(calculateExpression("-7 / 2") == "-3");
    assert(calculateExpression("0 + 0") == "0");
    assert(calculateExpression("100 - 200") == "-100");
    return 0;
}
