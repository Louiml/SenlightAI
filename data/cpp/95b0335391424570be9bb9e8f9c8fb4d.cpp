Write a standalone C++ function `std::string infixToPostfix(const std::string& infix)` that converts a fully parenthesized infix expression into postfix notation. The input string may contain uppercase letters `A`–`Z` as operands and operators `+`, `-`, `*`, `/`, `^` (with usual precedence: `+`/`-` lowest, `*`/`/` higher, `^` highest), as well as parentheses `(` and `)`. The expression is guaranteed to be syntactically valid and balanced. The function must return the equivalent postfix expression as a string. Operands are single characters; operators are single characters; no spaces or other symbols appear in the input. Handle racket-style implicit precedence where `^` is right-associative (i.e., `a^b^c` means `a^(b^c)`) while all other binary operators are left-associative. The solution must implement a stack using a linked list (do not use `std::stack` or dynamic arrays) and use the standard shunting-yard algorithm. Edge cases include expressions with multiple parentheses, nested parentheses, single operand (e.g., `"(A)"`), and maximal operator precedence. The function should not modify the input string and must be `const`-correct.

The core algorithm is the well-known shunting-yard algorithm for infix-to-postfix conversion. We scan the input left to right, maintaining an auxiliary stack for operators and left parentheses. For each character:
- If it is an operand (`A`–`Z`), append it directly to the output postfix string.
- If it is an operator, then while the stack is non-empty and the top of stack is not `(` and the precedence of the top operator is greater than or equal to (for left-associative) or strictly greater than (for right-associative `^`) the precedence of the current operator, pop operators from the stack and append them to output. Then push the current operator.
- If it is `(`, push it onto the stack.
- If it is `)`, pop operators until a `(` is encountered (which is popped and discarded) and append each popped operator to output.
After processing all characters, pop any remaining operators from the stack and append them to output.

Important edge cases:
- The stack must never be accessed when empty except when a valid `(` is present; we assume balanced input.
- Right-associativity of `^` means we do not pop a previous `^` when encountering another `^` (only when the new operator has lower precedence). The standard approach uses a stack precedence table for in-stack (`isp`) and incoming (`icp`) precedence: for left-associative operators, use `>`, for right-associative use `>=`.
- The output string must be null-terminated appropriately (or use `std::string`).
- The linked list stack must handle dynamic memory correctly via `new`/`delete`.

Time complexity is \(O(n)\) for a string of length \(n\), since each character is pushed and popped at most once. Space complexity is \(O(n)\) in the worst case (e.g., a deeply nested expression) for the stack and the output string.

#include <string>
#include <cstddef>

// Linked list node for the stack
struct StackNode {
    char data;
    StackNode* next;
    explicit StackNode(char c) : data(c), next(nullptr) {}
};

// Simple stack using linked list
class CharStack {
private:
    StackNode* topNode;
public:
    CharStack() : topNode(nullptr) {}
    ~CharStack() {
        while (topNode) {
            StackNode* temp = topNode;
            topNode = topNode->next;
            delete temp;
        }
    }
    void push(char c) {
        StackNode* newNode = new StackNode(c);
        newNode->next = topNode;
        topNode = newNode;
    }
    char pop() {
        if (!topNode) return '\0';
        char c = topNode->data;
        StackNode* temp = topNode;
        topNode = topNode->next;
        delete temp;
        return c;
    }
    char top() const {
        return topNode ? topNode->data : '\0';
    }
    bool isEmpty() const {
        return topNode == nullptr;
    }
};

// Precedence of operator when it is on the stack (in-stack)
int isp(char op) {
    switch (op) {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
        case '^': return 3;
        case '(': return 0;
        case ')': return -1;
        default: return -1;
    }
}

// Precedence of operator when it is the incoming token
int icp(char op) {
    switch (op) {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
        case '^': return 4; // higher incoming precedence for right-assoc
        case '(': return 5;
        default: return -1;
    }
}

// Convert a fully parenthesized infix expression to postfix
std::string infixToPostfix(const std::string& infix) {
    CharStack ops;
    std::string postfix;
    postfix.reserve(infix.size());

    for (std::size_t i = 0; i < infix.size(); ++i) {
        char ch = infix[i];
        if (ch >= 'A' && ch <= 'Z') {
            postfix += ch;
        } else if (ch == '(') {
            ops.push(ch);
        } else if (ch == ')') {
            while (!ops.isEmpty() && ops.top() != '(') {
                postfix += ops.pop();
            }
            if (!ops.isEmpty()) {
                ops.pop(); // pop the '('
            }
        } else { // operator
            while (!ops.isEmpty() && ops.top() != '(' &&
                   isp(ops.top()) >= icp(ch)) {
                postfix += ops.pop();
            }
            ops.push(ch);
        }
    }

    while (!ops.isEmpty()) {
        postfix += ops.pop();
    }
    return postfix;
}

#include <cassert>
#include <string>

// (declare the function prototype here)
std::string infixToPostfix(const std::string& infix);

int main() {
    // Basic precedence and left-associativity
    assert(infixToPostfix("A+B*C") == "ABC*+");
    assert(infixToPostfix("A*B+C") == "AB*C+");

    // Parentheses
    assert(infixToPostfix("(A+B)*C") == "AB+C*");
    assert(infixToPostfix("A*(B+C)") == "ABC+*");

    // Nested parentheses
    assert(infixToPostfix("((A+B)*C)") == "AB+C*");
    assert(infixToPostfix("(A*(B+(C-D)))") == "ABCD-+*");

    // Right-associativity of '^'
    assert(infixToPostfix("A^B^C") == "ABC^^");
    assert(infixToPostfix("(A^B)^C") == "AB^C^");

    // Mixed operators and precedence
    assert(infixToPostfix("A+B*C-D/E") == "ABC*+DE/-");
    assert(infixToPostfix("(A+B)*(C-D)") == "AB+CD-*");

    // Single operand with parentheses
    assert(infixToPostfix("(A)") == "A");
    assert(infixToPostfix("(A+B)") == "AB+");

    // Complex expression
    assert(infixToPostfix("(A+(B*C-(D/E^F))*G)") == "ABC*DEF^/-G*+");

    return 0;
}
