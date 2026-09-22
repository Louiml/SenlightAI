/*
Write a C++ function `int sequenceAlignmentScore(const std::string& x, const std::string& y, int matchScore, int mismatchPenalty, int gapPenalty)` that computes the optimal global alignment score between two DNA strings `x` and `y` (which may contain any characters, but typically A, C, G, T) using the standard Needleman-Wunsch dynamic programming algorithm. The score for aligning two equal characters is `matchScore` (positive), for two different characters is `-mismatchPenalty` (penalty for substitution), and for introducing a gap (either in `x` or `y`) is `-gapPenalty` per gap position. The function must return the maximum possible total score. The strings are indexed from 1 internally, but the function must handle empty strings correctly. The algorithm must fill a DP table of size `(x.size()+1) × (y.size()+1)` where `dp[i][j]` is the best score aligning the first `i` characters of `x` with the first `j` characters of `y`. Initialization: `dp[0][0]=0`, `dp[i][0] = -gapPenalty * i`, `dp[0][j] = -gapPenalty * j`. Recurrence: `dp[i][j] = max( dp[i-1][j-1] + (x[i-1]==y[j-1]? matchScore : -mismatchPenalty), dp[i-1][j] - gapPenalty, dp[i][j-1] - gapPenalty )`. Return `dp[x.size()][y.size()]`.
*/
#include <string>
#include <vector>
#include <algorithm>

// Compute the optimal global alignment score between two strings.
// matchScore: reward for matching characters (positive)
// mismatchPenalty: penalty for mismatching characters (positive, subtracted)
// gapPenalty: penalty per gap character (positive, subtracted)
int sequenceAlignmentScore(const std::string& x, const std::string& y,
                           int matchScore, int mismatchPenalty, int gapPenalty) {
    const int n = static_cast<int>(x.size());
    const int m = static_cast<int>(y.size());
    
    // dp[i][j] = best score aligning x[0..i-1] with y[0..j-1]
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));
    
    // Initialize first column: align prefix of x with gaps in y
    for (int i = 1; i <= n; ++i) {
        dp[i][0] = -gapPenalty * i;
    }
    // Initialize first row: align prefix of y with gaps in x
    for (int j = 1; j <= m; ++j) {
        dp[0][j] = -gapPenalty * j;
    }
    
    // Fill DP table
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            // Diagonal: match or mismatch
            int diag = dp[i-1][j-1] + (x[i-1] == y[j-1] ? matchScore : -mismatchPenalty);
            // Gap in y (skip character from x)
            int up = dp[i-1][j] - gapPenalty;
            // Gap in x (skip character from y)
            int left = dp[i][j-1] - gapPenalty;
            dp[i][j] = std::max({diag, up, left});
        }
    }
    
    return dp[n][m];
}
#include <cassert>

int main() {
    // Basic cases
    assert(sequenceAlignmentScore("", "", 2, 1, 1) == 0);
    assert(sequenceAlignmentScore("A", "", 2, 1, 1) == -1);
    assert(sequenceAlignmentScore("", "AC", 2, 1, 1) == -2);
    
    // Identical strings: all matches
    assert(sequenceAlignmentScore("ACGT", "ACGT", 3, 2, 1) == 12);
    
    // Completely different: best is all mismatches or gaps, but mismatch is cheaper here
    assert(sequenceAlignmentScore("AAA", "CCC", 5, 2, 1) == -6); // 3 mismatches * -2
    
    // Single character vs single character
    assert(sequenceAlignmentScore("A", "A", 4, 3, 2) == 4);
    assert(sequenceAlignmentScore("A", "G", 4, 3, 2) == -3);
    
    // Simple insertion/deletion case: score from matching one char and a gap
    // "A" vs "AT": best is match A then gap in x for T => 5 - 2 = 3
    assert(sequenceAlignmentScore("A", "AT", 5, 2, 2) == 3);
    
    // Longer test with known result (both strings same length, mix)
    // "AC" vs "AG": either match A (5) + mismatch C/G (-2) = 3, or use gaps.
    // With gap penalty 2, matching A (5) + mismatch (-2) = 3, gap approach: 5-2-2=1, so 3.
    assert(sequenceAlignmentScore("AC", "AG", 5, 2, 2) == 3);
    
    // Test with gap penalty higher than mismatch: prefer mismatch over gaps
    // "AA" vs "AT": match A (3) + mismatch (-1) = 2; gap approach: 3-5? gap=5, so 3-5-5=-7, so 2.
    assert(sequenceAlignmentScore("AA", "AT", 3, 1, 5) == 2);
    
    // Test with gap penalty lower: prefer gaps over mismatch
    // "A" vs "G": mismatch -5, or gap in x + match? Actually align A with _ and _ with G? Both gaps.
    // For length 1 each, either mismatch (-5) or gap in x and gap in y? That would be -2-2 = -4, worse. But actually
    // we align A with G directly: -5, or A with _ (gap in y) = -2 and then _ with G? That's two gaps in two steps? 
    // Actually one gap in y and then string lengths differ? No both length 1: only two options: mismatch -5 or gap in x (A-_) and gap in y (_-G)? That's two gaps => -4. So -4 > -5. So answer -4.
    assert(sequenceAlignmentScore("A", "G", 5, 5, 2) == -4);
    
    // Larger random-ish test (known answer via brute force not needed, but check consistency)
    // x="AGC", y="ATC": best: match A (5), mismatch G/C (-2), match C (5) => 8. Or gap options.
    assert(sequenceAlignmentScore("AGC", "ATC", 5, 2, 3) == 8);
    
    return 0;
}
// The solution uses a two-dimensional dynamic programming table `dp` of dimensions `(n+1) × (m+1)` where `n = x.size()` and `m = y.size()`. The base cases fill the first row and column with cumulative gap penalties, representing the cost of aligning the entire prefix of one string to gaps in the other. For each cell `(i,j)`, three possible moves are considered: a diagonal move (aligning `x[i-1]` and `y[j-1]`), a vertical move (gap in `y`), and a horizontal move (gap in `x`). The maximum of these three is stored. To avoid repeated ternary operations, we can use `std::max` with an initializer list. Edge cases include empty strings: if either is empty, the result is simply the gap penalty times the length of the other string. Also handle the case where both strings are empty (return 0). The algorithm runs in O(n·m) time and O(n·m) space. Since the problem only asks for the score, we do not need to reconstruct the alignment path, but the DP table itself is necessary. If memory optimization were required, we could reduce to O(min(n,m)) by keeping only two rows, but the specification does not demand that, so a full table is acceptable for clarity.
