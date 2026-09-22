// Implement a C++ function that takes a string containing a logical constraint expression and a set of variable-value pairs, and evaluates whether the constraint is satisfied. The constraint expression uses the following grammar: it may be a boolean literal (`true` or `false`), a comparison between a variable name and an integer (`variable == value`, `variable != value`, `variable < value`, `variable <= value`, `variable > value`, `variable >= value`), or a combination of two sub-expressions using logical operators (`&&` for AND, `||` for OR) with optional parentheses for grouping. Whitespace may appear anywhere but not inside variable names or integer literals. Variable names consist of letters and underscores only. The input string is always syntactically valid. The function takes a `std::unordered_map<std::string, int>` mapping variable names to their current integer values and returns `true` if the expression evaluates to true, `false` otherwise. The expression may reference variables not present in the map; such variables are treated as having a value of 0. An empty or whitespace-only constraint string evaluates to `true`.
#include <cassert>
#include <string>
#include <unordered_map>

// Forward declaration of the solution function
bool evaluateConstraint(const std::string& expr, const std::unordered_map<std::string, int>& vars);

int main() {
    std::unordered_map<std::string, int> vars;
    vars["x"] = 10;
    vars["y"] = -5;

    // Basic literals
    assert(evaluateConstraint("", vars) == true);
    assert(evaluateConstraint("   ", vars) == true);
    assert(evaluateConstraint("true", vars) == true);
    assert(evaluateConstraint("false", vars) == false);

    // Simple comparisons
    assert(evaluateConstraint("x == 10", vars) == true);
    assert(evaluateConstraint("x != 10", vars) == false);
    assert(evaluateConstraint("y < 0", vars) == true);
    assert(evaluateConstraint("y >= -5", vars) == true);
    assert(evaluateConstraint("x > 5", vars) == true);
    assert(evaluateConstraint("x <= 9", vars) == false);

    // Unknown variables default to 0
    assert(evaluateConstraint("z == 0", vars) == true);
    assert(evaluateConstraint("z != 0", vars) == false);
    assert(evaluateConstraint("z < 1", vars) == true);

    // Logical combinations and parentheses
    assert(evaluateConstraint("x > 5 && y < 0", vars) == true);
    assert(evaluateConstraint("x > 5 || y > 0", vars) == true);
    assert(evaluateConstraint("(x > 5 && y < 0) || false", vars) == true);
    assert(evaluateConstraint("!(x == 10)", vars) == false); // Note: '!' not supported, so this is invalid
    // But we have && and || with precedence
    assert(evaluateConstraint("x == 10 || y == 0 && false", vars) == true);
    assert(evaluateConstraint("(x == 10 && false) || (y == -5)", vars) == true);

    // Whitespace tolerance
    assert(evaluateConstraint("  x   ==   10  ", vars) == true);
    assert(evaluateConstraint("  (  x  <  20  )  &&  y  !=  0  ", vars) == true);

    // Negative numbers in comparisons
    assert(evaluateConstraint("y == -5", vars) == true);
    assert(evaluateConstraint("x < -1", vars) == false);
    assert(evaluateConstraint("y > -10", vars) == true);

    return 0;
}
#include <string>
#include <unordered_map>
#include <cctype>
#include <cstdlib>

// Evaluate a logical constraint expression with variable comparisons.
// Grammar: expression ::= term ('||' term)*; term ::= factor ('&&' factor)*;
// factor ::= '(' expression ')' | boolean | comparison;
// boolean ::= 'true' | 'false'; comparison ::= identifier op integer;
// op ::= '==' | '!=' | '<' | '<=' | '>' | '>=';
// Missing variables are treated as 0. Empty/whitespace expression returns true.
bool evaluateConstraint(const std::string& expr, const std::unordered_map<std::string, int>& vars) {
    size_t pos = 0;
    const size_t len = expr.size();

    // Helper to skip whitespace
    auto skipWS = [&]() {
        while (pos < len && std::isspace(static_cast<unsigned char>(expr[pos]))) {
            ++pos;
        }
    };

    // Parse an integer (supports negative numbers)
    auto parseInteger = [&]() -> int {
        skipWS();
        bool negative = false;
        if (pos < len && expr[pos] == '-') {
            negative = true;
            ++pos;
        }
        int value = 0;
        while (pos < len && std::isdigit(static_cast<unsigned char>(expr[pos]))) {
            value = value * 10 + (expr[pos] - '0');
            ++pos;
        }
        return negative ? -value : value;
    };

    // Parse an identifier (letters and underscores)
    auto parseIdentifier = [&]() -> std::string {
        skipWS();
        std::string id;
        while (pos < len && (std::isalnum(static_cast<unsigned char>(expr[pos])) || expr[pos] == '_')) {
            id.push_back(expr[pos]);
            ++pos;
        }
        return id;
    };

    // Parse a boolean literal
    auto parseBoolean = [&]() -> bool {
        skipWS();
        if (expr.compare(pos, 4, "true") == 0) {
            pos += 4;
            return true;
        } else if (expr.compare(pos, 5, "false") == 0) {
            pos += 5;
            return false;
        }
        // Should not happen for valid input; return false
        return false;
    };

    // Forward declaration for nested parsing
    std::function<bool()> parseExpression;

    // Parse a factor: parentheses, literal, or comparison
    auto parseFactor = [&]() -> bool {
        skipWS();
        if (pos < len && expr[pos] == '(') {
            ++pos; // consume '('
            bool result = parseExpression();
            skipWS();
            if (pos < len && expr[pos] == ')') {
                ++pos; // consume ')'
            }
            return result;
        }

        // Check for true/false first
        skipWS();
        if (expr.compare(pos, 4, "true") == 0 || expr.compare(pos, 5, "false") == 0) {
            return parseBoolean();
        }

        // Otherwise it's a comparison: identifier op integer
        std::string id = parseIdentifier();
        int varValue = vars.count(id) ? vars.at(id) : 0;

        skipWS();
        // Parse operator (check two-character first)
        std::string op;
        if (pos + 1 < len) {
            std::string two(expr.substr(pos, 2));
            if (two == "==" || two == "!=" || two == "<=" || two == ">=") {
                op = two;
                pos += 2;
            }
        }
        if (op.empty() && pos < len) {
            char c = expr[pos];
            if (c == '<' || c == '>') {
                op = std::string(1, c);
                ++pos;
            }
        }

        int rhs = parseInteger();

        if (op == "==") return varValue == rhs;
        if (op == "!=") return varValue != rhs;
        if (op == "<")  return varValue < rhs;
        if (op == "<=") return varValue <= rhs;
        if (op == ">")  return varValue > rhs;
        if (op == ">=") return varValue >= rhs;
        return false; // should not reach here
    };

    // Parse a term (AND)
    auto parseTerm = [&]() -> bool {
        bool result = parseFactor();
        while (true) {
            skipWS();
            if (pos + 1 < len && expr[pos] == '&' && expr[pos+1] == '&') {
                pos += 2;
                bool rhs = parseFactor();
                result = result && rhs;
            } else {
                break;
            }
        }
        return result;
    };

    // Parse expression (OR)
    parseExpression = [&]() -> bool {
        bool result = parseTerm();
        while (true) {
            skipWS();
            if (pos + 1 < len && expr[pos] == '|' && expr[pos+1] == '|') {
                pos += 2;
                bool rhs = parseTerm();
                result = result || rhs;
            } else {
                break;
            }
        }
        return result;
    };

    // Handle empty/whitespace-only
    skipWS();
    if (pos >= len) {
        return true;
    }

    return parseExpression();
}
// The solution is built around recursive descent parsing, a standard technique for evaluating expressions with a small, well-defined grammar. The grammar is: 
// - `expression` := `term` (`||` `term`)*
// - `term` := `factor` (`&&` `factor`)*
// - `factor` := `(` expression `)` | `boolean` | `comparison`
// - `boolean` := `true` | `false`
// - `comparison` := `identifier` `operator` integer, where operator is one of `==`, `!=`, `<`, `<=`, `>`, `>=`
//
// The parser maintains an index into the input string and uses helper functions to skip whitespace, parse identifiers, integers, and operators. The key algorithmic steps:
// 1. If the string is empty or all whitespace, return `true` immediately.
// 2. Parse the expression recursively: for `||`, start with parsing a term, then while the current token is `||`, parse another term and combine with logical OR. Similarly for `&&` within a term.
// 3. For a factor: if the next non-whitespace character is `(`, parse the nested expression and expect `)`. Otherwise, check if the token is `true` or `false` (case-sensitive) and return that boolean. Otherwise, parse an identifier, then an operator, then an integer. Look up the variable in the map (defaulting to 0), compare with the integer using the operator, and return the result.
// 4. Edge cases: unknown variables default to 0; comparisons like `x == 0` with no map entry will be true if x absent; operator parsing must handle two-character operators first (`==`, `!=`, `<=`, `>=`) and then one-character `<` and `>`; integer parsing must handle negative numbers (leading `-`). The parser must correctly skip whitespace between tokens but not inside identifiers or numbers.
// 5. Time complexity is O(n) where n is the length of the input string, as each character is processed at most a constant number of times by the recursive descent. Space complexity is O(d) where d is the depth of parentheses (worst-case O(n) for deeply nested expressions) plus the input string storage.
