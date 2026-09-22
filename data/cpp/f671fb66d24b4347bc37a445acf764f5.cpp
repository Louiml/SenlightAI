// Write a C++ function that takes a vector of strings and returns the number of strings that remain after repeatedly removing adjacent pairs of identical strings. When two consecutive strings in the vector are exactly equal, both are removed, and the removal may cause the strings before and after the removed pair to become adjacent and potentially also be identical, so this process continues until no adjacent equal strings exist. The function should process the vector from left to right, simulating the effect of removing such pairs as they appear. For example, given the sequence `["a", "b", "b", "a"]`, the middle pair `"b", "b"` is removed first, leaving `["a", "a"]`, which are then removed as well, resulting in an empty sequence and a return value of 0.
// The problem is equivalent to counting the number of elements that remain after repeatedly canceling adjacent identical pairs. This can be efficiently solved using a stack. Traverse the vector from left to right, and for each string, check if the stack is empty or the top of the stack is different from the current string. If the stack is non-empty and the top equals the current string, pop the stack because a matching pair has been found and both are removed. Otherwise, push the current string onto the stack. This approach correctly handles cascading removals because when a pair is popped, the new top may match the next incoming string, continuing the cancellation naturally. Edge cases include an empty input vector (returns 0), a vector with no equal adjacent pairs (returns its original size), and all elements being the same (e.g., `["x", "x", "x", "x"]` results in 0 because the first pair cancels, then the next two cancel). The time complexity is O(n) where n is the number of strings, since each string is processed once with constant-time stack operations. The auxiliary space complexity is O(n) in the worst case when no removals occur and the stack stores all elements.
#include <vector>
#include <string>
#include <stack>

// Returns the number of strings left after repeatedly removing adjacent equal pairs.
int removeConsecutiveSame(const std::vector<std::string>& v) {
    std::stack<std::string> st;
    for (const auto& s : v) {
        if (!st.empty() && st.top() == s) {
            st.pop();
        } else {
            st.push(s);
        }
    }
    return static_cast<int>(st.size());
}
#include <cassert>
#include <vector>
#include <string>

// Solution function declared above (include its definition before main).

int main() {
    // Basic removal of one adjacent pair
    assert(removeConsecutiveSame({"a", "a"}) == 0);
    // No adjacent equal pairs
    assert(removeConsecutiveSame({"a", "b", "c"}) == 3);
    // Cascade removal: "b","b" removed, then "a","a" removed
    assert(removeConsecutiveSame({"a", "b", "b", "a"}) == 0);
    // Multiple non-adjacent duplicates remain
    assert(removeConsecutiveSame({"a", "b", "a", "b"}) == 4);
    // Empty input
    assert(removeConsecutiveSame({}) == 0);
    // All identical strings
    assert(removeConsecutiveSame({"x", "x", "x", "x"}) == 0);
    // Mixed case with one remaining element
    assert(removeConsecutiveSame({"p", "q", "q", "r"}) == 2);
    // Large sequence with alternating duplicates
    assert(removeConsecutiveSame({"1", "1", "2", "2", "3", "3"}) == 0);
    // Single element
    assert(removeConsecutiveSame({"only"}) == 1);
    // Duplicates that are not adjacent
    assert(removeConsecutiveSame({"a", "b", "a", "b", "a"}) == 5);
    return 0;
}
