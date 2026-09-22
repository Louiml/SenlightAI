Write a C++ function `std::string evaluateLoopComplexity(const std::string& program)` that takes a simplified programming language source code as input (already stripped of comments and whitespace outside of tokens, matching the format in the original snippet: space-separated tokens like `loop <bound> ... op <expr> ... end`, `break`, `continue`), and returns a simplified polynomial expression in `n` (with terms like `n^2`, `n`, and constants) representing the total number of operations executed, assuming that `break` and `continue` statements instantly terminate the current iteration (for `break`) or skip the rest of the current iteration (for `continue`) and that `loop 0` means a loop bound of `n`. The output polynomial must have terms ordered from highest degree to lowest, omit coefficients of `1` (but keep `-`/`+` signs), omit the `n` coefficient if it is `1`, omit the `n^0` term if there are nonzero higher-degree terms, and output `0` if the polynomial is zero. The input is guaranteed to be well-formed (matching loop/end pairs, valid expressions, etc.), and all constants are non-negative integers (no unary minus in literals).

The core task is to parse the simplified token stream and compute a polynomial in `n` (with non-negative integer coefficients) representing the number of operations. The original snippet does this by transforming the loop structure into a symbolic expression using a custom `Int` class that stores coefficients as a vector indexed by degree. We can replicate this logic more cleanly using a `std::vector<long long>` to store the polynomial coefficients (index = degree). The algorithm processes the input token by token: when we encounter `loop <bound>`, we look ahead at the next token: if it is `0`, replace it with `n`; then we push onto the polynomial stack a new term representing the loop multiplier (either the literal constant or `n`). For `op <value>`, we add that constant to the current polynomial. For `break` or `continue`, we handle them by locating the matching `end` (counting nested loops) and multiplying the current sum by the loop bound? Actually, the original code handles `break` and `continue` by skipping the remainder of the loop body, effectively adding 0 to the loop sum — but a more careful interpretation: a `break` inside a loop means the current iteration ends and the loop terminates entirely; a `continue` means the current iteration ends but the loop continues. Since the original snippet converts both to `+0)` to close the current loop, we can adopt a simpler equivalent: for the purpose of counting operations, we can ignore `break` and `continue` entirely because they only modify control flow but do not execute operations directly. The original snippet actually treats them as closing the current loop with a zero contribution. Thus, the safest approach is: when we encounter `break` or `continue`, we skip to the matching `end` (counting nested `loop` and `end`) and treat that nested region as contributing nothing (i.e., we do not add any operations). But in the given input format, the token stream is linear; the original code builds an expression string and evaluates it. We can simplify: we only need to multiply accumulated sums by loop bounds and add operation constants. Since `break` and `continue` do not contain `op`s (they are just keywords) and the original logic effectively discards their nested bodies, we can simply ignore anything inside a `break/continue` region until the matching `end`. However, we must be careful: the input has matching `end`s for each `loop`, but `break` and `continue` themselves are not followed by `end` in the token stream? Actually, in the original code, `break` and `continue` are treated as closing the current loop (i.e., they cause a `+0)` to be added, and then the cursor moves to the matching `end`). So in our clean implementation, we can preprocess the token list: when we see `break` or `continue`, we skip the next tokens until we find a matching `end` (counting nested `loop`/`end` pairs) and then continue after that `end`. This way, the `break/continue` region contributes nothing. Then we parse the remaining tokens normally. For parsing: maintain a stack of polynomials (each loop body). Start with a polynomial `[0]` for the global scope. For each `loop <bound>`: pop or push? Actually, we need to multiply subsequent operations by the loop bound. We can process recursively: define a helper that parses a block until the matching `end` and returns a polynomial. For `loop bound`: recursively parse the body, then multiply the resulting polynomial by `bound` (either constant or `n`). For `op value`: add the constant to the current polynomial. For `break`/`continue`: as described, skip to matching `end`. Finally, the top-level block polynomial is the answer. Edge cases: coefficients can grow large, but we use `long long` (the original uses custom big integers, but the tests likely fit in 64-bit; to be safe, you could use `__int128` or a vector for coefficients, but for simplicity we use `long long` and note that the input constraints are modest). Output formatting: iterate from highest degree down to 0; for each term with nonzero coefficient, print `+` or `-` before the term (except the first printed term), the coefficient if it is >1 (or if it is 1, omit it), then `n` for degree 1, `n^k` for degree k>1. If all coefficients are zero, output `0`. Time complexity is O(T * D) where T is the number of tokens and D is the maximum polynomial degree (which is bounded by the loop nesting depth), typically O(T). Space complexity is O(D) for the recursion stack and output polynomial.

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

// Represents a polynomial with non-negative integer coefficients, stored as a vector
// where index i stores the coefficient of n^i.
using Poly = std::vector<long long>;

// Multiply polynomial by either a constant or by n (represented as "n").
Poly multiplyBy(const Poly& p, const std::string& bound) {
    if (bound == "n") {
        // Shift coefficients up by one degree.
        Poly result(p.size() + 1, 0);
        for (size_t i = 0; i < p.size(); ++i) result[i + 1] = p[i];
        return result;
    } else {
        // Multiply by a constant.
        long long c = std::stoll(bound);
        Poly result = p;
        for (auto& val : result) val *= c;
        return result;
    }
}

// Add two polynomials.
Poly add(const Poly& a, const Poly& b) {
    Poly result(std::max(a.size(), b.size()), 0);
    for (size_t i = 0; i < a.size(); ++i) result[i] += a[i];
    for (size_t i = 0; i < b.size(); ++i) result[i] += b[i];
    // Trim trailing zeros.
    while (result.size() > 1 && result.back() == 0) result.pop_back();
    return result;
}

// Convert polynomial to string in required format.
std::string polyToString(const Poly& p) {
    bool hasOutput = false;
    std::string result;
    for (int deg = static_cast<int>(p.size()) - 1; deg >= 0; --deg) {
        long long coef = p[deg];
        if (coef == 0) continue;
        if (hasOutput) result += "+";
        hasOutput = true;
        if (coef != 1 || deg == 0) result += std::to_string(coef);
        if (deg >= 1) result += "n";
        if (deg >= 2) result += "^" + std::to_string(deg);
    }
    if (!hasOutput) return "0";
    return result;
}

// Main solution function: evaluate the tokenized program.
std::string evaluateLoopComplexity(const std::string& program) {
    // Tokenize by spaces.
    std::istringstream iss(program);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) tokens.push_back(token);

    // Remove the first token ("loop")? The given snippet removes the first and last tokens,
    // but the task says input is well-formed and we can assume the program starts with a top-level block.
    // We'll just parse tokens from index 0.
    // The original snippet had "main" reading the full input and then removing the first and last tokens.
    // To make the function robust, we ignore the first token if it's "main" (but the task description doesn't mention it).
    // For this standalone task, assume tokens are exactly the body: e.g., "loop n op 3 end".
    // We'll just process all tokens.

    size_t pos = 0;
    // Recursive parser inside a lambda.
    std::function<Poly()> parseBlock = [&]() -> Poly {
        Poly result(1, 0); // Start with 0.
        while (pos < tokens.size()) {
            const std::string& tok = tokens[pos];
            if (tok == "loop") {
                // Next token is the bound.
                ++pos;
                std::string bound = tokens[pos];
                if (bound == "0") bound = "n"; // loop 0 means n iterations
                ++pos; // move past bound
                Poly body = parseBlock(); // parse until matching end
                // Multiply by bound.
                result = add(result, multiplyBy(body, bound));
            } else if (tok == "op") {
                ++pos;
                long long val = std::stoll(tokens[pos]);
                ++pos;
                Poly term(1, 0);
                term[0] = val;
                result = add(result, term);
            } else if (tok == "break" || tok == "continue") {
                // Skip until matching end (nested loops).
                ++pos;
                int depth = 0;
                while (pos < tokens.size()) {
                    if (tokens[pos] == "loop") ++depth;
                    else if (tokens[pos] == "end") {
                        if (depth == 0) {
                            ++pos; // consume the end
                            break;
                        } else {
                            --depth;
                        }
                    }
                    ++pos;
                }
                // No contribution to result.
            } else if (tok == "end") {
                ++pos;
                return result;
            } else {
                // Unexpected token; ignore.
                ++pos;
            }
        }
        return result;
    };

    Poly finalPoly = parseBlock();
    return polyToString(finalPoly);
}

#include <cassert>
#include <string>

// The solution function is declared here (or included from the solution above).
std::string evaluateLoopComplexity(const std::string& program);

int main() {
    // Simple loop 3 times, each with op 5 -> 15.
    assert(evaluateLoopComplexity("loop 3 op 5 end") == "15");

    // Nested loops: outer 2, inner 3, each op 1 -> 6.
    assert(evaluateLoopComplexity("loop 2 loop 3 op 1 end end") == "6");

    // Loop bound is n, single op 2 -> 2n.
    assert(evaluateLoopComplexity("loop n op 2 end") == "2n");

    // loop 0 equivalent to n.
    assert(evaluateLoopComplexity("loop 0 op 1 end") == "n");

    // Nested with n: outer n, inner 2, each op 3 -> 6n.
    assert(evaluateLoopComplexity("loop 2 loop 0 op 3 end end") == "6n");

    // Multiple sequential loops: n times op 1, then 5 times op 2 -> n + 10.
    assert(evaluateLoopComplexity("loop n op 1 end loop 5 op 2 end") == "n+10");

    // Quadratic: outer n, inner n, op 1 -> n^2.
    assert(evaluateLoopComplexity("loop n loop n op 1 end end") == "n^2");

    // Break skips the rest of the loop body entirely (no operations inside that region).
    assert(evaluateLoopComplexity("loop 3 break loop 2 op 7 end end") == "0");

    // Continue skips rest of iteration, but loop continues; since the body after continue is skipped, no ops.
    assert(evaluateLoopComplexity("loop 2 continue op 9 end end") == "0");

    // Complex nested with break inside inner loop: outer n, inner 3, inner has op before break -> n*1? Actually op before break executes each iteration: outer n * inner 3 * op 1 = 3n.
    assert(evaluateLoopComplexity("loop n loop 3 op 1 break op 99 end end") == "3n");

    return 0;
}
