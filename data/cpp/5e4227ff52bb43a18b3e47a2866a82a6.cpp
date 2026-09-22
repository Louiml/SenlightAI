Write a C++ function named `minimumReversals` that takes a string `s` consisting only of the characters `'{'` and `'}'`, and returns the minimum number of bracket reversals required to make the string balanced. A reversal means changing one `'{'` to `'}'` or vice versa. If it is impossible to balance the string by any number of reversals, return `-1`. The function must handle empty strings (returning `0`) and strings of odd length (returning `-1`).
The core idea is to first remove all already-balanced pairs of brackets using a stack. Traversing the string left to right: push every `'{'` onto the stack; for each `'}'`, if the stack is nonempty and its top is `'{'`, pop that matching `'{'` (they cancel out); otherwise, push the `'}'` onto the stack. After this pass, the stack contains only unmatched brackets, which will appear in the form `}}...}{...{` (some closing brackets followed by some opening brackets) because any matching pair would have been removed.

Let `o` be the count of `'{'` in the stack and `c` be the count of `'}'`. If `(o + c)` is odd, the total number of brackets is odd, so it is impossible to balance — return `-1`. Otherwise, we need to fix the mismatches. For the unmatched closing brackets, we can flip half of them to opening; for the unmatched opening brackets, we can flip half of them to closing. The formula `(o + 1) / 2 + (c + 1) / 2` works because integer division with rounding up gives the number of flips needed for each group (since flips can pair two of the same type together). Edge cases: an empty string already balanced returns `0`; odd total length always returns `-1`; strings like `"}{"` require 2 flips (both characters changed). Time complexity is O(n) for a single stack pass and stack draining; space complexity is O(n) in the worst case for the stack.
#include <string>
#include <stack>

// Returns the minimum number of bracket reversals to make the string balanced,
// or -1 if impossible. Input must contain only '{' and '}'.
int minimumReversals(const std::string& s) {
    if (s.empty()) {
        return 0;
    }

    std::stack<char> st;
    for (char ch : s) {
        if (ch == '{') {
            st.push(ch);
        } else { // ch == '}'
            if (!st.empty() && st.top() == '{') {
                st.pop();
            } else {
                st.push(ch);
            }
        }
    }

    int openCount = 0;
    int closeCount = 0;
    while (!st.empty()) {
        if (st.top() == '{') {
            ++openCount;
        } else {
            ++closeCount;
        }
        st.pop();
    }

    if ((openCount + closeCount) % 2 != 0) {
        return -1;
    }

    // Each group of unmatched same-type brackets can be fixed by flipping half,
    // rounding up, because flipping one bracket and pairing it with another flipped one.
    return (openCount + 1) / 2 + (closeCount + 1) / 2;
}
#include <cassert>

int main() {
    assert(minimumReversals("") == 0);
    assert(minimumReversals("{") == -1);
    assert(minimumReversals("}") == -1);
    assert(minimumReversals("{{") == 1);
    assert(minimumReversals("}}") == 1);
    assert(minimumReversals("}{") == 2);
    assert(minimumReversals("{}") == 0);
    assert(minimumReversals("{{{{") == 2);
    assert(minimumReversals("}}}}") == 2);
    assert(minimumReversals("{{{}") == -1);
    return 0;
}
