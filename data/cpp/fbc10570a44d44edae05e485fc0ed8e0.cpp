Write a C++ function that takes two strings of equal length, where each character is a lowercase English letter or a question mark (`?`), and returns `true` if the two strings can be made identical by replacing every `?` with exactly one lowercase letter (each `?` can be replaced independently), and `false` otherwise. If any position has two non-`?` characters that differ, matching is impossible. If all non-`?` positions agree, then all `?` positions can be assigned the same letter (e.g., `'a'`) to make the strings identical.

// The problem is a direct character-by-character comparison. For each index `i`, we examine the characters `a[i]` and `b[i]`. If both are not `?` and they are different, then no substitution can ever make them equal at that position, so we return `false`. If both are not `?` and equal, that position is already fixed and consistent. If at least one is `?`, we can always select the other character (if it is non-`?`) or any arbitrary letter (if both are `?`) to make them match at that position; hence no contradiction occurs. After scanning all positions, if no contradiction was found, return `true`. Edge cases include empty strings (trivially true), strings with only `?`, and strings where all non-`?` positions match. Time complexity is `O(n)` where `n` is the length of the input strings (we scan each character once). Space complexity is `O(1)` beyond the input storage, as we only use a boolean flag and an index.

#include <string>

// Returns true if the two strings can be made identical by replacing each '?' 
// with some lowercase letter independently.
bool canBeMatched(const std::string& a, const std::string& b) {
    // If lengths differ, they cannot be made identical.
    if (a.size() != b.size()) {
        return false;
    }

    for (std::size_t i = 0; i < a.size(); ++i) {
        const char left = a[i];
        const char right = b[i];

        // If both are fixed letters and they differ, impossible.
        if (left != '?' && right != '?' && left != right) {
            return false;
        }
        // Otherwise, at least one is '?', so we can match them.
    }

    return true;
}

#include <cassert>
#include <string>

int main() {
    // Both fixed and equal.
    assert(canBeMatched("abc", "abc") == true);

    // Fixed letters differ.
    assert(canBeMatched("abc", "abd") == false);

    // One string has '?', can match.
    assert(canBeMatched("a?c", "abc") == true);
    assert(canBeMatched("abc", "a?c") == true);

    // Both '?' at same position.
    assert(canBeMatched("??", "??") == true);

    // Mixed: mismatch at first char, rest okay.
    assert(canBeMatched("?b?", "?c?") == false);

    // All '?'.
    assert(canBeMatched("???", "???") == true);

    // Empty strings.
    assert(canBeMatched("", "") == true);

    // Different lengths.
    assert(canBeMatched("a", "ab") == false);

    // Single '?' vs fixed letter.
    assert(canBeMatched("?", "z") == true);

    // Multiple positions, all compatible.
    assert(canBeMatched("x?z", "?y?") == true);
}
