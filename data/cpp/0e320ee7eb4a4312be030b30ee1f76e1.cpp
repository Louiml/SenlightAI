Write a C++ function named `isInterleaved` that takes three strings `a`, `b`, and `c` and returns `true` if `c` can be formed by interleaving the characters of `a` and `b` while preserving the original order of characters within each string, and `false` otherwise. An interleaving means that the characters of `a` and `b` are merged into `c` without changing the relative order of characters within either source string, and every character of both `a` and `b` must be used exactly once. For example, if `a = "abc"`, `b = "def"`, then `c = "adbcef"` is a valid interleaving, but `c = "abdecf"` is not because the order in `b` is violated. The function should handle empty strings, strings with duplicate characters, and large inputs efficiently. It should not use recursion—implement a dynamic programming (DP) solution using a 2D table where `dp[i][j]` indicates whether the first `i+j` characters of `c` can be formed by interleaving the first `i` characters of `a` and the first `j` characters of `b`. The DP table should be filled from the bottom-right corner to the top-left, considering three cases: when the current character of `c` matches both `a[i-1]` and `b[j-1]`, when it matches only `a[i-1]`, and when it matches only `b[j-1]`. Handle base cases where `i` or `j` reaches the length of its string by checking if the remaining characters of the other string match the remaining suffix of `c`. The time complexity should be `O(m*n)` and the space complexity `O(m*n)`, where `m` and `n` are the lengths of `a` and `b`.
#include <cassert>
#include <string>

// Declaration of the function to test (assume it is defined above or included).
bool isInterleaved(const std::string& a, const std::string& b, const std::string& c);

int main() {
    // Basic cases
    assert(isInterleaved("abc", "def", "adbcef") == true);
    assert(isInterleaved("abc", "def", "abdecf") == false);

    // One string empty
    assert(isInterleaved("abc", "", "abc") == true);
    assert(isInterleaved("", "abc", "abc") == true);
    assert(isInterleaved("abc", "", "ab") == false);

    // Both empty
    assert(isInterleaved("", "", "") == true);

    // Length mismatch
    assert(isInterleaved("a", "b", "abx") == false);

    // Duplicate characters and ambiguity
    assert(isInterleaved("aab", "aac", "aaabac") == true);
    assert(isInterleaved("aab", "aac", "aaaabc") == false);

    // All characters same
    assert(isInterleaved("aaa", "aaa", "aaaaaa") == true);
    assert(isInterleaved("aaa", "aaa", "aaaaab") == false);

    // Larger example
    assert(isInterleaved("ab", "cd", "acbd") == true);
    assert(isInterleaved("ab", "cd", "acdb") == false);

    // Case where matches both but only one path works
    assert(isInterleaved("aa", "ab", "aaba") == true);
    assert(isInterleaved("aa", "ab", "abaa") == true);

    return 0;
}
#include <string>
#include <vector>

// Returns true if c can be formed by interleaving a and b preserving order.
bool isInterleaved(const std::string& a, const std::string& b, const std::string& c) {
    const int m = static_cast<int>(a.size());
    const int n = static_cast<int>(b.size());
    const int p = static_cast<int>(c.size());

    // Length condition must hold for any valid interleaving.
    if (m + n != p) {
        return false;
    }

    // dp[i][j] = true if first i+j chars of c interleave first i of a and first j of b.
    std::vector<std::vector<bool>> dp(m + 1, std::vector<bool>(n + 1, false));

    // Fill DP table from bottom-right to top-left.
    for (int i = m; i >= 0; --i) {
        for (int j = n; j >= 0; --j) {
            int k = i + j; // current position in c

            if (k == p) {
                // Reached end of c; since i+j == p, both strings fully used? Actually if i<m or j<n, length mismatch already handled, but for safety:
                // If we are here, i+j == p, so m+n == p implies i==m and j==n, so true.
                dp[i][j] = (i == m && j == n);
            } else if (i == m) {
                // a is fully consumed; check remaining b[j..] against c[k..]
                bool ok = true;
                int jj = j;
                int kk = k;
                while (jj < n && kk < p) {
                    if (b[jj] != c[kk]) {
                        ok = false;
                        break;
                    }
                    ++jj;
                    ++kk;
                }
                // The while loop consumes equal lengths; if b not fully consumed but kk reached p, impossible.
                if (jj != n || kk != p) ok = false;
                dp[i][j] = ok;
            } else if (j == n) {
                // b is fully consumed; check remaining a[i..] against c[k..]
                bool ok = true;
                int ii = i;
                int kk = k;
                while (ii < m && kk < p) {
                    if (a[ii] != c[kk]) {
                        ok = false;
                        break;
                    }
                    ++ii;
                    ++kk;
                }
                if (ii != m || kk != p) ok = false;
                dp[i][j] = ok;
            } else {
                // General case: both a and b have remaining characters.
                char ca = a[i];
                char cb = b[j];
                char cc = c[k];

                if (ca == cc && cb == cc) {
                    dp[i][j] = dp[i + 1][j] || dp[i][j + 1];
                } else if (ca == cc) {
                    dp[i][j] = dp[i + 1][j];
                } else if (cb == cc) {
                    dp[i][j] = dp[i][j + 1];
                } else {
                    dp[i][j] = false;
                }
            }
        }
    }

    return dp[0][0];
}
// The problem is the classic "Interleaving String" problem. The main insight is to use dynamic programming to avoid exponential recursion. We define a 2D boolean table `dp` of size `(m+1) x (n+1)`, where `dp[i][j]` means that the prefix of `c` of length `i+j` can be formed by interleaving the first `i` characters of `a` and the first `j` characters of `b`. We fill the table from the bottom-right corner (`i=m`, `j=n`) toward the top-left (`i=0`, `j=0`), because the recurrence depends on `dp[i+1][j]` and `dp[i][j+1]`. 
//
// First, we check the length condition: if `a.size() + b.size() != c.size()`, return `false` immediately. Then we initialize the base cases: when `i == m` and `j == n`, `k = i+j` equals the length of `c`, so `dp[m][n] = true`. For cells where `i == m` (all of `a` consumed), we need to check if the remaining suffix of `b` (from index `j` to end) equals the corresponding suffix of `c` (from index `i+j` to end). If yes, `dp[i][j] = true`, else `false`. Similarly for `j == n`. For general cells `(i,j)` with `i < m` and `j < n`, let `k = i+j`. If `a[i] == c[k]` and `b[j] == c[k]`, then `dp[i][j] = dp[i+1][j] || dp[i][j+1]`. If only `a[i]` matches, then `dp[i][j] = dp[i+1][j]`. If only `b[j]` matches, then `dp[i][j] = dp[i][j+1]`. Otherwise, `dp[i][j] = false`. 
//
// Edge cases: empty `a` or `b`; characters that match both but only one path leads to success; large inputs requiring `O(m*n)` time; and the length check. The solution uses `std::vector<std::vector<bool>>` for the DP table. Time complexity is `O(m*n)` because each cell is computed in constant time after the base cases. Space complexity is also `O(m*n)` due to the table.
