Given three strings `a`, `b`, and `c` of equal length consisting of lowercase English letters, write a C++ function named `canTransform` that returns `true` if it is possible to transform `a` into `b` by applying a single operation: for each position `i`, you may either keep the character from `a[i]` or replace it with `c[i]` (but you cannot use both `a[i]` and `b[i]` simultaneously — the operation is simply that at each index, you choose either `a[i]` or `c[i]` as the final character). The goal is to see if the resulting string equals `b`. The function should return `true` if for every index `i`, at least one of `a[i]` or `c[i]` equals `b[i]`. If any index fails this condition, return `false`. The function must handle empty strings (in which case return `true`), and up to arbitrary length (e.g., length up to 10^5). Provide the solution with proper `const` correctness and no global state.
The core problem reduces to a simple per-character check: for each index `i`, we need `b[i]` to be obtainable as either `a[i]` or `c[i]`. Since we can choose independently at each position, the only way transformation fails is if there exists an index where `b[i]` equals neither `a[i]` nor `c[i]`. So the algorithm iterates through all characters and returns `false` immediately upon finding such a mismatch; otherwise returns `true` after the full scan. Edge cases: empty strings trivially return `true` (loop runs zero times). The strings are guaranteed to have equal length per the problem statement, but the function can defensively check and return `false` if lengths differ. Time complexity is O(n) where n is the string length, and space complexity is O(1) extra (no additional data structures). This is optimal because we must at least read every character to verify the condition.
#include <string>

// Returns true if for every index i, b[i] equals either a[i] or c[i].
// Assumes a, b, c have equal length (returns false if they don't).
bool canTransform(const std::string& a, const std::string& b, const std::string& c) {
    if (a.length() != b.length() || b.length() != c.length()) {
        return false;
    }
    for (std::size_t i = 0; i < a.length(); ++i) {
        if (a[i] != b[i] && c[i] != b[i]) {
            return false;
        }
    }
    return true;
}
#include <cassert>

int main() {
    // Basic positive case
    assert(canTransform("abc", "axc", "xyc") == true); // index 1: b[1]='x' equals c[1]
    // Basic negative case
    assert(canTransform("abc", "def", "ghi") == false); // no char matches
    // All positions match a directly
    assert(canTransform("hello", "hello", "world") == true);
    // All positions match c directly
    assert(canTransform("abc", "xyz", "xaz") == false); // index 1: b[1]='y' not in {a[1]='b', c[1]='a'}
    // Edge: empty strings
    assert(canTransform("", "", "") == true);
    // Edge: mismatched lengths
    assert(canTransform("a", "ab", "c") == false);
    // Single character
    assert(canTransform("a", "b", "b") == true);
    assert(canTransform("a", "b", "c") == false);
    // Longer test where only one fails
    assert(canTransform("abcde", "abxde", "xxxxx") == true); // index 2 in c
    assert(canTransform("abcde", "abxde", "abcde") == false); // index 2 not in either
    return 0;
}
