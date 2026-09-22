/*
Write a C++ function `bool isExpressionTreeValid(const std::string& postfix)` that takes a postfix expression string (containing single-character alphanumeric operands and binary operators `+`, `-`, `*`, `/`, `^`) and returns `true` if the given postfix expression can be successfully converted into a valid binary expression tree using a stack-based construction algorithm (as shown in the code snippet), and `false` otherwise. The input string is guaranteed to be non-empty and contain only alphanumeric characters and the specified operators. A valid postfix expression must have exactly one more operand than operator, and the tree construction must not underflow the stack (i.e., every operator must have two operands available). Also, at the end, exactly one node must remain on the stack.
*/
#include <string>
#include <cctype>

// Validates whether a postfix expression can be built into a binary expression tree.
bool isExpressionTreeValid(const std::string& postfix) {
    int stackSize = 0;  // Simulates the stack of nodes from the snippet

    for (char ch : postfix) {
        if (std::isalnum(static_cast<unsigned char>(ch))) {
            ++stackSize;  // Operand: push a leaf node
        } else {
            // Operator: requires two operands already on the stack
            if (stackSize < 2) {
                return false;  // Underflow: insufficient operands
            }
            // Pop two operands and push one result
            stackSize -= 1;  // pop two, push one => net -1
        }
    }

    // Valid tree if exactly one node remains
    return stackSize == 1;
}
#include <cassert>
#include <string>

bool isExpressionTreeValid(const std::string& postfix);

int main() {
    // Valid postfix expressions
    assert(isExpressionTreeValid("a") == true);
    assert(isExpressionTreeValid("ab+") == true);
    assert(isExpressionTreeValid("ab+c*") == true);
    assert(isExpressionTreeValid("12+34*+") == true);
    assert(isExpressionTreeValid("xy*") == true);

    // Invalid expressions: underflow or leftover operands
    assert(isExpressionTreeValid("+") == false);
    assert(isExpressionTreeValid("+ab") == false);
    assert(isExpressionTreeValid("a+") == false);
    assert(isExpressionTreeValid("ab") == false);          // two operands, no operator
    assert(isExpressionTreeValid("ab+cd") == false);       // leftover operands
    assert(isExpressionTreeValid("a b +") == false);       // space not allowed per spec
}
// The solution simulates the stack-based tree construction algorithm from the snippet. Iterate through each character in the postfix string. If the character is an alphanumeric operand (isalnum), it represents a leaf node and should be treated as a successful "push" onto the stack. If the character is an operator, it requires two operands; we simulate popping them from the stack. Before popping, check that the stack has at least two elements; if not, the expression is invalid (underflow) → return `false`. After handling the operator, we "push" a new internal node back onto the stack (just incrementing a counter representing stack size). At the end, a valid tree requires exactly one node remaining on the stack (stack size == 1), otherwise the expression is malformed (e.g., too many operands or too few operators). This mimics the behavior of the snippet's `constructTree` without actually allocating memory. The time complexity is O(n) where n is the length of the string, and space complexity is O(1) beyond the stack counter. Edge cases include expressions starting with an operator (should return false), expressions with more than one remaining node (e.g., "ab+cd"), and empty or single-operand strings (single operand is valid).
