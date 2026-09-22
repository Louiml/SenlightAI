/*
Write a standalone C++ function named `isSubsequence` that takes two constant string references, `s` (the target sequence) and `t` (the source string), and returns a boolean indicating whether `s` is a subsequence of `t`. A string `s` is a subsequence of `t` if all characters of `s` appear in `t` in the same order, but not necessarily consecutively. The function must handle empty strings correctly (an empty `s` is always a subsequence; a non-empty `s` with an empty `t` is never a subsequence), ignore any extra characters in `t`, and must not modify the input strings. The implementation should be efficient and avoid unnecessary string copies or output. Provide a clean, self-contained solution that works for arbitrary ASCII strings including spaces and special characters, and must be callable from external code without requiring any global state.
*/
#include <string>

// Returns true if 's' is a subsequence of 't', meaning all characters of 's'
// appear in 't' in the same order (not necessarily consecutively).
bool isSubsequence(const std::string& s, const std::string& t) {
    if (s.empty()) {
        return true;
    }

    size_t sIndex = 0;  // current position in s

    for (size_t tIndex = 0; tIndex < t.size(); ++tIndex) {
        if (s[sIndex] == t[tIndex]) {
            ++sIndex;
            if (sIndex == s.size()) {
                return true;  // matched all of s
            }
        }
    }

    return false;  // reached end of t without matching all of s
}
#include <cassert>
#include <string>

// Declaration of the function to test
bool isSubsequence(const std::string& s, const std::string& t);

int main() {
    // Basic cases
    assert(isSubsequence("abc", "ahbgdc") == true);
    assert(isSubsequence("axc", "ahbgdc") == false);

    // Empty s is always a subsequence
    assert(isSubsequence("", "anything") == true);
    assert(isSubsequence("", "") == true);

    // Empty t with non-empty s is false
    assert(isSubsequence("a", "") == false);

    // Identical strings
    assert(isSubsequence("abc", "abc") == true);

    // Subsequence with gaps and multiple matches
    assert(isSubsequence("ace", "abcde") == true);
    assert(isSubsequence("aec", "abcde") == false);

    // Characters appearing multiple times
    assert(isSubsequence("aaa", "aaaa") == true);
    assert(isSubsequence("aaa", "aa") == false);

    // Single character
    assert(isSubsequence("z", "zzz") == true);
    assert(isSubsequence("z", "a") == false);

    // Special characters and spaces
    assert(isSubsequence("a c", "a x c") == true);
    assert(isSubsequence("a c", "ac") == false);

    // Larger source, small target
    assert(isSubsequence("hello", "hxe l l o!") == true);
    assert(isSubsequence("hello", "hxe l l!") == false);

    return 0;
}
// The core algorithm is a two-pointer greedy scan. We iterate over `t` once, maintaining an index `i` into `s`. For each character in `t`, if it matches the current character of `s`, we advance `i`. If we reach the end of `s` (i.e., `i == s.size()`), we have found all characters in order, so we return `true`. If we finish scanning `t` without completing `s`, we return `false`. This greedy approach works because if a character of `s` appears later in `t`, we can always take the earliest match, which leaves maximum room for subsequent characters. Edge cases: if `s` is empty, we immediately return `true` (the empty sequence is a subsequence of any string, including an empty `t`). If `t` is empty and `s` is non-empty, the loop never runs and we return `false`. Also, characters are compared directly using `==`, which works for all ASCII values including spaces and punctuation. Time complexity is O(|t|) because we scan `t` once. Space complexity is O(1) auxiliary, as we only store an integer index. This is superior to the provided snippet, which incorrectly mutates the loop variable `i` and uses a costly string concatenation.
