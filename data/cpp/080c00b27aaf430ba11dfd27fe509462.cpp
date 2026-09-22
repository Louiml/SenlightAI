// Given two DNA strings `S` and `T` (each containing only characters 'A', 'T', 'G', 'C'), a 4x4 substitution score matrix `d` (indexed by nucleotide type order A, T, G, C), and two gap penalties `A` (opening a gap) and `B` (extending an existing gap, with `B < A`), write a C++ function that computes the optimal global alignment score between `S` and `T` under an affine gap penalty model. The alignment allows matches, mismatches (scored by `d`), and gaps in either sequence. The score of a gap of length `L` is `-A - (L-1)*B` (i.e., first gap position costs `A`, each additional consecutive gap position costs `B`). The function should return the maximum possible alignment score. Inputs are given as: two strings (1-indexed in the original snippet, but your function can use 0-indexed internally), a 4x4 integer matrix `d` (row = character from `S`, column = character from `T`), and integers `A` and `B`. The function must handle empty strings (score 0 if both empty, otherwise gap costs for the non-empty string). The score matrix values may be negative or positive, and gap penalties are positive integers.
#include <cassert>
#include <vector>
#include <string>

// The solution function is declared here (hidden in test environment, assume available).

int main() {
    // Standard substitution matrix: identity match +5, mismatch -2
    std::vector<std::vector<int>> d = {
        { 5, -2, -2, -2},
        {-2,  5, -2, -2},
        {-2, -2,  5, -2},
        {-2, -2, -2,  5}
    };
    int A = 3, B = 1; // open gap cost 3, extend cost 1

    // Test 1: empty vs empty -> 0
    assert(optimalAlignmentScore("", "", d, A, B) == 0);

    // Test 2: empty vs "A" -> -3 (one gap open)
    assert(optimalAlignmentScore("", "A", d, A, B) == -3);

    // Test 3: same single character -> +5
    assert(optimalAlignmentScore("A", "A", d, A, B) == 5);

    // Test 4: "A" vs "T" -> -2 (mismatch)
    assert(optimalAlignmentScore("A", "T", d, A, B) == -2);

    // Test 5: "AT" vs "AT" -> +10
    assert(optimalAlignmentScore("AT", "AT", d, A, B) == 10);

    // Test 6: "AT" vs "A" -> best align A-A=5, then gap in T for T cost -3 => 2
    assert(optimalAlignmentScore("AT", "A", d, A, B) == 2);

    // Test 7: "A" vs "AT" -> same, 2
    assert(optimalAlignmentScore("A", "AT", d, A, B) == 2);

    // Test 8: "AAAA" vs "AA" -> two matches +5 each, two gaps in T: -3 -1 = -4 => 6
    assert(optimalAlignmentScore("AAAA", "AA", d, A, B) == 6);

    // Test 9: "AC" vs "CA" -> perhaps align A-A (5), C-C (5) would require one mismatch? Actually best is A vs C mismatch -2, then C vs A mismatch -2 => -4, or gap open: align A with gap (-3) + then C with A mismatch (-2) + gap? Better: align A-A (5) and C-C? No, characters reversed. So best is: S: A C, T: C A => align A-C (-2) and C-A (-2) = -4. Or align A-A (5) and gap for C, then gap for C? That would be -3-1 + 5 = 1, not better. So -4.
    assert(optimalAlignmentScore("AC", "CA", d, A, B) == -4);

    // Test 10: Longer test "AGT" vs "AGT" -> 15
    assert(optimalAlignmentScore("AGT", "AGT", d, A, B) == 15);

    return 0;
}
#include <string>
#include <vector>
#include <algorithm>
#include <climits>

// Compute optimal global alignment score with affine gap penalties.
// d is 4x4, order: A, T, G, C. S and T contain only these letters.
// A = gap open penalty, B = gap extend penalty (positive integers).
// Return the maximum alignment score.
int optimalAlignmentScore(const std::string& S, const std::string& T,
                          const std::vector<std::vector<int>>& d,
                          int A, int B) {
    int n = (int)S.size();
    int m = (int)T.size();
    const int NEG_INF = -1e9;
    
    // dp[i][j][x][y] where x,y are 0/1 flags:
    // [1][1] both matched, [1][0] gap in T, [0][1] gap in S.
    // Size (n+1) x (m+1) x 2 x 2.
    std::vector<std::vector<std::vector<std::vector<int>>>> dp(
        n+1, std::vector<std::vector<std::vector<int>>>(
            m+1, std::vector<std::vector<int>>(
                2, std::vector<int>(2, NEG_INF))));

    // Base cases
    dp[0][0][1][1] = 0;
    for (int j = 1; j <= m; ++j) {
        // All gaps in S: cost -A - (j-1)*B
        dp[0][j][0][1] = -A - (j-1)*B;
    }
    for (int i = 1; i <= n; ++i) {
        // All gaps in T: cost -A - (i-1)*B
        dp[i][0][1][0] = -A - (i-1)*B;
    }

    // Map character to index 0..3: A=0, T=1, G=2, C=3
    auto idx = [](char c) -> int {
        if (c == 'A') return 0;
        if (c == 'T') return 1;
        if (c == 'G') return 2;
        return 3; // 'C'
    };

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            int si = idx(S[i-1]);
            int tj = idx(T[j-1]);

            // State [1][1]: pair S[i] with T[j]
            int bestPrev = std::max({
                dp[i-1][j-1][1][1],
                dp[i-1][j-1][1][0],
                dp[i-1][j-1][0][1]
            });
            dp[i][j][1][1] = bestPrev + d[si][tj];

            // State [1][0]: gap in T, consume S[i]
            dp[i][j][1][0] = std::max({
                dp[i-1][j][1][1] - A,
                dp[i-1][j][1][0] - B,
                dp[i-1][j][0][1] - A
            });

            // State [0][1]: gap in S, consume T[j]
            dp[i][j][0][1] = std::max({
                dp[i][j-1][1][1] - A,
                dp[i][j-1][0][1] - B,
                dp[i][j-1][1][0] - A
            });
        }
    }

    return std::max({dp[n][m][1][1], dp[n][m][1][0], dp[n][m][0][1]});
}
// This is a classic dynamic programming problem with affine gap penalties. We define a DP table with four states per cell `(i,j)` representing the best score for aligning the first `i` characters of `S` with the first `j` characters of `T`, where:
// - State `[1][1]`: both `S[i]` and `T[j]` are aligned (i.e., we place a pair or substitute).
// - State `[1][0]`: `S[i]` is aligned with a gap in `T` (i.e., we have a gap in `T` at position `j` of the alignment, and the previous character in `S` was used, meaning we are extending a gap in `T`).
// - State `[0][1]`: `T[j]` is aligned with a gap in `S` (i.e., we have a gap in `S`).
//
// The recursion:
// - `dp[i][j][1][1]` = max of all four states at `(i-1, j-1)` plus `d[char(S[i])][char(T[j])]`.
// - `dp[i][j][1][0]` (gap in T, consuming S[i]) = max of:
//   - `dp[i-1][j][1][1] - A` (open a new gap in T)
//   - `dp[i-1][j][1][0] - B` (extend an existing gap in T)
//   - `dp[i-1][j][0][1] - A` (open a new gap after a gap in S? Actually this state means S[i] is paired with gap, so previous state could have been T[?]. The original snippet uses this, but careful: `dp[i-1][j][0][1]` means at `(i-1,j)` we had a gap in S, but now we are introducing a gap in T – that is allowed but it's a new gap, cost `-A`.
// - `dp[i][v][0][1]` (gap in S, consuming T[j]) = symmetric to above.
//
// Base cases:
// - `dp[0][0][1][1] = 0`
// - `dp[0][j][0][1] = -A - (j-1)*B` for `j>=1` (all gaps in S)
// - `dp[i][0][1][0] = -A - (i-1)*B` for `i>=1` (all gaps in T)
// - All other states at boundaries are `-INF` (unreachable).
//
// Answer is max of the three reachable states at `(n,m)` (states `[1][1]`, `[1][0]`, `[0][1]`). Note that `dp[n][m][0][0]` is not defined; only the three above are valid final states.
//
// Edge cases: empty strings (both or one), gap penalties with `B >= A` (but B < A is typical, still code handles it), and the possibility of negative alignment scores (since d can be negative). Use a large negative sentinel like `-1e9` for unreachable states.
//
// Time complexity: O(n*m) for states, each with constant transitions, so O(n*m) time and O(n*m) space. Space can be optimized to O(m) with rolling arrays, but here we'll present a straightforward O(n*m) version for clarity.
