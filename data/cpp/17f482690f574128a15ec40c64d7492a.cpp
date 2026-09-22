/*
Implement a C++ function `minConversionCost` that takes two integers `xCost`, `yCost`, and a string `pattern` consisting of characters `'C'`, `'J'`, and `'?'`, and returns the minimum total cost required to replace every `'?'` with either `'C'` or `'J'` such that the final string contains no adjacent pair `"CJ"` or `"JC"` unless we pay a penalty. Specifically, each occurrence of an adjacent `"CJ"` (i.e., a `C` immediately followed by a `J`) costs `yCost`, and each occurrence of an adjacent `"JC"` costs `xCost`. The goal is to assign values to all question marks to minimize the sum of these penalties. The function must handle strings of length 1 to 10^5, with costs between 0 and 10^6, and must run efficiently.
*/

#include <bits/stdc++.h>
using namespace std;

// Returns the minimum total cost to replace '?' with 'C' or 'J' to minimize penalties.
// Penalty: "CJ" costs yCost, "JC" costs xCost.
long long minConversionCost(int xCost, int yCost, const string& pattern) {
    const long long INF = 1e18;
    int n = (int)pattern.size();
    if (n == 1) return 0; // no adjacent pairs possible

    // dp[i][0] = min cost for prefix ending at i with char 'C'
    // dp[i][1] = min cost for prefix ending at i with char 'J'
    vector<vector<long long>> dp(n, vector<long long>(2, INF));

    if (pattern[0] == 'C' || pattern[0] == '?') dp[0][0] = 0;
    if (pattern[0] == 'J' || pattern[0] == '?') dp[0][1] = 0;

    for (int i = 1; i < n; ++i) {
        for (int cur = 0; cur < 2; ++cur) {
            // Skip if current character is fixed and doesn't match cur
            if ((cur == 0 && pattern[i] == 'J') || (cur == 1 && pattern[i] == 'C')) continue;

            for (int prev = 0; prev < 2; ++prev) {
                long long cost = dp[i-1][prev];
                if (cost == INF) continue; // unreachable previous state

                if (prev == 0 && cur == 1) cost += yCost; // "CJ"
                else if (prev == 1 && cur == 0) cost += xCost; // "JC"

                dp[i][cur] = min(dp[i][cur], cost);
            }
        }
    }

    return min(dp[n-1][0], dp[n-1][1]);
}

#include <cassert>
#include <string>

int main() {
    // Basic cases
    assert(minConversionCost(2, 3, "CJ") == 3);
    assert(minConversionCost(2, 3, "JC") == 2);
    assert(minConversionCost(2, 3, "CC") == 0);
    assert(minConversionCost(2, 3, "JJ") == 0);
    assert(minConversionCost(2, 3, "C") == 0);
    assert(minConversionCost(2, 3, "?") == 0);

    // Single question mark, any choice cost 0
    assert(minConversionCost(5, 7, "?") == 0);

    // Fixed string with no '?' 
    assert(minConversionCost(1, 1, "CJJC") == 3); // CJ(+1) + JC(+1) + JC(+1) = 3

    // All question marks, choose best pattern (e.g., all C's gives 0)
    assert(minConversionCost(100, 100, "???") == 0);

    // Mixed choice to minimize cost
    // "C?J" -> best "CCJ" cost y=4, "CJJ" cost 4, "CJC" cost x=5+? no. Let's test:
    assert(minConversionCost(5, 4, "C?J") == 4); // C C J or C J J both give 4, C J C gives 5+? actually C J C has CJ (4) + JC (5) = 9, so 4 is correct

    // "J?C" -> best "JJC" cost x=5, "JCC" cost 5, "JCC" or "JJC"
    assert(minConversionCost(5, 4, "J?C") == 5); // both J JC? wait compute: JJC has JC(5) + (CC,JC?) actually J J C has JC (5)+ no more, so 5; JCC has JC(5)+CC(0)=5; JJC also 5. So 5.

    // Larger test
    assert(minConversionCost(1, 1, "??????????") == 0); // all same choice

    // Check all patterns with small size by brute force (conceptually) - here a few known
    assert(minConversionCost(10, 1, "C?J") == 1); // C C J has CJ cost 1, or C J J has CJ cost 1, so 1
    assert(minConversionCost(1, 10, "C?J") == 10); // C C J has CJ 10, C J J has CJ 10, C J C has CJ 10 + JC 1 = 11, so min 10

    // Mixed with fixed characters
    assert(minConversionCost(3, 4, "?CJ?") == 4); // e.g., C C J C -> CJ (4) + JC (3) =7, C C J J -> CJ 4, so min 4? Actually C C J J: CJ at positions 2-3 =4, other pairs CC, JJ, so total 4. C C J C gives CJ 4 + JC 3 =7. J C J J: JC 3 + CJ 4? wait J C (JC cost 3), C J (CJ cost 4) total 7. So answer 4.

    return 0;
}

// This is a classic dynamic programming problem on a sequence. Let `dp[i][state]` represent the minimum cost for the prefix of the string up to index `i` (inclusive), where `state = 0` means the character at position `i` is `C`, and `state = 1` means it is `J`. Initialize `dp[0][0] = 0` if the first character can be `C` (either `C` or `?`), and similarly `dp[0][1] = 0` if first char can be `J`. For each position `i` from 1 to n-1, we consider only valid assignments for position `i` (i.e., if the original character is fixed, we cannot assign the opposite). For each valid current state `j` at position `i`, we transition from every valid state `k` at position `i-1`, adding the penalty for the transition: if `k=0` (C) and `j=1` (J), add `yCost`; if `k=1` (J) and `j=0` (C), add `xCost`; otherwise add 0. Take the minimum over all `k`. The answer is the minimum of `dp[n-1][0]` and `dp[n-1][1]`, ignoring states that were not reachable (initialized to a large value). Edge cases: string of length 1 (answer is 0 since no adjacent pairs exist); if a position is fixed, we must skip the incompatible state; if both states are unreachable at the end (shouldn't happen with `?` alone), but we default to `INF`. Time complexity is O(n) because we have two states per position and two transitions each; space complexity can be reduced to O(1) if we only keep previous row, but an O(n) DP table is also acceptable. The large maximum value (e.g., `1e18`) prevents overflow since costs up to 1e6 and length up to 1e5 give max sum 1e11.
