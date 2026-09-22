Write a C++ function `std::string largestBracketSequence(const std::string& s)` that takes a string consisting only of characters `'('`, `')'`, `'['`, and `']'`. The function must find the longest contiguous substring that is a *valid bracket sequence* (i.e., correctly matched parentheses and square brackets, like `()` or `[()]`), and among all valid sequences with the same maximum length, choose the one with the largest number of square brackets `'['`. If there is still a tie, choose the one that occurs earliest in the string. The function should return that substring as a string. If no valid bracket sequence exists (i.e., the longest valid substring has length 0), return an empty string. The input string length will be at most 10^5. You must implement the solution in O(n) time and O(n) space.

#include <cassert>
#include <string>

// This is the solution function declaration. Normally it would be included from the header, but for testing we redeclare.
std::string largestBracketSequence(const std::string& s);

int main() {
    // Basic valid sequences
    assert(largestBracketSequence("()") == "()");
    assert(largestBracketSequence("[]") == "[]");

    // Nested sequences
    assert(largestBracketSequence("([()])") == "([()])");
    assert(largestBracketSequence("[()]") == "[()]");

    // Mixed with invalid segments
    assert(largestBracketSequence(")(") == "");
    assert(largestBracketSequence("())(") == "()");
    assert(largestBracketSequence("([)]") == "()"); // "()" is length 2, also "[]" but "()" appears earlier? Actually both length 2, "()" at index 1-2, "[]" at index 0-3? Wait "([)]" has "()" at positions 1-2 and no valid "[]", so answer "()"
    assert(largestBracketSequence("([)]") == "()");

    // Tie-breaking: same length, more square brackets wins
    // "()[]" has two valid sequences of length 2: "()" and "[]". Both have 0 squares. "()" earlier.
    assert(largestBracketSequence("()[]") == "()");
    // "[[()]]" has length 6, square count 2. "([[]])" also length 6 but square count 2? Let's craft a case: "[[()]]" vs "[[]]" length 4. 
    // Better test: "[[()]]" (squares=2, len=6) vs "[[]()]" (squares=2, len=6, but start later). 
    assert(largestBracketSequence("[[()]]") == "[[()]]");
    // "()[]()" contains "()[]" (len 4, squares 0) and "()" etc. "()[]" is longest.
    assert(largestBracketSequence("()[]()") == "()[]");

    // Tie-break by square count: "([])" has length 4, squares 1; "()()" has length 4, squares 0. So "([])" wins.
    assert(largestBracketSequence("([])()") == "([])");

    // Complex: "([][])" length 6, squares 2. "[[()]]" length 6, squares 2. Both length 6 and same squares. Earliest appears first.
    assert(largestBracketSequence("([][])[[()]]") == "([][])"); // first occurrence wins on tie

    // Edge case: empty string
    assert(largestBracketSequence("") == "");

    // Long sequence with chaining
    assert(largestBracketSequence("((()))") == "((()))");
    assert(largestBracketSequence("({}())") == "{}()"); // but only brackets allowed, so test with brackets only

    return 0;
}

#include <string>
#include <vector>
#include <stack>
#include <algorithm>

// Returns the longest valid bracket substring with tie-breaking by square count then earliest start.
std::string largestBracketSequence(const std::string& s) {
    int n = static_cast<int>(s.size());
    if (n == 0) return "";

    std::vector<int> match(n, -1);
    std::vector<int> squarePrefix(n, 0);
    std::stack<int> st;

    for (int i = 0; i < n; ++i) {
        // Prefix count of '[' up to and including i
        squarePrefix[i] = (i > 0 ? squarePrefix[i-1] : 0) + (s[i] == '[');

        if (!st.empty()) {
            char topChar = s[st.top()];
            char curChar = s[i];
            bool matches = (topChar == '(' && curChar == ')') || (topChar == '[' && curChar == ']');
            if (matches) {
                int openPos = st.top();
                st.pop();
                // Start of the valid sequence ending at i
                int start = openPos;
                // Try to chain with previous valid sequence lying just before openPos
                if (openPos > 0 && match[openPos - 1] != -1) {
                    start = match[openPos - 1];
                }
                match[i] = start;
            } else {
                st.push(i);
            }
        } else {
            st.push(i);
        }
    }

    int bestLen = 0;
    int bestSquare = 0;
    int bestStart = -1;
    int bestEnd = -1;

    for (int i = 0; i < n; ++i) {
        if (match[i] != -1) {
            int start = match[i];
            int len = i - start + 1;
            int squareCount = squarePrefix[i] - (start > 0 ? squarePrefix[start - 1] : 0);
            // Compare: longer length, then more square brackets, then earlier start
            if (len > bestLen ||
                (len == bestLen && squareCount > bestSquare) ||
                (len == bestLen && squareCount == bestSquare && (bestStart == -1 || start < bestStart))) {
                bestLen = len;
                bestSquare = squareCount;
                bestStart = start;
                bestEnd = i;
            }
        }
    }

    if (bestStart == -1) return "";
    return s.substr(bestStart, bestEnd - bestStart + 1);
}

// The problem is a classic longest valid parentheses substring, extended to two bracket types and a tie-breaking rule. The key is to compute for each position `i` the start index `match[i]` of the longest valid bracket sequence ending at `i`. We use a stack to track unmatched opening brackets. While scanning left to right, when we see a closing bracket that matches the top of the stack, we form a valid sequence. The start of that sequence can "chain" if the character just before the matched opening bracket is the end of another valid sequence (i.e., `match[match[i]-1]` is defined), allowing us to extend the current sequence backward. This gives the longest valid sequence ending at `i`. We also maintain a prefix count of `'['` for quick counting of square brackets in any range. For each position with a valid match, we compute the length `(i - match[i] + 1)` and the number of square brackets in that range. We track the best sequence by (length, square-bracket count, earliest start). Edge cases: empty input, no valid sequences, multiple overlapping sequences, and sequences that are fully contained in longer ones (the chaining handles this). Time complexity is O(n) because each character is pushed/popped once. Space is O(n) for the stack, match array, and prefix counts.
