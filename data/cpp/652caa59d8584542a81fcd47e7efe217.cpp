Write a C++ function `bool hasRedundantBrackets(const std::string& expression)` that determines whether a given mathematical expression contains any redundant (unnecessary) pair of parentheses. The expression consists only of lowercase letters (as operands), parentheses `(`, `)`, and the binary operators `+`, `-`, `*`, `/`. A pair of parentheses is considered redundant if it encloses a single operand (like `(a)`) or encloses an expression without any operator inside (like `((a+b))` where the outer pair wraps an already valid inner expression without adding any operator). The function should return `true` if at least one redundant bracket exists, and `false` otherwise. Assume the input expression is always well-formed (balanced parentheses, valid operands and operators), non-empty, and contains no spaces. Your implementation must use a stack-based approach and be efficient in both time and space.
#include <cassert>
#include <string>

// Declaration of the function under test
bool hasRedundantBrackets(const std::string& expression);

int main() {
    assert(hasRedundantBrackets("(a)") == true);
    assert(hasRedundantBrackets("(a+b)") == false);
    assert(hasRedundantBrackets("((a+b))") == true);
    assert(hasRedundantBrackets("(a+(b))") == true);
    assert(hasRedundantBrackets("a+b") == false);
    assert(hasRedundantBrackets("(a*b)+(c/d)") == false);
    assert(hasRedundantBrackets("(a)") == true);
    assert(hasRedundantBrackets("((a))") == true); // both pairs redundant
    assert(hasRedundantBrackets("a+(b*c)") == false);
    assert(hasRedundantBrackets("(a+(b*c))") == false); // operators inside both pairs
    return 0;
}
#include <stack>
#include <string>

// Returns true if the given expression contains at least one redundant pair of parentheses.
bool hasRedundantBrackets(const std::string& expression) {
    std::stack<char> st;
    for (char ch : expression) {
        if (ch == '(' || ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            st.push(ch);
        } else if (ch == ')') {
            bool hasOperator = false;
            while (!st.empty() && st.top() != '(') {
                char top = st.top();
                if (top == '+' || top == '-' || top == '*' || top == '/') {
                    hasOperator = true;
                }
                st.pop();
            }
            // st.top() is '(' now
            if (!hasOperator) {
                return true; // Redundant brackets found
            }
            st.pop(); // remove the '('
        }
        // lowercase letters are ignored
    }
    return false;
}
// The core algorithm processes the expression character by character using a stack. Operators (`+`, `-`, `*`, `/`) and opening parentheses `(` are pushed onto the stack. When a closing parenthesis `)` is encountered, we pop elements from the stack until we find the matching opening parenthesis. While popping, we track whether we encountered any operator. If no operator was found inside the parentheses, then the bracket pair is redundant because it either wrapped a single operand or already fully parenthesized expression without any new operation. In that case, return `true` immediately. After processing the entire string, if no redundant pair was found, return `false`. Edge cases include expressions like `(a)` (redundant), `(a+b)` (not redundant), `((a+b))` (outer pair redundant because inside is just `(a+b)` which has no operator at that level), and `(a+(b))` (inner `(b)` is redundant). The stack grows with the depth of nested parentheses and operators, so the time complexity is O(n) where n is the length of the string, and the space complexity is O(n) in the worst case for the stack.
