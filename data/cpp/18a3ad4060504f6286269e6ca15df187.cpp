Given a lowercase English string, write a C++ function `int minInsertionsToPalindrome(const std::string& s)` that returns the minimum number of character insertions required to make the string a palindrome. An insertion can place any character at any position. For example, for `"mbadm"`, the answer is `2` (e.g., insert `'m'` at the end and `'d'` at the beginning to get `"mbdadbm"`). Handle empty strings (return `0`) and strings with up to 1000 characters. The function must be efficient enough for the maximum input size.
// The problem is classic dynamic programming on substrings. Let `dp[i][j]` represent the minimum insertions needed to make the substring `s[i..j]` a palindrome. Base case: when `i >= j`, the substring is already a palindrome (either empty or a single character), so `dp[i][j] = 0`. Recurrence: if `s[i] == s[j]`, then the characters match, and the cost is `dp[i+1][j-1]` (we just need to fix the inner part). If they differ, we must insert either `s[j]` next to `s[i]` on the left (cost `1 + dp[i+1][j]`) or insert `s[i]` next to `s[j]` on the right (cost `1 + dp[i][j-1]`), and we take the minimum. We compute this bottom-up by increasing substring length. The answer is `dp[0][n-1]`. Edge cases: empty string (`n == 0`) returns `0`. Single character returns `0`. Time complexity is `O(n^2)`, space `O(n^2)` — suitable for `n <= 1000` (1e6 cells). The iterative approach avoids recursion depth issues.
#include <vector>
#include <algorithm>
#include <string>

// Returns the minimum number of insertions needed to make s a palindrome.
int minInsertionsToPalindrome(const std::string& s) {
    const int n = static_cast<int>(s.size());
    if (n <= 1) return 0;

    // dp[i][j] = min insertions for substring s[i..j] (inclusive).
    std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));

    // Build by increasing substring length.
    for (int len = 2; len <= n; ++len) {
        for (int i = 0; i + len - 1 < n; ++i) {
            int j = i + len - 1;
            if (s[i] == s[j]) {
                // If len == 2, dp[i+1][j-1] is 0 (i+1 > j-1), so fine.
                dp[i][j] = (i + 1 <= j - 1) ? dp[i + 1][j - 1] : 0;
            } else {
                // Insert a character to match either left or right.
                dp[i][j] = 1 + std::min(dp[i + 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[0][n - 1];
}
#include <cassert>

int main() {
    // Empty and single character.
    assert(minInsertionsToPalindrome("") == 0);
    assert(minInsertionsToPalindrome("a") == 0);
    // Already palindrome.
    assert(minInsertionsToPalindrome("aba") == 0);
    assert(minInsertionsToPalindrome("abba") == 0);
    // Standard cases.
    assert(minInsertionsToPalindrome("mbadm") == 2);
    assert(minInsertionsToPalindrome("leetcode") == 5);
    assert(minInsertionsToPalindrome("zzazz") == 0);
    assert(minInsertionsToPalindrome("abc") == 2);
    // Longer string.
    assert(minInsertionsToPalindrome("aabaa") == 0);
    assert(minInsertionsToPalindrome("abcdcba") == 0);
    assert(minInsertionsToPalindrome("abcde") == 4);
    // Case with mixed characters.
    assert(minInsertionsToPalindrome("abca") == 1);
    return 0;
}
