// Write a standalone C++ function named `findSubstringIndex` that accepts two `std::string` parameters, `needle` and `haystack`, and returns an `int` representing the 0-based starting index of the first occurrence of `needle` in `haystack`, or `-1` if `needle` does not appear. The function must implement the Knuth–Morris–Pratt (KMP) string-matching algorithm, not any brute-force or standard library method. The inputs can be any ASCII strings, including empty strings; if `needle` is empty, return `0` (the empty string is always considered present at index 0). The function must be efficient even when `haystack` is very long and `needle` is a short or long pattern, handling cases where the pattern partially matches repeatedly (e.g., `"aaa"` in `"aaaa"`). You must not use any of the C++ standard library string-search facilities (e.g., `std::string::find`, `std::search`). Provide a reference implementation with proper `const` correctness and no global variables.

The KMP algorithm precomputes a prefix function (often called the "failure function" or `next` array) for the pattern (`needle`). This array, of length `n` (the pattern length), stores for each position `i` (0-indexed) the length of the longest proper prefix of `needle[0..i]` that is also a suffix of that substring. In the given reference, they store `j` such that `next[i] = j` means the best next comparison index is `j+1` (they use a shifted version from typical implementations). The key idea: when a character mismatch occurs while scanning the text (`haystack`), instead of restarting from the beginning of the pattern, we use the prefix function to skip ahead intelligently, preserving previous matches.

The algorithm consists of two passes: first, build the `next` array for the pattern in `O(n)` time. For `i` from 1 to n-1, maintain a variable `j` (which is `next[i-1]` conceptually) and while `j > -1` and `needle[i] != needle[j+1]`, set `j = next[j]` (which effectively falls back to a shorter matching prefix). After the while loop, if the characters match, increment `j`, then set `next[i] = j`. The base case `next[0] = -1`.

Second, scan the text (`haystack`) of length `m`. Keep a `j` variable representing how many characters of the pattern have matched so far (with `j = -1` meaning zero matched). For each character `haystack[i]`, if `j > -1` and `haystack[i] != needle[j+1]`, fall back using `j = next[j]` until a match is possible or `j` becomes -1. Then if the current text character matches the next pattern character (needle[j+1]), increment `j`. After each step, check if `j == n-1` — if so, we have matched the entire pattern ending at index `i`, so the starting index is `i - n + 1`. Return that. If the loop finishes without return, return -1.

Edge cases: empty `needle` → return 0 (per task). Empty `haystack` with non-empty `needle` → return -1. Single-character patterns work. Patterns with repeated characters like `"aaa"` in `"aaaa"` correctly return 0 because the prefix function allows shifting by one after a full match in the text scan (since after full match, the loop continues and the algorithm naturally resets to `next[n-1]`). The time complexity is `O(n + m)` because each character in the text is examined at most twice (once when it matches, and once when it falls back), and building the prefix table is linear. The space complexity is `O(n)` for the `next` array. No extra data structures beyond the vector are needed.

#include <string>
#include <vector>

// Returns the 0-based index of the first occurrence of needle in haystack,
// using the Knuth–Morris–Pratt algorithm. Returns -1 if needle is not found.
// If needle is empty, returns 0.
int findSubstringIndex(const std::string& needle, const std::string& haystack) {
    const std::size_t n = needle.size();
    const std::size_t m = haystack.size();

    // Edge case: empty needle is always present at index 0.
    if (n == 0) {
        return 0;
    }

    // Precompute the prefix function (next array) for the pattern.
    // next[i] stores the index j such that needle[0..j] is the longest proper
    // prefix that is also a suffix of needle[0..i]. We use j = -1 meaning no match.
    std::vector<int> next(n, 0);
    next[0] = -1;
    for (std::size_t i = 1; i < n; ++i) {
        int j = next[i - 1];
        // While there's a match and current char doesn't match, fall back.
        while (j >= 0 && needle[i] != needle[j + 1]) {
            j = next[j];
        }
        // If characters match, extend the prefix length.
        if (needle[i] == needle[j + 1]) {
            ++j;
        }
        next[i] = j;
    }

    // Scan the text (haystack) using the prefix function.
    int j = -1; // number of matched characters in needle minus 1.
    for (std::size_t i = 0; i < m; ++i) {
        // While we have a partial match and the current text char fails,
        // fall back to a shorter prefix.
        while (j >= 0 && haystack[i] != needle[j + 1]) {
            j = next[j];
        }
        // If current char matches the next pattern char, extend the match.
        if (haystack[i] == needle[j + 1]) {
            ++j;
        }
        // If we've matched the full pattern, return the starting index.
        if (static_cast<std::size_t>(j) == n - 1) {
            return static_cast<int>(i) - static_cast<int>(n) + 1;
        }
    }

    // Pattern not found.
    return -1;
}

#include <cassert>
#include <string>

// Forward declaration of the solution function (already defined above).
int findSubstringIndex(const std::string& needle, const std::string& haystack);

int main() {
    // Basic cases
    assert(findSubstringIndex("hello", "hello world") == 0);
    assert(findSubstringIndex("world", "hello world") == 6);
    assert(findSubstringIndex("abc", "xabcy") == 1);
    assert(findSubstringIndex("", "anything") == 0);
    assert(findSubstringIndex("zzz", "abc") == -1);

    // Pattern and text with repeated characters (overlapping matches)
    assert(findSubstringIndex("aaa", "aaaa") == 0);
    assert(findSubstringIndex("abab", "abababab") == 0);
    assert(findSubstringIndex("abac", "abababac") == 4);

    // Single character pattern
    assert(findSubstringIndex("a", "banana") == 1);
    assert(findSubstringIndex("z", "banana") == -1);

    // Needle longer than haystack
    assert(findSubstringIndex("longpattern", "short") == -1);

    // Pattern appears at the end
    assert(findSubstringIndex("tail", "the tail") == 4);

    // Empty haystack with non-empty needle
    assert(findSubstringIndex("abc", "") == -1);

    // Pattern is a repeated suffix/prefix word, panic case
    assert(findSubstringIndex("abcabc", "abcabcabc") == 0);

    // Case with many mismatches and backtracking
    assert(findSubstringIndex("abcabd", "abcabcabd") == 3);

    return 0;
}
