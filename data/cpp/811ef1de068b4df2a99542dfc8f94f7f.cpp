Given two non-empty strings `a` and `b`, write a C++ function that returns the length of the maximum overlap between the suffix of `a` and the prefix of `b`. Specifically, find the largest integer `k` (where `0 ≤ k ≤ min(a.size(), b.size())`) such that the last `k` characters of `a` are identical to the first `k` characters of `b`. If no positive overlap exists, return 0. The function must be efficient for strings up to 10^5 characters and must handle overlapping when `a` and `b` have different lengths. Use the provided KMP-based partial match table logic to compute this efficiently in linear time without constructing extra substrings.

The problem is equivalent to finding the longest prefix of `b` that is also a suffix of `a`. A naive approach would compare every possible suffix length, taking O(n*m) time. Instead, we can use the KMP prefix function (partial match table) computed on `b`, then simulate a search of `b` within `a` but stop when we reach a position where the matched prefix length reaches the end of `a`. The key insight: we scan `a` as the text and `b` as the pattern. Whenever we have a character match, we increment `matched`. If at any point `matched` equals `m` (the full length of `b`), that would mean `b` appears as a substring, so the overlap length is `m`. However, since we only need a suffix of `a`, we don't necessarily require the full pattern to appear—just the longest prefix of `b` that matches a suffix of `a`. The KMP search naturally gives us this: while scanning `a`, the variable `matched` at each position represents the length of the longest prefix of `b` that is a suffix of the prefix of `a` processed so far. At the end, when we finish scanning all of `a`, the final value of `matched` is exactly the maximum overlap. Edge cases: if `a` is shorter than `b`, the overlap cannot exceed `a.size()`, but the algorithm handles this naturally because we stop at the end of `a`. Also, if no characters match, `matched` remains 0. Time complexity is O(n + m) because building the partial match table is O(m) and scanning `a` is O(n). Space complexity is O(m) for the partial match table.

#include <vector>
#include <string>

// Returns the length of the longest suffix of 'a' that equals a prefix of 'b'.
int maxOverlap(const std::string& a, const std::string& b) {
    int m = static_cast<int>(b.size());
    // Build KMP partial match table for pattern b
    std::vector<int> pi(m, 0);
    for (int i = 1, j = 0; i < m; ++i) {
        while (j > 0 && b[i] != b[j]) {
            j = pi[j - 1];
        }
        if (b[i] == b[j]) {
            ++j;
        }
        pi[i] = j;
    }

    // Scan text 'a' against pattern 'b'
    int n = static_cast<int>(a.size());
    int matched = 0;  // length of current matched prefix of b
    for (int i = 0; i < n; ++i) {
        while (matched > 0 && (matched == m || a[i] != b[matched])) {
            matched = pi[matched - 1];
        }
        if (a[i] == b[matched]) {
            ++matched;
        }
        // If matched == m, we found full overlap; but we continue to allow
        // longer overlaps? Actually maximum possible is m, so we can break.
        if (matched == m) {
            // Full pattern matched, but since we only need suffix overlap,
            // this is the maximum possible.
            break;
        }
    }
    return matched;
}

#include <cassert>
#include <string>

// Function declaration (for testing)
int maxOverlap(const std::string& a, const std::string& b);

int main() {
    // Basic cases
    assert(maxOverlap("abcde", "defgh") == 0);
    assert(maxOverlap("abcde", "cdefg") == 3);  // "cde" matches
    assert(maxOverlap("aaaa", "aa") == 2);      // full b matches suffix
    assert(maxOverlap("abc", "abc") == 3);      // full match
    assert(maxOverlap("abc", "bcd") == 2);      // "bc" matches

    // Different lengths where a is longer
    assert(maxOverlap("abcdef", "cdefgh") == 4); // "cdef" matches
    assert(maxOverlap("hello world", "world peace") == 5); // "world" matches

    // Edge cases: a is shorter than b
    assert(maxOverlap("ab", "abc") == 2); // only "ab" can match
    assert(maxOverlap("a", "b") == 0);

    // Overlap with repeated characters
    assert(maxOverlap("ababa", "aba") == 3); // "aba" matches
    assert(maxOverlap("zzzz", "zzz") == 3);  // "zzz" matches

    // No overlap due to mismatch at end
    assert(maxOverlap("abc", "xyz") == 0);
}
