// Write a C++ function `bool isValidParenthesisString(const std::string& s)` that determines whether a given string consisting only of characters `'('`, `')'`, and `'*'` represents a valid parenthesis string. The `'*'` character can be interpreted as either an opening parenthesis `'('`, a closing parenthesis `')'`, or an empty string. A valid parenthesis string must satisfy: every closing parenthesis has a corresponding opening parenthesis before it (in the left-to-right scan), and every opening parenthesis has a corresponding closing parenthesis after it (in the right-to-left scan). The function should return `true` if the string can be made valid by choosing appropriate interpretations for each `'*'`, and `false` otherwise. The input string may be empty (which is valid), and you must handle edge cases such as leading closing parentheses, trailing opening parentheses, and strings consisting only of asterisks.
// The solution uses two passes to check feasibility. In the first pass, we treat every `'*'` as an opening parenthesis (`'('`) to ensure that we never run out of open parentheses when scanning from left to right. We maintain a counter `open` that increments on `'('` or `'*'` and decrements on `')'`. If `open` ever becomes negative, it means there are more closing parentheses than opening ones available even in the most favorable interpretation, so the string is invalid. In the second pass, we scan from right to left and treat every `'*'` as a closing parenthesis (`')'`) to ensure that we never run out of closing parentheses when matching from the right side. We maintain a counter `close` that increments on `')'` or `'*'` and decrements on `'('`. If `close` becomes negative, there are more opening parentheses than closing ones available, so the string is invalid. If both passes succeed, the string is valid because we have simultaneously satisfied both left-to-right and right-to-left constraints, which together guarantee a valid matching. Edge cases include empty strings (both counters stay at zero, return `true`), strings with only `'*'` (both passes never go negative), and strings like `"*)("` where the left pass succeeds but the right pass fails. Time complexity is \(O(n)\) for two linear scans, and space complexity is \(O(1)\) beyond the input string.
#include <string>

// Checks if a string of '(', ')', and '*' can form a valid parenthesis string.
// '*' can be treated as '(', ')', or an empty string.
bool isValidParenthesisString(const std::string& s) {
    int open = 0;   // Counter for left-to-right pass (treat '*' as '(')
    for (char c : s) {
        if (c == ')') {
            --open;
        } else {
            // '(' or '*' increases the possible open count
            ++open;
        }
        if (open < 0) {
            return false; // Too many closing parentheses
        }
    }

    int close = 0;  // Counter for right-to-left pass (treat '*' as ')')
    for (int i = static_cast<int>(s.size()) - 1; i >= 0; --i) {
        if (s[i] == '(') {
            --close;
        } else {
            // ')' or '*' increases the possible close count
            ++close;
        }
        if (close < 0) {
            return false; // Too many opening parentheses
        }
    }

    return true;
}
#include <cassert>
#include <string>

// The function is declared above, so we only need main for tests.
int main() {
    assert(isValidParenthesisString("()") == true);
    assert(isValidParenthesisString("(*)") == true);
    assert(isValidParenthesisString("(*))") == true);
    assert(isValidParenthesisString("") == true);
    assert(isValidParenthesisString("****") == true);
    assert(isValidParenthesisString(")(") == false);
    assert(isValidParenthesisString("())") == false);
    assert(isValidParenthesisString("(()") == false);
    assert(isValidParenthesisString("*)(") == false);
    assert(isValidParenthesisString("((*)") == true);
    return 0;
}
