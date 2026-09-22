Given a valid arithmetic expression string that uses only non-negative integers and two binary operators: `$` defined as `a $ b = 3*a + b + 2`, and `#` defined as `a # b = 2*a + 3*b + 4`. The expression follows standard precedence rules where `$` has higher precedence than `#`, and both are left-associative. The expression contains no parentheses, no spaces, and at least one operator. Write a C++ function `int evaluateExpression(const std::string& expr)` that parses the expression and returns the integer result. The expression always has exactly one integer between operators, and every operator is surrounded by a non-negative integer on each side (no unary operators). The input may contain multi-digit integers. The result and all intermediate values fit within a 32-bit signed integer.

#include <cassert>
#include <string>

int evaluateExpression(const std::string& expr);

int main() {
    // Basic single operator
    assert(evaluateExpression("5$3") == 3*5 + 3 + 2); // 20
    assert(evaluateExpression("2#3") == 2*2 + 3*3 + 4); // 17

    // Precedence: $ before #
    // 2 $ 3 # 4 => (2$3) # 4 = 11 # 4 = 2*11 + 3*4 + 4 = 38
    assert(evaluateExpression("2$3#4") == 38);
    // 2 # 3 $ 4 => 2 # (3$4) = 2 # 14 = 2*2 + 3*14 + 4 = 50
    assert(evaluateExpression("2#3$4") == 50);

    // Left associativity for same precedence
    // 1$2$3 => (1$2)$3 = (3*1+2+2)=7 $ 3 = 3*7+3+2=26
    assert(evaluateExpression("1$2$3") == 26);
    // 1#2#3 => (1#2)#3 = (2*1+3*2+4)=12 # 3 = 2*12+3*3+4=37
    assert(evaluateExpression("1#2#3") == 37);

    // Mixed with multi-digit numbers
    // 10 $ 20 # 30 => (10$20) # 30 = (3*10+20+2=52) # 30 = 2*52+3*30+4=198
    assert(evaluateExpression("10$20#30") == 198);
    // 12#34$56 => 12 # (34$56) = 12 # (3*34+56+2=160) = 2*12+3*160+4=508
    assert(evaluateExpression("12#34$56") == 508);

    // Longer chain
    // 1$2#3$4#5 => (1$2) # (3$4) # 5? 
    // Because $ first: 1$2=7, 3$4=15, then 7#15#5 left-assoc: (7#15)=2*7+3*15+4=63, then 63#5=2*63+3*5+4=145
    assert(evaluateExpression("1$2#3$4#5") == 145);

    return 0;
}

#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>

// Evaluate an expression with operators $ (higher precedence) and # (lower precedence)
int evaluateExpression(const std::string& expr) {
    // Tokenize: numbers and operators
    std::vector<std::string> tokens;
    int i = 0;
    int n = expr.size();
    while (i < n) {
        if (isdigit(expr[i])) {
            int j = i;
            while (j < n && isdigit(expr[j])) ++j;
            tokens.push_back(expr.substr(i, j - i));
            i = j;
        } else { // operator
            tokens.push_back(std::string(1, expr[i]));
            ++i;
        }
    }

    // First pass: compute all $ operations
    std::vector<long long> pass1; // store numbers and '#'
    for (const auto& tok : tokens) {
        if (tok == "$") {
            long long x = pass1.back();
            pass1.pop_back();
            // next token must be a number
            long long y = std::stoll(tokens[++i]); // but we need index; better use iterator style
        }
    }
    // The above has a bug: need to track index in tokens; rewrite cleanly.
    // Correct implementation:
    std::vector<long long> temp;
    int idx = 0;
    while (idx < (int)tokens.size()) {
        if (tokens[idx] == "$") {
            long long x = temp.back();
            temp.pop_back();
            long long y = std::stoll(tokens[++idx]); // consume number after $
            temp.push_back(3 * x + y + 2);
        } else if (tokens[idx] == "#") {
            temp.push_back(-1); // placeholder for '#'
        } else {
            temp.push_back(std::stoll(tokens[idx]));
        }
        ++idx;
    }

    // Second pass: compute all # operations
    std::vector<long long> st;
    for (long long val : temp) {
        if (val == -1) { // it's '#'
            long long x = st.back();
            st.pop_back();
            // next value must be actual number, but we are iterating; need index
            // Let's use index-based again
        }
    }
    // To avoid complexity, use a simpler two-pass approach directly on tokens.
    // Alternative: use a stack of longs and process in two explicit passes with indices.
    // Let's implement cleanly.
    return 0; // placeholder; replaced below
}
The above solution is incomplete; a cleaner implementation is provided below (since the task requires a high-quality self-contained implementation). I will present a corrected, complete solution.

#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>

// Helper to tokenize the expression into numbers and operator characters
static std::vector<std::string> tokenize(const std::string& expr) {
    std::vector<std::string> tokens;
    int i = 0;
    int n = expr.size();
    while (i < n) {
        if (isdigit(expr[i])) {
            int j = i;
            while (j < n && isdigit(expr[j])) ++j;
            tokens.push_back(expr.substr(i, j - i));
            i = j;
        } else {
            tokens.push_back(std::string(1, expr[i]));
            ++i;
        }
    }
    return tokens;
}

// Evaluate the expression with precedence: $ higher than #
int evaluateExpression(const std::string& expr) {
    auto tokens = tokenize(expr);

    // Pass 1: Evaluate all $ operations, leaving only numbers and '#'.
    std::vector<long long> pass1;
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == "$") {
            long long x = pass1.back();
            pass1.pop_back();
            long long y = std::stoll(tokens[++i]); // next token is a number
            pass1.push_back(3 * x + y + 2);
        } else if (tokens[i] == "#") {
            // Use a sentinel value that cannot be a real number (all numbers are non-negative)
            pass1.push_back(-1); // sentinel for '#'
        } else {
            pass1.push_back(std::stoll(tokens[i]));
        }
    }

    // Pass 2: Evaluate all # operations left-to-right.
    std::vector<long long> pass2;
    for (size_t i = 0; i < pass1.size(); ++i) {
        if (pass1[i] == -1) { // it's '#'
            long long x = pass2.back();
            pass2.pop_back();
            long long y = pass1[++i]; // next must be a real number
            pass2.push_back(2 * x + 3 * y + 4);
        } else {
            pass2.push_back(pass1[i]);
        }
    }

    // The final result is the only remaining element
    return static_cast<int>(pass2.back());
}

// The core challenge is correctly applying operator precedence without parentheses. Since `$` has higher precedence than `#`, we must first evaluate all `$` subexpressions, then evaluate the remaining `#` operations left-to-right. A robust approach is to tokenize the string into a sequence of numbers and operators, then perform two passes over the token list using a vector as a stack. First pass: iterate through tokens; when encountering a `$`, pop the last number from the result vector, read the next token (which must be a number), compute `3*x + y + 2`, and push the result back; otherwise, push the token (number or `#`) onto the result vector. After this pass, the result vector contains only numbers and `#` operators. Second pass similar to the first but for `#`: when encountering a `#`, pop the last number, read the next number, compute `2*x + 3*y + 4`, and push the result; otherwise push the token. At the end, the remaining single element is the final answer. This works because the first pass effectively collapses every `$` operation into a single value, respecting left-associativity since we process tokens sequentially and immediately combine when we see `$`. Edge cases: leading or trailing spaces are not present; numbers are always non-negative; ensure the parsing correctly extracts multi-digit numbers. Time complexity is O(n) where n is the length of the string, and space complexity is O(n) for the token vector, but the auxiliary stack usage is O(number of tokens), which is O(n).
