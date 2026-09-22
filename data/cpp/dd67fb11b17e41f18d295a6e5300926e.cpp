Write a C++ function named `maximumNestingDepth` that takes a string `s` containing only parentheses `'('` and `')'` and returns the maximum nesting depth of parentheses. The nesting depth is defined as the maximum number of unmatched opening parentheses encountered at any point while scanning the string from left to right. The input string is guaranteed to be non-empty and may contain only these two characters (no digits, spaces, or other symbols). For example, for `s = "((()))"`, the depth is 3; for `s = "()(())"`, the depth is 2; and for `s = "()"`, the depth is 1. Handle strings that may start with closing parentheses (even though such strings are not mathematically balanced, the task still requires computing the depth as described), and ensure the function does not mutate the input.

// The solution uses a simple counter-based scan. Initialize `currentDepth` to 0 and `maxDepth` to 0. Iterate through each character of the input string using a range-based `for` loop over `const char&` (or `char`, since it's a value type). When encountering an opening parenthesis `'('`, increment `currentDepth`; when encountering a closing parenthesis `')'`, first update `maxDepth` to the maximum of `maxDepth` and `currentDepth`, then decrement `currentDepth` if it is greater than 0 (to avoid going negative, which handles malformed strings with extra closing parens). The key insight is that the maximum nesting depth is simply the peak value of the counter during the scan. Edge cases include: a string with no parentheses (though the problem guarantees at least one character, it could be all closing or all opening — the algorithm handles both), and a string like `"))("` where the counter becomes 0, then 1; the max is correctly 1. Time complexity is O(n), where n is the string length, since we scan once. Space complexity is O(1), using only two integer variables and the input string passed by const reference (no copying).

#include <string>
#include <algorithm>

// Returns the maximum nesting depth of parentheses in the given string.
// Only '(' and ')' characters are considered; the string is not modified.
int maximumNestingDepth(const std::string& s) {
    int currentDepth = 0;
    int maxDepth = 0;
    for (char ch : s) {
        if (ch == '(') {
            ++currentDepth;
        } else if (ch == ')') {
            maxDepth = std::max(maxDepth, currentDepth);
            if (currentDepth > 0) {
                --currentDepth;
            }
        }
    }
    return maxDepth;
}

#include <cassert>
#include <string>
#include "solution.h" // or include the function directly here

int main() {
    assert(maximumNestingDepth("()") == 1);
    assert(maximumNestingDepth("((()))") == 3);
    assert(maximumNestingDepth("()(())") == 2);
    assert(maximumNestingDepth("((())") == 2); // unbalanced but depth 2
    assert(maximumNestingDepth(")(") == 0); // closing first then opening, max depth encountered is 0
    assert(maximumNestingDepth("(())()") == 2);
    assert(maximumNestingDepth("((((()))))") == 5);
    assert(maximumNestingDepth(")())") == 0); // no opening unmatched at any point gives positive depth
    assert(maximumNestingDepth("((((") == 4); // all opens, no closes
    assert(maximumNestingDepth("))))") == 0); // all closes, no opens
    return 0;
}
