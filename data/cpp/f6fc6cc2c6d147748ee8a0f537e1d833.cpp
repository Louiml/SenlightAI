Write a C++ function that parses a logical expression string into an abstract syntax tree (AST) using the following grammar (precedence from highest to lowest): parentheses `( )`, negation `!` (prefix), conjunction `&`, disjunction `|`, and implication `->` (right-associative). The input may contain identifiers (single lowercase letters) as atomic propositions. The parsing must be **recursive descent** and return a pointer to the root `Tree` node. If the input is invalid (e.g., unexpected token, unbalanced parentheses, trailing characters), the function should return `nullptr`. The `Tree` structure is provided as a simple node with an operation type (`enum op_type`), left/right child pointers, and a string `value` (for identifiers or the operator symbol). You must implement a function `Tree* parse_expression(const std::string& text)` that performs the full parse. The solution must handle whitespace between any tokens (including inside operators like `- >` for implication). Edge cases: empty string, only whitespace, unbalanced parentheses, missing operand (e.g., `&` at start), and unary `!` applied repeatedly (e.g., `!!a`). Time complexity should be O(n) for the length of the input string, and space complexity O(n) for the AST in the worst-case (e.g., deeply nested or long chain of binary operations).
// The solution uses a recursive descent parser with a mutable position index over the input string. The grammar is implemented as a set of mutually recursive functions: `parse_expression` handles implication (lowest precedence, right-associative), `parse_disjunction` handles `|`, `parse_conjunction` handles `&`, `parse_negation` handles `!` and parentheses, and `parse_atom` handles identifiers. At each level, we skip leading whitespace before checking for the expected token. For binary operators, we parse the left operand, then loop while the next non-whitespace token matches the operator; if found, we consume it and parse the right operand (with appropriate precedence: for implication, we recursively parse the right side to achieve right-associativity; for `|` and `&`, we parse the next lower precedence level). For negation, we check for `!` and recursively parse a negation (allowing `!!a`), or for `(` we consume it, parse a full expression, then check for a closing `)` (if missing, return `nullptr`). For atoms, we accept a single lowercase letter `a`-`z`. After the root parse, we skip trailing whitespace and ensure the position equals the string length; otherwise, return `nullptr`. Important edge cases: leading/trailing whitespace is handled by the `skip_spaces` helper; the parser must not read past the end; when parsing a parenthesized expression, the closing parenthesis must be exactly after the inner expression (allowing whitespace before it). The recursion depth is bounded by the number of binary operators (which is O(n) in the worst case). Time complexity is O(n) because each character is examined at most twice (once in skip and once in token matching), and each node creation is O(1). Space complexity is O(n) for storing the AST (one node per operator or atom) plus O(depth) stack space.
#include <string>
#include <cctype>
#include <memory>

enum class op_type {
    TEMP,       // atomic identifier
    NEGATION,   // !
    CONJUNCTION,// &
    DISJUNCTION,// |
    IMPLICATION // ->
};

struct Tree {
    op_type type;
    Tree* left;
    Tree* right;
    std::string value;

    Tree(op_type t, Tree* l, Tree* r, const std::string& v)
        : type(t), left(l), right(r), value(v) {}
};

// The parser state
struct Parser {
    const std::string& text;
    size_t pos;

    explicit Parser(const std::string& t) : text(t), pos(0) {}

    void skip_spaces() {
        while (pos < text.size() && std::isspace(static_cast<unsigned char>(text[pos]))) {
            ++pos;
        }
    }

    bool starts_with(const std::string& token) const {
        if (pos + token.size() > text.size()) return false;
        for (size_t i = 0; i < token.size(); ++i) {
            if (text[pos + i] != token[i]) return false;
        }
        return true;
    }

    bool consume(const std::string& token) {
        skip_spaces();
        if (starts_with(token)) {
            pos += token.size();
            return true;
        }
        return false;
    }

    Tree* parse_expression();
    Tree* parse_disjunction();
    Tree* parse_conjunction();
    Tree* parse_negation();
    Tree* parse_atom();

    Tree* make_binary(op_type type, Tree* left, Tree* right, const std::string& op) {
        return new Tree(type, left, right, op);
    }

    Tree* make_unary(op_type type, Tree* child, const std::string& op) {
        return new Tree(type, child, nullptr, op);
    }

    Tree* make_atom(const std::string& id) {
        return new Tree(op_type::TEMP, nullptr, nullptr, id);
    }
};

Tree* Parser::parse_expression() {
    skip_spaces();
    Tree* left = parse_disjunction();
    if (left == nullptr) return nullptr;

    while (true) {
        skip_spaces();
        if (starts_with("->")) {
            pos += 2;
            Tree* right = parse_expression(); // right-associative
            if (right == nullptr) {
                delete left;
                return nullptr;
            }
            left = make_binary(op_type::IMPLICATION, left, right, "->");
        } else {
            break;
        }
    }
    return left;
}

Tree* Parser::parse_disjunction() {
    Tree* left = parse_conjunction();
    if (left == nullptr) return nullptr;

    while (true) {
        skip_spaces();
        if (pos < text.size() && text[pos] == '|') {
            ++pos;
            Tree* right = parse_conjunction();
            if (right == nullptr) {
                delete left;
                return nullptr;
            }
            left = make_binary(op_type::DISJUNCTION, left, right, "|");
        } else {
            break;
        }
    }
    return left;
}

Tree* Parser::parse_conjunction() {
    Tree* left = parse_negation();
    if (left == nullptr) return nullptr;

    while (true) {
        skip_spaces();
        if (pos < text.size() && text[pos] == '&') {
            ++pos;
            Tree* right = parse_negation();
            if (right == nullptr) {
                delete left;
                return nullptr;
            }
            left = make_binary(op_type::CONJUNCTION, left, right, "&");
        } else {
            break;
        }
    }
    return left;
}

Tree* Parser::parse_negation() {
    skip_spaces();
    if (pos < text.size() && text[pos] == '!') {
        ++pos;
        Tree* child = parse_negation();
        if (child == nullptr) return nullptr;
        return make_unary(op_type::NEGATION, child, "!");
    } else if (pos < text.size() && text[pos] == '(') {
        ++pos;
        Tree* inner = parse_expression();
        if (inner == nullptr) return nullptr;
        skip_spaces();
        if (pos < text.size() && text[pos] == ')') {
            ++pos;
            return inner;
        } else {
            delete inner;
            return nullptr; // missing closing parenthesis
        }
    } else {
        return parse_atom();
    }
}

Tree* Parser::parse_atom() {
    skip_spaces();
    if (pos < text.size() && std::islower(static_cast<unsigned char>(text[pos]))) {
        std::string id(1, text[pos]);
        ++pos;
        return make_atom(id);
    }
    return nullptr;
}

// The main parsing function as required
Tree* parse_expression(const std::string& text) {
    Parser parser(text);
    Tree* result = parser.parse_expression();
    if (result == nullptr) return nullptr;
    parser.skip_spaces();
    if (parser.pos != text.size()) {
        // Trailing characters
        delete result;
        return nullptr;
    }
    return result;
}
#include <cassert>
#include <string>

// Helper to free the tree (simple recursive delete)
void delete_tree(Tree* node) {
    if (!node) return;
    delete_tree(node->left);
    delete_tree(node->right);
    delete node;
}

int main() {
    // Test valid expressions
    Tree* t1 = parse_expression("a");
    assert(t1 != nullptr && t1->type == op_type::TEMP && t1->value == "a");
    delete_tree(t1);

    Tree* t2 = parse_expression(" a & b ");
    assert(t2 != nullptr && t2->type == op_type::CONJUNCTION);
    assert(t2->left->value == "a" && t2->right->value == "b");
    delete_tree(t2);

    Tree* t3 = parse_expression("!a");
    assert(t3 != nullptr && t3->type == op_type::NEGATION && t3->left->value == "a");
    delete_tree(t3);

    Tree* t4 = parse_expression("a | b & c");
    // Precedence: & binds tighter, so root is |
    assert(t4 != nullptr && t4->type == op_type::DISJUNCTION);
    assert(t4->right->type == op_type::CONJUNCTION);
    delete_tree(t4);

    Tree* t5 = parse_expression("a -> b -> c");
    // Right-associative: a -> (b -> c)
    assert(t5 != nullptr && t5->type == op_type::IMPLICATION);
    assert(t5->right->type == op_type::IMPLICATION);
    delete_tree(t5);

    Tree* t6 = parse_expression("( a | b ) & !c");
    assert(t6 != nullptr && t6->type == op_type::CONJUNCTION);
    assert(t6->right->type == op_type::NEGATION);
    delete_tree(t6);

    Tree* t7 = parse_expression("!!a");
    assert(t7 != nullptr && t7->type == op_type::NEGATION && t7->left->type == op_type::NEGATION);
    delete_tree(t7);

    // Test invalid expressions
    assert(parse_expression("") == nullptr);
    assert(parse_expression("   ") == nullptr);
    assert(parse_expression("&") == nullptr);
    assert(parse_expression("a &") == nullptr);
    assert(parse_expression("(a") == nullptr);
    assert(parse_expression("a)") == nullptr);
    assert(parse_expression("a b") == nullptr);
    assert(parse_expression("a &| b") == nullptr);
    assert(parse_expression("A") == nullptr);
    assert(parse_expression("->") == nullptr);

    return 0;
}
