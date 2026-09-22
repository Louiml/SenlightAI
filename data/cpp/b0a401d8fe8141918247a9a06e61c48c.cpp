// You are given two sequences of city names, each on its own line, where a sequence may contain spaces and may be empty. A traveler wants to visit cities in the same relative order as they appear in the first sequence while also respecting the order in the second sequence, meaning the chosen cities must form a common subsequence between the two sequences — but the traveler can only visit each city once and must preserve order. Write a C++ function `int longestCommonSubsequenceLength(const std::string& seq1, const std::string& seq2)` that returns the maximum number of cities the traveler can visit. For example, if seq1 = "Paris London Rome" and seq2 = "London Paris Rome", the maximum is 2 (e.g., "Paris Rome" or "London Rome"). The function must handle empty sequences, sequences with repeated names, and very long inputs (up to 10^3 characters) efficiently. Do not read from standard input; the function receives the two full strings as arguments.

#include <cassert>
#include <string>

// Forward declaration of the solution function (assume it's declared above).
int longestCommonSubsequenceLength(const std::string& seq1, const std::string& seq2);

int main() {
    // Basic cases
    assert(longestCommonSubsequenceLength("Paris London Rome", "London Paris Rome") == 2);
    assert(longestCommonSubsequenceLength("A B C", "D E F") == 0);
    assert(longestCommonSubsequenceLength("", "") == 0);
    assert(longestCommonSubsequenceLength("X", "") == 0);
    assert(longestCommonSubsequenceLength("", "Y") == 0);

    // Single token matches
    assert(longestCommonSubsequenceLength("City", "City") == 1);
    assert(longestCommonSubsequenceLength("City", "Town City") == 1);

    // Repeated tokens, order matters
    assert(longestCommonSubsequenceLength("A A B", "A B A") == 2); // e.g., "A A" or "A B"
    assert(longestCommonSubsequenceLength("A B C D", "B C D A") == 3); // B C D

    // All identical sequences
    assert(longestCommonSubsequenceLength("X Y Z", "X Y Z") == 3);
    assert(longestCommonSubsequenceLength("X Y Z", "X Y Z W") == 3);

    // Leading/trailing/multiple spaces
    assert(longestCommonSubsequenceLength("   alpha   beta  ", "beta alpha") == 1);
    assert(longestCommonSubsequenceLength(" a b ", " a b ") == 2);

    // Longer sequences with mixed matches
    assert(longestCommonSubsequenceLength("abc def ghi jkl", "def abc jkl ghi") == 2); // abc jkl or def ghi

    return 0;
}

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

// Splits a string by spaces, ignoring leading/trailing/multiple spaces.
static std::vector<std::string> splitTokens(const std::string& s) {
    std::istringstream iss(s);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

// Returns the length of the longest common subsequence of two token sequences.
int longestCommonSubsequenceLength(const std::string& seq1, const std::string& seq2) {
    const std::vector<std::string> a = splitTokens(seq1);
    const std::vector<std::string> b = splitTokens(seq2);
    const int n = static_cast<int>(a.size());
    const int m = static_cast<int>(b.size());

    // dp[i][j] = LCS of a[i..n-1] and b[j..m-1] using bottom-up.
    // We use n+1 x m+1 table, with dp[n][j] = 0 and dp[i][m] = 0.
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));

    for (int i = n - 1; i >= 0; --i) {
        for (int j = m - 1; j >= 0; --j) {
            if (a[i] == b[j]) {
                dp[i][j] = dp[i + 1][j + 1] + 1;
            } else {
                dp[i][j] = std::max(dp[i + 1][j], dp[i][j + 1]);
            }
        }
    }
    return dp[0][0];
}

// This is the classic Longest Common Subsequence (LCS) problem applied to sequences of tokens (city names separated by spaces). The approach is dynamic programming on two indices `i` and `j` representing positions in the two sequences. A recursive function with memoization (top-down) or an iterative 2D table (bottom-up) can be used. For each pair `(i, j)`: if the token at position `i` in seq1 equals the token at position `j` in seq2, then the answer is 1 + LCS(i+1, j+1). Otherwise, it is the maximum of LCS(i+1, j) and LCS(i, j+1). The base case is when either index reaches the end of its sequence, returning 0.  
// Important edge cases: empty sequences (return 0), sequences with no common tokens (return 0), sequences with all tokens equal (return min length), repeated tokens (they are treated as distinct positions, so ties are handled correctly). The function must first split both strings into token vectors by spaces, ignoring leading/trailing/multiple spaces.  
// Time complexity: O(n*m) where n and m are token counts, with O(n*m) space for the DP table if using bottom-up, or O(n*m) for memoization plus recursion stack depth O(n+m). Space can be optimized to O(min(n,m)) but for simplicity and clarity, a full 2D table is acceptable since n,m ≤ 10^3 yields at most 10^6 entries.
