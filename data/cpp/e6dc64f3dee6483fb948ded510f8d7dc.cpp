// Write a C++ function `generateBalancedParentheses(int n)` that returns a `std::vector<std::string>` containing all valid combinations of `n` pairs of parentheses, i.e., strings of length `2n` consisting of `'('` and `')'` such that every prefix has at least as many opening parentheses as closing ones, and the total numbers of opening and closing parentheses are equal. The order of the resulting strings does not matter, but the vector should contain each valid combination exactly once. For example, for `n = 3`, the function must return exactly six strings: `((()))`, `(()())`, `(())()`, `()(())`, `()()()`. Handle edge cases: for `n = 0`, return a vector containing exactly one empty string; for negative `n`, return an empty vector.

The problem is a classic backtracking/recursive construction problem. At each step, we maintain a partial string `str`, along with the counts `open` and `close` of parentheses already added. The base case occurs when `str.length()` equals `2 * n`, meaning we have placed all parentheses; at that point, we add the complete string to the answer vector. Otherwise, we can append a `'('` if `open < n` (we haven't used all opening brackets), and we can append a `')'` if `close < open` (we must not close more than we have opened). By exploring these two branches recursively, we generate every valid string exactly once, because the constraints `open ≤ n` and `close ≤ open` are necessary and sufficient for validity. Edge cases: if `n <= 0`, we return either an empty vector (for negative) or a vector with a single empty string (for zero), which aligns with the recursion stopping immediately. Time complexity: The number of valid strings is the Catalan number \(C_n = \frac{1}{n+1}\binom{2n}{n}\), and each string takes \(O(n)\) to copy, so total time is \(O(n \cdot C_n)\). Space complexity: The recursion depth is at most \(2n\), and the output vector itself uses \(O(n \cdot C_n)\) space, so auxiliary space excluding output is \(O(n)\).

#include <vector>
#include <string>

// Return all valid combinations of n pairs of parentheses.
std::vector<std::string> generateBalancedParentheses(int n) {
    std::vector<std::string> result;
    if (n < 0) {
        return result;
    }
    if (n == 0) {
        result.push_back("");
        return result;
    }

    std::string current;
    current.reserve(2 * static_cast<size_t>(n));

    // Recursive helper: builds valid strings by adding '(' or ')'.
    // Parameters: open = count of '(' used, close = count of ')' used.
    auto backtrack = [&](int open, int close) -> void {
        if (static_cast<int>(current.length()) == 2 * n) {
            result.push_back(current);
            return;
        }
        if (open < n) {
            current.push_back('(');
            backtrack(open + 1, close);
            current.pop_back();
        }
        if (close < open) {
            current.push_back(')');
            backtrack(open, close + 1);
            current.pop_back();
        }
    };

    backtrack(0, 0);
    return result;
}

#include <cassert>
#include <vector>
#include <string>
#include <algorithm>

int main() {
    // Test n = 0
    auto res0 = generateBalancedParentheses(0);
    assert(res0.size() == 1);
    assert(res0[0] == "");

    // Test n = 1
    auto res1 = generateBalancedParentheses(1);
    assert(res1.size() == 1);
    assert(res1[0] == "()");

    // Test n = 2
    auto res2 = generateBalancedParentheses(2);
    std::vector<std::string> expected2 = {"(())", "()()"};
    assert(res2.size() == expected2.size());
    for (const auto& s : expected2) {
        assert(std::find(res2.begin(), res2.end(), s) != res2.end());
    }

    // Test n = 3
    auto res3 = generateBalancedParentheses(3);
    std::vector<std::string> expected3 = {"((()))", "(()())", "(())()", "()(())", "()()()"};
    assert(res3.size() == expected3.size());
    for (const auto& s : expected3) {
        assert(std::find(res3.begin(), res3.end(), s) != res3.end());
    }

    // Test n = 4 (Catalan number C4 = 14)
    auto res4 = generateBalancedParentheses(4);
    assert(res4.size() == 14);

    // Ensure all strings in a valid result have length 2n and are balanced
    for (const auto& s : res4) {
        assert(s.length() == 8);
        int bal = 0;
        for (char c : s) {
            if (c == '(') bal++;
            else bal--;
            assert(bal >= 0);
        }
        assert(bal == 0);
    }

    // Test negative input
    auto resNeg = generateBalancedParentheses(-3);
    assert(resNeg.empty());

    // Test n = 5 (Catalan number C5 = 42) for upper bound sanity
    auto res5 = generateBalancedParentheses(5);
    assert(res5.size() == 42);

    return 0;
}
