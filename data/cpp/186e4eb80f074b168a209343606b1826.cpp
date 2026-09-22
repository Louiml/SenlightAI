// Write a C++ function `generateBalancedParentheses(int n)` that returns a vector of all strings of length `2n` consisting of `n` left parentheses `'('` and `n` right parentheses `')'` such that the string is a valid parenthesis sequence (every prefix has at least as many left as right parentheses, and the total counts are equal). The input `n` is a non-negative integer; for `n = 0`, return a vector containing a single empty string. The order of the generated strings in the output vector does not matter, but the function must be efficient enough for `n` up to 8 (the output size is the Catalan number, e.g., 1430 strings for `n = 8`).

#include <cassert>
#include <vector>
#include <string>
#include <algorithm>

// The solution function is declared above; include it here in a real project.
// For this test block, we assume the function is available.
// We include the implementation here for completeness (as part of the task it's separate).

int main() {
    // Test n = 0: single empty string
    {
        auto res = generateBalancedParentheses(0);
        assert(res.size() == 1);
        assert(res[0] == "");
    }

    // Test n = 1: only "()"
    {
        auto res = generateBalancedParentheses(1);
        assert(res.size() == 1);
        assert(res[0] == "()");
    }

    // Test n = 2: all strings, order independent
    {
        auto res = generateBalancedParentheses(2);
        std::vector<std::string> expected = {"(())", "()()"};
        std::sort(res.begin(), res.end());
        std::sort(expected.begin(), expected.end());
        assert(res == expected);
    }

    // Test n = 3: 5 strings
    {
        auto res = generateBalancedParentheses(3);
        assert(res.size() == 5);
        // Verify each is valid and length 6
        for (const auto& s : res) {
            assert(s.size() == 6);
            int balance = 0;
            for (char c : s) {
                assert(c == '(' || c == ')');
                if (c == '(') balance++;
                else balance--;
                assert(balance >= 0);
            }
            assert(balance == 0);
        }
        // Check that all strings are distinct
        std::vector<std::string> copy = res;
        std::sort(copy.begin(), copy.end());
        assert(std::unique(copy.begin(), copy.end()) == copy.end());
    }

    // Test n = 4: Catalan number 14
    {
        auto res = generateBalancedParentheses(4);
        assert(res.size() == 14);
        for (const auto& s : res) {
            assert(s.size() == 8);
            int balance = 0;
            for (char c : s) {
                if (c == '(') balance++;
                else balance--;
                assert(balance >= 0);
            }
            assert(balance == 0);
        }
    }

    // Test n = 8: Catalan number 1430
    {
        auto res = generateBalancedParentheses(8);
        assert(res.size() == 1430);
    }

    // Test invalid negative input (should be empty)
    {
        auto res = generateBalancedParentheses(-1);
        assert(res.empty());
    }

    return 0;
}

#include <vector>
#include <string>

// Generate all valid parentheses strings of length 2n (n pairs).
// Returns a vector of all valid combinations (order is not specified).
std::vector<std::string> generateBalancedParentheses(int n) {
    std::vector<std::string> result;
    if (n < 0) {
        return result; // invalid input, empty vector
    }
    if (n == 0) {
        result.push_back("");
        return result;
    }

    // Helper recursive function.
    // left: number of '(' placed so far
    // right: number of ')' placed so far
    // current: the partial string built so far
    auto backtrack = [&](auto&& self, int left, int right, const std::string& current) -> void {
        if (current.size() == static_cast<size_t>(2 * n)) {
            result.push_back(current);
            return;
        }
        if (left < n) {
            self(self, left + 1, right, current + '(');
        }
        if (right < left) {
            self(self, left, right + 1, current + ')');
        }
    };

    backtrack(backtrack, 0, 0, "");
    return result;
}

// The core algorithm is a depth-first search (backtracking) that builds the string character by character while maintaining two counters: `left` (number of `'('` placed so far) and `right` (number of `')'` placed so far). At each recursive step, we can add a left parenthesis as long as `left < n`, because we cannot exceed the maximum allowed count. We can add a right parenthesis as long as `right < left`, because every prefix must have at least as many left as right parentheses. When the current string length reaches `2n`, the base case is reached, and we push the completed string into the result vector. The key edge cases are `n = 0` (the empty string is valid) and `n = 1` (only `"()"`). The recursion explores the full tree of valid partial sequences; there are no invalid branches because the guards prevent illegal placements. Time complexity is \(O(C_n \cdot n)\) where \(C_n = \frac{1}{n+1}\binom{2n}{n}\) is the nth Catalan number (number of valid sequences), and each of the \(2n\) characters is copied into the result. Auxiliary space (excluding the output vector) is \(O(n)\) due to the maximum recursion depth of `2n`, plus the string `curr` that is passed by value (copied at each call), giving \(O(n)\) additional space per recursion level, so total recursion stack space is \(O(n^2)\) if counting copied strings; however, in practice this is fine for n ≤ 8.
