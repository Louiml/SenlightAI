// Given a DNF (Disjunctive Normal Form) expression as a string (using variable names like `x1`, `x2`, etc., operators `&` for AND, `|` for OR, `~` for NOT, and parentheses) and a vector of boolean values representing a truth table for some Boolean function, write a C++ function `std::wstring checkDNF(const std::wstring& expr, const std::vector<bool>& truthTable)`. The function should return the string `"1"` if the expression is a valid DNF (only literals combined by ANDs, with ORs at the top level, no EQUAL expressions) and its truth table exactly matches the given `truthTable` vector. Return `"2"` if the expression is syntactically invalid (parsing fails), `"3"` if it is syntactically valid but not a DNF, and `"0"` if it is a DNF but its computed truth table does not match the given vector. The number of variables in the expression must be exactly \(\log_2(\text{size of truthTable})\). Variables named in the expression must be from `x1` to `x_{n}` where `n` is the number of variables, and all such variable names must appear in the expression or be added implicitly (i.e., missing variables in the expression are treated as unused but still considered). The truth table order for `n` variables follows the binary counting order with `x1` as the most significant bit (like `0,0,...0`, `0,0,...1`, ..., `1,1,...1`). The function should output only `"0"`, `"1"`, `"2"`, or `"3"` (no extra whitespace).

#include <cassert>
#include <string>
#include <vector>

// Declaration of the solution function (in practice include the header)
std::wstring checkDNF(const std::wstring& expr, const std::vector<bool>& truthTable);

// Minimal parser stub for testing (replace with actual parser in real environment)
namespace BoolApp {
    // ... (full parser implementation needed for tests; here we assume it's provided)
}

int main() {
    // Test 1: Valid DNF x1 & ~x2 OR x3, truth table for 3 vars (size 8)
    // Use a known truth table (e.g., all zeros except one)
    std::vector<bool> tt(8, false);
    tt[0] = true;  // only assignment x1=0,x2=0,x3=0 gives true? Actually x1&~x2 is true when x1=1,x2=0, plus x3 true
    // Let’s manually construct a correct truth table for expression (x1 & ~x2) | x3
    tt = {false, true, false, true, true, true, true, true};  // order: x1,x2,x3 as MSB->LSB? Wait: our order is x1 MSB, then x2, then x3. So mask=0 gives (0,0,0)=false; mask=1 (0,0,1)=true; etc. Let's verify: (0,0,0): x1&~x2=0, x3=0 → false. (0,0,1): x3=1 → true. (0,1,0): x1=0 → false. (0,1,1): x3=1 → true. (1,0,0): x1=1 & ~x2=1 → true. (1,0,1): true. (1,1,0): x1=1 & ~0=0 → false? Actually ~x2 = ~1 =0, so 0; x3=0 → false, but our list says true at index 6? Let's compute: index 6 = binary 110 → x1=1,x2=1,x3=0 → x1&~x2 = 1&0=0, x3=0 → false. So correct table is {false, true, false, true, true, true, false, true}.
    tt = {false, true, false, true, true, true, false, true};
    assert(checkDNF(L"x1 & ~x2 | x3", tt) == L"1");

    // Test 2: Not a DNF (contains EQUAL)
    assert(checkDNF(L"x1 = x2", std::vector<bool>(4, false)) == L"3");

    // Test 3: Invalid syntax (unbalanced parenthesis)
    assert(checkDNF(L"(x1 & x2", std::vector<bool>(4, false)) == L"2");

    // Test 4: DNF but wrong truth table
    std::vector<bool> wrong(4, false);
    assert(checkDNF(L"x1", wrong) == L"0");

    // Test 5: DNF with wrong number of variables (truth table size 4 but expr uses x1,x2,x3)
    assert(checkDNF(L"x1 | x2 | x3", std::vector<bool>(4, false)) == L"0");

    // Test 6: DNF with correct table for x1 XOR x2 is not DNF anyway, so use valid
    std::vector<bool> tt2 = {false, true, true, false};  // x1 XOR x2, but that's not DNF, so test a DNF: x1 | (~x1 & x2) → table: (0,0)=0|(1&0)=0 → false? Actually ~x1 & x2: (0,0):1&0=0, (0,1):1&1=1, (1,0):0&0=0, (1,1):0&1=0. OR with x1: (1,0):1|0=1, (1,1):1|0=1. So final: (0,0)=0, (0,1)=1, (1,0)=1, (1,1)=1 → {false,true,true,true}
    std::vector<bool> tt3 = {false, true, true, true};
    assert(checkDNF(L"x1 | (~x1 & x2)", tt3) == L"1");

    // Test 7: Constants only
    std::vector<bool> tt4 = {true};  // n_arg = 0
    assert(checkDNF(L"1", tt4) == L"1");
    assert(checkDNF(L"1", std::vector<bool>{false}) == L"0");

    // Test 8: Empty string
    assert(checkDNF(L"", std::vector<bool>{false}) == L"2");

    return 0;
}

#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <cmath>

// Minimal term hierarchy for parsing (simplified for the task; assume BoolApp namespace provides these)
namespace BoolApp {
    struct term {
        virtual ~term() = default;
        virtual bool correct() const = 0;
        virtual bool calculate(const std::map<std::wstring, bool>& data) const = 0;
        virtual void get_name_list(std::set<std::wstring>& st) const = 0;
    };
    struct termVAR : term {
        std::wstring name;
        termVAR(const std::wstring& n) : name(n) {}
        bool correct() const override { return !name.empty(); }
        bool calculate(const std::map<std::wstring, bool>& data) const override { return data.at(name); }
        void get_name_list(std::set<std::wstring>& st) const override { st.insert(name); }
    };
    struct termNOT : term {
        term* t1;
        termNOT(term* x) : t1(x) {}
        ~termNOT() { delete t1; }
        bool correct() const override { return t1 && t1->correct(); }
        bool calculate(const std::map<std::wstring, bool>& data) const override { return !t1->calculate(data); }
        void get_name_list(std::set<std::wstring>& st) const override { t1->get_name_list(st); }
    };
    struct termAND : term {
        term* t1; term* t2;
        termAND(term* a, term* b) : t1(a), t2(b) {}
        ~termAND() { delete t1; delete t2; }
        bool correct() const override { return t1 && t2 && t1->correct() && t2->correct(); }
        bool calculate(const std::map<std::wstring, bool>& data) const override { return t1->calculate(data) && t2->calculate(data); }
        void get_name_list(std::set<std::wstring>& st) const override { t1->get_name_list(st); t2->get_name_list(st); }
    };
    struct termOR : term {
        term* t1; term* t2;
        termOR(term* a, term* b) : t1(a), t2(b) {}
        ~termOR() { delete t1; delete t2; }
        bool correct() const override { return t1 && t2 && t1->correct() && t2->correct(); }
        bool calculate(const std::map<std::wstring, bool>& data) const override { return t1->calculate(data) || t2->calculate(data); }
        void get_name_list(std::set<std::wstring>& st) const override { t1->get_name_list(st); t2->get_name_list(st); }
    };
    struct termEQUAL : term {
        term* t1; term* t2;
        termEQUAL(term* a, term* b) : t1(a), t2(b) {}
        ~termEQUAL() { delete t1; delete t2; }
        bool correct() const override { return t1 && t2 && t1->correct() && t2->correct(); }
        bool calculate(const std::map<std::wstring, bool>& data) const override { return t1->calculate(data) == t2->calculate(data); }
        void get_name_list(std::set<std::wstring>& st) const override { t1->get_name_list(st); t2->get_name_list(st); }
    };

    // Minimal parser (simplified): supports x1, ~, &, |, parens, constants 0 and 1
    term* parsing(wchar_t*& ch);
}

// Helper functions for DNF checking
static bool is_literal(BoolApp::term* t) {
    if (!t) return false;
    if (dynamic_cast<BoolApp::termVAR*>(t)) return true;
    if (auto nt = dynamic_cast<BoolApp::termNOT*>(t))
        return nt->t1 && dynamic_cast<BoolApp::termVAR*>(nt->t1);
    return false;
}

static bool is_and(BoolApp::term* t) {
    if (!t) return false;
    if (auto at = dynamic_cast<BoolApp::termAND*>(t))
        return is_literal(at->t1) && is_and(at->t2);
    return is_literal(t);
}

static bool is_dnf(BoolApp::term* t) {
    if (!t) return false;
    if (dynamic_cast<BoolApp::termEQUAL*>(t)) return false;
    if (auto ot = dynamic_cast<BoolApp::termOR*>(t))
        return is_and(ot->t1) && is_dnf(ot->t2);
    if (auto at = dynamic_cast<BoolApp::termAND*>(t))
        return is_and(at);
    return is_literal(t);
}

std::wstring checkDNF(const std::wstring& expr, const std::vector<bool>& truthTable) {
    // Copy input string to mutable buffer for parser
    std::wstring mutableStr = expr;
    wchar_t* ch = mutableStr.data();
    BoolApp::term* t = BoolApp::parsing(ch);

    if (!t) return L"2";  // Invalid expression

    if (!t->correct() || !is_dnf(t)) {
        delete t;
        return L"3";  // Not a DNF
    }

    // Determine number of variables from truth table size
    int n_arg = 0;
    long long sz = truthTable.size();
    while (sz > 1) { sz /= 2; ++n_arg; }
    if (truthTable.size() != (1LL << n_arg)) {
        delete t;
        return L"0";  // Size not power of two -> mismatch
    }

    // Collect variable names
    std::set<std::wstring> st;
    t->get_name_list(st);
    st.erase(L"0");
    st.erase(L"1");

    // Add all canonical x1..xn
    for (int i = 1; i <= n_arg; ++i)
        st.insert(L"x" + std::to_wstring(i));

    // Build variable list (ordered deterministically as set)
    std::vector<std::wstring> varList(st.begin(), st.end());
    int m = varList.size();
    if (m != n_arg) {
        delete t;
        return L"0";  // Invalid variable names or extra variables
    }

    // Evaluate term over all assignments
    std::vector<bool> result;
    result.reserve(1LL << m);
    for (long long mask = 0; mask < (1LL << m); ++mask) {
        std::map<std::wstring, bool> data;
        for (int j = 0; j < m; ++j) {
            // x1 is most significant bit: bit j corresponds to varList[m-1-j]
            bool bit = (mask >> j) & 1;
            data[varList[m - 1 - j]] = bit;
        }
        result.push_back(t->calculate(data));
    }

    delete t;

    if (result.size() != truthTable.size()) return L"0";
    for (size_t i = 0; i < result.size(); ++i)
        if (result[i] != truthTable[i]) return L"0";
    return L"1";
}

// The solution must first parse the input expression into a term structure. If parsing fails (null pointer), return `"2"`. Then check syntactic correctness (`term::correct()`) and structural DNF validity: a term is a DNF if it is a literal (variable or negation of a variable), an AND of literals (where each operand is a literal), or an OR of such AND-terms (and recursively the right operand is a DNF). Use helper functions `is_literal` and `is_and` to implement this. Collect all variable names present in the expression; if any name is not in the form `x1...xn` (where `n = log2(size of truthTable)`), still accept it? The reference solution adds variables `x1` to `x_{n}` to the set regardless, but also includes any variable from the expression, which could cause mismatches. The task requires: after parsing and DNF check, insert all `x_i` for `i=1..n` into the set of names, but also keep any other names already present (though the expression should only use those names — if it uses others, they will be considered as variables, changing the truth table size). Actually, the algorithm: compute `n_arg` as `log2(v.size())` (number of variables implied by truth table). Add to the set all `x1` to `x_n`. Then, for each variable in the set, evaluate the term over all `2^m` assignments where `m` is the size of this set. The order of assignments uses indexing from `0` to `2^m - 1`, with `x1` as the most significant bit (position `m-1` in the bitmask). Compare the resulting vector to the given truth table; if lengths differ, return `"0"` (since the set size might exceed `n` if extra names appear). If all match, return `"1"`. Edge cases: empty expression → parse fails → `"2"`; expression using only constants `0` or `1`? The code removes `"0"` and `"1"` from the name set, but if the expression contains constants, they are handled by the term structure. The truth table for a constant is single-value, but our `n_arg` would be 0 (`log2(1)=0`), so we add no variables. Evaluate over one assignment (empty set) → good. Also handle NOT of NOT? Not literal → fail DNF check. Time complexity: parsing O(L) where L is expression length; DNF check O(T) where T is number of nodes; evaluation over up to `2^m` assignments with each evaluation O(T) gives O(T * 2^m). Space O(T + 2^m).
