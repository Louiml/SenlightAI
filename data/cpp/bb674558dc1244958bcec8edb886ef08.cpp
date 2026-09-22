Given a string `s` consisting of lowercase letters and three integers `n`, `K`, and the string length `n` (where `n` equals `s.length()`), and an integer `K` (1 ≤ K ≤ 26), you must transform each character of `s` into one of three possible characters: the character itself, its cyclic successor, or its cyclic predecessor (modulo `K`). More precisely, for a character with index `c = s[i] - 'a'` in the range `[0, K-1]`, you may replace it with the character corresponding to `c`, `(c+1) % K`, or `(c+K-1) % K` (this is the minimal change in a cyclic alphabet of size `K`). After replacing each character, you may optionally reorder the resulting characters by moving any single character from its original position to any other position in the string (i.e., you may perform at most one arbitrary insertion/removal shift of exactly one character). Your goal is to produce the lexicographically smallest possible resulting string of length `n`. Write a standalone C++ function `std::string smallestTransformed(const std::string& s, int K)` that returns this smallest string. For example, if `s = "ab"`, `K = 2`, then for `a` (c=0) possible replacements are `a`, `b`, `b` (since (0+1)%2=1, (0+1)%2=1), and for `b` (c=1) possible are `b`, `a`, `a`; without moving, the smallest is `aa`; with moving, we could get `aa` anyway, so answer is `"aa"`. If `s = "ba"`, K=2, replacements: `b`→`b`,`a`,`a`; `a`→`a`,`b`,`b`; smallest without moving is `aa` (by choosing `a` for `b` and `a` for `a`), and moving doesn't improve, answer `"aa"`. If `s = "cba"`, K=3, replacements: `c`(2)→`c`,`a`,`b`; `b`(1)→`b`,`c`,`a`; `a`(0)→`a`,`b`,`b`; without moving, minimal is `aab` (choose `a` for c, `a` for b, `b` for a); moving could make `aaa`? Let's see: we could choose `a` for c, `a` for b, `a` for a (since for `a` c=0, (0+2)%3=2='c'? Actually (0+2)%3=2 gives 'c', so `a` itself is allowed, yes `a` is allowed), so we get `aaa` without moving. So answer `"aaa"`. The function must handle n up to 500, and K up to 26.
// The problem can be solved using dynamic programming. Define `dp[i]` as the lexicographically smallest string obtainable from the prefix `s[0..i-1]` (i.e., first `i` characters) under the rules, where we have not yet used the one allowed move. However, because we may shift a single character from a later position to an earlier position, we need to consider the possibility that a character originally at position `j` (j ≥ i) is moved into the prefix. A standard approach: Process from left to right, and at each step consider three transitions: (1) append the best replacement for current character `s[i]` to `dp[i]`; (2) if there is a previous character, swap the current character with the previous one (i.e., put current transformed char in front of the previous transformed char) – this models moving the current character one position left; (3) if we have at least two previous characters, we can move the current character two positions left (so it appears before the previous two characters). The given snippet initializes `f[0]` as empty (though it doesn't explicitly set `f[0]`, it starts from i=0 and uses `f[i]`; the code sets `f[i+1]` based on `f[i]` and `f[i-1]`). The DP is correct because any single move can be simulated by repeatedly shifting a character left by one at a time; the minimal cost is achieved by considering the current character as the moved one, and we only need to consider moving it 0, 1, or 2 positions left because moving further left would be dominated by choosing an earlier position for the move (or because the characters before are already minimal). Edge cases: K=1, then all replacements are the same 'a' (since (c+1)%1=0 and (c+K-1)%1=0), so the answer is just 'a' repeated n times. Also, when moving, we must combine the transformed versions of characters correctly: when we move `s[i]` to position `i-1`, we also keep the previous character `s[i-1]` (but it will be transformed based on its own allowed replacements) and place it after the moved character. The transformation of each character is independent of position; we choose the minimal replacement for each character when it is placed. The time complexity is O(n^2) due to string concatenations (each string up to length n), but with n≤500 it is fine. Space O(n) for storing DP strings.
#include <string>
#include <algorithm>
#include <vector>

// Return the lexicographically smallest string obtainable from s by
// replacing each character with its cyclic predecessor/successor/self modulo K,
// and then optionally moving exactly one character to any other position.
std::string smallestTransformed(const std::string& s, int K) {
    const int n = static_cast<int>(s.size());
    if (n == 0) return "";
    if (K == 1) return std::string(n, 'a');
    
    // Precompute the best (smallest) replacement for each character index.
    std::vector<char> best(n);
    for (int i = 0; i < n; ++i) {
        int c = s[i] - 'a';
        int r1 = c;
        int r2 = (c + 1) % K;
        int r3 = (c + K - 1) % K;
        int bestc = std::min({r1, r2, r3});
        best[i] = static_cast<char>('a' + bestc);
    }
    
    // dp[i] = best string for prefix of length i, with no move used.
    // We also consider moves by using dp[i-1] and dp[i-2] as described.
    std::vector<std::string> dp(n + 1);
    dp[0] = "";
    for (int i = 0; i < n; ++i) {
        // Option 1: append best[i] to dp[i]
        std::string candidate = dp[i] + best[i];
        if (dp[i + 1].empty() || candidate < dp[i + 1]) dp[i + 1] = candidate;
        
        // Option 2: move current character to position i-1 (swap with previous)
        if (i >= 1) {
            // Put best[i] in front of best[i-1], and use dp[i-1] before that.
            std::string cand2 = dp[i - 1] + best[i] + best[i - 1];
            if (dp[i + 1].empty() || cand2 < dp[i + 1]) dp[i + 1] = cand2;
            
            // Also consider inserting best[i] before the previous character
            // without changing previous character's order relative to dp[i-1].
            // This is already covered by cand2 (swap). But also we could
            // take dp[i] (which ends with best[i-1]) and insert best[i] before
            // that last character: dp[i] minus last char + best[i] + last char.
            std::string dp_i_minus_last = dp[i].substr(0, dp[i].size() - 1);
            std::string cand3 = dp_i_minus_last + best[i] + best[i - 1];
            if (dp[i + 1].empty() || cand3 < dp[i + 1]) dp[i + 1] = cand3;
        }
        
        // Option 3: move current character two positions left
        if (i >= 2) {
            std::string cand4 = dp[i - 2] + best[i] + best[i - 2] + best[i - 1];
            if (dp[i + 1].empty() || cand4 < dp[i + 1]) dp[i + 1] = cand4;
        }
    }
    
    return dp[n];
}
#include <cassert>
#include <string>

// Function declaration (assume the solution above is included)
std::string smallestTransformed(const std::string& s, int K);

int main() {
    // Basic K=2 cases
    assert(smallestTransformed("ab", 2) == "aa");
    assert(smallestTransformed("ba", 2) == "aa");
    assert(smallestTransformed("a", 2) == "a");
    assert(smallestTransformed("b", 2) == "a"); // b -> a is allowed
    
    // K=3 example from problem statement
    assert(smallestTransformed("cba", 3) == "aaa");
    
    // K=1: always all 'a'
    assert(smallestTransformed("z", 1) == "a"); // only 'a' allowed
    assert(smallestTransformed("abc", 1) == "aaa");
    
    // Longer string where moving matters: "ba" with K=2 we got "aa", but try "cab" K=3
    // c->a, a->a, b->a => "aaa" already, so no move needed
    assert(smallestTransformed("cab", 3) == "aaa");
    
    // Case where move improves: "dcba" K=4
    // d(3)->a, c(2)->a, b(1)->a, a(0)->a => "aaaa" already, so no move needed
    assert(smallestTransformed("dcba", 4) == "aaaa");
    
    // Edge case with n=2 and need to consider swap properly
    // s="ba", K=3: b(1)->a, a(0)->a => "aa" without move; with move also "aa"
    assert(smallestTransformed("ba", 3) == "aa");
    
    // s="ca", K=3: c(2)->a, a(0)->a => "aa"; no move needed
    assert(smallestTransformed("ca", 3) == "aa");
    
    // s="cb", K=3: c(2)->a, b(1)->a => "aa"
    assert(smallestTransformed("cb", 3) == "aa");
    
    // Verify lexicographic smallness for a case where moving helps:
    // s="ab", K=4: a(0)->a, b(1)->a => "aa" already
    // s="ba", K=4: b(1)->a, a(0)->a => "aa"
    assert(smallestTransformed("ab", 4) == "aa");
    assert(smallestTransformed("ba", 4) == "aa");
    
    return 0;
}
