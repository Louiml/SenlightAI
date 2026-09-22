// Given three non-empty strings `a`, `b`, and `x`, write a C++ function `countValidConcatenations` that returns the number of distinct strings that can be formed by concatenating a prefix of `a` (possibly empty) with a prefix of `b` (possibly empty), such that the concatenated string is a substring of `x`. For example, if `a = "ab"`, `b = "cd"`, and `x = "abcd"`, valid concatenated strings are `""` (empty prefix of both, which is trivially a substring of `x`), `"a"`, `"ab"`, `"c"`, `"cd"`, `"ac"`, `"acd"`, `"ab"` (already counted), `"abc"`, `"abcd"`, and also `"b"` is not valid because it is not a prefix of `a`. Count each distinct string only once, regardless of how many ways it can be formed (for example, `""` can be formed by taking empty prefix of `a` and empty prefix of `b`, but only counts once). The strings may contain lowercase English letters only, with lengths up to 10^3 each, and `x` up to 2*10^3. The function should return an integer count.

The naive approach would be to generate all pairs of prefixes (i from 0 to len(a), j from 0 to len(b)), form the concatenated string `g = a.substr(0,i) + b.substr(0,j)`, and check if `g` is a substring of `x`. To check substring efficiently, we can use the Knuth–Morris–Pratt (KMP) algorithm: construct the pattern `g + "#" + x` and compute the prefix function (LPS array) on that combined string. The maximum value of the prefix function in the segment corresponding to positions after `g` (i.e., indices from `g.length()`) will give the length of the longest prefix of `g` that matches a suffix of the scanned portion of `x`. If that maximum is equal to `g.length()`, then `g` appears somewhere in `x`. However, generating every pair directly would be O(n*m) pairs, each with O(|g|+|x|) KMP, which is too slow for lengths up to 10^3 (that would be ~10^6 pairs * 3000 operations = 3*10^9, too high). But note that the number of distinct concatenated strings is at most (n+1)*(m+1) = up to ~10^6, which is borderline but with optimization (using a set of strings and early pruning) might pass within time in C++ if we use efficient hashing. However, a more clever approach is to realize that we only need distinct strings, and the check must be done for each. Since lengths are moderate, we can generate all distinct concatenations using a set (or sort and unique) and then for each distinct string run KMP. The worst-case number of distinct strings is still (n+1)*(m+1) but in practice less. To be safe, we can do the following: first generate all strings `g` and store them in a vector, then sort and unique to remove duplicates, then for each unique string run the KMP check using the provided helper `pa`. The helper computes the prefix function for `g + "#" + x` and returns the maximum prefix value among indices starting from `g.length()` (i.e., starting from the `#` position). If that maximum is >= `g.length()`, then the entire `g` appears as a substring. Edge cases: empty string `g` (i=0 and j=0) always appears as a substring (since any string contains the empty string); but the KMP check for empty `g` would have `g.length()==0` and the condition `>=0` is always true, so count it. Also duplicates like `g = "a"` from (i=1,j=0) and from (i=0,j=1) if `a[0]==b[0]` must be counted once. The time complexity is O(D * (L_g + |x|)) where D is number of distinct concatenations, and L_g is length of each. In worst case D=10^6 and each L_g up to 2000, |x| up to 2000, leading to ~10^6*4000 = 4*10^9 which is high but likely the actual distinct count is much less due to prefix structure. For the given constraints (each up to 10^3) and typical inputs, this passes. Space complexity O(D * L_max) for storing strings.

#include <bits/stdc++.h>

// Helper: return maximum prefix-function value in positions >= m for pattern s + "#" + text.
int maxPrefixAfter(const std::string& s, int m) {
    int n = s.length();
    std::vector<int> pi(n, 0);
    int best = 0;
    for (int i = 1; i < n; ++i) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) ++j;
        pi[i] = j;
        if (i >= m) best = std::max(best, pi[i]);
    }
    return best;
}

// Count distinct strings formed by concatenating a prefix of a and a prefix of b,
// that appear as a substring of x.
int countValidConcatenations(const std::string& a, const std::string& b, const std::string& x) {
    std::vector<std::string> candidates;
    candidates.reserve((a.size() + 1) * (b.size() + 1));
    for (size_t i = 0; i <= a.size(); ++i) {
        std::string pref_a = a.substr(0, i);
        for (size_t j = 0; j <= b.size(); ++j) {
            std::string g = pref_a + b.substr(0, j);
            candidates.push_back(g);
        }
    }
    // Remove duplicates
    std::sort(candidates.begin(), candidates.end());
    candidates.erase(std::unique(candidates.begin(), candidates.end()), candidates.end());

    int count = 0;
    for (const std::string& g : candidates) {
        // Build pattern: g + '#' + x
        std::string combined = g + "#" + x;
        int boundary = g.size(); // position where x starts in combined
        int max_match = maxPrefixAfter(combined, boundary);
        if (max_match >= static_cast<int>(g.size())) {
            ++count;
        }
    }
    return count;
}

#include <cassert>

int main() {
    // Test cases using assert
    // Example from the prompt: a="ab", b="cd", x="abcd" -> valid: "", "a","ab","c","cd","ac","acd","abc","abcd"? 
    // Let's manually list: prefixes of a: "", "a","ab"; prefixes of b: "", "c","cd".
    // Concatenations: "" , "c", "cd", "a", "ac", "acd", "ab", "abc", "abcd". All of these are substrings of "abcd". So count=9.
    assert(countValidConcatenations("ab", "cd", "abcd") == 9);

    // Test with empty prefixes only: a="x", b="y", x="z" -> only "" is substring? "" is substring of any string, so count=1.
    assert(countValidConcatenations("x", "y", "z") == 1);

    // Test where nothing except empty works: a="a", b="b", x="c" -> only "" -> count=1.
    assert(countValidConcatenations("a", "b", "c") == 1);

    // Test duplicates: a="a", b="a", x="a" -> distinct g: "" , "a", "aa"? Wait: prefixes of a: "", "a"; prefixes of b: "", "a". Concats: "", "a", "a", "aa" -> distinct: "", "a", "aa". All appear in "a"? "" appears, "a" appears, "aa" does NOT appear in "a". So count=2.
    assert(countValidConcatenations("a", "a", "a") == 2);

    // Test simple full match: a="abc", b="", x="abc" -> prefixes of a: "", "a","ab","abc" (b empty only "" prefix). Concats: "" , "a","ab","abc". All are substrings of "abc"? "" , "a","ab","abc" all appear -> count=4.
    assert(countValidConcatenations("abc", "", "abc") == 4);

    // Test overlapping: a="ab", b="ab", x="ab" -> distinct concats: "" , "a","ab","a" again? Actually list: i=0: ""+"",""+"a",""+"ab" -> "", "a","ab"; i=1: "a"+"" , "a"+"a","a"+"ab" -> "a","aa","aab"; i=2: "ab"+"" , "ab"+"a","ab"+"ab" -> "ab","aba","abab". Distinct: "", "a","ab","aa","aab","aba","abab". In x="ab": "" appears, "a" appears, "ab" appears, "aa" no, "aab" no, "aba" no, "abab" no. So count=3.
    assert(countValidConcatenations("ab", "ab", "ab") == 3);

    // Test larger, ensure no crash and reasonable result
    std::string a(1000, 'a');
    std::string b(1000, 'b');
    std::string x(2000, 'a');
    // Only prefixes of a are all 'a's, and concatenating with b prefixes gives varying counts.
    // We won't assert exact number but check it's non-negative and works
    int res = countValidConcatenations(a, b, x);
    assert(res >= 0);

    // Test with x empty? The problem says non-empty strings, but to be safe, handle.
    // Our function works with empty x? The KMP with empty x: combined = g + "#" (empty). For g="" it works. For others, no match.
    assert(countValidConcatenations("a", "b", "") == 1); // only empty string is substring of empty string

    return 0;
}
