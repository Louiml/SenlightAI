/*
Write a C++ function that performs constant folding and constant propagation on a simplified Three-Address Code (TAC) representation of arithmetic expressions, where each instruction is a `Quadruple` containing a result variable, an optional binary operation (+, -, *, /), and two operands (variable names or integer constant strings). The function should take a `std::vector<Quadruple>&` and modify it in-place so that all computations involving only constants are evaluated at compile time, and variables that are assigned constant values are substituted into subsequent uses wherever possible. The final optimized list should contain only assignments where the right-hand side is either a single integer constant (for folded/propagated results) or a variable-expression that cannot be further simplified (e.g., `c = a + b` where neither `a` nor `b` is known to be constant). Simple assignments of the form `x = constant` may remain if the constant is not used later, but all binary operations with two constant operands must be eliminated. The order of instructions must be preserved. Handle division by zero safely by treating the result as `0` (as in the original snippet). The function should be self-contained, not depend on any global state, and use only the `Quadruple` struct defined as `struct Quadruple { std::string result; std::string arg1; std::string op; std::string arg2; };`. The solution must be implemented as a single free function `void optimizeTAC(std::vector<Quadruple>& tac)` that performs both folding and propagation in a correct and efficient manner.
*/
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>

struct Quadruple {
    std::string result;
    std::string arg1;
    std::string op;
    std::string arg2;
};

// Helper to check if a string represents a non-negative integer
static bool isConstant(const std::string& s) {
    if (s.empty()) return false;
    return std::all_of(s.begin(), s.end(), ::isdigit);
}

// Evaluate binary operation; division by zero yields 0
static int eval(int a, int b, const std::string& op) {
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") return b != 0 ? a / b : 0;
    return 0;
}

// Perform constant folding and propagation in-place on a TAC list.
void optimizeTAC(std::vector<Quadruple>& tac) {
    // Map from variable name to known constant string
    std::map<std::string, std::string> constTable;

    for (auto& line : tac) {
        // Substitute known constants into operands
        auto it1 = constTable.find(line.arg1);
        if (it1 != constTable.end()) line.arg1 = it1->second;
        auto it2 = constTable.find(line.arg2);
        if (it2 != constTable.end()) line.arg2 = it2->second;

        // If binary operation with both operands constant -> fold
        if (!line.op.empty() && isConstant(line.arg1) && isConstant(line.arg2)) {
            int val = eval(std::stoi(line.arg1), std::stoi(line.arg2), line.op);
            line.arg1 = std::to_string(val);
            line.op.clear();
            line.arg2.clear();
        }

        // Record result if it is a constant
        if (line.op.empty() && isConstant(line.arg1)) {
            constTable[line.result] = line.arg1;
        } else {
            // Non-constant assignment: invalidate any previous constant for this variable
            constTable.erase(line.result);
        }
    }
}
#include <cassert>
#include <vector>
#include <string>

int main() {
    // Test 1: Simple folding
    std::vector<Quadruple> t1 = {{"x", "2", "+", "3"}};
    optimizeTAC(t1);
    assert(t1.size() == 1);
    assert(t1[0].result == "x" && t1[0].arg1 == "5" && t1[0].op.empty());

    // Test 2: Propagation chain
    std::vector<Quadruple> t2 = {
        {"a", "1", "", ""},
        {"b", "a", "+", "2"},
        {"c", "b", "+", "3"}
    };
    optimizeTAC(t2);
    assert(t2[0].op.empty() && t2[0].arg1 == "1");
    assert(t2[1].op.empty() && t2[1].arg1 == "3");
    assert(t2[2].op.empty() && t2[2].arg1 == "6");

    // Test 3: Reassignment invalidates constant
    std::vector<Quadruple> t3 = {
        {"a", "5", "", ""},
        {"a", "b", "+", "c"},
        {"d", "a", "+", "1"}
    };
    optimizeTAC(t3);
    // Second line stays non-foldable
    assert(t3[1].op == "+" && t3[1].arg1 == "b" && t3[1].arg2 == "c");
    // Third line cannot use old a=5; remains as is
    assert(t3[2].arg1 == "a" && t3[2].arg2 == "1");

    // Test 4: Division by zero yields 0
    std::vector<Quadruple> t4 = {{"r", "10", "/", "0"}};
    optimizeTAC(t4);
    assert(t4[0].arg1 == "0" && t4[0].op.empty());

    // Test 5: Mixed variables not folded
    std::vector<Quadruple> t5 = {{"p", "x", "*", "2"}};
    optimizeTAC(t5);
    assert(t5[0].op == "*" && t5[0].arg1 == "x" && t5[0].arg2 == "2");

    // Test 6: Multiple constant definitions used later
    std::vector<Quadruple> t6 = {
        {"a", "7", "", ""},
        {"b", "3", "", ""},
        {"c", "a", "-", "b"}
    };
    optimizeTAC(t6);
    assert(t6[2].op.empty() && t6[2].arg1 == "4");

    return 0;
}
// The solution requires a two-pass approach: first, perform constant folding on all instructions where both `arg1` and `arg2` are numbers (using `std::all_of` and `::isdigit` to detect numbers, allowing negative? But the snippet only handles non-negative integers; we'll follow that). For each such instruction, evaluate the binary operation and replace it with a simple assignment `result = constant`. Then, perform constant propagation by maintaining a map from variable names to known constant string values. Iterate through the list of instructions in order: for each instruction, substitute any operand that appears in the constant map with its mapped constant. After substitution, if the instruction is a binary operation and both operands are now numbers, evaluate it and replace with a simple assignment to the constant, and also record the result in the map. If the instruction is a simple assignment (`op` empty) and the RHS is a number, record it in the map. However, careful: after substitution, a simple assignment of a variable that is not a number should not be recorded. Important edge cases: (1) Variables may be reassigned later; the map should only reflect the most recent known constant for each variable, which is naturally handled by overwriting in the map. (2) A binary operation where one operand becomes a constant after propagation and the other is still a variable cannot be folded yet; but if later that variable becomes constant, we need another pass. The original snippet performs folding, propagation, then folding again, but that is insufficient in some chains (e.g., `a=1; b=a+2; c=b+3;` – first folding does nothing, propagation substitutes `a` into `b` making `b=1+2` which is folded to `b=3`, but then `c=b+3` still sees `b` as variable; a second propagation would substitute `b` into `c` and then fold). A robust solution is to iterate propagation and folding until no changes occur. Alternatively, apply constant folding first to eliminate all constant folds, then perform a single propagation pass with folding inside; but to handle multi-step propagation like `a=1; b=a+1; c=b+1;`, we need to propagate within the same pass because when we process `b`, we store `3` in the map, and when we process `c`, we can substitute `3` for `b`. That works with a single pass because the map is updated during the iteration. However, consider `a=b+1; b=2;` – here `b` is defined after use; propagation cannot help. The algorithm must process instructions in order, which is correct for forward propagation. So the approach: Step 1: Perform pure constant folding (replace any binary op with both constant operands by a constant assignment) to simplify the list. Step 2: Run a single constant propagation pass that also performs folding on-the-fly: maintain a map `constTable`; for each instruction, substitute operands from the map; then if binary op and both operands are numbers, fold and store result in map; else if simple assignment and RHS is number, store result in map. This will handle chains because as we go forward, newly found constants are stored and used immediately for later instructions. However, consider `a=1; b=a+1; c=b+1;` – first instruction stores `a=1`; second instruction substitutes `a=1` resulting in `b=1+1`, folds to `b=2`, stores `b=2`; third instruction substitutes `b=2` resulting in `c=2+1`, folds to `c=3`. Works. Consider `a=1; b=a+1; c=b; d=c+1;` – works. So a single propagation pass with folding is sufficient after an initial folding pass? Actually, the initial folding is not strictly needed because the propagation pass also folds when both operands are constants after substitution. But if an instruction is `x=1+2` with no variables, the propagation pass will see both numbers, fold, and store. So we can skip the separate folding pass entirely. However, if there is an instruction `x=1+2` that is never used later, it still gets folded. So the algorithm is: simply run the propagation pass that includes folding. But careful: the map should only be updated for variables that are assigned a constant value (either simple assignment or after folding). Also, if a variable is reassigned to a non-constant later, we should remove it from the map (since it's no longer constant). The original snippet does not remove entries on non-constant assignment; that's a bug because if `a` is later assigned `b+c` (non-constant), we should not substitute the old constant for `a` in later uses. So we must handle that: when processing an instruction that assigns to `result`, if the right-hand side after substitution is not a number (i.e., it's a variable expression), remove `result` from the map. Also, if it is a simple assignment where `arg1` is a variable that is not a number, remove `result` (since not constant). After removal, continue. Time complexity: O(n) for the single pass, plus O(n) for the initial optional folding pass (if we do it) but we can skip it. Space complexity: O(v) for the map where v is number of distinct variable names. All operations on strings are O(length) but operand strings are short.
