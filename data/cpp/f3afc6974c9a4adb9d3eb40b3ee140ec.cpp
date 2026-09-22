// Write a C++ function that evaluates a valid Reverse Polish Notation (RPN) expression represented as a vector of strings, where each string is either an integer (which may be negative) or one of the four binary operators "+", "-", "*", "/". The function must return the integer result of the expression, using integer division that truncates toward zero. The input vector is non-empty and guaranteed to be a valid RPN expression (e.g., ["2","1","+","3","*"] = 9). Do not modify the original vector; take it by const reference.

// The given snippet uses a recursive approach that processes tokens from the end (postfix) backward. The idea: the last token is always an operator (unless the expression is a single number). When an operator is found, the two operands are the results of recursively evaluating the sub-expressions that precede it. Because the vector is processed from back to front, the recursive calls evaluate the right operand first, then the left operand, which matches the order of RPN evaluation. The base case is a token that is not an operator, which is converted to an integer via `stoi`. Edge cases: negative numbers are handled because `stoi` accepts them, and division by zero will not occur because the input is valid. Integer division truncates toward zero in C++ (since C++11), which matches typical RPN requirements. Time complexity is O(n) where n is the number of tokens, as each token is processed exactly once. Space complexity is O(n) due to recursion depth (which in the worst case equals the number of tokens for skewed expressions, but for valid RPN the depth is typically O(n) in pathological cases like a long chain of operators). To avoid modifying the input, we can pass a copy or use an index reference; the reference solution below uses a recursive helper with an index that moves from the end to the beginning, ensuring the original vector is unchanged.

#include <string>
#include <vector>
#include <cstdlib>

// Evaluate a valid Reverse Polish Notation expression from a vector of tokens.
// The vector is taken by const reference and is not modified.
// Returns the integer result, using integer division truncating toward zero.
int evalRPN(const std::vector<std::string>& tokens) {
    // Helper function that processes tokens from the end of the vector.
    // pos is a reference to the current index; it moves backward.
    std::function<int()> solve = [&]() -> int {
        // Since we process from the end, the last unprocessed token is at 'pos'.
        // We need pos to be shared across recursive calls, so use a mutable local.
        // Instead, we'll use a separate index variable captured by reference.
        // The lambda below is defined inside a block that holds an index.
        return 0; // Placeholder, real implementation below.
    };
    
    // Proper implementation using an index variable.
    int pos = static_cast<int>(tokens.size()) - 1;
    std::function<int()> evaluate = [&]() -> int {
        const std::string& token = tokens[pos];
        --pos;
        if (token != "+" && token != "-" && token != "*" && token != "/") {
            return std::stoi(token);
        }
        // Evaluate right operand first, then left operand (because we're going backward).
        int right = evaluate();
        int left = evaluate();
        if (token == "+") return left + right;
        if (token == "-") return left - right;
        if (token == "*") return left * right;
        return left / right; // integer division truncates toward zero.
    };
    return evaluate();
}

#include <cassert>
#include <vector>
#include <string>

// The solution function is assumed to be declared above.
int main() {
    std::vector<std::string> expr1 = {"2", "1", "+", "3", "*"};
    assert(evalRPN(expr1) == 9);

    std::vector<std::string> expr2 = {"4", "13", "5", "/", "+"};
    assert(evalRPN(expr2) == 6);

    std::vector<std::string> expr3 = {"10", "6", "9", "3", "+", "-11", "*", "/", "*", "17", "+", "5", "+"};
    assert(evalRPN(expr3) == 22);

    std::vector<std::string> expr4 = {"18"};
    assert(evalRPN(expr4) == 18);

    std::vector<std::string> expr5 = {"-2", "3", "*"};
    assert(evalRPN(expr5) == -6);

    std::vector<std::string> expr6 = {"5", "-2", "/"};
    assert(evalRPN(expr6) == -2); // truncates toward zero: 5 / -2 = -2

    std::vector<std::string> expr7 = {"0", "3", "/"};
    assert(evalRPN(expr7) == 0);

    std::vector<std::string> expr8 = {"-3", "-4", "-"};
    assert(evalRPN(expr8) == 1); // (-3) - (-4) = 1

    std::vector<std::string> expr9 = {"2", "3", "4", "*", "+"};
    assert(evalRPN(expr9) == 14);

    std::vector<std::string> expr10 = {"5", "1", "2", "+", "4", "*", "+", "3", "-"};
    assert(evalRPN(expr10) == 14);
}
