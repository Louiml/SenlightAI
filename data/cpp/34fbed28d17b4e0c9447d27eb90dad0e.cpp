Write a C++ function named `monitorQuery` that simulates the behavior of the monitor query logic from the provided code snippet. The function should take a string representing a monitor name (we ignore its content), a vector of `double` values representing the historical data points of that monitor, and a vector of constraint expressions (each a string like `"value > 5"` or `"value <= 10"`) plus a parallel vector of boolean flags indicating whether an action should be executed when the constraint is satisfied. For each constraint, the function must evaluate it against the **last** data point in the vector (simulating the monitor's current value) and count how many constraints are satisfied AND have their associated action flag set to `true`. The function returns that count. Constraints are only simple comparisons between the literal `value` and a numeric constant, using operators `<`, `<=`, `>`, `>=`, `==`, `!=`. If the data vector is empty, the function should return 0. If a constraint is malformed or uses an unsupported operator, skip it (do not count it). Provide a reference implementation that parses the constraint strings manually without using external libraries beyond standard C++.

#include <cassert>
#include <vector>
#include <string>

// Declaration of the solution function (assumed to be in the same translation unit).
int monitorQuery(const std::string&, const std::vector<double>&, const std::vector<std::string>&, const std::vector<bool>&);

int main() {
    // Basic case with mixed constraints.
    std::vector<double> data1 = {1.0, 2.0, 3.0, 7.0};
    std::vector<std::string> cons1 = {"value > 5", "value <= 10", "value == 7", "value != 7"};
    std::vector<bool> acts1 = {true, false, true, true};
    assert(monitorQuery("m", data1, cons1, acts1) == 2); // "value > 5" and "value == 7" pass and have action true.

    // Empty data returns 0.
    assert(monitorQuery("m", {}, cons1, acts1) == 0);

    // Malformed constraints skipped.
    std::vector<std::string> cons2 = {"value > 5", "value < abc", "x > 3", "value >"};
    std::vector<bool> acts2 = {true, true, true, true};
    assert(monitorQuery("m", {10.0}, cons2, acts2) == 1); // Only first is valid and satisfied.

    // Operator with no spaces.
    std::vector<std::string> cons3 = {"value>5", "value<=4"};
    std::vector<bool> acts3 = {true, false};
    assert(monitorQuery("m", {6.0}, cons3, acts3) == 1);

    // Negative and decimal numbers.
    std::vector<std::string> cons4 = {"value >= -3.5", "value < 0.5"};
    std::vector<bool> acts4 = {true, true};
    assert(monitorQuery("m", {-2.0}, cons4, acts4) == 2);

    // Constraint satisfied but action disabled.
    std::vector<std::string> cons5 = {"value == 3"};
    std::vector<bool> acts5 = {false};
    assert(monitorQuery("m", {3.0}, cons5, acts5) == 0);

    // Mismatched sizes returns 0.
    assert(monitorQuery("m", {1.0}, {"value > 0"}, {}) == 0);
}

#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <cstdlib>

// Trims leading and trailing whitespace from a string.
static std::string trim(const std::string& s) {
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) ++start;
    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end-1]))) --end;
    return s.substr(start, end - start);
}

// Counts how many constraints are satisfied by the last data point and have action enabled.
int monitorQuery(const std::string& /* monitorName */,
                 const std::vector<double>& data,
                 const std::vector<std::string>& constraints,
                 const std::vector<bool>& actions) {
    if (data.empty() || constraints.size() != actions.size()) return 0;
    double currentValue = data.back();
    int satisfiedWithAction = 0;

    for (size_t i = 0; i < constraints.size(); ++i) {
        const std::string& expr = constraints[i];
        // Find the operator: check two-char operators first.
        size_t opPos = std::string::npos;
        std::string op;
        size_t numStart = 0;

        const char* twoCharOps[] = {"<=", ">=", "==", "!="};
        for (const char* oc : twoCharOps) {
            size_t p = expr.find(oc);
            if (p != std::string::npos) {
                opPos = p;
                op = oc;
                break;
            }
        }
        if (opPos == std::string::npos) {
            // Try single-char operators.
            size_t pLess = expr.find('<');
            size_t pGreater = expr.find('>');
            if (pLess != std::string::npos && pGreater != std::string::npos) {
                // Should not happen, but skip if ambiguous.
                continue;
            }
            if (pLess != std::string::npos) { opPos = pLess; op = "<"; }
            else if (pGreater != std::string::npos) { opPos = pGreater; op = ">"; }
            else continue;
        }

        // Extract left side and right side.
        std::string left = trim(expr.substr(0, opPos));
        size_t rightStart = opPos + op.size();
        while (rightStart < expr.size() && std::isspace(static_cast<unsigned char>(expr[rightStart]))) ++rightStart;
        std::string right = trim(expr.substr(rightStart));
        if (left != "value") continue;

        // Parse the numeric constant.
        char* endPtr = nullptr;
        double constant = std::strtod(right.c_str(), &endPtr);
        // Ensure the whole right string was parsed as a number.
        if (endPtr == right.c_str() || *endPtr != '\0') continue;

        bool satisfied = false;
        if (op == "<") satisfied = currentValue < constant;
        else if (op == "<=") satisfied = currentValue <= constant;
        else if (op == ">") satisfied = currentValue > constant;
        else if (op == ">=") satisfied = currentValue >= constant;
        else if (op == "==") satisfied = currentValue == constant;
        else if (op == "!=") satisfied = currentValue != constant;

        if (satisfied && actions[i]) ++satisfiedWithAction;
    }
    return satisfiedWithAction;
}

// The solution needs to parse each constraint string and evaluate it against the last element of the data vector. The parsing algorithm: for each constraint string, we can locate the operator by searching for the first occurrence of any of the six comparison operators (`<`, `<=`, `>`, `>=`, `==`, `!=`). Since `<=`, `>=`, `==`, `!=` are two-character operators, we should check for two-character operators first, then fall back to single-character ones. Before the operator, we expect the literal substring `"value"` (ignoring leading/trailing spaces). After the operator, we parse a double using `std::stod`. If the prefix trimmed is not exactly `"value"`, or if parsing the number fails, we skip the constraint. For each valid constraint, we compare the last data value against the parsed constant using the operator and, if true and the action flag is true, increment the count. Edge cases: empty data vector → return 0 immediately. Multiple spaces around the operator and value — we trim the prefix and suffix. Malformed constraints (e.g., missing operator, bad number) are skipped. The time complexity is O(C * L) where C is the number of constraints and L is the average length of a constraint string (for parsing), plus O(1) per comparison. Space complexity is O(1) auxiliary beyond input storage.
