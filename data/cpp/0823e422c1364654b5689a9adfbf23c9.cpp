// Write a C++ function `std::string addParenthesesForMixedPrecedence(const std::string& expression)` that takes a mathematical expression string containing integers and binary arithmetic operators (`+`, `-`, `*`, `/`, `%`, `&`, `^`, `|`) without any parentheses, and returns a new string where parentheses are added around every sub-expression that is a binary operation whose precedence differs from its parent binary operation. Precedence order from highest to lowest: `*`, `/`, `%` (level 5) > `+`, `-` (level 4) > `&` (level 3) > `^` (level 2) > `|` (level 1). If an operation has the same precedence as its parent or is at the top level (not nested inside another binary operation), no parentheses are needed for that sub-expression. The input is guaranteed to be well-formed, contain no whitespace or parentheses, integers are non-negative, and operators are single characters. For example, `"1+2*3"` becomes `"1+(2*3)"` because `*` (precedence 5) is inside `+` (precedence 4); but `"1+2+3"` remains unchanged because all `+` have equal precedence. Also, handle cases where the sub-expression itself contains mixed precedence deeper inside, so apply recursively. Do not add redundant parentheses for equal precedence or for operators at the top level. Return the transformed expression string.
#include <cassert>
#include <string>

// The solution function is declared above; this is the test driver.
int main() {
    // Basic mixed precedence: multiplication inside addition
    assert(addParenthesesForMixedPrecedence("1+2*3") == "1+(2*3)");
    // Same precedence: no parentheses added
    assert(addParenthesesForMixedPrecedence("1+2+3") == "1+2+3");
    // Multiple mixed operators
    assert(addParenthesesForMixedPrecedence("1+2*3-4") == "1+(2*3)-4");
    // Nested mixed precedence: (2+3) has different precedence than *1
    assert(addParenthesesForMixedPrecedence("1*2+3*4") == "1*2+3*4");
    // No operators: unchanged
    assert(addParenthesesForMixedPrecedence("123") == "123");
    // Lower precedence bitwise operators
    assert(addParenthesesForMixedPrecedence("1&2+3") == "1&(2+3)");
    // Left-associative same precedence: unchanged
    assert(addParenthesesForMixedPrecedence("8/4/2") == "8/4/2");
    // Mixed precedence with bitwise XOR
    assert(addParenthesesForMixedPrecedence("2^3*4") == "2^(3*4)");
    // Deeper nesting: a-b*c+d becomes a-(b*c)+d
    assert(addParenthesesForMixedPrecedence("100-20*3+5") == "100-(20*3)+5");
    return 0;
}
#include <string>
#include <cctype>

// Node structure for the binary expression tree
struct ExprNode {
    std::string value;      // either an integer or an operator character
    bool isOperator;
    ExprNode* left;
    ExprNode* right;

    ExprNode(const std::string& v, bool op = false)
        : value(v), isOperator(op), left(nullptr), right(nullptr) {}
};

// Precedence levels: 5 for *, /, %; 4 for +, -; 3 for &; 2 for ^; 1 for |; 0 for non-operator or invalid
static int precedenceOf(const std::string& op) {
    if (op == "*" || op == "/" || op == "%") return 5;
    if (op == "+" || op == "-") return 4;
    if (op == "&") return 3;
    if (op == "^") return 2;
    if (op == "|") return 1;
    return 0;
}

// Recursive descent parser: parse expressions with precedence >= minPrec
static ExprNode* parseExpr(const std::string& s, size_t& pos, int minPrec) {
    // Parse primary (operand) first
    ExprNode* left;
    if (pos < s.size() && std::isdigit(s[pos])) {
        size_t start = pos;
        while (pos < s.size() && std::isdigit(s[pos])) ++pos;
        left = new ExprNode(s.substr(start, pos - start), false);
    } else {
        // Should not happen with valid input
        return nullptr;
    }

    // Now parse binary operators with precedence >= minPrec
    while (pos < s.size()) {
        char c = s[pos];
        if (c != '+' && c != '-' && c != '*' && c != '/' && c != '%' &&
            c != '&' && c != '^' && c != '|') {
            break; // not an operator
        }
        std::string op(1, c);
        int prec = precedenceOf(op);
        if (prec < minPrec) break;

        ++pos; // consume operator
        ExprNode* right = parseExpr(s, pos, prec + 1);
        ExprNode* node = new ExprNode(op, true);
        node->left = left;
        node->right = right;
        left = node;
    }
    return left;
}

// Recursive output generator: wraps sub-expression in parentheses if precedence differs from parent
static void generateOutput(ExprNode* node, ExprNode* parent, std::string& out) {
    if (!node) return;

    bool needParens = false;
    if (node->isOperator && parent && parent->isOperator) {
        int childPrec = precedenceOf(node->value);
        int parentPrec = precedenceOf(parent->value);
        if (childPrec > 0 && parentPrec > 0 && childPrec != parentPrec) {
            needParens = true;
        }
    }

    if (needParens) out.push_back('(');
    if (node->isOperator) {
        generateOutput(node->left, node, out);
        out += node->value;
        generateOutput(node->right, node, out);
    } else {
        out += node->value;
    }
    if (needParens) out.push_back(')');
}

// Free function: adds parentheses for mixed-precedence binary operators
std::string addParenthesesForMixedPrecedence(const std::string& expression) {
    size_t pos = 0;
    ExprNode* tree = parseExpr(expression, pos, 1);
    std::string result;
    generateOutput(tree, nullptr, result);
    return result;
}
// The solution requires parsing the expression into a binary expression tree and then re-serializing it while inserting parentheses based on precedence comparisons. The main algorithm is recursive descent parsing to build the tree, followed by an output traversal that decides whether to wrap each node in parentheses. For each binary node, we compare its precedence with its parent's precedence. If the child's precedence is different from the parent's and both are valid arithmetic operators (levels 1–5), we insert parentheses around the child's entire subtree when generating the output. If the child is the top-level node (parent is null), no parentheses are added. Equal precedence requires no parentheses (so `"a-b-c"` stays as-is, even though `-` is left-associative; the task does not require associativity handling, only mixed precedence). Edge cases include single integers (no binary operators) returning the input unchanged, nested mixed precedence (e.g., `"a+b*c-d"` should become `"a+(b*c)-d"` because `b*c` has higher precedence than both `+` and `-`), and operators like `&`, `^`, `|` with lower precedence than arithmetic. Time complexity is O(n) for parsing and output, where n is the length of the input string, and space complexity is O(n) for the tree and output string. The recursive descent parser uses a standard precedence-climbing approach: parse the lowest-precedence operator `|`, then `^`, then `&`, then `+`/`-`, then `*`/`/`/`%`, and finally primary operands (integers). Each node stores its operator or value, and the output function recursively visits left and right children, inserting parentheses around a child if needed based on the precedence comparison.
