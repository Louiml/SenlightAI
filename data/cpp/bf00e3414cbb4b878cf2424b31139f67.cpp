Write a C++ function named `minimumWordWrapCost` that takes a vector of positive integers representing word lengths (in order) and an integer `K` representing the maximum number of characters that can fit on a line. A line is formed by concatenating words with exactly one space between them; no line may exceed `K` characters (including spaces). The cost of a line is the square of the number of extra blank spaces at the end of that line (i.e., `(K - used_chars)^2`), except for the last line of the entire text, which has zero cost. The function must return the minimum possible total cost to arrange all words into lines, respecting the word order. Words cannot be split across lines, and every word must be placed. If no valid arrangement exists (e.g., a single word longer than `K`), return a sentinel value of `-1`. You may assume `K > 0` and the vector is non-empty. Provide a self-contained implementation.

The problem is a classic dynamic programming task similar to text justification with a squared penalty. We define `dp[i]` as the minimum cost to arrange words from index `i` to the end (0-based). The base case is when we start at the last word; the cost is 0 regardless of line length because it’s the last line. For any `i`, we try every possible `j >= i` such that words `i` through `j` can fit on one line (including the spaces). If they fit, the cost for that line is either 0 if `j` is the last word, or `(K - line_length)^2`; then we add `dp[j+1]` if `j+1` is valid. We take the minimum over all valid splits. We precompute a 2D cost matrix for efficiency, but an alternative is to compute on the fly. Key edge cases: (1) A word longer than `K` means impossible; return -1. (2) The last line always costs 0, so when `j == n-1`, the cost is 0. (3) If a split results in an invalid next state (i.e., `dp[j+1]` is INF), we skip. The time complexity is O(n^2) because we try all `i` and `j` pairs. The space complexity is O(n^2) for the cost matrix plus O(n) for DP, but we can reduce to O(n) by computing costs on the fly; however, for clarity we use the matrix. The algorithm correctly handles the sentinel -1 by initializing DP with a large sentinel (e.g., INT_MAX) and returning -1 if dp[0] remains large.

#include <vector>
#include <algorithm>
#include <climits>

// Compute the minimum total cost to format words into lines of length K.
// Returns -1 if impossible (a word longer than K exists).
int minimumWordWrapCost(const std::vector<int>& words, int K) {
    const int n = static_cast<int>(words.size());
    const long long INF = LLONG_MAX / 4; // Use large sentinel to avoid overflow

    // Early termination: impossible if any single word exceeds K
    for (int len : words) {
        if (len > K) return -1;
    }

    // dp[i] = minimal cost for words i..n-1
    std::vector<long long> dp(n, INF);

    // Process from the last word backwards
    for (int i = n - 1; i >= 0; --i) {
        long long lineLength = 0; // starts with no characters
        for (int j = i; j < n; ++j) {
            // Add the length of word j; if j>i, add a space before it
            if (j == i) {
                lineLength += words[j];
            } else {
                lineLength += 1 + words[j]; // space + word length
            }

            // If the line exceeds K, no longer can extend this line
            if (lineLength > K) break;

            long long lineCost;
            if (j == n - 1) {
                lineCost = 0; // Last line has no cost
            } else {
                long long extra = K - lineLength;
                lineCost = extra * extra;
            }

            // Combine with optimal cost for the remaining words
            if (j == n - 1) {
                dp[i] = std::min(dp[i], lineCost);
            } else if (dp[j + 1] != INF) {
                dp[i] = std::min(dp[i], lineCost + dp[j + 1]);
            }
        }
    }

    return (dp[0] >= INF) ? -1 : static_cast<int>(dp[0]);
}

#include <cassert>
#include <vector>

int main() {
    // Basic example: words of lengths 3,2,2,5 with K=6
    // Optimal: [3,2] cost (6- (3+1+2)=0)^2=0, [2,5] cost (6-(2+1+5)= -2?) actually 2+1+5=8>6 so not possible. Try [3,2,2] cost (6-(3+1+2+1+2)= -3? Actually 3+2+2+2 spaces? Let's compute: words: 3,2,2 => line length = 3+1+2+1+2 = 9 >6 invalid. So only valid splits: [3,2] then [2,5] invalid; [3] then [2,2,5] invalid. So must be [3,2,2] impossible. Wait, K=6, word lengths 3,2,2,5. The 5 alone is fine. So possible lines: [3,2] (length 3+1+2=6, cost 0), [2,5] (2+1+5=8>6) invalid, [2,2] (2+1+2=5, cost 1), [5] last line cost 0. So arrangement: [3,2] cost 0, [2,2] cost 1, [5] cost 0 => total 1. Another: [3] cost (6-3)^2=9, [2,2] cost1, [5] 0 => 10. So answer 1.
    assert(minimumWordWrapCost({3,2,2,5}, 6) == 1);

    // Single word, fits, last line cost 0
    assert(minimumWordWrapCost({5}, 10) == 0);

    // Single word exactly fits
    assert(minimumWordWrapCost({10}, 10) == 0);

    // Impossible: word longer than K
    assert(minimumWordWrapCost({11}, 10) == -1);

    // All words fit on one line (last line zero cost)
    assert(minimumWordWrapCost({1,2,3}, 10) == 0); // length=1+1+2+1+3=8 <=10, last line cost 0

    // Exactly K with multiple lines, no last line cost
    // K=5, words {2,2} => first line length=2+1+2=5 cost0, last line (whole) is last line? Actually n=2, if we put all on one line, that's last line cost 0. So answer 0.
    assert(minimumWordWrapCost({2,2}, 5) == 0);

    // Classic example: K=6, words {3,2,2,5} already tested. 
    // Another: K=6, words {3,3} => line length=3+1+3=7>6, so separate lines: [3] cost9, [3] last line 0 => total 9.
    assert(minimumWordWrapCost({3,3}, 6) == 9);

    // Three words K=5: {2,2,2} -> [2,2] length5 cost0, [2] last line cost0 => total 0. But could also [2] cost9, [2,2] last line? Actually n=3, last line is the last group. If we do [2] then [2,2] last line cost0, total9. So min is 0.
    assert(minimumWordWrapCost({2,2,2}, 5) == 0);

    // K=4, words {1,1,1,1} -> one line length=1+1+1+1+1+1+1? Actually 4 words: length = 1+1+1+1+1? Wait 4 words with 3 spaces: 1+1+1+1+1+1+1? No: 4*1 + 3*1 = 7 >4. So need multi-lines. Possible: [1,1] length3 cost1, [1,1] last line 0 => total1. Or [1] cost9, [1,1,1] length5>4 invalid. So answer 1.
    assert(minimumWordWrapCost({1,1,1,1}, 4) == 1);

    return 0;
}
