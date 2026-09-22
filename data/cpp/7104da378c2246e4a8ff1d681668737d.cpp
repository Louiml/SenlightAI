// Given an integer `N` and a string `S` of length `N` consisting of lowercase English letters, write a C++ function `minimumChangesToNonDecreasing` that returns a `std::vector<std::string>` (or a suitable data structure) representing the minimum number of character changes needed to make the entire string non-decreasing (i.e., `S[0] <= S[1] <= ... <= S[N-1]`), and also lists, in order from index 0 to N-1, the optimal changes to make: for each index where you change the character, output a string of the form `"i c"` (index and new character, separated by a space). If multiple optimal solutions exist, produce the lexicographically smallest final string (tie-breaking by choosing the earliest possible change to a smaller character). The function should return a vector containing: first, the minimum number of changes as a string, followed by the list of change instructions (in increasing index order). For example, if no changes are needed, return just `{"0"}`.

We solve this using dynamic programming. Let `dp[i][x]` for `0 <= i <= N` and `0 <= x < 26` represent the minimum changes needed to make the suffix `S[i..N-1]` non-decreasing, assuming the character at position `i-1` (or the "previous" character) is exactly `'a'+x` (for `i=0`, there is no previous, so we just consider any starting character). The DP is computed backwards: `dp[N][x] = 0` for all `x` (empty suffix). For each `i` from N-1 down to 0, for each possible previous character `x`, we choose the next character `y >= x` (since non-decreasing) and incur cost `(y != S[i])` plus `dp[i+1][y]`. Take the minimum. The answer is the minimum over all starting `x` (i.e., `min(dp[0][x])`). After computing the DP, we reconstruct the optimal solution greedily from left to right: at each index `i` we try characters from the smallest possible (starting from `last` used at previous index, initially `'a'`) upward; pick the smallest `c` such that the cost `used + (c != S[i]) + dp[i+1][c-'a']` equals the minimum total cost. If `c != S[i]`, record a change as `"i c"`. Update `last` to `c`. This yields the lexicographically smallest final string because we always pick the smallest valid character at each position. Edge cases: `N=0` (empty string) – return `{"0"}`; when no changes are needed, the result vector contains only the cost string. Time complexity: O(N * 26 * 26) for DP plus O(N * 26) for reconstruction, which is O(N) with a constant factor of ~676, so effectively O(N). Space: O(N * 26) for DP table.

#include <vector>
#include <string>
#include <algorithm>
#include <climits>

// Given a string S of lowercase letters, return the minimum number of changes
// to make it non-decreasing, and the list of optimal changes (index and new char).
std::vector<std::string> minimumChangesToNonDecreasing(const std::string& S) {
    const int N = static_cast<int>(S.size());
    const int ALPHA = 26;
    const int INF = 1e9;

    // dp[i][x] = min changes for suffix S[i..N-1] if previous char is 'a'+x.
    std::vector<std::vector<int>> dp(N + 1, std::vector<int>(ALPHA, INF));
    for (int x = 0; x < ALPHA; ++x) {
        dp[N][x] = 0;
    }

    // Backward DP.
    for (int i = N - 1; i >= 0; --i) {
        for (int x = 0; x < ALPHA; ++x) {
            int best = INF;
            for (int y = x; y < ALPHA; ++y) {
                int cost = (y != (S[i] - 'a')) + dp[i + 1][y];
                best = std::min(best, cost);
            }
            dp[i][x] = best;
        }
    }

    int minTotal = *std::min_element(dp[0].begin(), dp[0].end());

    std::vector<std::string> result;
    result.push_back(std::to_string(minTotal));
    if (N == 0) {
        return result; // just "0"
    }

    // Reconstruct lexicographically smallest optimal string.
    int used = 0;
    int last = 0; // 'a'
    for (int i = 0; i < N; ++i) {
        for (int c = last; c < ALPHA; ++c) {
            if (used + (c != (S[i] - 'a')) + dp[i + 1][c] == minTotal) {
                if (c != (S[i] - 'a')) {
                    result.push_back(std::to_string(i) + " " + std::string(1, 'a' + c));
                    ++used;
                }
                last = c;
                break;
            }
        }
    }

    return result;
}

#include <cassert>
#include <string>
#include <vector>

// Include the function definition here (or link).

int main() {
    // Already non-decreasing
    {
        std::vector<std::string> r = minimumChangesToNonDecreasing("abcd");
        assert(r.size() == 1);
        assert(r[0] == "0");
    }
    // Simple change needed
    {
        std::vector<std::string> r = minimumChangesToNonDecreasing("ba");
        assert(r.size() == 2);
        assert(r[0] == "1");
        assert(r[1] == "0 b"); // change 'b' at index 0 to 'a'? But lexicographically smallest: change index 0 to 'a' -> "aa" cost 1.
        // Actually lexicographically smallest final string: "aa" from changing S[0]='b' to 'a' (since 'a' >= 'a'? Wait previous none; at index 0 we can choose any char. Smallest is 'a', so change index 0 to 'a' gives "aa".)
        // But our reconstruction starts with last='a'. At i=0, try c='a' first: cost = ( 'a' != 'b') + dp[1]['a'] . Need dp[1]['a'] for suffix "a" with previous 'a' -> dp[1]['a'] = 0 (since 'a' >= 'a'). So cost=1+0=1 = minTotal. So we change to 'a'. So output "0 a".
        assert(r[1] == "0 a");
    }
    // Mixed case
    {
        std::vector<std::string> r = minimumChangesToNonDecreasing("cba");
        assert(r[0] == "2");
        // Optimal: change to "aab" or "aac" etc? Let's compute: need non-decreasing length 3. Smallest lexicographic with 2 changes: change index0 to 'a', index1 to 'a'? Then "aab" – but "aab" has index1='a' and index2='b' so OK. But original index1='b', change to 'a' cost 1. index2='a', need >= 'a' so keep 'a'? But then "aaa" would be cost? change index0 to 'a' (1), index1 to 'a' (1), index2 keep 'a' (0) total 2, final "aaa". That's lexicographically smallest among cost 2. So changes: "0 a", "1 a". Our reconstruction: i=0, last='a', try c='a' -> cost 1 + dp[1]['a']? dp[1]['a'] for suffix "ba": previous 'a', need non-decreasing. At i=1, S[1]='b', we can choose y>='a': try y='a' cost (a!=b)+dp[2]['a'] =1+dp[2]['a'] for suffix "a" with prev 'a' =0 => total 1. So dp[1]['a']=1. So total cost =1+1=2 = minTotal. So change index0 to 'a'. Then i=1, last='a', try c='a' -> cost used=1 + (a!=b)+dp[2]['a'] =1+1+0=2 => matches. So "1 a". So vector: ["2","0 a","1 a"].
        assert(r.size() == 3);
        assert(r[1] == "0 a");
        assert(r[2] == "1 a");
    }
    // Empty string
    {
        std::vector<std::string> r = minimumChangesToNonDecreasing("");
        assert(r.size() == 1);
        assert(r[0] == "0");
    }
    // All same characters, no change needed but lexicographically smallest string already same
    {
        std::vector<std::string> r = minimumChangesToNonDecreasing("zzz");
        assert(r.size() == 1);
        assert(r[0] == "0");
    }
    // Multiple optimal solutions, we pick lexicographically smallest final string
    {
        std::vector<std::string> r = minimumChangesToNonDecreasing("az");
        // Already non-decreasing, cost 0
        assert(r.size() == 1 && r[0] == "0");
    }
    {
        std::vector<std::string> r = minimumChangesToNonDecreasing("za");
        // Change index0 to 'a' -> "aa" cost 1, or change index1 to 'z' -> "zz" cost 1, lexicographically smallest is "aa" so change index0 to 'a'.
        assert(r[0] == "1");
        assert(r[1] == "0 a");
    }
    return 0;
}
