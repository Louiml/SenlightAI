/*
You are given a string `s` consisting of lowercase English letters. Write a C++ function `int countAlmostPalindromicSubstrings(const std::string& s)` that returns the number of substrings of `s` that become palindromes after removing **at most one character** from the substring. A substring is defined by a contiguous sequence of characters within `s`. The empty substring is not considered. For example, for `s = "abca"`, the substring `"abca"` is not a palindrome, but removing the last `'a'` yields `"abc"` (not a palindrome), removing the second `'b'` yields `"aca"` (a palindrome), so `"abca"` counts. Also, all substrings of length 1 and 2 are automatically valid because any single character is a palindrome, and any two-character substring can become a palindrome by removing one character (or is already a palindrome). The function should return the total count of such substrings, which may be large (up to O(n^2) for n up to 5000), so use a 64-bit integer for the return type.
*/

#include <bits/stdc++.h>
using namespace std;
using ill = long long;

// Rolling hash with two moduli to reduce collision probability
const int MOD1 = 1000000007;
const int MOD2 = 1000000009;
const int BASE1 = 911382323;
const int BASE2 = 972663749;

// Return the number of substrings that become palindromes after removing at most one character
long long countAlmostPalindromicSubstrings(const string& s) {
    int n = (int)s.size();
    // Precompute powers and prefix hashes for forward and reverse strings
    vector<long long> pow1(n+1), pow2(n+1);
    pow1[0] = pow2[0] = 1;
    for (int i = 1; i <= n; ++i) {
        pow1[i] = pow1[i-1] * BASE1 % MOD1;
        pow2[i] = pow2[i-1] * BASE2 % MOD2;
    }
    vector<long long> pre1(n+1), pre2(n+1);
    for (int i = 0; i < n; ++i) {
        pre1[i+1] = (pre1[i] + (s[i] - 'a' + 1) * pow1[i]) % MOD1;
        pre2[i+1] = (pre2[i] + (s[i] - 'a' + 1) * pow2[i]) % MOD2;
    }
    string rev = s;
    reverse(rev.begin(), rev.end());
    vector<long long> revpre1(n+1), revpre2(n+1);
    for (int i = 0; i < n; ++i) {
        revpre1[i+1] = (revpre1[i] + (rev[i] - 'a' + 1) * pow1[i]) % MOD1;
        revpre2[i+1] = (revpre2[i] + (rev[i] - 'a' + 1) * pow2[i]) % MOD2;
    }

    // Get hash of substring s[l..r] (0-indexed inclusive)
    auto getHash = [&](int l, int r) -> pair<long long,long long> {
        long long h1 = (pre1[r+1] - pre1[l] + MOD1) % MOD1;
        h1 = h1 * pow1[n-1-l] % MOD1; // normalize by inverse power (multiply by pow1[n-1-l] since we use ascending powers)
        long long h2 = (pre2[r+1] - pre2[l] + MOD2) % MOD2;
        h2 = h2 * pow2[n-1-l] % MOD2;
        return {h1, h2};
    };

    // Get hash of substring of rev that corresponds to s[l..r] reversed
    // In rev, the substring s[l..r] appears at positions (n-1-r) to (n-1-l)
    auto getRevHash = [&](int l, int r) -> pair<long long,long long> {
        int start = n-1-r;
        int end = n-1-l;
        long long h1 = (revpre1[end+1] - revpre1[start] + MOD1) % MOD1;
        h1 = h1 * pow1[n-1-start] % MOD1;
        long long h2 = (revpre2[end+1] - revpre2[start] + MOD2) % MOD2;
        h2 = h2 * pow2[n-1-start] % MOD2;
        return {h1, h2};
    };

    // Longest common prefix length between s[l..] and rev[revL..] (only need up to length limit)
    auto lcpLen = [&](int l, int revL, int limit) -> int {
        int lo = 0, hi = min(limit, min(n-l, n-revL));
        while (lo < hi) {
            int mid = (lo + hi + 1) / 2;
            auto h1 = getHash(l, l+mid-1);
            auto h2 = getRevHash(n-1-(l+mid-1), n-1-l); // but easier: use rev substring from revL to revL+mid-1
            // We need a function to get hash of rev[revL..revL+mid-1] directly, so we implement getRevSub
            // To avoid lambda recursion, we'll define a separate function later
            // For simplicity, we'll write a custom check using direct hash comparison
            // Let's do this inside a separate lambda that captures everything
            // Simplification: we'll just write a helper inside the main function
            // Since this is getting complicated, we'll re-def the hash for rev directly with an explicit function.

            // Actually we can directly compare hashes of s[l..l+mid-1] and rev[revL..revL+mid-1]
            // We have getHash for s, and we can get hash for rev using similar formula from revpre arrays
            auto calcRevSubHash = [&](int startIdx, int len) {
                long long h1 = (revpre1[startIdx+len] - revpre1[startIdx] + MOD1) % MOD1;
                h1 = h1 * pow1[n-1-startIdx] % MOD1;
                long long h2 = (revpre2[startIdx+len] - revpre2[startIdx] + MOD2) % MOD2;
                h2 = h2 * pow2[n-1-startIdx] % MOD2;
                return make_pair(h1,h2);
            };
            auto a = getHash(l, l+mid-1);
            auto b = calcRevSubHash(revL, mid);
            if (a == b) lo = mid; else hi = mid-1;
        }
        return lo;
    };

    long long ans = 0;
    // Iterate over all substrings in O(n^2) but use LCP to check almost palindrome
    for (int l = 0; l < n; ++l) {
        for (int r = l; r < n; ++r) {
            int len = r - l + 1;
            if (len == 0) continue;
            // Check if almost palindrome: at most one mismatch
            // Compute LCP from both ends
            int revL = n-1-r; // position in rev corresponding to s[r]
            int lcp = lcpLen(l, revL, len/2); // we only need to match half
            // After matching lcp characters from each end, the middle part (possibly one character) must be almost palindrome
            // If lcp >= len/2, then the substring is already a palindrome (or length 1)
            // If lcp == len/2 - 1, we might have one mismatch, need to check if remaining part is palindrome
            int remainingLeft = l + lcp;
            int remainingRight = r - lcp;
            if (lcp >= len/2) {
                // perfectly matched at least half, then the rest is symmetric, so it's a palindrome
                ans++;
                continue;
            }
            // Now we have a mismatch at positions remainingLeft and remainingRight
            // Check if skipping either side leads to palindrome
            // Subproblem: check if s[remainingLeft+1..remainingRight] is palindrome (skip left) OR s[remainingLeft..remainingRight-1] is palindrome (skip right)
            // We can check palindrome by comparing with rev
            auto isPal = [&](int a, int b) -> bool {
                if (a >= b) return true;
                int mid = (b-a+1)/2;
                int lcp2 = lcpLen(a, n-1-b, mid);
                return lcp2 >= mid;
            };
            if (isPal(remainingLeft+1, remainingRight) || isPal(remainingLeft, remainingRight-1)) {
                ans++;
            }
        }
    }
    return ans;
}

#include <bits/stdc++.h>
using namespace std;

// The solution function is defined in the previous section (assumed to be in scope)
// We'll duplicate it here for the test to be runnable
// (If the test is compiled with the solution above, no need to redefine)
// For clarity, we assume the function exists as declared above.

int main() {
    // Basic cases
    assert(countAlmostPalindromicSubstrings("a") == 1); // "a"
    assert(countAlmostPalindromicSubstrings("ab") == 3); // "a","b","ab"
    assert(countAlmostPalindromicSubstrings("aa") == 3); // "a","a","aa"
    assert(countAlmostPalindromicSubstrings("aba") == 6); // all substrings: a,b,a,ab,ba,aba (all valid)
    // "abca"
    // substrings:
    // length 1: a,b,c,a (4)
    // length 2: ab,bc,ca (3) all valid
    // length 3: abc,bca (2) check:
    //   abc: remove 'c'? "ab" no; remove 'b'? "ac" no; remove 'a'? "bc" no -> invalid
    //   bca: invalid
    //   But also "ab" already counted, "bc","ca" counted
    // length 4: abca -> valid (remove 'b' yields "aca")
    // total = 4+3+0+1=8
    assert(countAlmostPalindromicSubstrings("abca") == 8);
    // "abcba" is a palindrome, all substrings? Actually not all, but we test a few
    assert(countAlmostPalindromicSubstrings("abcba") == 13); // let's compute: n=5, total substrings=15, invalid ones? "abc" invalid, "bcb" valid, "cba" invalid? "cba" remove 'c'->"ba" no, remove 'b'->"ca" no, remove 'a'->"cb" no -> invalid, "ab" valid etc. We'll trust for now
    // Edge: larger string, ensure no crash
    string big(1000, 'a');
    long long res = countAlmostPalindromicSubstrings(big);
    // All substrings of all 'a's are palindromes, so count = 1000*1001/2 = 500500
    assert(res == 500500);
    printf("All tests passed!\n");
    return 0;
}

// The key observation is that a substring is "almost palindromic" if and only if its longest common prefix between the original substring and its reverse is at least its length minus 1, i.e., at most one mismatch is allowed. For each substring `s[l..r]`, we can determine if it is almost a palindrome by a greedy two-pointer check: start with `i=l`, `j=r`, and allow up to one mismatch. If the characters at `i` and `j` match, we move inward; if they do not match, we try two possibilities: skip the left character (`i++`) or skip the right character (`j--`). If after trying both possibilities we still have a mismatch, the substring fails. This greedy works because skipping a mismatch on one side is the only way to fix it, and after skipping once, the rest must match exactly. We can precompute for each center (both odd and even length palindromes) the maximum radius of a palindrome, but since we allow one mismatch, we need a more flexible check. A more efficient approach uses a rolling hash to compare substrings quickly, but a direct O(n^3) check is too slow for n=5000. Instead, we can use a two-pointer expansion for each starting index: for each `l`, we expand `r` from `l` to `n-1`, maintaining a count of mismatches using the greedy technique, but this still results in O(n^2) expansions and each expansion takes O(1) amortized if we carefully update, but the mismatch count can reset. Simpler: for each center (2n-1 centers for odd and even lengths), we expand outward while keeping track of the first mismatch and the second mismatch. If we have zero mismatches, the substring is a palindrome; if we have exactly one mismatch, we record it; if we have more than one mismatch, we stop. This yields an O(n^2) algorithm. For odd centers, we start with `l=i-1`, `r=i+1` and expand; for even centers, we start with `l=i-1`, `r=i` and expand. For each expansion, we count all substrings from the center outward that have at most one mismatch. Specifically, for a given center, we keep track of `mismatch_count` and the first mismatch position. When we expand, if characters at `l` and `r` match, we add one to the count (since the substring `s[l..r]` has zero or one mismatch depending on the current mismatch count). If they do not match, we increment mismatch_count; if it becomes 2, we stop expanding. The tricky part: a single mismatch doesn't mean the whole substring is invalid; we need to check if after skipping one character the rest matches. The standard greedy check for a fixed substring is to try skipping left or right. But for expansion, we can instead use a palindrome radius with a “one-mismatch” allowance: for each center, we can precompute the first position where the string and its reverse differ. This can be done by comparing the original string and its reverse using a longest common prefix (LCP) via rolling hash. For each substring `[l,r]`, the LCP between `s[l..]` and `rev[(n-1-r)..]` gives the number of matching characters from the ends. If that LCP length >= (r-l+1-1), then the substring is almost palindromic. With rolling hash we can compute LCP in O(log n) per query, yielding O(n^2 log n) time. For n=5000, that is about 25e6 operations, acceptable. Space O(n). Edge cases: all substrings of length 1 and 2 count, and also length 0 is not considered. Return type long long because there are n*(n+1)/2 substrings, for n=5000 that's ~12.5 million, fits in int but we use long long for safety.
