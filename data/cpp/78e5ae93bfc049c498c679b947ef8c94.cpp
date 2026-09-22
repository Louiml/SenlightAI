Write a C++ function that analyzes a class member function's body (represented as an AST-like structure using the provided simplified node hierarchy) to determine whether the function mutates any class fields and whether its return value refers to a field or is a literal/no-op. The function should take a pointer to a `MethodInfo` structure containing the method's name, return type (as a string), and a vector of statement nodes (representing the body in a simplified IR), and return a `MethodResult` containing a `MutationStatus` (`NO_MUTATION` or `MAYBE_MUTATION`) and a `ReturnStatus` (`NOOP`, `FIELD`, or `OTHER`). The analysis must handle: assignments to dereferenced fields via pointers, member function calls on field objects, increment/decrement operations on fields, return statements returning field references or field values, and comparison operators that should be ignored.
The solution must traverse the simplified statement tree recursively, tracking whether any operation affects a class field. The key challenge is distinguishing field accesses from local variables and literals. We need a helper function `isFieldAccess(expr)` that returns true if the expression represents a field access—this includes member access expressions (`.` or `->`), dereferenced pointers that are fields, and direct field references. We also need to handle the case where assignments occur through compound operators (like `+=`) and handle member call expressions where the implicit object is a field. The algorithm processes each statement in order: for assignment statements, check if the left-hand side is a field access; for call expressions, check if the callee is a field member access or if any argument (excluding primitive types like int/bool/char/float) is a field; for unary increment/decrement, check the operand; for return statements, classify the return value as NOOP (literal/void), FIELD (field access), or OTHER (anything else). Once any mutation is detected, we can short-circuit and mark `MAYBE_MUTATION`. Time complexity is O(N) where N is the total number of AST nodes, and space complexity is O(D) where D is the maximum recursion depth.
#include <string>
#include <vector>
#include <memory>
#include <variant>
#include <cassert>

// Simplified AST node hierarchy
struct Expr;
struct Stmt;

struct Expr {
    enum class Kind { LITERAL, FIELD_ACCESS, LOCAL_VAR, OTHER };
    Kind kind;
    std::string name;
    bool isDereference = false;
    std::vector<std::unique_ptr<Expr>> children;
};

struct Stmt {
    enum class Kind { ASSIGNMENT, CALL, MEMBER_CALL, RETURN, UNARY_OP, BINARY_OP, NOOP };
    Kind kind;
    std::unique_ptr<Expr> lhs;          // for assignment
    std::unique_ptr<Expr> rhs;          // for assignment
    std::unique_ptr<Expr> callee;       // for call
    std::vector<std::unique_ptr<Expr>> args; // for call
    std::string typeName;               // for return type of call
    std::unique_ptr<Expr> operand;      // for unary and return
    bool isComparison = false;          // for call/binary
    bool isAssignmentOp = false;        // for binary
    std::vector<std::unique_ptr<Stmt>> body; // for compound statements
};

enum class MutationStatus { NO_MUTATION, MAYBE_MUTATION };
enum class ReturnStatus { NOOP, FIELD, OTHER };

struct MethodResult {
    MutationStatus mutation;
    ReturnStatus returnStatus;
};

struct MethodInfo {
    std::string methodName;
    std::string returnType;
    std::vector<std::unique_ptr<Stmt>> body;
};

// Helper: check if an expression is a field access
bool isFieldAccess(const Expr* expr) {
    if (!expr) return false;
    if (expr->kind == Expr::Kind::FIELD_ACCESS) return true;
    if (expr->kind == Expr::Kind::OTHER && expr->isDereference && !expr->children.empty()) {
        return isFieldAccess(expr->children[0].get());
    }
    return false;
}

// Helper: check if a type is a primitive value type (ignored for mutation)
bool isPrimitiveType(const std::string& type) {
    return type == "int" || type == "bool" || type == "char" || 
           type == "float" || type == "double" || type == "void";
}

// Recursive visitor for statements
MutationStatus visitStmt(const Stmt* stmt, bool& sawNonLiteralReturn) {
    if (!stmt) return MutationStatus::NO_MUTATION;

    MutationStatus result = MutationStatus::NO_MUTATION;

    switch (stmt->kind) {
        case Stmt::Kind::ASSIGNMENT: {
            if (isFieldAccess(stmt->lhs.get())) {
                return MutationStatus::MAYBE_MUTATION;
            }
            // Also check RHS if it's a function call that might mutate (handled in CALL)
            break;
        }
        case Stmt::Kind::CALL: {
            if (stmt->isComparison) break; // ignore comparisons
            if (isFieldAccess(stmt->callee.get())) {
                return MutationStatus::MAYBE_MUTATION;
            }
            for (const auto& arg : stmt->args) {
                if (!isPrimitiveType(arg->name) && isFieldAccess(arg.get())) {
                    return MutationStatus::MAYBE_MUTATION;
                }
            }
            break;
        }
        case Stmt::Kind::MEMBER_CALL: {
            // Implicit object argument is the callee's first child (if it's a field access)
            if (!stmt->args.empty() && isFieldAccess(stmt->args[0].get())) {
                return MutationStatus::MAYBE_MUTATION;
            }
            // Recurse as a regular call
            result = visitStmt(stmt, sawNonLiteralReturn);
            break;
        }
        case Stmt::Kind::UNARY_OP: {
            if (stmt->operand && isFieldAccess(stmt->operand.get())) {
                return MutationStatus::MAYBE_MUTATION;
            }
            break;
        }
        case Stmt::Kind::BINARY_OP: {
            if (stmt->isAssignmentOp && stmt->lhs && isFieldAccess(stmt->lhs.get())) {
                return MutationStatus::MAYBE_MUTATION;
            }
            break;
        }
        case Stmt::Kind::RETURN: {
            if (stmt->operand && stmt->operand->kind == Expr::Kind::LITERAL) {
                // NOOP, no change to return status
            } else if (stmt->operand && isFieldAccess(stmt->operand.get())) {
                sawNonLiteralReturn = true;
            } else if (stmt->operand && stmt->operand->kind == Expr::Kind::OTHER) {
                sawNonLiteralReturn = true;
            }
            break;
        }
        case Stmt::Kind::NOOP:
            break;
    }

    // Recurse into nested body if any
    for (const auto& child : stmt->body) {
        MutationStatus childResult = visitStmt(child.get(), sawNonLiteralReturn);
        if (childResult == MutationStatus::MAYBE_MUTATION) {
            return MutationStatus::MAYBE_MUTATION;
        }
    }

    return result;
}

// Main analysis function
MethodResult analyzeMethod(const MethodInfo& method) {
    bool sawNonLiteralReturn = false;
    MutationStatus mut = visitStmt(method.body.empty() ? nullptr : method.body[0].get(), sawNonLiteralReturn);

    ReturnStatus ret;
    if (method.returnType == "void") {
        ret = ReturnStatus::NOOP;
    } else if (sawNonLiteralReturn) {
        ret = ReturnStatus::FIELD;
    } else {
        ret = ReturnStatus::NOOP;
    }

    return {mut, ret};
}
#include <cassert>
#include <memory>

// Test helper to create expressions and statements
std::unique_ptr<Expr> makeField(const std::string& name, bool deref = false) {
    auto e = std::make_unique<Expr>();
    e->kind = Expr::Kind::FIELD_ACCESS;
    e->name = name;
    e->isDereference = deref;
    return e;
}

std::unique_ptr<Expr> makeLiteral() {
    auto e = std::make_unique<Expr>();
    e->kind = Expr::Kind::LITERAL;
    e->name = "lit";
    return e;
}

std::unique_ptr<Expr> makeLocal() {
    auto e = std::make_unique<Expr>();
    e->kind = Expr::Kind::LOCAL_VAR;
    e->name = "local";
    return e;
}

std::unique_ptr<Expr> makeOther() {
    auto e = std::make_unique<Expr>();
    e->kind = Expr::Kind::OTHER;
    e->name = "other";
    return e;
}

int main() {
    // Case 1: Simple assignment to field => mutation
    {
        MethodInfo m;
        m.methodName = "test1";
        m.returnType = "void";
        auto stmt = std::make_unique<Stmt>();
        stmt->kind = Stmt::Kind::ASSIGNMENT;
        stmt->lhs = makeField("x");
        stmt->rhs = makeLiteral();
        m.body.push_back(std::move(stmt));
        auto res = analyzeMethod(m);
        assert(res.mutation == MutationStatus::MAYBE_MUTATION);
        assert(res.returnStatus == ReturnStatus::NOOP);
    }

    // Case 2: Return field literal, no mutation
    {
        MethodInfo m;
        m.methodName = "test2";
        m.returnType = "int";
        auto stmt = std::make_unique<Stmt>();
        stmt->kind = Stmt::Kind::RETURN;
        stmt->operand = makeLiteral();
        m.body.push_back(std::move(stmt));
        auto res = analyzeMethod(m);
        assert(res.mutation == MutationStatus::NO_MUTATION);
        assert(res.returnStatus == ReturnStatus::NOOP);
    }

    // Case 3: Return field access, no mutation
    {
        MethodInfo m;
        m.methodName = "test3";
        m.returnType = "int";
        auto stmt = std::make_unique<Stmt>();
        stmt->kind = Stmt::Kind::RETURN;
        stmt->operand = makeField("y");
        m.body.push_back(std::move(stmt));
        auto res = analyzeMethod(m);
        assert(res.mutation == MutationStatus::NO_MUTATION);
        assert(res.returnStatus == ReturnStatus::FIELD);
    }

    // Case 4: Member call on field => mutation
    {
        MethodInfo m;
        m.methodName = "test4";
        m.returnType = "void";
        auto stmt = std::make_unique<Stmt>();
        stmt->kind = Stmt::Kind::MEMBER_CALL;
        stmt->args.push_back(makeField("obj"));
        stmt->callee = makeField("method");
        m.body.push_back(std::move(stmt));
        auto res = analyzeMethod(m);
        assert(res.mutation == MutationStatus::MAYBE_MUTATION);
        assert(res.returnStatus == ReturnStatus::NOOP);
    }

    // Case 5: Increment field => mutation
    {
        MethodInfo m;
        m.methodName = "test5";
        m.returnType = "void";
        auto stmt = std::make_unique<Stmt>();
        stmt->kind = Stmt::Kind::UNARY_OP;
        stmt->operand = makeField("counter");
        m.body.push_back(std::move(stmt));
        auto res = analyzeMethod(m);
        assert(res.mutation == MutationStatus::MAYBE_MUTATION);
        assert(res.returnStatus == ReturnStatus::NOOP);
    }

    // Case 6: Comparison operator call ignored
    {
        MethodInfo m;
        m.methodName = "test6";
        m.returnType = "bool";
        auto stmt = std::make_unique<Stmt>();
        stmt->kind = Stmt::Kind::CALL;
        stmt->isComparison = true;
        stmt->callee = makeField("compare");
        stmt->args.push_back(makeField("x"));
        m.body.push_back(std::move(stmt));
        // Also return a literal
        auto ret = std::make_unique<Stmt>();
        ret->kind = Stmt::Kind::RETURN;
        ret->operand = makeLiteral();
        m.body.push_back(std::move(ret));
        auto res = analyzeMethod(m);
        assert(res.mutation == MutationStatus::NO_MUTATION);
        assert(res.returnStatus == ReturnStatus::NOOP);
    }

    // Case 7: Compound assignment (binary op) to field => mutation
    {
        MethodInfo m;
        m.methodName = "test7";
        m.returnType = "void";
        auto stmt = std::make_unique<Stmt>();
        stmt->kind = Stmt::Kind::BINARY_OP;
        stmt->isAssignmentOp = true;
        stmt->lhs = makeField("x");
        stmt->rhs = makeLiteral();
        m.body.push_back(std::move(stmt));
        auto res = analyzeMethod(m);
        assert(res.mutation == MutationStatus::MAYBE_MUTATION);
        assert(res.returnStatus == ReturnStatus::NOOP);
    }

    // Case 8: Empty body, void return
    {
        MethodInfo m;
        m.methodName = "test8";
        m.returnType = "void";
        auto res = analyzeMethod(m);
        assert(res.mutation == MutationStatus::NO_MUTATION);
        assert(res.returnStatus == ReturnStatus::NOOP);
    }

    return 0;
}
