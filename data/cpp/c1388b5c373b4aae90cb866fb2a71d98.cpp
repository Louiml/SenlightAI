// Write a C++ function `bitStatementValue(int n, const std::vector<std::string>& statements)` that processes `n` bitwise statements, each a string of length 3 consisting of a variable name `X` and two operators from `{+, -}`, such as `"X++"`, `"++X"`, `"X--"`, or `"--X"`. Each statement increments or decrements the value of `X` by 1 (initially 0) depending on whether the operators are `++` (increment) or `--` (decrement). The operators always appear as a pair either before or after the `X`, and no invalid formats are given. After processing all statements in order, return the final integer value of `X`. The input vector size will equal `n`, and n can be 0 (in which case return 0). Example: statements `{"++X", "X++", "X--"}` yield 1 + 1 - 1 = 1.
#include <cassert>
#include <vector>
#include <string>

// Assume bitStatementValue is declared above.

int main() {
    // Single increment at front
    assert(bitStatementValue(1, {"++X"}) == 1);
    // Single increment at back
    assert(bitStatementValue(1, {"X++"}) == 1);
    // Single decrement at front
    assert(bitStatementValue(1, {"--X"}) == -1);
    // Single decrement at back
    assert(bitStatementValue(1, {"X--"}) == -1);
    // Multiple operations
    assert(bitStatementValue(5, {"++X", "X++", "X--", "--X", "X++"}) == 1);
    // Mixed order
    assert(bitStatementValue(6, {"X--", "++X", "X++", "--X", "X--", "++X"}) == 0);
    // Empty vector
    assert(bitStatementValue(0, {}) == 0);
    // All increments
    assert(bitStatementValue(3, {"X++", "++X", "++X"}) == 3);
    // All decrements
    assert(bitStatementValue(4, {"X--", "--X", "X--", "--X"}) == -4);
    return 0;
}
#include <string>
#include <vector>

// Process bitwise statements and return the final value of X.
int bitStatementValue(int n, const std::vector<std::string>& statements) {
    int value = 0;
    for (int i = 0; i < n; ++i) {
        const std::string& s = statements[i];
        // Check for increment: either "++X" or "X++"
        if ((s[0] == '+' && s[1] == '+') || (s[1] == '+' && s[2] == '+')) {
            ++value;
        }
        // Check for decrement: either "--X" or "X--"
        else if ((s[0] == '-' && s[1] == '-') || (s[1] == '-' && s[2] == '-')) {
            --value;
        }
        // No other cases exist per constraints.
    }
    return value;
}
// The solution is straightforward: iterate through each statement string and check if it contains either `"++"` or `"--"` by examining the first two characters or the last two characters. Since the statement format is always length 3, we can check if `s[1] == s[2]` (both `+` or both `-`)—this covers cases like `"X++"` and `"--X"`. Alternatively, explicitly check `(s[0]=='+' && s[1]=='+') || (s[1]=='+' && s[2]=='+')` for increment, and similarly for decrement. For each increment, add 1; for each decrement, subtract 1. Edge cases: empty vector returns 0; no need to handle invalid input since format is guaranteed. Time complexity: O(n) since each string is O(1) to process. Space complexity: O(1) beyond the input.
