// Write a C++ function `simplifyRedundantNegations` that takes a vector of strings representing a sequence of boolean expressions, where each string is either "true", "false", or "NOT <expr>" (with `<expr>` being a previously defined expression or a literal). The function should return a new vector where each expression is simplified by removing redundant double negations (i.e., "NOT NOT X" becomes "X") wherever possible, processing expressions left to right. A double negation is only simplified if the inner expression's first token is "NOT" and its operand is a single identifier (no spaces), such as "NOT NOT x" → "x". Do not simplify "NOT NOT (x)" or expressions with spaces inside parentheses. Each input expression uses spaces only between tokens (e.g., "NOT NOT true"). The output should preserve the original order and all other expressions unchanged.

// The task requires left-to-right processing of each expression independently (no cross-expression dependency). For each string, we need to detect the pattern "NOT NOT <operand>" where `<operand>` is a single non-empty token without spaces (e.g., "true", "false", or a variable name like "x"). The simplification replaces "NOT NOT <operand>" with just "<operand>". Edge cases: the pattern must be exactly two "NOT" tokens followed by one operand token, with exactly one space between each. "NOT NOT" alone, "NOT NOT NOT x" (only first two NOTs pair), or "NOT NOT (x)" (operand with parentheses) should not be simplified. We can split the input string by spaces. If the first token is "NOT", second is "NOT", and there are exactly three tokens total (i.e., the operand is a single token without internal spaces), then return the third token. Otherwise, return the original string. For each of the \(n\) input strings, splitting and checking takes \(O(m)\) time where \(m\) is the string length. Overall time is \(O(n \cdot m)\), space is \(O(n \cdot m)\) for the output vector.

#include <vector>
#include <string>
#include <sstream>
#include <cassert>

// Simplify expressions of the form "NOT NOT X" to "X", where X is a single token.
std::vector<std::string> simplifyRedundantNegations(const std::vector<std::string>& expressions) {
    std::vector<std::string> result;
    result.reserve(expressions.size());
    
    for (const auto& expr : expressions) {
        std::istringstream iss(expr);
        std::string first, second, third;
        if (iss >> first >> second >> third) {
            // Check if there is any extra token remaining after the first three.
            std::string extra;
            if (!(iss >> extra) && first == "NOT" && second == "NOT") {
                // Exactly three tokens: "NOT NOT X" -> output "X".
                result.push_back(third);
                continue;
            }
        }
        // Either not enough tokens, too many tokens, or pattern doesn't match.
        result.push_back(expr);
    }
    return result;
}

#include <vector>
#include <string>
#include <cassert>

std::vector<std::string> simplifyRedundantNegations(const std::vector<std::string>& expressions);

int main() {
    // Basic double negation of literals and variables.
    assert(simplifyRedundantNegations({"NOT NOT true"}) == std::vector<std::string>{"true"});
    assert(simplifyRedundantNegations({"NOT NOT false"}) == std::vector<std::string>{"false"});
    assert(simplifyRedundantNegations({"NOT NOT x"}) == std::vector<std::string>{"x"});
    
    // Single negation and literals unchanged.
    assert(simplifyRedundantNegations({"NOT x", "true"}) == (std::vector<std::string>{"NOT x", "true"}));
    
    // Triple negation: only the first two NOTs are redundant? Actually "NOT NOT NOT x" has three tokens after NOT NOT? The operand is "NOT x" which contains a space, so should not simplify.
    assert(simplifyRedundantNegations({"NOT NOT NOT x"}) == (std::vector<std::string>{"NOT NOT NOT x"}));
    
    // Operand with spaces (e.g., "NOT NOT (x)" or "NOT NOT ( x )") should not simplify.
    assert(simplifyRedundantNegations({"NOT NOT (x)"}) == (std::vector<std::string>{"NOT NOT (x)"}));
    assert(simplifyRedundantNegations({"NOT NOT ( x )"}) == (std::vector<std::string>{"NOT NOT ( x )"}));
    
    // Empty string (if any unexpected) passes through unchanged.
    assert(simplifyRedundantNegations({""}) == (std::vector<std::string>{""}));
    
    // Mixed sequence.
    assert(simplifyRedundantNegations({"NOT NOT a", "b", "NOT NOT NOT c"}) == (std::vector<std::string>{"a", "b", "NOT NOT NOT c"}));
    
    // Multiple items in sequence.
    assert(simplifyRedundantNegations({"NOT NOT p", "NOT NOT q", "NOT r"}) == (std::vector<std::string>{"p", "q", "NOT r"}));
    
    return 0;
}
