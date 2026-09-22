Write a C++ function `alignStrings` that takes two DNA sequences (as `std::string` parameters, potentially containing only characters `A`, `C`, `G`, `T`), a gap penalty `gapPenalty` (positive integer), and a substitution matrix encoded as a `std::map<std::pair<char,char>, int>` (where the score for matching/mismatching two characters is provided for all 16 ordered pairs, e.g., key `{'A','C'}` gives the score for aligning `A` with `C`). The function must perform local sequence alignment (Smith–Waterman algorithm) and return a `std::string` containing the optimal local alignment score followed by a space, the aligned substring from the first sequence, a space, and the aligned substring from the second sequence, using `'-'` for gaps in both aligned substrings. If the best local alignment score is non-positive (i.e., 0 or negative), return only the string `"0"` (no aligned substrings). For ties, any optimal alignment is acceptable, but the aligned substrings must correspond to the returned score. The two input sequences may be empty, and the gap penalty is strictly positive. The function must handle arbitrary sequence lengths up to 1000 characters each, and the substitution matrix must always contain all 16 entries.

#include <cassert>
#include <string>
#include <map>
#include <utility>

// Test function's dependencies are already declared in the solution header.
int main() {
    // Simple scoring: match = 2, mismatch = -1
    std::map<std::pair<char,char>, int> scoring;
    const char bases[] = {'A','C','G','T'};
    for (char a : bases)
        for (char b : bases)
            scoring[{a,b}] = (a == b) ? 2 : -1;

    // Test 1: Simple local alignment with a common substring
    std::string result1 = alignStrings("ACGT", "TACG", 1, scoring);
    // Expected best alignment: "ACGT" vs "ACG" with score of 6 (3 matches)
    assert(result1 == "6 ACG ACG");

    // Test 2: Empty sequences -> score 0
    assert(alignStrings("", "", 1, scoring) == "0");

    // Test 3: One empty sequence -> score 0
    assert(alignStrings("AC", "", 1, scoring) == "0");

    // Test 4: Completely dissimilar sequences with high gap penalty -> score 0
    std::string result4 = alignStrings("AAA", "CCC", 10, scoring);
    // Best might be a single match? No match: scores are -1 or 0; with gap -10, best is 0.
    assert(result4 == "0");

    // Test 5: Exact match with no gaps
    std::string result5 = alignStrings("GCTA", "GCTA", 3, scoring);
    assert(result5 == "8 GCTA GCTA");

    // Test 6: Gap inside one sequence
    // seqA: "A" seqB: "AT"; align "A" with "A" and ignore T? Actually local: best is "A" with score 2
    std::string result6 = alignStrings("A", "AT", 5, scoring);
    assert(result6 == "2 A A");

    // Test 7: Multiple possible alignments; use explicit expected
    // seqA: "AG", seqB: "AC" -> a match A (2) + mismatch G/C (-1) = 1, but local best might be just A (2)
    std::string result7 = alignStrings("AG", "AC", 1, scoring);
    assert(result7 == "2 A A");

    // Test 8: Longer with gaps, gap penalty = 1
    // seqA: "ACGT", seqB: "AGT" -> best align A-C-G-T vs A-G-T with gap for C? Actually align A, gap, G, T -> score 2+2+2 - 1 = 5? Or A, C/G mismatch, G, T? That's 2-1+2+2=5. Both give 5.
    std::string result8 = alignStrings("ACGT", "AGT", 1, scoring);
    // We'll check it starts with "5 " and has aligned strings of length 4 and 4? Not fixed. Just check score.
    assert(result8.substr(0, 2) == "5 ");

    // Test 9: Negative mismatch in a region with no matches but positive gaps? Gaps are negative so best is 0.
    std::string result9 = alignStrings("AAA", "CCC", 0, scoring);
    // gap penalty 0, mismatches -1; local best would be a single -1? But we take max with 0, so 0.
    assert(result9 == "0");

    // Test 10: Check that aligned strings have same length and no invalid chars
    std::string result10 = alignStrings("AACC", "CCAA", 1, scoring);
    // Best local alignment might be "A" or "C" or "AA" etc. We just verify format.
    auto space1 = result10.find(' ');
    auto space2 = result10.find(' ', space1 + 1);
    assert(space1 != std::string::npos);
    assert(space2 != std::string::npos);
    assert(space2 - space1 - 1 > 0); // alignedA length > 0
    assert(result10.substr(space1+1, space2-space1-1).size() == result10.substr(space2+1).size());
}

#include <string>
#include <vector>
#include <map>
#include <algorithm>

// Perform Smith-Waterman local alignment on two DNA sequences.
// Returns "score alignedA alignedB" or "0" if best score is non-positive.
std::string alignStrings(const std::string& seqA, const std::string& seqB,
                         int gapPenalty,
                         const std::map<std::pair<char,char>, int>& subMatrix) {
    int n = static_cast<int>(seqA.size());
    int m = static_cast<int>(seqB.size());

    // DP table; dp[i][j] = max local alignment score ending at seqA[i-1], seqB[j-1]
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));

    int bestScore = 0;
    int bestI = 0;
    int bestJ = 0;

    // Fill DP table
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            int match = dp[i-1][j-1] + subMatrix.at({seqA[i-1], seqB[j-1]});
            int del = dp[i-1][j] - gapPenalty;
            int ins = dp[i][j-1] - gapPenalty;
            dp[i][j] = std::max({0, match, del, ins});

            if (dp[i][j] > bestScore) {
                bestScore = dp[i][j];
                bestI = i;
                bestJ = j;
            }
        }
    }

    if (bestScore <= 0) {
        return "0";
    }

    // Traceback from best cell
    std::string alignedA, alignedB;
    int i = bestI;
    int j = bestJ;
    while (i > 0 && j > 0 && dp[i][j] > 0) {
        if (dp[i][j] == dp[i-1][j-1] + subMatrix.at({seqA[i-1], seqB[j-1]})) {
            alignedA.push_back(seqA[i-1]);
            alignedB.push_back(seqB[j-1]);
            --i;
            --j;
        } else if (dp[i][j] == dp[i-1][j] - gapPenalty) {
            alignedA.push_back(seqA[i-1]);
            alignedB.push_back('-');
            --i;
        } else {
            alignedA.push_back('-');
            alignedB.push_back(seqB[j-1]);
            --j;
        }
    }

    // Reverse the aligned strings since we built them backwards
    std::reverse(alignedA.begin(), alignedA.end());
    std::reverse(alignedB.begin(), alignedB.end());

    return std::to_string(bestScore) + " " + alignedA + " " + alignedB;
}

// The solution uses the classic Smith–Waterman dynamic programming algorithm for local alignment. We create a 2D DP table `dp[i][j]` where `i` ranges from 0 to length of first sequence and `j` from 0 to length of second, initialized with 0 in the first row and column. For each cell `(i,j)` with `i>0` and `j>0`, we compute three candidate scores: match/mismatch from `dp[i-1][j-1]` plus `subMatrix[{seqA[i-1], seqB[j-1]}]`, deletion from `dp[i-1][j]` minus `gapPenalty`, and insertion from `dp[i][j-1]` minus `gapPenalty`. The cell value is the maximum of these three candidates and 0 (to allow restarting local alignment). We track the global maximum score and its coordinates. If the maximum score is 0 or negative, we return `"0"`. Otherwise, we trace back from the cell with the maximum score: while the current cell > 0, we move diagonally if that came from `dp[i-1][j-1]` (append both characters or gaps), up if it came from `dp[i-1][j]` (append `-` to sequence B and character from A), or left if from `dp[i][j-1]` (append character from B and `-` to A). After reaching a 0 cell, we reverse the two constructed strings (since we built them backwards) and format the result as `score + " " + alignedA + " " + alignedB`. Edge cases: empty sequences: the only possible alignment score is 0, so return `"0"`. Negative scores are never used because of the 0 restart; the result is always non-negative. Time complexity is O(n*m) for the DP fill and O(n+m) for traceback, total O(n*m). Space complexity is O(n*m) for the DP table. We must be careful with indexing and ensure we don't access out-of-bounds for the substitution matrix—the problem guarantees all 16 entries exist.
