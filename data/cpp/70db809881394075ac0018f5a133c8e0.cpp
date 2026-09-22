/*
Write a standalone C++ function `countDistinctSubstrings` that takes a null-terminated C-style string `s` (using 1-based indexing, i.e., `s[1]` is the first character) and returns the number of distinct substrings of `s`. The input string contains only lowercase English letters (`'a'`–`'z'`). The function must construct a suffix array and the LCP (longest common prefix) array using the algorithm provided in the snippet (prefix-doubling with counting sort), then compute the answer as `total substrings - sum of LCP`. The function should be robust for empty strings (return 0) and strings of length up to 10000. Do not modify the input string; make it a `const char*` parameter. The reference solution must not use any global variables except constants.
*/

#include <bits/stdc++.h>

// Count distinct substrings of a null-terminated string (1-based indexing).
// Returns number of distinct substrings. Handles empty string.
int countDistinctSubstrings(const char* s) {
    if (!s || s[1] == '\0') {
        return 0;
    }
    const int MAXN = 10005;
    int n = 0;
    while (s[n + 1] != '\0') ++n;  // length, ignoring s[0]

    // Arrays for suffix array construction
    static int sa[MAXN], rk[MAXN], tmp[MAXN], t1[MAXN], t2[MAXN];
    static int cnt1[MAXN], cnt2[MAXN], H[MAXN];  // H is LCP array

    // Initial sort by single character
    std::vector<std::pair<char,int>> A(n+1);
    for (int i = 1; i <= n; ++i) A[i] = {s[i], i};
    std::sort(A.begin()+1, A.end());
    for (int i = 1; i <= n; ++i) sa[i] = A[i].second;
    rk[sa[1]] = 1;
    for (int i = 2; i <= n; ++i) {
        rk[sa[i]] = rk[sa[i-1]];
        if (s[sa[i]] != s[sa[i-1]]) rk[sa[i]]++;
    }

    // Prefix-doubling
    for (int l = 1; rk[sa[n]] < n; l <<= 1) {
        for (int i = 0; i <= n; ++i) cnt1[i] = cnt2[i] = 0;
        for (int i = 1; i <= n; ++i) {
            cnt1[t1[i] = rk[i]]++;
            cnt2[t2[i] = (i + l <= n) ? rk[i + l] : 0]++;
        }
        for (int i = 1; i <= n; ++i) {
            cnt1[i] += cnt1[i-1];
            cnt2[i] += cnt2[i-1];
        }
        // Sort by second key
        for (int i = n; i >= 1; --i) {
            tmp[cnt2[t2[i]]--] = i;
        }
        // Sort by first key
        for (int i = n; i >= 1; --i) {
            sa[cnt1[t1[tmp[i]]]--] = tmp[i];
        }
        // Update ranks
        rk[sa[1]] = 1;
        for (int i = 2; i <= n; ++i) {
            rk[sa[i]] = rk[sa[i-1]];
            if (t1[sa[i]] != t1[sa[i-1]] || t2[sa[i]] != t2[sa[i-1]]) {
                rk[sa[i]]++;
            }
        }
    }

    // Compute LCP (H array) using Kasai's algorithm
    for (int i = 1, j = 0; i <= n; ++i) {
        j -= (j > 0);
        if (rk[i] > 1) {
            while (s[i + j] == s[sa[rk[i] - 1] + j]) ++j;
            H[rk[i]] = j;
        } else {
            j = 0;
            H[rk[i]] = 0;
        }
    }

    // Total substrings minus sum of LCP
    long long total = (long long)n * (n + 1) / 2;
    long long lcpSum = 0;
    for (int i = 1; i <= n; ++i) lcpSum += H[i];
    return (int)(total - lcpSum);
}

#include <cassert>
#include <iostream>

// Assume countDistinctSubstrings is defined above

int main() {
    // Empty string
    assert(countDistinctSubstrings("") == 0);
    // Single character
    assert(countDistinctSubstrings("a") == 1);
    // All same characters: only n distinct substrings (lengths 1..n)
    assert(countDistinctSubstrings("aaa") == 3);
    // "ab": substrings "a", "b", "ab" -> 3
    assert(countDistinctSubstrings("ab") == 3);
    // "aba": distinct substrings: "a","b","ab","ba","aba" -> 5
    assert(countDistinctSubstrings("aba") == 5);
    // "abc": all 6 possible substrings are distinct
    assert(countDistinctSubstrings("abc") == 6);
    // "banana": known answer is 15
    assert(countDistinctSubstrings("banana") == 15);
    // "mississippi": known answer is 53
    assert(countDistinctSubstrings("mississippi") == 53);
    // "abcdabcd": distinct substrings? Total = 36, but duplicates exist; compute manually:
    // Length1: a,b,c,d -> 4
    // Length2: ab,bc,cd,da,ab,bc,cd -> distinct: ab,bc,cd,da -> 4
    // Length3: abc,bcd,cda,dab,abc,bcd -> distinct: abc,bcd,cda,dab -> 4
    // Length4: abcd,bcda,cdab,dabc,abcd -> distinct: abcd,bcda,cdab,dabc -> 4
    // Length5: abcda,bcdab,cdabc,dabcd -> 4
    // Length6: abcdab,bcdabc,cdabcd -> 3
    // Length7: abcdabc,bcdabcd -> 2
    // Length8: abcdabcd -> 1
    // Total = 4+4+4+4+4+3+2+1 = 26
    assert(countDistinctSubstrings("abcdabcd") == 26);
    // Stress test: 1000 'a's -> should be 1000
    std::string big(1000, 'a');
    std::string bigStr = " " + big; // shift to 1-indexed
    assert(countDistinctSubstrings(bigStr.c_str()) == 1000);
    std::cout << "All tests passed!\n";
    return 0;
}

// The number of distinct substrings of a string of length `n` is `n*(n+1)/2 - sum(LCP[i])` for `i=1..n`, where `LCP[i]` is the length of the longest common prefix between suffix `sa[i]` and suffix `sa[i-1]`. The suffix array is built using prefix-doubling: start with ranks based on single characters, then for each length `l` double the comparison length by using two keys: `rk[i]` and `rk[i+l]` (or 0 if out of bounds). Counting sort is applied twice (first on second key, then on first key) to sort suffixes by these pairs. After sorting, new ranks are assigned, repeating until all ranks are unique. The LCP array is then computed using Kasai's algorithm: for each suffix position `i` in the original order, keep a running `j` (decrementing by 1 each time), and advance `j` while characters match between suffix `i` and the suffix right before it in the suffix array order. Edge cases: empty string (return 0), single character (answer 1), all same characters (answer is `n` because only one distinct substring of each length). Time complexity: `O(n log n)` from the doubling (each iteration sorts in `O(n)` via counting sort, and there are `O(log n)` iterations), plus `O(n)` for LCP computation. Space complexity: `O(n)` for arrays.
