// Write a C++ function named `evaluateExpression` that takes three string parameters: a function name, an expression, and an input string. The function should mimic the behavior of the provided CLI, returning a string result. If the function name is `"-a"` or `"-a"` (where the first character is U+2212 minus sign or ASCII hyphen), treat the expression as an arithmetic expression where the input string contains a variable assignment in the form `name=value`, compute and return the numeric result as a string (you may assume the expression contains only `+`, `-`, `*`, `/`, parentheses, the variable name, and integer literals). If the function name is `"-s"` or `"-s"`, treat the expression as a boolean satisfaction problem where the input provides assignments for boolean variables in the form `var=0` or `var=1`, and return `"1"` followed by a space and the satisfying assignment (list all variables in alphabetical order with their values) if the expression evaluates to true, otherwise return `"0"`. If the function name is anything else, or if the expression or input is malformed (e.g., undefined variable, division by zero, invalid syntax), return the phrase `"The parameters entered are invalid."`. Only literal integer operands (for arithmetic) or boolean literals `0`/`1` and single-letter variable names are allowed. You may implement a simple recursive descent parser and evaluator. The function must be stand-alone (no external libraries beyond standard headers) and must not use global state.

// The solution requires parsing two types of expressions. For the arithmetic case, we need to parse the expression string by first locating the variable assignment from the input (split at `=`), then replacing the variable name in the expression with its integer value. After that, we can evaluate the expression using a recursive descent parser that handles `+`, `-`, `*`, `/`, parentheses, and integer literals. We must check for division by zero and any syntax errors (e.g., unexpected characters, unbalanced parentheses) and throw an exception or return an error indicator. For the boolean satisfaction case, we parse the input to extract assignments for variables, then evaluate the expression using Boolean logic with operators `&`, `|`, `!`, parentheses, and boolean literals `0`/`1` and variable names. If all variables used in the expression have assignments and the expression evaluates to true, we collect all variables (from both expression and input) and output them sorted alphabetically with their values. If an undefined variable is used or expression is syntactically invalid, we return the error message. To keep the function simple, we can wrap parsing and evaluation in helper functions that throw `std::runtime_error` on any error. Time complexity is O(n) for parsing and evaluation where n is the expression length; space complexity is O(n) due to recursion stack and intermediate storage. Edge cases include malformed input (no `=` in input), missing variable assignments, division by zero, unknown function names, and empty expressions. We must also handle both Unicode minus (U+2212) and ASCII hyphen in function names.

#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <cctype>
#include <unordered_map>

// Helper: replace all occurrences of a substring.
static void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    size_t pos = 0;
    while ((pos = str.find(from, pos)) != std::string::npos) {
        str.replace(pos, from.length(), to);
        pos += to.length();
    }
}

// Arithmetic expression evaluator (recursive descent).
class ArithmeticParser {
public:
    explicit ArithmeticParser(const std::string& expr) : s(expr), idx(0) {}
    
    int parse() {
        int value = parseExpression();
        skipWhitespace();
        if (idx != s.size()) {
            throw std::runtime_error("Unexpected character");
        }
        return value;
    }
    
private:
    std::string s;
    size_t idx;
    
    void skipWhitespace() {
        while (idx < s.size() && std::isspace(s[idx])) idx++;
    }
    
    int parseExpression() {
        int value = parseTerm();
        while (true) {
            skipWhitespace();
            if (idx < s.size() && (s[idx] == '+' || s[idx] == '-')) {
                char op = s[idx++];
                int rhs = parseTerm();
                if (op == '+') value += rhs;
                else value -= rhs;
            } else {
                break;
            }
        }
        return value;
    }
    
    int parseTerm() {
        int value = parseFactor();
        while (true) {
            skipWhitespace();
            if (idx < s.size() && (s[idx] == '*' || s[idx] == '/')) {
                char op = s[idx++];
                int rhs = parseFactor();
                if (op == '*') value *= rhs;
                else {
                    if (rhs == 0) throw std::runtime_error("Division by zero");
                    value /= rhs;
                }
            } else {
                break;
            }
        }
        return value;
    }
    
    int parseFactor() {
        skipWhitespace();
        if (idx < s.size() && s[idx] == '-') {
            idx++;
            return -parseFactor();
        }
        if (idx < s.size() && s[idx] == '(') {
            idx++;
            int val = parseExpression();
            skipWhitespace();
            if (idx >= s.size() || s[idx] != ')') throw std::runtime_error("Missing ')'");
            idx++;
            return val;
        }
        // integer literal
        size_t start = idx;
        while (idx < s.size() && std::isdigit(s[idx])) idx++;
        if (start == idx) throw std::runtime_error("Invalid factor");
        return std::stoi(s.substr(start, idx - start));
    }
};

// Boolean expression evaluator (recursive descent).
class BooleanParser {
public:
    BooleanParser(const std::string& expr, const std::unordered_map<char, bool>& vars)
        : s(expr), idx(0), vars(vars) {}
    
    bool parse() {
        bool value = parseOr();
        skipWhitespace();
        if (idx != s.size()) throw std::runtime_error("Unexpected character");
        return value;
    }
    
private:
    std::string s;
    size_t idx;
    const std::unordered_map<char, bool>& vars;
    
    void skipWhitespace() {
        while (idx < s.size() && std::isspace(s[idx])) idx++;
    }
    
    bool parseOr() {
        bool value = parseAnd();
        while (true) {
            skipWhitespace();
            if (idx < s.size() && s[idx] == '|') {
                idx++;
                // expect another '|'
                if (idx >= s.size() || s[idx] != '|') throw std::runtime_error("Expected '|'");
                idx++;
                bool rhs = parseAnd();
                value = value || rhs;
            } else {
                break;
            }
        }
        return value;
    }
    
    bool parseAnd() {
        bool value = parseNot();
        while (true) {
            skipWhitespace();
            if (idx < s.size() && s[idx] == '&') {
                idx++;
                if (idx >= s.size() || s[idx] != '&') throw std::runtime_error("Expected '&'");
                idx++;
                bool rhs = parseNot();
                value = value && rhs;
            } else {
                break;
            }
        }
        return value;
    }
    
    bool parseNot() {
        skipWhitespace();
        if (idx < s.size() && s[idx] == '!') {
            idx++;
            return !parseNot();
        }
        if (idx < s.size() && s[idx] == '(') {
            idx++;
            bool val = parseOr();
            skipWhitespace();
            if (idx >= s.size() || s[idx] != ')') throw std::runtime_error("Missing ')'");
            idx++;
            return val;
        }
        if (idx < s.size() && (s[idx] == '0' || s[idx] == '1')) {
            bool val = (s[idx] == '1');
            idx++;
            return val;
        }
        if (idx < s.size() && std::isalpha(s[idx])) {
            char var = s[idx];
            idx++;
            if (vars.find(var) == vars.end()) throw std::runtime_error("Undefined variable");
            return vars.at(var);
        }
        throw std::runtime_error("Invalid factor");
    }
};

// Main function implementing the task.
std::string evaluateExpression(const std::string& function, const std::string& expressionBase, const std::string& input) {
    // Normalize function name (handle Unicode minus sign and ASCII hyphen).
    std::string func = function;
    // Replace U+2212 with ASCII hyphen for simplicity.
    for (char& c : func) {
        if (static_cast<unsigned char>(c) == 0xE2) {
            // Handle multi-byte: could be U+2212 encoded as 3 bytes.
            // This is simplified: we'll just check equality with both forms later.
        }
    }
    // Since exact handling of Unicode is tricky, we'll compare both forms directly.
    bool isArithmetic = (function == "-a" || function == "\u2212a" ||
                         function == "-a" || function == "\u2212a");
    bool isSatisfaction = (function == "-s" || function == "\u2212s" ||
                           function == "-s" || function == "\u2212s");
    
    if (!isArithmetic && !isSatisfaction) {
        return "The parameters entered are invalid.";
    }
    
    try {
        if (isArithmetic) {
            // Parse input for variable assignment.
            auto eqPos = input.find('=');
            if (eqPos == std::string::npos) throw std::runtime_error("Invalid input");
            std::string varName = input.substr(0, eqPos);
            if (varName.empty()) throw std::runtime_error("Empty variable name");
            // Variable name should be a single alphabetic character.
            if (varName.size() != 1 || !std::isalpha(varName[0])) throw std::runtime_error("Invalid variable");
            int varValue = std::stoi(input.substr(eqPos + 1));
            
            // Replace variable name in expression.
            std::string expr = expressionBase;
            replaceAll(expr, varName, std::to_string(varValue));
            
            ArithmeticParser parser(expr);
            int result = parser.parse();
            return std::to_string(result);
        } else {
            // Parse input for boolean variable assignments.
            std::unordered_map<char, bool> assignedVars;
            std::istringstream iss(input);
            std::string token;
            while (iss >> token) {
                auto eqPos = token.find('=');
                if (eqPos == std::string::npos) throw std::runtime_error("Invalid assignment");
                std::string varName = token.substr(0, eqPos);
                if (varName.size() != 1 || !std::isalpha(varName[0])) throw std::runtime_error("Invalid variable");
                std::string valueStr = token.substr(eqPos + 1);
                if (valueStr != "0" && valueStr != "1") throw std::runtime_error("Invalid boolean");
                assignedVars[varName[0]] = (valueStr == "1");
            }
            
            BooleanParser parser(expressionBase, assignedVars);
            bool result = parser.parse();
            
            if (result) {
                // Collect all variables from expression and input.
                std::vector<char> usedVars;
                for (char ch : expressionBase) {
                    if (std::isalpha(ch)) {
                        if (std::find(usedVars.begin(), usedVars.end(), ch) == usedVars.end()) {
                            usedVars.push_back(ch);
                        }
                    }
                }
                for (const auto& kv : assignedVars) {
                    if (std::find(usedVars.begin(), usedVars.end(), kv.first) == usedVars.end()) {
                        usedVars.push_back(kv.first);
                    }
                }
                std::sort(usedVars.begin(), usedVars.end());
                
                std::string output = "1 ";
                for (char var : usedVars) {
                    output += var;
                    output += '=';
                    output += (assignedVars.count(var) && assignedVars.at(var)) ? '1' : '0';
                    output += ' ';
                }
                if (!usedVars.empty()) output.pop_back(); // remove trailing space
                return output;
            } else {
                return "0";
            }
        }
    } catch (...) {
        return "The parameters entered are invalid.";
    }
}

#include <cassert>
#include <string>

// Declare the function (from the solution).
std::string evaluateExpression(const std::string& function, const std::string& expressionBase, const std::string& input);

int main() {
    // Arithmetic tests.
    assert(evaluateExpression("-a", "x+2*3", "x=4") == "10");
    assert(evaluateExpression("-a", "(x-1)/2", "x=5") == "2");
    assert(evaluateExpression("-a", "x/0", "x=1") == "The parameters entered are invalid.");
    assert(evaluateExpression("-a", "x+y", "x=1") == "The parameters entered are invalid.");
    assert(evaluateExpression("-a", "2+", "x=1") == "The parameters entered are invalid.");
    assert(evaluateExpression("bad", "x+1", "x=2") == "The parameters entered are invalid.");
    
    // Satisfaction tests.
    assert(evaluateExpression("-s", "a&b", "a=1 b=1") == "1 a=1 b=1");
    assert(evaluateExpression("-s", "a|b", "a=0 b=0") == "0");
    assert(evaluateExpression("-s", "a|!b", "a=0 b=1") == "1 a=0 b=1");
    assert(evaluateExpression("-s", "c&a", "a=1 c=1") == "1 a=1 c=1");
    assert(evaluateExpression("-s", "a&c", "a=1") == "The parameters entered are invalid.");
    assert(evaluateExpression("-s", "(a&b)|c", "a=1 b=0 c=1") == "1 a=1 b=0 c=1");
    
    return 0;
}
