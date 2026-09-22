Write a C++ function `long long countDistinctSubstrings(const std::string& s)` that takes a non-empty string containing only lowercase English letters and returns the number of distinct non-empty substrings of `s`. The function must compute this without constructing all substrings explicitly. For example, for `"ababa"` the distinct substrings are `"a"`, `"b"`, `"ab"`, `"ba"`, `"aba"`, `"bab"`, `"abab"`, `"baba"`, `"ababa"`, so the result is `9`. The result may be up to `n*(n+1)/2`, which fits in a 64-bit signed integer for `n` up to `10^5`. The function must handle edge cases like a single character (`"a"` returns `1`) and strings with all identical characters (`"aaaa"` returns `4`).

The solution uses a suffix array and the Longest Common Prefix (LCP) array of the input string with a sentinel character appended. The sentinel `'$'` is lexicographically smaller than any lowercase letter and ensures that no suffix is a prefix of another suffix, which simplifies LCP construction. First, build the suffix array `p` and the equivalence class array `c` using the standard doubling algorithm: initialize by sorting characters, then for each power of two `2^k`, sort the cyclic shifts by the pair `(class of current position, class of position + 2^k)` using counting sort, and assign new classes. The total time is `O(n log n)` and space `O(n)`. Next, compute the LCP array using Kasai's algorithm, which runs in `O(n)` time: for each position `i` in the original string (excluding the sentinel), find its rank `ci = c[i]`, the previous suffix in sorted order `j = p[ci-1]`, and extend the common prefix length `k`. The value `lcp[ci]` is the longest common prefix of suffix `i` and its previous suffix in sorted order. The number of distinct substrings is the sum over all suffixes `i` (from 0 to n-1, excluding the sentinel at position `n-1`) of `(n - 1 - p[i]) - lcp[i]`, where `n` is the length of the string including the sentinel. The term `n - 1 - p[i]` is the length of the suffix (excluding the sentinel). Subtracting `lcp[i]` removes the substrings already counted by previous suffixes. Edge cases: for `n=1` (string of length 0 after sentinel? But input is non-empty, so at least one lowercase letter), the loop handles it correctly. The sentinel ensures `c` has at least two classes if the string has at least one letter. Ensure the counting sort is stable and uses the current `c` values as indices in `[0, n-1]`. The time complexity is `O(n log n)` for suffix array construction plus `O(n)` for LCP, and space is `O(n)`.

#include <bits/stdc++.h>

// Compute the number of distinct non-empty substrings of s.
// Uses suffix array with a sentinel '$' and LCP via Kasai's algorithm.
long long countDistinctSubstrings(const std::string& s) {
    std::string str = s + "$";
    const int n = (int)str.size();

    // Suffix array construction (doubling algorithm)
    std::vector<int> p(n), c(n);
    {
        std::vector<std::pair<char, int>> a(n);
        for (int i = 0; i < n; ++i) {
            a[i] = {str[i], i};
        }
        std::sort(a.begin(), a.end());

        for (int i = 0; i < n; ++i) {
            p[i] = a[i].second;
        }
        c[p[0]] = 0;
        for (int i = 1; i < n; ++i) {
            c[p[i]] = c[p[i - 1]];
            if (a[i].first != a[i - 1].first) {
                ++c[p[i]];
            }
        }
    }

    int k = 0;
    while ((1 << k) < n) {
        // Shift each suffix by 2^k
        for (int i = 0; i < n; ++i) {
            p[i] = (p[i] - (1 << k) + n) % n;
        }

        // Counting sort by class of the shifted suffix
        std::vector<int> cnt(n, 0);
        for (int x : c) {
            ++cnt[x];
        }
        std::vector<int> pos(n, 0);
        pos[0] = 0;
        for (int i = 1; i < n; ++i) {
            pos[i] = pos[i - 1] + cnt[i - 1];
        }
        std::vector<int> p_new(n);
        for (int x : p) {
            int i = c[x];
            p_new[pos[i]] = x;
            ++pos[i];
        }
        p = p_new;

        // Recompute classes
        std::vector<int> c_new(n);
        c_new[p[0]] = 0;
        for (int i = 1; i < n; ++i) {
            std::pair<int, int> prev = {c[p[i - 1]], c[(p[i - 1] + (1 << k)) % n]};
            std::pair<int, int> now = {c[p[i]], c[(p[i] + (1 << k)) % n]};
            c_new[p[i]] = c_new[p[i - 1]];
            if (prev != now) {
                ++c_new[p[i]];
            }
        }
        c = c_new;
        ++k;
    }

    // LCP array construction (Kasai's algorithm)
    std::vector<int> lcp(n, 0);
    k = 0;
    for (int i = 0; i < n - 1; ++i) {
        int ci = c[i];
        if (ci == 0) continue; // no previous suffix for the smallest rank
        int j = p[ci - 1];
        while (str[i + k] == str[j + k]) {
            ++k;
        }
        lcp[ci] = k;
        if (k > 0) --k;
    }

    // Sum over suffixes: length - lcp
    long long res = 0;
    for (int i = 1; i < n; ++i) {
        // p[i] is the starting index of suffix; length excluding sentinel = n-1-p[i]
        res += (long long)(n - 1) - p[i] - lcp[i];
    }
    return res;
}

#include <cassert>
#include <string>

// Declare the function under test
long long countDistinctSubstrings(const std::string& s);

int main() {
    // Single character
    assert(countDistinctSubstrings("a") == 1);
    // All identical
    assert(countDistinctSubstrings("aaaa") == 4);
    // Simple case with repeated characters
    assert(countDistinctSubstrings("ababa") == 9);
    // No repeated characters
    assert(countDistinctSubstrings("abc") == 6);
    // Two characters with repetition
    assert(countDistinctSubstrings("aa") == 2);
    // Longer string with overlapping substrings
    assert(countDistinctSubstrings("banana") == 15);
    // Edge case: sentinel handling, string length 1
    assert(countDistinctSubstrings("z") == 1);
    // Pattern with all same plus one different
    assert(countDistinctSubstrings("aaab") == 8);
    // A string where LCP matters significantly
    assert(countDistinctSubstrings("ababab") == 9);
    return 0;
}
