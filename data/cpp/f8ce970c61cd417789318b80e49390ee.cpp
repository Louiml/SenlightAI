/*
Write a C++ function `std::vector<std::string> classifyLoopVariables(const std::vector<Statement>& loopBody)` that analyzes a simplified abstract representation of a loop's body and classifies variables into three categories: loop-invariant constants (including those defined as constants or with a single assignment composed only of loop-constant operands), basic induction variables (BIVs) that follow the pattern `var = var + const` or `var = var - const` where the increment is a loop-invariant constant, and other variables. The input is a vector of `Statement` objects, where each statement is either an assignment of the form `var = expression` (with `expression` being either a constant integer, a variable reference, or a binary add/subtract of two operands), a conditional `if (expression) break;` that marks a loop terminator, or a variable declaration (which is skipped). The function must return a vector of strings in the order: first all loop-invariant constants, then all induction variables, then the remaining variables; within each group, list variable names in the order they were first encountered in the loop body (declarations are skipped for ordering, but variable references in assignments count). If a variable is assigned more than once in the loop (including inside any conditional, even if the condition is simple), or if its assignment depends on any non-loop-constant variable, it cannot be classified as invariant; if the assignment is unconditional and has the BIV pattern with a loop-invariant increment, it is an induction variable; otherwise it is "other". Note that a variable that is read before any assignment in the loop (i.e., read before write) must be treated as non-invariant even if it has a single assignment. Consider that the loop body is flat (no nested loops), and expressions are simple: constants are integers, and binary operations only involve variables or constants.
*/

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cassert>

// Simplified AST node for a loop body statement
struct Expression {
    enum class Kind { Const, Var, Binary };
    Kind kind;
    int value;                     // for Const
    std::string var;               // for Var
    std::string op;                // "+" or "-" for Binary
    std::string left, right;       // variable names or "CONST" placeholder
    bool leftIsConst, rightIsConst;
    int leftConst, rightConst;

    Expression() : kind(Kind::Const), value(0), leftIsConst(true), rightIsConst(true), leftConst(0), rightConst(0) {}
};

struct Statement {
    enum class Kind { Decl, Assign, Terminator };
    Kind kind;
    std::string var;               // for Assign: LHS variable
    Expression expr;               // for Assign: RHS expression
    bool isConditional;            // true if inside an if/loop terminator
};

// Internal record for analysis
struct VarRecord {
    std::string name;
    int assignments = 0;
    bool isConditional = false;
    bool readBeforeWrite = false;
    bool selfReference = false;
    Expression* firstExpr = nullptr; // store the first assignment's RHS
    bool isConstant = false;
    bool isInduction = false;
};

static bool exprUsesOnlyConstants(const Expression& e, const std::unordered_map<std::string, VarRecord*>& records) {
    if (e.kind == Expression::Kind::Const) return true;
    if (e.kind == Expression::Kind::Var) {
        auto it = records.find(e.var);
        if (it == records.end()) return false; // variable not assigned in loop -> not loop-constant by definition
        return it->second->isConstant;
    }
    // Binary
    bool leftOk = e.leftIsConst ? true : exprUsesOnlyConstants(Expression{Expression::Kind::Var, 0, e.left, "", "", "", false, false, 0, 0}, records);
    bool rightOk = e.rightIsConst ? true : exprUsesOnlyConstants(Expression{Expression::Kind::Var, 0, e.right, "", "", "", false, false, 0, 0}, records);
    return leftOk && rightOk;
}

static int evalConstantFromExpr(const Expression& e, const std::unordered_map<std::string, VarRecord*>& records) {
    // Only used for binary expressions where operands are constants (literals or loop-constant vars)
    int lval, rval;
    if (e.leftIsConst) lval = e.leftConst;
    else {
        auto it = records.find(e.left);
        // must be constant and have a single assignment with constant RHS; we don't actually store the constant value, so assume it is known from the expression tree; to keep this simple, we precompute values in a map.
        // For brevity, we assume the function is only used for validation and we don't need the actual value.
        (void)it; lval = 0; // placeholder
    }
    if (e.rightIsConst) rval = e.rightConst;
    else {
        auto it = records.find(e.right);
        (void)it; rval = 0;
    }
    return (e.op == "+") ? lval + rval : (e.op == "-") ? lval - rval : 0;
}

std::vector<std::string> classifyLoopVariables(const std::vector<Statement>& loopBody) {
    std::unordered_map<std::string, VarRecord*> records;
    std::vector<std::string> order; // first appearance order of assigned variables

    for (const auto& stmt : loopBody) {
        if (stmt.kind == Statement::Kind::Decl) continue; // skip declarations
        if (stmt.kind == Statement::Kind::Terminator) continue; // just a break marker

        // Assignment
        const std::string& var = stmt.var;
        auto it = records.find(var);
        if (it == records.end()) {
            VarRecord* rec = new VarRecord();
            rec->name = var;
            rec->isConditional = stmt.isConditional;
            rec->firstExpr = new Expression(stmt.expr);
            records[var] = rec;
            order.push_back(var);
        } else {
            it->second->assignments++;
            if (stmt.isConditional) it->second->isConditional = true;
        }

        // Check read-before-write / self-reference
        // If variable appears in RHS and it's the first assignment, mark readBeforeWrite
        // We need to traverse the expression for variable references.
        // For simplicity, handle direct cases:
        if (stmt.expr.kind == Expression::Kind::Var && stmt.expr.var == var) {
            // self-reference
            if (it == records.end() && records[var]->assignments == 1) {
                records[var]->readBeforeWrite = true;
            }
        }
        if (stmt.expr.kind == Expression::Kind::Binary) {
            if ((!stmt.expr.leftIsConst && stmt.expr.left == var) ||
                (!stmt.expr.rightIsConst && stmt.expr.right == var)) {
                if (records[var]->assignments == 1) {
                    records[var]->readBeforeWrite = true;
                }
            }
            // also, variables used on RHS that are assigned later count as reads; but read-before-write only matters for the assigned variable itself.
        }
    }

    // Initialize: variables with exactly one assignment, unconditional, not read-before-write, and no self-reference are candidates for constant
    for (auto& kv : records) {
        VarRecord* rec = kv.second;
        if (rec->assignments == 1 && !rec->isConditional && !rec->readBeforeWrite) {
            // check if RHS contains the variable itself; if so, it's not a pure constant
            if (rec->firstExpr->kind == Expression::Kind::Var && rec->firstExpr->var == rec->name) {
                // self-ref, skip
            } else if (rec->firstExpr->kind == Expression::Kind::Binary) {
                if ((!rec->firstExpr->leftIsConst && rec->firstExpr->left == rec->name) ||
                    (!rec->firstExpr->rightIsConst && rec->firstExpr->right == rec->name)) {
                    // self-ref
                }
            }
        }
    }

    // Fixed-point to determine constants
    bool changed = true;
    while (changed) {
        changed = false;
        for (auto& kv : records) {
            VarRecord* rec = kv.second;
            if (rec->isConstant) continue;
            if (rec->assignments == 1 && !rec->isConditional && !rec->readBeforeWrite) {
                // check if RHS uses only constants (literals or already marked constants)
                if (exprUsesOnlyConstants(*rec->firstExpr, records)) {
                    rec->isConstant = true;
                    changed = true;
                }
            }
        }
    }

    // Identify induction variables
    for (auto& kv : records) {
        VarRecord* rec = kv.second;
        if (rec->isConstant) continue;
        if (rec->assignments != 1 || rec->isConditional || rec->readBeforeWrite) continue;
        // Must be of form var = var + const  or var = var - const
        const Expression& e = *rec->firstExpr;
        if (e.kind != Expression::Kind::Binary) continue;
        bool varOnLeft = (e.left == rec->name && !e.leftIsConst);
        bool varOnRight = (e.right == rec->name && !e.rightIsConst);
        if (varOnLeft && e.op == "+") {
            // increment is right side
            if (e.rightIsConst) {
                rec->isInduction = true;
            } else {
                // increment must be a loop-constant variable
                auto it = records.find(e.right);
                if (it != records.end() && it->second->isConstant) rec->isInduction = true;
            }
        } else if (varOnRight && e.op == "-") {
            // invalid: var on right of subtraction not allowed
        } else if (varOnLeft && e.op == "-") {
            // increment is right side, but negated
            if (e.rightIsConst) {
                rec->isInduction = true;
            } else {
                auto it = records.find(e.right);
                if (it != records.end() && it->second->isConstant) rec->isInduction = true;
            }
        }
    }

    // Build output lists
    std::vector<std::string> constants, induction, others;
    for (const auto& name : order) {
        VarRecord* rec = records[name];
        if (rec->isConstant) constants.push_back(name);
        else if (rec->isInduction) induction.push_back(name);
        else others.push_back(name);
    }

    // Cleanup
    for (auto& kv : records) delete kv.second;
    for (auto& kv : records) delete kv.second->firstExpr; // memory leak if not careful; but this is standalone, we can skip for brevity.

    std::vector<std::string> result;
    result.insert(result.end(), constants.begin(), constants.end());
    result.insert(result.end(), induction.begin(), induction.end());
    result.insert(result.end(), others.begin(), others.end());
    return result;
}

#include <cassert>
#include <string>
#include <vector>

// Include the solution header or paste the solution above here.
// The test uses the provided function.

int main() {
    // Helper to create Expression constants and variables
    auto constExpr = [](int v) { Expression e; e.kind = Expression::Kind::Const; e.value = v; return e; };
    auto varExpr = [](const std::string& v) { Expression e; e.kind = Expression::Kind::Var; e.var = v; return e; };
    auto binExpr = [](const std::string& op, int lc, const std::string& lv, int rc, const std::string& rv) {
        Expression e; e.kind = Expression::Kind::Binary; e.op = op;
        if (lc != 0) { e.leftIsConst = true; e.leftConst = lc; } else { e.leftIsConst = false; e.left = lv; }
        if (rc != 0) { e.rightIsConst = true; e.rightConst = rc; } else { e.rightIsConst = false; e.right = rv; }
        return e;
    };

    // Test 1: Simple loop with i = i + 1 and constant c = 5
    std::vector<Statement> body1;
    body1.push_back({Statement::Kind::Assign, "i", binExpr("+", 1, "", 0, "i"), false}); // note: this is i = i + 1 but our pattern expects var on left; need to adjust: we want LHS=i, RHS=i+1
    // Actually create correctly: i = i + 1 means LHS i, RHS binary with left=i, right=1
    body1.clear();
    Expression rhs1 = binExpr("+", 0, "i", 1, "");
    Statement s1; s1.kind = Statement::Kind::Assign; s1.var = "i"; s1.expr = rhs1; s1.isConditional = false;
    body1.push_back(s1);
    Expression rhs2 = constExpr(5);
    Statement s2; s2.kind = Statement::Kind::Assign; s2.var = "c"; s2.expr = rhs2; s2.isConditional = false;
    body1.push_back(s2);
    auto res1 = classifyLoopVariables(body1);
    // c is constant, i is induction, no others
    assert(res1.size() == 2);
    assert(res1[0] == "c" && res1[1] == "i");

    // Test 2: Two assignments to same variable -> other
    std::vector<Statement> body2;
    Statement a1; a1.kind = Statement::Kind::Assign; a1.var = "x"; a1.expr = constExpr(1); a1.isConditional = false;
    body2.push_back(a1);
    Statement a2; a2.kind = Statement::Kind::Assign; a2.var = "x"; a2.expr = constExpr(2); a2.isConditional = false;
    body2.push_back(a2);
    auto res2 = classifyLoopVariables(body2);
    assert(res2.size() == 1 && res2[0] == "x");

    // Test 3: Read before write
    std::vector<Statement> body3;
    Statement a3; a3.kind = Statement::Kind::Assign; a3.var = "y"; a3.expr = binExpr("+", 0, "y", 1, ""); a3.isConditional = false;
    body3.push_back(a3);
    auto res3 = classifyLoopVariables(body3);
    assert(res3.size() == 1 && res3[0] == "y"); // y is self-referential, becomes other

    // Test 4: Conditional assignment
    std::vector<Statement> body4;
    Statement a4; a4.kind = Statement::Kind::Assign; a4.var = "z"; a4.expr = constExpr(5); a4.isConditional = true;
    body4.push_back(a4);
    auto res4 = classifyLoopVariables(body4);
    assert(res4.size() == 1 && res4[0] == "z");

    // Test 5: Dependent constants: a = 1; b = a + 2;
    std::vector<Statement> body5;
    Statement a5a; a5a.kind = Statement::Kind::Assign; a5a.var = "a"; a5a.expr = constExpr(1); a5a.isConditional = false;
    body5.push_back(a5a);
    Statement a5b; a5b.kind = Statement::Kind::Assign; a5b.var = "b"; a5b.expr = binExpr("+", 0, "a", 2, ""); a5b.isConditional = false;
    body5.push_back(a5b);
    auto res5 = classifyLoopVariables(body5);
    // Both a and b should be constants; order a then b
    assert(res5.size() == 2 && res5[0] == "a" && res5[1] == "b");

    // Test 6: Mixed: const, induction, other
    std::vector<Statement> body6;
    Statement s6a; s6a.kind = Statement::Kind::Assign; s6a.var = "k"; s6a.expr = constExpr(0); s6a.isConditional = false; body6.push_back(s6a);
    Statement s6b; s6b.kind = Statement::Kind::Assign; s6b.var = "i"; s6b.expr = binExpr("+", 0, "i", 0, "k"); s6b.isConditional = false; body6.push_back(s6b);
    Statement s6c; s6c.kind = Statement::Kind::Assign; s6c.var = "j"; s6c.expr = binExpr("+", 0, "i", 1, ""); s6c.isConditional = false; body6.push_back(s6c);
    Statement s6d; s6d.kind = Statement::Kind::Assign; s6d.var = "j"; s6d.expr = constExpr(9); s6d.isConditional = false; body6.push_back(s6d); // second assignment to j
    auto res6 = classifyLoopVariables(body6);
    // k is constant, i is induction? i = i + k where k is constant -> yes, induction; j has two assignments -> other
    assert(res6.size() == 3);
    assert(res6[0] == "k");
    assert(res6[1] == "i");
    assert(res6[2] == "j");

    // Test 7: Subtraction pattern: i = i - 2
    std::vector<Statement> body7;
    Statement s7; s7.kind = Statement::Kind::Assign; s7.var = "i"; s7.expr = binExpr("-", 0, "i", 2, ""); s7.isConditional = false; body7.push_back(s7);
    auto res7 = classifyLoopVariables(body7);
    assert(res7.size() == 1 && res7[0] == "i"); // induction

    // Test 8: Declerations are skipped
    std::vector<Statement> body8;
    body8.push_back({Statement::Kind::Decl, "dummy", {}, false});
    Statement s8; s8.kind = Statement::Kind::Assign; s8.var = "x"; s8.expr = constExpr(3); s8.isConditional = false; body8.push_back(s8);
    auto res8 = classifyLoopVariables(body8);
    assert(res8.size() == 1 && res8[0] == "x");

    return 0;
}

// The solution builds a small syntactic analyzer. First, we traverse the loop body in order, maintaining a list of "variable records" keyed by variable name. For each statement:
// - If it's a declaration, skip it (no effect on variable records).
// - If it's an assignment, we immediately record the variable: if it's the first time we see it, create a record; increment its assignment count; mark it as conditional if we are inside a conditional (i.e., we've seen an `if` without a matching `else` or the assignment is inside a loop terminator's then-branch, but for simplicity treat any assignment appearing within an `if` block as conditional). Also, if the variable appears on the RHS and is the same as the LHS, mark it as read-before-write if it's the first assignment (self-reference).
// - If it's an `if (expr) break;`, we treat it as a loop terminator and do not affect variable classification directly (but we must handle nested statements inside the then-branch; in our simplified AST, we only have a flat vector, so we can simulate by introducing a depth counter: when we see an `if` statement, we increment depth, and any assignments after it until the matching `else` or `end` are conditional; but to keep the input simple, we require that the loop body is already fully expanded and each assignment carries a flag `isConditional`).
//
// To simplify, we design the `Statement` structure to have a boolean `isConditional` that indicates whether the assignment is inside any conditional (including the then-branch of a loop terminator). This makes the analysis straightforward.
//
// After collecting all variable records, we perform a fixed-point iteration over the variables to determine which are loop-invariant constants. A variable is initially marked as a "candidate" if it has exactly one assignment, is not conditional, is not read-before-write, and has no self-reference (i.e., the RHS does not contain the variable itself). Then, we repeatedly check each candidate's assignment RHS: all variables referenced in the RHS must already be classified as loop-invariant constants (or be a constant literal). We also require that the RHS is a valid expression (constant, variable, or binary add/sub). If all operands are loop-constant, we mark it as a constant.
//
// After the fixed point, any variable that is not a constant and has exactly one assignment, is unconditional, and has the pattern `var = var + inc` or `var = var - inc` where `inc` is a loop-invariant constant (either a literal or a variable already classified as constant), is an induction variable. For subtraction, the variable must be on the left operand; the increment is the right operand negated conceptually (we just store the variable name as induction variable with a note).
//
// All remaining variables (including those with multiple assignments, conditional assignments, or read-before-write) fall into "other".
//
// Finally, we output three lists in encounter order: constants, induction variables, then others.
//
// Edge cases: A variable that appears only on the RHS of an assignment but is never assigned in the loop (e.g., a loop-invariant variable from outside) is not part of the loop's variable set; we only consider variables that are assigned at least once. However, if a variable is read but never assigned, we ignore it because it cannot be assigned in the loop. If a variable is assigned more than once, it's automatically "other". If an assignment is conditional (even if the condition is a simple constant), it's conditional. A variable that is read before its first assignment (i.e., appears on RHS of an assignment before any prior assignment in the loop, or appears on RHS of an assignment and is the same as LHS at the first assignment) is read-before-write.
//
// Time complexity: O(V * E) where V is number of distinct variables and E is total number of expressions examined in the fixed-point; in practice, O(V^2) worst case. Space complexity: O(V).
