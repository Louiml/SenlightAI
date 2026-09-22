Write a C++ function that takes two strings, `text` and `pattern`, and returns the number of times `pattern` appears as a contiguous substring in `text`. The function should be case-sensitive and handle overlapping occurrences (e.g., in "aaaa", pattern "aa" occurs 3 times). Input strings may contain any printable ASCII characters, and the length of `text` can be up to 1,000,000 characters while `pattern` length up to 100,000. If either string is empty, return 0. The function should be efficient enough for large inputs. Provide a function named `countSubstringOccurrences(const std::string& text, const std::string& pattern)` that returns an integer count.
#include <cassert>

int main() {
    // Basic cases
    assert(countSubstringOccurrences("hello world", "world") == 1);
    assert(countSubstringOccurrences("hello world", "hello") == 1);
    assert(countSubstringOccurrences("hello", "x") == 0);

    // Overlapping occurrences
    assert(countSubstringOccurrences("aaaa", "aa") == 3);
    assert(countSubstringOccurrences("abababa", "aba") == 3);
    assert(countSubstringOccurrences("aaaaa", "aaa") == 3);

    // Empty strings
    assert(countSubstringOccurrences("", "abc") == 0);
    assert(countSubstringOccurrences("abc", "") == 0);

    // Pattern longer than text
    assert(countSubstringOccurrences("abc", "abcd") == 0);

    // Case sensitivity
    assert(countSubstringOccurrences("AbcAbc", "abc") == 0);
    assert(countSubstringOccurrences("AbcAbc", "Abc") == 2);

    // Single character pattern
    assert(countSubstringOccurrences("mississippi", "s") == 4);
    assert(countSubstringOccurrences("mississippi", "p") == 2);

    // Non-overlapping occurrences
    assert(countSubstringOccurrences("abcabcabc", "abc") == 3);
    assert(countSubstringOccurrences("aaaa", "a") == 4);

    // Large stress test (not exhaustive but ensures no crash)
    std::string bigText(1000000, 'a');
    std::string smallPattern(1000, 'a');
    assert(countSubstringOccurrences(bigText, smallPattern) == 1000000 - 1000 + 1);

    return 0;
}
#include <string>
#include <vector>

// Count occurrences of pattern as a contiguous substring in text, allowing overlaps.
int countSubstringOccurrences(const std::string& text, const std::string& pattern) {
    const int n = static_cast<int>(text.size());
    const int m = static_cast<int>(pattern.size());
    if (n == 0 || m == 0 || m > n) return 0;

    // Build prefix function (failure function) for KMP.
    std::vector<int> pi(m, 0);
    for (int i = 1, j = 0; i < m; ++i) {
        while (j > 0 && pattern[i] != pattern[j]) {
            j = pi[j - 1];
        }
        if (pattern[i] == pattern[j]) {
            ++j;
        }
        pi[i] = j;
    }

    // Scan text using KMP.
    int count = 0;
    int q = 0; // number of matched characters so far
    for (int i = 0; i < n; ++i) {
        while (q > 0 && text[i] != pattern[q]) {
            q = pi[q - 1];
        }
        if (text[i] == pattern[q]) {
            ++q;
        }
        if (q == m) {
            ++count;
            q = pi[q - 1]; // allow overlapping occurrences
        }
    }
    return count;
}
// The naive approach of checking each position with string comparison would take O(n*m) time, which is too slow for the given constraints. The optimal solution uses the Knuth-Morris-Pratt (KMP) algorithm, which preprocesses the pattern to build a prefix function (also called failure function) in O(m) time, then scans the text in O(n) time. The prefix function `pi[i]` stores the length of the longest proper prefix of `pattern[0..i]` that is also a suffix. During the scan, we maintain a state `q` representing how many characters of the pattern have matched so far. When a mismatch occurs, we fall back using `pi` to avoid re-scanning. Each time `q` reaches `m` (length of pattern), we increment the count and then set `q = pi[m-1]` to allow overlapping matches. Edge cases: empty strings return 0; if pattern length > text length, return 0 immediately. Time complexity: O(n + m). Space complexity: O(m) for the prefix array.
