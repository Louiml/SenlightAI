/*
Write a standalone C++ function `std::string eliminateSmallBitVectors(const std::string& formula, unsigned maxBits = 4)` that takes a logic formula expressed in a simplified quantified Boolean/bit-vector language and returns an equivalent formula where all quantified variables whose sort is a bit-vector with bit-width ≤ `maxBits` are eliminated by unfolding (expanding) the quantifier over all possible values. The input formula uses the following grammar: terms are either bit-vector constants (non-negative integers without leading zeros, optionally prefixed with `#b` for binary), bit-vector variables (identifiers starting with a letter), or expressions of the form `(op ...)` where `op` is one of `and`, `or`, `not`, `bvadd`, `bvsub`, `bvmul`, `bvand`, `bvor`, `bvxor`, `bvnot`, `ite` (if-then-else with three arguments), or comparisons `bvslt`, `bvsle`, `bvsgt`, `bvsge`, `bveq`, and binary operations take exactly two arguments. Quantified formulas are written as `(forall ((x BitVec<width>)) <body>)` or `(exists ((x BitVec<width>)) <body>)`. The function must return a formula string with no quantifiers if all quantified variables are eliminated; if any quantified variable has a sort larger than `maxBits` or is not a bit-vector, the quantifier may remain unchanged (or the whole variable is skipped for elimination). Unfold a `forall` by conjoining the body with each possible constant value substituted, and an `exists` by disjoining. The output must be simplified: remove trivial equalities where both sides are syntactically equal, simplify boolean constants `true`/`false` (represented as `#b1`/`#b0`), and for `and`/`or` with a single argument return that argument. The function should handle nested quantifiers (innermost first is fine) and variables with the same name but different scopes correctly via a symbol table. Assume well-formed input with no free variables.
*/

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <stdexcept>

// Abstract syntax tree for the formula language
struct Expr {
    virtual ~Expr() = default;
};
using ExprPtr = std::shared_ptr<Expr>;

struct Constant : Expr {
    unsigned long long value;
    unsigned width;
    Constant(unsigned long long v, unsigned w) : value(v), width(w) {}
};

struct Variable : Expr {
    std::string name;
    Variable(const std::string& n) : name(n) {}
};

struct UnaryOp : Expr {
    std::string op;
    ExprPtr operand;
    UnaryOp(const std::string& o, ExprPtr e) : op(o), operand(std::move(e)) {}
};

struct BinaryOp : Expr {
    std::string op;
    ExprPtr left, right;
    BinaryOp(const std::string& o, ExprPtr l, ExprPtr r) : op(o), left(std::move(l)), right(std::move(r)) {}
};

struct TernaryOp : Expr {
    std::string op;
    ExprPtr cond, thenE, elseE;
    TernaryOp(const std::string& o, ExprPtr c, ExprPtr t, ExprPtr e)
        : op(o), cond(std::move(c)), thenE(std::move(t)), elseE(std::move(e)) {}
};

struct Quantifier : Expr {
    bool isForall;
    std::string varName;
    unsigned varWidth;
    ExprPtr body;
    Quantifier(bool forall, const std::string& n, unsigned w, ExprPtr b)
        : isForall(forall), varName(n), varWidth(w), body(std::move(b)) {}
};

// ---------- Parser ----------
class Parser {
public:
    explicit Parser(const std::string& s) : input(s), pos(0) {}
    ExprPtr parse() {
        skipWhitespace();
        if (pos >= input.size() || input[pos] != '(') {
            return parseTerm();
        }
        // It must be an operator or quantifier
        pos++; // consume '('
        skipWhitespace();
        std::string op;
        while (pos < input.size() && !isspace(input[pos]) && input[pos] != '(' && input[pos] != ')') {
            op.push_back(input[pos++]);
        }
        skipWhitespace();
        if (op == "forall" || op == "exists") {
            // Expect ((name BitVecW))
            if (pos >= input.size() || input[pos] != '(') throw std::runtime_error("Expected variable declaration");
            pos++;
            skipWhitespace();
            std::string varName;
            while (pos < input.size() && !isspace(input[pos]) && input[pos] != ')') {
                varName.push_back(input[pos++]);
            }
            skipWhitespace();
            // Now expect "BitVec<number>"
            std::string sortStr;
            while (pos < input.size() && !isspace(input[pos]) && input[pos] != ')') {
                sortStr.push_back(input[pos++]);
            }
            if (sortStr.size() < 6 || sortStr.substr(0, 6) != "BitVec") throw std::runtime_error("Invalid sort");
            unsigned width = std::stoul(sortStr.substr(6));
            skipWhitespace();
            if (pos >= input.size() || input[pos] != ')') throw std::runtime_error("Missing close var decl");
            pos++; // consume closing of var decl
            skipWhitespace();
            ExprPtr body = parse();
            skipWhitespace();
            if (pos >= input.size() || input[pos] != ')') throw std::runtime_error("Missing close quantifier");
            pos++; // consume closing of quantifier
            return std::make_shared<Quantifier>(op == "forall", varName, width, body);
        } else {
            // Binary or unary or ternary operation
            if (op == "not" || op == "bvnot") {
                ExprPtr operand = parse();
                skipWhitespace();
                if (pos >= input.size() || input[pos] != ')') throw std::runtime_error("Missing close paren");
                pos++;
                return std::make_shared<UnaryOp>(op, operand);
            } else if (op == "ite") {
                ExprPtr c = parse(); skipWhitespace();
                ExprPtr t = parse(); skipWhitespace();
                ExprPtr e = parse(); skipWhitespace();
                if (pos >= input.size() || input[pos] != ')') throw std::runtime_error("Missing close paren");
                pos++;
                return std::make_shared<TernaryOp>(op, c, t, e);
            } else {
                ExprPtr left = parse(); skipWhitespace();
                ExprPtr right = parse(); skipWhitespace();
                if (pos >= input.size() || input[pos] != ')') throw std::runtime_error("Missing close paren");
                pos++;
                return std::make_shared<BinaryOp>(op, left, right);
            }
        }
    }
private:
    const std::string& input;
    size_t pos;

    void skipWhitespace() {
        while (pos < input.size() && isspace(input[pos])) pos++;
    }

    ExprPtr parseTerm() {
        skipWhitespace();
        if (pos >= input.size()) throw std::runtime_error("Unexpected end");
        // Check for constant: either #b... (binary) or decimal number
        if (input[pos] == '#') {
            pos++; // skip '#'
            if (pos >= input.size() || input[pos] != 'b') throw std::runtime_error("Invalid bit-vector constant");
            pos++;
            std::string bits;
            while (pos < input.size() && (input[pos] == '0' || input[pos] == '1')) {
                bits.push_back(input[pos++]);
            }
            unsigned width = bits.size();
            unsigned long long value = 0;
            for (char c : bits) {
                value = (value << 1) | (c == '1');
            }
            return std::make_shared<Constant>(value, width);
        }
        // Decimal number or variable
        std::string token;
        while (pos < input.size() && !isspace(input[pos]) && input[pos] != '(' && input[pos] != ')') {
            token.push_back(input[pos++]);
        }
        if (token.empty()) throw std::runtime_error("Expected term");
        // Check if all digits
        bool allDigits = !token.empty() && std::all_of(token.begin(), token.end(), ::isdigit);
        if (allDigits) {
            unsigned long long value = std::stoull(token);
            // Guess width: minimal bits needed, at least 1
            unsigned width = 1;
            while ((1ULL << width) <= value) width++;
            return std::make_shared<Constant>(value, width);
        }
        return std::make_shared<Variable>(token);
    }
};

// ---------- Simplification ----------
ExprPtr simplify(ExprPtr e) {
    // Recursively simplify children first
    if (auto* c = dynamic_cast<Constant*>(e.get())) return e;
    if (auto* v = dynamic_cast<Variable*>(e.get())) return e;
    if (auto* u = dynamic_cast<UnaryOp*>(e.get())) {
        ExprPtr op = simplify(u->operand);
        if (auto* c = dynamic_cast<Constant*>(op.get())) {
            if (u->op == "bvnot" || u->op == "not") {
                unsigned long long mask = (1ULL << c->width) - 1;
                return std::make_shared<Constant>((~c->value) & mask, c->width);
            }
        }
        return std::make_shared<UnaryOp>(u->op, op);
    }
    if (auto* b = dynamic_cast<BinaryOp*>(e.get())) {
        ExprPtr l = simplify(b->left);
        ExprPtr r = simplify(b->right);
        if (auto* lc = dynamic_cast<Constant*>(l.get())) {
            if (auto* rc = dynamic_cast<Constant*>(r.get())) {
                unsigned width = std::max(lc->width, rc->width);
                unsigned long long a = lc->value, bb = rc->value;
                unsigned long long mask = (width >= 64) ? ~0ULL : ((1ULL << width) - 1);
                if (b->op == "bvadd") return std::make_shared<Constant>((a + bb) & mask, width);
                if (b->op == "bvsub") return std::make_shared<Constant>((a - bb) & mask, width);
                if (b->op == "bvmul") return std::make_shared<Constant>((a * bb) & mask, width);
                if (b->op == "bvand") return std::make_shared<Constant>(a & bb & mask, width);
                if (b->op == "bvor") return std::make_shared<Constant>((a | bb) & mask, width);
                if (b->op == "bvxor") return std::make_shared<Constant>((a ^ bb) & mask, width);
                if (b->op == "bveq") return std::make_shared<Constant>((a == bb) ? 1 : 0, 1);
                if (b->op == "bvslt") {
                    // signed compare: interpret as signed with width bits
                    long long sa = (long long)((a & mask) << (64 - width)) >> (64 - width);
                    long long sb = (long long)((bb & mask) << (64 - width)) >> (64 - width);
                    return std::make_shared<Constant>(sa < sb ? 1 : 0, 1);
                }
                if (b->op == "bvsle") {
                    long long sa = (long long)((a & mask) << (64 - width)) >> (64 - width);
                    long long sb = (long long)((bb & mask) << (64 - width)) >> (64 - width);
                    return std::make_shared<Constant>(sa <= sb ? 1 : 0, 1);
                }
                if (b->op == "bvsgt") {
                    long long sa = (long long)((a & mask) << (64 - width)) >> (64 - width);
                    long long sb = (long long)((bb & mask) << (64 - width)) >> (64 - width);
                    return std::make_shared<Constant>(sa > sb ? 1 : 0, 1);
                }
                if (b->op == "bvsge") {
                    long long sa = (long long)((a & mask) << (64 - width)) >> (64 - width);
                    long long sb = (long long)((bb & mask) << (64 - width)) >> (64 - width);
                    return std::make_shared<Constant>(sa >= sb ? 1 : 0, 1);
                }
                if (b->op == "and" || b->op == "or") {
                    bool isAnd = b->op == "and";
                    unsigned long long av = lc->value & 1, bv = rc->value & 1;
                    unsigned long long res = isAnd ? (av & bv) : (av | bv);
                    return std::make_shared<Constant>(res, 1);
                }
            }
        }
        // Simplifications with constants
        if (b->op == "and") {
            if (dynamic_cast<Constant*>(l.get()) && dynamic_cast<Constant*>(l.get())->value == 0) return std::make_shared<Constant>(0, 1);
            if (dynamic_cast<Constant*>(r.get()) && dynamic_cast<Constant*>(r.get())->value == 0) return std::make_shared<Constant>(0, 1);
            if (dynamic_cast<Constant*>(l.get()) && dynamic_cast<Constant*>(l.get())->value == 1) return r;
            if (dynamic_cast<Constant*>(r.get()) && dynamic_cast<Constant*>(r.get())->value == 1) return l;
        }
        if (b->op == "or") {
            if (dynamic_cast<Constant*>(l.get()) && dynamic_cast<Constant*>(l.get())->value == 1) return std::make_shared<Constant>(1, 1);
            if (dynamic_cast<Constant*>(r.get()) && dynamic_cast<Constant*>(r.get())->value == 1) return std::make_shared<Constant>(1, 1);
            if (dynamic_cast<Constant*>(l.get()) && dynamic_cast<Constant*>(l.get())->value == 0) return r;
            if (dynamic_cast<Constant*>(r.get()) && dynamic_cast<Constant*>(r.get())->value == 0) return l;
        }
        if (b->op == "bveq") {
            // syntactic equality check
            std::string ls = toString(l), rs = toString(r);
            if (ls == rs) return std::make_shared<Constant>(1, 1);
            // both constants already handled
        }
        return std::make_shared<BinaryOp>(b->op, l, r);
    }
    if (auto* t = dynamic_cast<TernaryOp*>(e.get())) {
        ExprPtr c = simplify(t->cond);
        ExprPtr th = simplify(t->thenE);
        ExprPtr el = simplify(t->elseE);
        if (auto* cc = dynamic_cast<Constant*>(c.get())) {
            if (cc->value & 1) return th;
            else return el;
        }
        return std::make_shared<TernaryOp>(t->op, c, th, el);
    }
    if (auto* q = dynamic_cast<Quantifier*>(e.get())) {
        // Should not appear after elimination, but keep if happens
        return std::make_shared<Quantifier>(q->isForall, q->varName, q->varWidth, simplify(q->body));
    }
    return e;
}

std::string toString(ExprPtr e);

// ---------- Substitution ----------
ExprPtr substitute(ExprPtr e, const std::unordered_map<std::string, ExprPtr>& env) {
    if (auto* c = dynamic_cast<Constant*>(e.get())) return e;
    if (auto* v = dynamic_cast<Variable*>(e.get())) {
        auto it = env.find(v->name);
        if (it != env.end()) return it->second;
        return e;
    }
    if (auto* u = dynamic_cast<UnaryOp*>(e.get())) {
        return std::make_shared<UnaryOp>(u->op, substitute(u->operand, env));
    }
    if (auto* b = dynamic_cast<BinaryOp*>(e.get())) {
        return std::make_shared<BinaryOp>(b->op, substitute(b->left, env), substitute(b->right, env));
    }
    if (auto* t = dynamic_cast<TernaryOp*>(e.get())) {
        return std::make_shared<TernaryOp>(t->op, substitute(t->cond, env), substitute(t->thenE, env), substitute(t->elseE, env));
    }
    if (auto* q = dynamic_cast<Quantifier*>(e.get())) {
        // If the quantifier binds the same name as we are substituting, skip; otherwise substitute inside
        std::unordered_map<std::string, ExprPtr> newEnv = env;
        newEnv.erase(q->varName); // shadow
        return std::make_shared<Quantifier>(q->isForall, q->varName, q->varWidth, substitute(q->body, newEnv));
    }
    return e;
}

// ---------- Elimination ----------
ExprPtr eliminate(ExprPtr e, unsigned maxBits) {
    // First recursively eliminate in children
    if (auto* q = dynamic_cast<Quantifier*>(e.get())) {
        // Eliminate inside body first
        ExprPtr body = eliminate(q->body, maxBits);
        if (q->varWidth <= maxBits && q->varWidth < 31) {
            unsigned long long num = 1ULL << q->varWidth;
            std::unordered_map<std::string, ExprPtr> env;
            std::vector<ExprPtr> clauses;
            for (unsigned long long v = 0; v < num; ++v) {
                ExprPtr constExpr = std::make_shared<Constant>(v, q->varWidth);
                env[q->varName] = constExpr;
                clauses.push_back(simplify(substitute(body, env)));
            }
            ExprPtr combined;
            if (clauses.empty()) {
                combined = std::make_shared<Constant>(q->isForall ? 1 : 0, 1);
            } else if (clauses.size() == 1) {
                combined = clauses[0];
            } else {
                combined = clauses[0];
                for (size_t i = 1; i < clauses.size(); ++i) {
                    if (q->isForall)
                        combined = std::make_shared<BinaryOp>("and", combined, clauses[i]);
                    else
                        combined = std::make_shared<BinaryOp>("or", combined, clauses[i]);
                }
            }
            return simplify(combined);
        } else {
            // Cannot eliminate; keep quantifier but with simplified body
            return std::make_shared<Quantifier>(q->isForall, q->varName, q->varWidth, body);
        }
    }
    if (auto* u = dynamic_cast<UnaryOp*>(e.get())) {
        return simplify(std::make_shared<UnaryOp>(u->op, eliminate(u->operand, maxBits)));
    }
    if (auto* b = dynamic_cast<BinaryOp*>(e.get())) {
        return simplify(std::make_shared<BinaryOp>(b->op, eliminate(b->left, maxBits), eliminate(b->right, maxBits)));
    }
    if (auto* t = dynamic_cast<TernaryOp*>(e.get())) {
        return simplify(std::make_shared<TernaryOp>(t->op, eliminate(t->cond, maxBits), eliminate(t->thenE, maxBits), eliminate(t->elseE, maxBits)));
    }
    return e; // constant or variable
}

// ---------- Printer ----------
std::string toString(ExprPtr e) {
    if (auto* c = dynamic_cast<Constant*>(e.get())) {
        // Print as #b... binary
        if (c->width == 0) return "#b";
        std::string bits;
        unsigned long long val = c->value;
        unsigned w = c->width;
        for (int i = w - 1; i >= 0; --i) {
            bits.push_back(((val >> i) & 1) ? '1' : '0');
        }
        return "#b" + bits;
    }
    if (auto* v = dynamic_cast<Variable*>(e.get())) return v->name;
    if (auto* u = dynamic_cast<UnaryOp*>(e.get())) {
        return "(" + u->op + " " + toString(u->operand) + ")";
    }
    if (auto* b = dynamic_cast<BinaryOp*>(e.get())) {
        return "(" + b->op + " " + toString(b->left) + " " + toString(b->right) + ")";
    }
    if (auto* t = dynamic_cast<TernaryOp*>(e.get())) {
        return "(" + t->op + " " + toString(t->cond) + " " + toString(t->thenE) + " " + toString(t->elseE) + ")";
    }
    if (auto* q = dynamic_cast<Quantifier*>(e.get())) {
        std::string quant = q->isForall ? "forall" : "exists";
        return "(" + quant + " ((" + q->varName + " BitVec" + std::to_string(q->varWidth) + ")) " + toString(q->body) + ")";
    }
    return "";
}

// ---------- Public API ----------
std::string eliminateSmallBitVectors(const std::string& formula, unsigned maxBits = 4) {
    Parser parser(formula);
    ExprPtr ast = parser.parse();
    ExprPtr result = eliminate(ast, maxBits);
    return toString(result);
}

#include <cassert>
#include <string>

// assume the solution is included above

int main() {
    // Simple forall over 1-bit variable: eliminate to conjunction of two cases
    assert(eliminateSmallBitVectors("(forall ((x BitVec1)) (bveq x x))") == "#b1");
    assert(eliminateSmallBitVectors("(exists ((x BitVec1)) (bveq x #b1))") == "#b1");

    // 2-bit variable, exists with equality check -> true because x can be 01 or 10 etc.
    assert(eliminateSmallBitVectors("(exists ((x BitVec2)) (bveq x #b10))") == "#b1");

    // Forall with 2-bit variable and non-trivial predicate: (forall x) (bveq x #b00) -> false
    assert(eliminateSmallBitVectors("(forall ((x BitVec2)) (bveq x #b00))") == "#b0");

    // Nested quantifiers: forall x (exists y) (bveq (bvadd x y) #b00) with 1-bit each -> all combos work? x=0,y=0 works; x=1,y=1 works (1+1=10? no, result is 0 since width 1); actually bvadd width 1 wraps: 1+1 = 0 mod 2, so true
    assert(eliminateSmallBitVectors("(forall ((x BitVec1)) (exists ((y BitVec1)) (bveq (bvadd x y) #b0)))") == "#b1");

    // Quantifier with variable larger than maxBits should remain
    std::string resultBig = eliminateSmallBitVectors("(forall ((x BitVec5)) (bveq x #b00000))", 4);
    assert(resultBig.find("forall") != std::string::npos);

    // Boolean logic simplification
    assert(eliminateSmallBitVectors("(and #b1 #b0)") == "#b0");
    assert(eliminateSmallBitVectors("(or #b1 #b0)") == "#b1");
    assert(eliminateSmallBitVectors("(not #b0)") == "#b1");

    // Constant folding
    assert(eliminateSmallBitVectors("(bvadd #b01 #b01)") == "#b10"); // 1+1=2 width 2

    // Empty quantifier (width 0) should handle edge (though grammar may not produce, but test)
    // Not required but robust

    // Variable shadowing with nested quantifiers
    // forall((x BitVec1)) exists((x BitVec1))? Should be fine, but use different names
    assert(eliminateSmallBitVectors("(forall ((x BitVec1)) (exists ((y BitVec1)) (bveq x y)))") == "#b1"); // any x, choose y=x

    return 0;
}

// The core algorithm is a recursive descent parser and transformer over the formula grammar. We parse each formula into an abstract syntax tree (AST) where nodes represent constants, variables, unary/binary/ternary operations, and quantifiers. The transformation is applied recursively: for a quantifier whose bound variable's bit-width is ≤ `maxBits`, we generate all constants `0` to `2^width - 1` (for width ≤ 30 to avoid overflow), substitute each constant for the variable in the body, and combine results with `and` (for `forall`) or `or` (for `exists`). Substitution must respect variable scoping: maintain a map from variable name to its binding (either a constant expression or a "free" marker) and recursively replace variable nodes accordingly. After unfolding, we simplify the AST: rewrite `(bveq x x)` to `#b1`, `(and e)` to `e`, `(or e)` to `e`, and for boolean-true/false we keep them as bit-vector constants `#b1`/`#b0` but when used in `and`/`or`, we can apply identity/absorption rules (e.g., `and` with `#b1` simplifies to the other operand, `and` with `#b0` simplifies to `#b0`; similarly for `or`). However, because our operations are all bit-vector level, we treat `#b1` and `#b0` as truth values for boolean combinators. Edge cases include: bit-width 0 (though not typical, avoid infinite loops), width > 30 to prevent overflow (we then skip elimination), repeated variable names in nested scopes (need to shadow correctly), and constants with binary notation. Time complexity: For each eliminated quantifier with width `w`, we generate `2^w` substitutions and simplify each resultant subtree; in the worst case with `k` eliminated variables, it is `O(2^(sum of widths) * size)`. Space complexity is proportional to the size of the expanded formula, which can be exponential in the number of eliminated variables.
