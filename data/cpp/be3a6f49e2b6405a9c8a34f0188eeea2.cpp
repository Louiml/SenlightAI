/*
Write a C++ function that takes a string `s` containing lowercase English letters and the character `'*'`, and returns the resulting string after repeatedly applying the following operation: whenever a `'*'` appears, it removes the closest non-`'*'` character to its left that has not yet been removed. If there is no such character (i.e., the `'*'` appears before any non-removed character or all previous characters are already removed), the `'*'` itself is simply ignored and has no effect. The final output should be the string of remaining characters in their original left‑to‑right order. For example, given `"leet**cod*e"`, the result is `"lecoe"`.
*/

#include <string>
#include <stack>
#include <algorithm>

// Removes characters immediately to the left of every '*' in a string,
// ignoring '*' characters that have no character to remove.
std::string removeStars(const std::string& s) {
    std::stack<char> st;
    for (char ch : s) {
        if (ch != '*') {
            st.push(ch);
        } else {
            if (!st.empty()) {
                st.pop();
            }
        }
    }
    std::string result;
    result.reserve(st.size());
    while (!st.empty()) {
        result.push_back(st.top());
        st.pop();
    }
    std::reverse(result.begin(), result.end());
    return result;
}

#include <cassert>
#include <string>

// The function declaration is assumed to be provided above.
std::string removeStars(const std::string& s);

int main() {
    // Basic examples
    assert(removeStars("leet**cod*e") == "lecoe");
    assert(removeStars("erase*****") == "");
    assert(removeStars("a*b*c") == "c");
    assert(removeStars("abc") == "abc");
    assert(removeStars("***") == "");
    
    // Edge cases: leading stars, multiple stars after a character, single char
    assert(removeStars("*a*") == "");
    assert(removeStars("a**b") == "b");
    assert(removeStars("a*") == "");
    assert(removeStars("*") == "");
    assert(removeStars("a") == "a");
    
    return 0;
}

// The operation described is naturally modeled by a stack. We traverse the input from left to right. For each character:  
// - If it is not `'*'`, we push it onto the stack.  
// - If it is `'*'`, we check the stack: if it is non‑empty, we pop the top element (which corresponds to the most recent non‑removed character to the left); if it is empty, we do nothing (the `'*'` is ignored).  
// After processing all characters, the stack contains exactly the characters that survive, in reverse order (since the stack stores them from bottom to top as they appeared). To obtain the final string in correct left‑to‑right order, we pop all elements into a temporary string and then reverse it.  
// Edge cases include: leading `'*'` characters (they are ignored because the stack is empty), consecutive `'*'` after a character (each removes the most recent surviving character), and strings with no `'*'` (the output equals the input). The algorithm runs in `O(n)` time and `O(n)` auxiliary space in the worst case (when there are no `'*'`), where `n` is the length of the input string.
