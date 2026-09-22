// Write a C++ function `bool areEquivalentExpressions(const std::string& expr1, const std::string& expr2)` that determines whether two mathematical expressions, composed of lowercase English letters (representing variables), plus signs `+`, minus signs `-`, and parentheses `( )`, are equivalent. Two expressions are equivalent if, after removing all parentheses by distributing the signs correctly (i.e., a minus before a parenthesis flips the sign of every term inside), the simplified expressions contain the same multiset of signed terms. For example, `a-(b-c)` simplifies to `a-b+c`, which is equivalent to `a-b+c` and `a+c-b`, but not to `a-b-c`. The input expressions are guaranteed to be syntactically valid, contain no spaces, and have at least one variable or term. Variables are single lowercase letters, and operands are always separated by explicit `+` or `-` signs; there are no unary operators except possibly a leading `-` or `+` at the very beginning. Parentheses may be nested. The function should return `true` if the two simplified expressions are equivalent, and `false` otherwise.
#include <cassert>

int main() {
    // Basic equivalence
    assert(areEquivalentExpressions("a+b", "b+a") == true);
    // Difference in sign
    assert(areEquivalentExpressions("a-b", "b-a") == false);
    // Parentheses with minus
    assert(areEquivalentExpressions("a-(b-c)", "a-b+c") == true);
    assert(areEquivalentExpressions("a-(b-c)", "a-b-c") == false);
    // Nested parentheses
    assert(areEquivalentExpressions("a-(b-(c-d))", "a-b+c-d") == true);
    assert(areEquivalentExpressions("a-(b-(c-d))", "a-b-c+d") == false);
    // Leading minus
    assert(areEquivalentExpressions("-a+b", "b-a") == true);
    assert(areEquivalentExpressions("-a+b", "a+b") == false);
    // Terms that cancel
    assert(areEquivalentExpressions("a-a", "0") == false); // 0 is not a valid variable, so this is mismatch; but test proper cancellation
    assert(areEquivalentExpressions("a+b-a-b", "") == true); // empty simplified map
    assert(areEquivalentExpressions("a+b-a-b", "c-c") == true); // both simplify to empty map
    // Multiple variables and parentheses
    assert(areEquivalentExpressions("x-(y+z)", "x-y-z") == true);
    assert(areEquivalentExpressions("x-(y+z)", "x+y+z") == false);
    // No parentheses
    assert(areEquivalentExpressions("a+b+c", "a+b+c") == true);
    assert(areEquivalentExpressions("a+b-c", "a-b+c") == false);
    return 0;
}
#include <string>
#include <map>
#include <stack>

// Simplify an expression by removing parentheses and returning a map of net variable coefficients.
static std::map<char, int> simplify(const std::string& expr) {
    std::map<char, int> coeff;
    std::stack<char> signStack;
    signStack.push('+'); // global positive context

    char currentSign = '+'; // sign for the next variable encountered

    for (size_t i = 0; i < expr.size(); ++i) {
        char ch = expr[i];
        if (ch == '+' || ch == '-') {
            currentSign = ch;
        } else if (ch == '(') {
            // Determine the sign to push based on the character before '(' (if any)
            if (i > 0 && expr[i-1] == '-') {
                // Flip the current top sign
                signStack.push(signStack.top() == '-' ? '+' : '-');
            } else {
                // For '+' or no operator, keep the same top sign
                signStack.push(signStack.top());
            }
            // Reset currentSign to treat the next term inside the parentheses
            currentSign = '+';
        } else if (ch == ')') {
            if (signStack.size() > 1) {
                signStack.pop();
            }
            currentSign = '+'; // reset for after closing parenthesis
        } else {
            // ch is a variable letter (a-z)
            char effectiveSign = currentSign;
            if (signStack.top() == '-') {
                effectiveSign = (effectiveSign == '+') ? '-' : '+';
            }
            // If it's the first character and no sign is set, treat as '+'
            if (i == 0 && expr[i] == ch) {
                effectiveSign = '+';
            }
            coeff[ch] += (effectiveSign == '+') ? 1 : -1;
            currentSign = '+'; // reset, since each variable must have its own sign
        }
    }
    return coeff;
}

// Determine if two expressions are equivalent after simplifying parentheses and signs.
bool areEquivalentExpressions(const std::string& expr1, const std::string& expr2) {
    std::map<char, int> m1 = simplify(expr1);
    std::map<char, int> m2 = simplify(expr2);
    return m1 == m2;
}
// The core idea is to simplify each expression by removing all parentheses while tracking the current sign context using a stack. Traverse the string character by character: maintain a stack of signs (initially containing `+` for the outermost context). When a `(` is encountered, push the sign that will apply to the next term inside, based on the current top-of-stack and the operator immediately before the parenthesis (if any). When a `)` is encountered, pop the stack. For each variable letter, determine its sign as the product of the top-of-stack sign and any immediately preceding `+` or `-`. Simplify the parentheses without actually expanding variables; instead, after producing a linear signed-term sequence (e.g., `+a-b+c`), store counts of each variable's net coefficient into a map (`map<char,int>`). The coefficient is incremented for a `+` and decremented for a `-`, handling leading terms without explicit sign. After simplifying both expressions, compare the two maps; if they are identical, the expressions are equivalent. Edge cases include expressions like `a-(b-c)` where the minus flips signs inside, nested parentheses like `a-(b-(c-d))`, leading plus signs, and expressions that simplify to zero (e.g., `a-a`). The algorithm runs in O(n) time per expression, where n is the length of the string (with the map operations being O(1) because variables are only lowercase letters). Space complexity is O(n) for the stack and O(1) for the map (since at most 26 entries).
