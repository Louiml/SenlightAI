Write a C++ function `std::string removeAllOccurrences(const std::string& source, const std::string& pattern)` that takes a source string and a pattern string (both non-empty, pattern length ≥ 1), and returns a string that is produced by repeatedly removing **all** occurrences of the pattern from the source, but doing so in a way that respects newly formed occurrences: after each removal, if the concatenation of characters on either side of the gap creates a new occurrence of the pattern, that occurrence must also be removed. The process continues until no occurrence of the pattern exists anywhere in the current string. For example, if `source = "abcabc"` and `pattern = "abc"`, the result should be an empty string because the first removal leaves `abc` (formed by parts), which is then removed. If `source = "aababa"` and `pattern = "aba"`, the expected result is `"ab"` (first remove middle `aba` from positions 1-3, leaving `aab`, then no more `aba`). The function must preserve the order of remaining characters and work efficiently for large inputs (up to 1e6 characters). Assume characters are lowercase English letters.
// The algorithm must simulate the repeated removal process efficiently using a stack (or a vector acting as a stack). We iterate through each character of the source, pushing it onto the stack. After each push, we check whether the top of the stack ends with exactly the pattern. If yes, we pop exactly `pattern.size()` characters from the stack (removing that occurrence). This check-and-remove loop is repeated because after popping, new occurrences might form with the characters now exposed at the top (since we only check after each push, but also after each pop we should check again). However, since we process characters one at a time, the only new occurrences that can form are at the top of the stack, so after each push we run a `while` loop that checks if the top matches the pattern; if it does, pop the pattern length and continue checking (since popping could expose another match at the top). This is safe because any occurrence that forms as a result of a pop must be suffix-aligned with the stack top. Edge cases: pattern longer than current stack size → no match; overlapping patterns are handled naturally because the stack method only removes the latest occurrence at the top, and then rechecks. The final string is constructed from the remaining stack characters. Time complexity is O(n * m) in the worst case for naive checking, but we can optimize the check to O(1) amortized by comparing the last `m` characters each time (which is O(m) per check, but since each character is popped at most once, total time is O(n*m) worst-case, but for typical patterns m is small; we can also use a prefix-function‑based KMP stack to get O(n+m), but for simplicity the direct suffix check is acceptable and still passes typical constraints if m is moderate; we'll present the direct suffix check and note complexity). Space: O(n) for the stack. Edge case: empty pattern not allowed (but task says non-empty). Also note: the process stops when no occurrence remains, and our stack-based loop ensures that.
#include <string>
#include <vector>

// Repeatedly remove all occurrences of 'pattern' from 'source', including
// those that form after previous removals. Returns the final remaining string.
std::string removeAllOccurrences(const std::string& source, const std::string& pattern) {
    if (pattern.empty()) {
        return source; // not expected per spec, but safe
    }

    const std::size_t patternLen = pattern.size();
    std::vector<char> stack;
    stack.reserve(source.size());

    // Check if the top of the stack ends with 'pattern'
    auto endsWithPattern = [&]() -> bool {
        if (stack.size() < patternLen) {
            return false;
        }
        for (std::size_t i = 0; i < patternLen; ++i) {
            if (stack[stack.size() - patternLen + i] != pattern[i]) {
                return false;
            }
        }
        return true;
    };

    for (char ch : source) {
        stack.push_back(ch);
        // Remove any occurrences that end exactly at the top of the stack
        while (endsWithPattern()) {
            for (std::size_t i = 0; i < patternLen; ++i) {
                stack.pop_back();
            }
        }
    }

    // Build the result string
    std::string result;
    result.reserve(stack.size());
    for (char ch : stack) {
        result.push_back(ch);
    }
    return result;
}
#include <cassert>
#include <string>

// Declaration from the solution (normally would be in a header)
std::string removeAllOccurrences(const std::string& source, const std::string& pattern);

int main() {
    // Basic removal
    assert(removeAllOccurrences("hello", "ll") == "heo");
    // Repeated removal forming new patterns
    assert(removeAllOccurrences("abcabc", "abc") == "");
    assert(removeAllOccurrences("aababa", "aba") == "ab");
    // Pattern not found
    assert(removeAllOccurrences("xyzxyz", "abc") == "xyzxyz");
    // Single character pattern
    assert(removeAllOccurrences("aabbaa", "a") == "bb");
    // Overlapping patterns that require repeated checks after pop
    assert(removeAllOccurrences("aaa", "aa") == "a");
    // Pattern longer than string
    assert(removeAllOccurrences("ab", "abc") == "ab");
    // Multiple separate removals
    assert(removeAllOccurrences("abababa", "aba") == "ab");
    // Edge case: source equal to pattern
    assert(removeAllOccurrences("delete", "delete") == "");
    // All removed to empty
    assert(removeAllOccurrences("aaaa", "aa") == "");
    return 0;
}
