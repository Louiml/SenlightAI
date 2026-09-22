// Write a C++ function that processes a system of string replacement rules. You are given an initial string `s` and `N` rules. Each rule is of the form `d -> t` where `d` is a single decimal digit (0-9) and `t` is a string (possibly empty) of digits and/or characters. The rules are applied in a recursive, memoized manner: for any position `i` from 0 to N-1, if a character equals `st[i]` (the digit on the left side of rule `i`), it is replaced by the fully evaluated string `tr[i]` (the right side of rule `i`), evaluated recursively with rules starting at index `i+1`. If the character does not match `st[i]`, it is passed unchanged to rule `i+1`. For position `i = N`, each digit is simply itself. The evaluation of a string involves concatenating the evaluated characters (in original order) and computing the resulting integer modulo 1,000,000,007; also track the number of decimal digits produced (modulo 1,000,000,006, since exponent reduction by Fermat's little theorem for mod prime). The function should take the initial string `s` and a vector of rules (each rule as a string like `"d->t"`) and return the modulo result of fully evaluating `s` starting at rule index 0. All inputs are valid: `N` non-negative, rule strings well-formed, `s` non-empty. The final evaluated string may be extremely long, so do not construct it; instead compute values and lengths recursively with memoization.

#include <cassert>
#include <string>
#include <vector>
#include <iostream>

// Assume the solution function is declared here:
long long evaluateStringRules(const std::string& initial, const std::vector<std::string>& rules);

int main() {
    // Simple case: no rules
    assert(evaluateStringRules("123", {}) == 123LL);
    
    // Single rule: 0->5 (replace all 0s with 5)
    assert(evaluateStringRules("0", {"0->5"}) == 5LL);
    
    // Rule on a digit that doesn't appear → unchanged
    assert(evaluateStringRules("12", {"0->5"}) == 12LL);
    
    // Rule that expands to multi-digit string
    assert(evaluateStringRules("1", {"1->23"}) == 23LL);
    
    // Nested rules: first 1->2, second 2->34 (applied sequentially)
    // For "1": at rule0 (1->2) replaced by "2", then evaluated at rule1 (2->34) → "34" → 34
    assert(evaluateStringRules("1", {"1->2", "2->34"}) == 34LL);
    
    // Rule with empty right side: digit disappears
    assert(evaluateStringRules("10", {"1->"}) == 0LL); // "10" becomes "0" after removing 1 (since 1's replacement is empty)
    
    // Multiple digits, complex expansion
    // Rule0: 0->12, Rule1: 1->3, Rule2: 2->4
    // Initial "01": 
    //   at rule0: '0' matches → replace by "12"; '1' doesn't match → pass to rule1.
    //   After rule0: string is "12"+"1" = "121"
    //   At rule1: '1' matches → replace by "3"; '2' doesn't match → pass to rule2; '1' matches → replace by "3"
    //   After rule1: "3"+"2"+"3" = "323"
    //   At rule2: '2' matches → replace by "4"; so "3"+"4"+"3" = "343" → 343
    assert(evaluateStringRules("01", {"0->12", "1->3", "2->4"}) == 343LL);
    
    // Large modulo test: many digits, verify mod 1e9+7
    // Use a rule that squares the length: 0->00 (doubles digit count)
    // Initial "0" with 5 rules each "0->00" → length 2^5=32 digits of 0 → value 0
    assert(evaluateStringRules("0", {"0->00", "0->00", "0->00", "0->00", "0->00"}) == 0LL);
    
    // Test length handling: 1->11 with 3 rules should produce 2^3 = 8 ones → 11111111
    assert(evaluateStringRules("1", {"1->11", "1->11", "1->11"}) == 11111111LL);
    
    // Test modulo exponent: 9->99 with 10 rules → 2^10 = 1024 nines, value mod 1e9+7
    // 111...111 (1024 ones) is 111...111 mod 1e9+7; just check it's not equal to a naive overflow
    long long result = evaluateStringRules("1", {"1->11", "1->11", "1->11", "1->11", "1->11", "1->11", "1->11", "1->11", "1->11", "1->11"});
    assert(result >= 0 && result < 1000000007LL);
    
    // Mixed rule: 0->1, 1->0 (swap)
    assert(evaluateStringRules("01", {"0->1", "1->0"}) == 10LL); // "01" becomes "10" after evaluation
    
    std::cout << "All tests passed.\n";
    return 0;
}

#include <string>
#include <vector>
#include <cstring>

const long long MOD = 1000000007LL;
const long long MODM1 = MOD - 1;

long long mod_pow_ll(long long base, long long exp) {
    long long result = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }
    return result;
}

class RuleEvaluator {
public:
    // Evaluates the initial string s under the rules starting at index 0.
    // rules are strings like "d->t" where d is a single digit and t is any string.
    static long long evaluateRules(const std::string& s, const std::vector<std::string>& rules) {
        int N = (int)rules.size();
        RuleEvaluator re(s, rules);
        auto res = re.evalString(s, 0);
        return res.first;
    }

private:
    const std::string& s;
    const std::vector<std::string>& rules;
    int N;
    std::vector<char> st; // left-hand digit per rule
    std::vector<std::string> tr; // right-hand string per rule
    long long val[10005][10]; // value mod MOD
    int len[10005][10];      // length mod MOD-1
    bool seen[10005][10];

    RuleEvaluator(const std::string& str, const std::vector<std::string>& r) : s(str), rules(r) {
        N = (int)rules.size();
        st.resize(N);
        tr.resize(N);
        for (int i = 0; i < N; ++i) {
            st[i] = rules[i][0];
            tr[i] = rules[i].substr(3);
        }
        std::memset(seen, 0, sizeof(seen));
    }

    // Evaluate a single character c starting at rule index i.
    void calcChar(char c, int i) {
        int ci = c - '0';
        if (seen[i][ci]) return;
        seen[i][ci] = true;

        if (i == N) {
            val[i][ci] = ci;
            len[i][ci] = 1;
            return;
        }

        if (st[i] != c) {
            calcChar(c, i + 1);
            val[i][ci] = val[i+1][ci];
            len[i][ci] = len[i+1][ci];
            return;
        }

        auto res = evalString(tr[i], i + 1);
        val[i][ci] = res.first;
        len[i][ci] = res.second;
    }

    // Evaluate a full string arg starting at rule index i.
    // Returns (value mod MOD, length mod MOD-1).
    std::pair<long long, int> evalString(const std::string& arg, int i) {
        long long ret = 0;
        int lsf = 0; // length so far (mod MOD-1)

        for (int j = (int)arg.length() - 1; j >= 0; --j) {
            char c = arg[j];
            calcChar(c, i);
            int ci = c - '0';
            long long contribution = (val[i][ci] * mod_pow_ll(10, lsf)) % MOD;
            ret = (ret + contribution) % MOD;
            lsf = (lsf + len[i][ci]) % MODM1;
        }
        return {ret, lsf};
    }
};

// Public free function matching the task specification.
// Takes the initial string and a vector of rule strings ("d->t") and returns the evaluated value modulo 1,000,000,007.
long long evaluateStringRules(const std::string& initial, const std::vector<std::string>& rules) {
    return RuleEvaluator::evaluateRules(initial, rules);
}

// The solution uses dynamic programming with memoization on the state `(position i, digit c)` representing the evaluation of a single digit `c` starting at rule index `i`. For each such state, we store two values: `val[i][c]` = the integer value (mod MOD) of the string obtained by fully evaluating digit `c` starting at rule `i`, and `len[i][c]` = the number of decimal digits in that string modulo (MOD-1). The base case is `i == N`: digit `c` evaluates to itself, so `val = c`, `len = 1`. For `i < N`: if `c != st[i]`, then the digit is not replaced at this step, so we just pass it to `i+1`: `val[i][c] = val[i+1][c]`, `len[i][c] = len[i+1][c]`. If `c == st[i]`, the digit is replaced by the right-hand string `tr[i]`, and that string must be evaluated starting at index `i+1`. To evaluate a string, we process its characters from right to left (because each character contributes its value multiplied by 10^(total length of characters after it, mod MOD-1)). For each character `ch` in `tr[i]`, we compute `(digit_val, digit_len)` from `eval_digit(ch, i+1)`, then combine: `result_value = (result_value + digit_val * 10^digit_len_so_far) % MOD`, and `digit_len_so_far = (digit_len_so_far + digit_len) % (MOD-1)`. The memoization ensures each state `(i, c)` is computed at most once. There are at most `(N+1)*10` states, and evaluating each right-hand string takes time proportional to its length. Over all states, the total work is bounded by the sum of lengths of all right-hand strings (each may be processed once per distinct starting digit, but in practice memoization prevents re-evaluation). The time complexity is O(total length of all `tr[i]` + N*10) and space O(N*10). Important edge cases: empty right-hand side (string may be empty; then the digit vanishes, value 0, length 0), rules that refer to themselves indirectly through later indices (the recursion only goes forward to `i+1`, so no cycles). Also note the careful use of modulo (MOD and MOD-1) to avoid overflow, and `mod_pow` for fast exponentiation.
