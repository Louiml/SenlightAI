// Write a C++ function `findLCSLengthAndSequence` that takes two C-style character arrays (`const char* X` and `const char* Y`) along with their respective lengths (`int m` and `int n`), computes the length of their Longest Common Subsequence (LCS), and also returns the actual LCS as a `std::string`. The function should return a `std::pair<int, std::string>` where the first element is the LCS length and the second is the LCS string itself. If multiple LCSs exist, return any one of them. The function must not modify the input arrays, must handle empty strings (returning length 0 and an empty string), and should be self-contained (no global variables). The solution must be efficient for moderate input sizes (e.g., up to 1000 characters each).

// The problem is the classic Longest Common Subsequence dynamic programming problem. We create a 2D table `dp` of size `(m+1) x (n+1)` where `dp[i][j]` stores the length of the LCS of the prefixes `X[0..i-1]` and `Y[0..j-1]`. The recurrence is: if `X[i-1] == Y[j-1]` then `dp[i][j] = 1 + dp[i-1][j-1]`; otherwise `dp[i][j] = max(dp[i-1][j], dp[i][j-1])`. The base cases are all `dp[i][0] = 0` and `dp[0][j] = 0`. After filling the table, `dp[m][n]` gives the LCS length. To reconstruct one LCS, we start from `(m,n)` and trace back: if the characters match, we include that character and move diagonally up-left; otherwise we move in the direction of the larger value (up or left). If both are equal, we can choose either (for determinism, we could prefer going up). This reconstruction yields a valid LCS. Time complexity is O(mn) for both filling the table and reconstruction, and space complexity is O(mn) for the table. Edge cases include one or both strings empty, and cases where the LCS is empty (length 0). The function must be `const`-correct, meaning it does not modify the inputs. We'll use `std::vector<std::vector<int>>` for dynamic allocation to avoid manual memory management.

#include <string>
#include <vector>
#include <utility>
#include <algorithm>

// Returns a pair: first is the length of the longest common subsequence,
// second is one possible LCS string.
std::pair<int, std::string> findLCSLengthAndSequence(const char* X, int m, const char* Y, int n) {
    // dp[i][j] = length of LCS of X[0..i-1] and Y[0..j-1]
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (X[i - 1] == Y[j - 1]) {
                dp[i][j] = 1 + dp[i - 1][j - 1];
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    // Reconstruct one LCS
    std::string lcs;
    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (X[i - 1] == Y[j - 1]) {
            lcs.push_back(X[i - 1]);
            --i;
            --j;
        } else if (dp[i - 1][j] >= dp[i][j - 1]) {
            --i;
        } else {
            --j;
        }
    }

    // The string was built backwards; reverse it
    std::reverse(lcs.begin(), lcs.end());

    return {dp[m][n], lcs};
}

#include <cassert>
#include <string>
#include <utility>

// function declaration (assume the solution is included)
std::pair<int, std::string> findLCSLengthAndSequence(const char* X, int m, const char* Y, int n);

int main() {
    // Test case 1: classic example from the snippet
    const char X1[] = "ABCBDAB";
    const char Y1[] = "BDCABA";
    auto res1 = findLCSLengthAndSequence(X1, 7, Y1, 6);
    assert(res1.first == 4);
    assert(res1.second == "BCBA" || res1.second == "BDAB" || res1.second == "BCAB"); // any valid LCS

    // Test case 2: empty strings
    const char X2[] = "";
    const char Y2[] = "ABC";
    auto res2 = findLCSLengthAndSequence(X2, 0, Y2, 3);
    assert(res2.first == 0);
    assert(res2.second.empty());

    // Test case 3: identical strings
    const char X3[] = "HELLO";
    const char Y3[] = "HELLO";
    auto res3 = findLCSLengthAndSequence(X3, 5, Y3, 5);
    assert(res3.first == 5);
    assert(res3.second == "HELLO");

    // Test case 4: no common characters
    const char X4[] = "abc";
    const char Y4[] = "def";
    auto res4 = findLCSLengthAndSequence(X4, 3, Y4, 3);
    assert(res4.first == 0);
    assert(res4.second.empty());

    // Test case 5: single character match
    const char X5[] = "x";
    const char Y5[] = "abxcd";
    auto res5 = findLCSLengthAndSequence(X5, 1, Y5, 5);
    assert(res5.first == 1);
    assert(res5.second == "x");

    // Test case 6: multiple matches and length must match LCS length
    const char X6[] = "AGGTAB";
    const char Y6[] = "GXTXAYB";
    auto res6 = findLCSLengthAndSequence(X6, 6, Y6, 7);
    assert(res6.first == 4);
    assert((int)res6.second.size() == res6.first); // length consistency

    // Test case 7: all characters same but different lengths
    const char X7[] = "AAA";
    const char Y7[] = "AAAA";
    auto res7 = findLCSLengthAndSequence(X7, 3, Y7, 4);
    assert(res7.first == 3);
    assert(res7.second == "AAA");

    return 0;
}
