// Given a valid parentheses string `s` consisting only of the characters `'('` and `')'`, write a C++ function `int nestingDepth(const std::string& s)` that returns the maximum nesting depth of the parentheses. The nesting depth is defined as the maximum number of nested parentheses at any point in the string. For example, the string `"( () ( () ) )"` (with spaces removed) has a nesting depth of 3 because the innermost parentheses are nested inside three levels. The input is guaranteed to be a valid parentheses string, meaning every opening bracket has a matching closing bracket and no mismatched brackets appear. The function should handle the empty string (returning 0) and strings of length up to 10^5 efficiently. The time complexity must be O(n) with O(1) auxiliary space, where n is the length of the string.
#include <cassert>
#include <string>

int nestingDepth(const std::string& s); // forward declaration

int main() {
    assert(nestingDepth("()") == 1);
    assert(nestingDepth("(())") == 2);
    assert(nestingDepth("((()))") == 3);
    assert(nestingDepth("()()") == 1);
    assert(nestingDepth("(()(()))") == 3);
    assert(nestingDepth("") == 0);
    assert(nestingDepth("((((()))))") == 5);
    assert(nestingDepth("(())()") == 2);
    assert(nestingDepth("()(())") == 2);
    assert(nestingDepth("((())(()))") == 3);
    return 0;
}
#include <string>
#include <algorithm>

// Returns the maximum nesting depth of a valid parentheses string.
// The string must consist only of '(' and ')' and be balanced.
int nestingDepth(const std::string& s) {
    int currentDepth = 0;
    int maxDepth = 0;
    for (char ch : s) {
        if (ch == '(') {
            ++currentDepth;
            maxDepth = std::max(maxDepth, currentDepth);
        } else if (ch == ')') {
            --currentDepth;
        }
    }
    return maxDepth;
}
// The approach is a simple linear scan using a counter. Initialize `currentDepth` to 0 and `maxDepth` to 0. Iterate through each character of the string:
// - If the character is `'('`, increment `currentDepth`. Then update `maxDepth` to be the maximum of `maxDepth` and `currentDepth`.
// - If the character is `')'`, decrement `currentDepth` (since the string is valid, this will never go negative, but we need not check).
// After processing all characters, return `maxDepth`. The key observation is that the depth only increases when we encounter an opening parenthesis, and only here can the depth exceed what we’ve seen before. Closing parentheses only decrease the depth, so they never contribute to a new maximum. Edge cases: the empty string returns 0 (since maxDepth starts at 0 and the loop never runs). A string with no nesting like `"()"` returns 1. A string like `"((()))"` returns 3. Because the input is guaranteed valid, we don’t need to worry about unbalanced brackets. The algorithm runs in O(n) time and uses only two integer variables, so O(1) space.
