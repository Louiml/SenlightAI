Write a C++ function that takes two strings, `word1` and `word2`, and returns the minimum number of single-character edit operations needed to transform `word1` into `word2`. Allowed operations are: insert a character into `word1`, delete a character from `word1`, and replace a character in `word1` with a different character. The function must handle empty strings (including both empty), strings with identical characters, and strings of arbitrary length (up to reasonable memory limits). The function signature must be `int editDistance(const std::string& word1, const std::string& word2)`.

// The classic solution is dynamic programming (or memoised recursion, as shown). The key idea is to define `dp[i][j]` as the minimum operations to convert the prefix `word1[0..i-1]` into `word2[0..j-1]`. Base cases: if one string is empty, the only way is to insert or delete all characters of the other, so `dp[i][0] = i` and `dp[0][j] = j`. For non-empty prefixes, if the current characters are equal (`word1[i-1] == word2[j-1]`), then no operation is needed, so `dp[i][j] = dp[i-1][j-1]`. If they differ, we consider three possibilities: replace (`dp[i-1][j-1] + 1`), delete from `word1` (`dp[i-1][j] + 1`), or insert into `word1` (equivalent to deleting from `word2`, `dp[i][j-1] + 1`), and take the minimum. This yields a 2D table of size `(len1+1) x (len2+1)`. Time complexity is \(O(n m)\) and space complexity is \(O(n m)\) for the full table, but we can optimize space to \(O(m)\) by keeping only the previous row since each row depends only on the previous row and the current row. Edge cases include one or both strings empty, identical strings (output 0), and strings where only insertions/deletions are needed (e.g., "abc" -> "ab" requires 1 deletion). The algorithm correctly handles all characters including digits, spaces, and punctuation.

#include <string>
#include <vector>
#include <algorithm>

// Returns the minimum number of insert/delete/replace operations to convert word1 to word2.
int editDistance(const std::string& word1, const std::string& word2) {
    size_t n = word1.size();
    size_t m = word2.size();

    // dp[j] represents edit distance for current row (prefix of word1 up to i) and prefix of word2 up to j.
    // Initialize for i = 0: converting empty word1 to word2[0..j-1] costs j insertions.
    std::vector<int> dp(m + 1, 0);
    for (size_t j = 0; j <= m; ++j) {
        dp[j] = static_cast<int>(j);
    }

    // Process each character of word1.
    for (size_t i = 1; i <= n; ++i) {
        int prev = dp[0]; // stores dp[i-1][j-1]
        dp[0] = static_cast<int>(i); // dp[i][0] = i (delete all from word1)

        for (size_t j = 1; j <= m; ++j) {
            int temp = dp[j]; // store dp[i-1][j] before overwriting

            if (word1[i - 1] == word2[j - 1]) {
                dp[j] = prev; // no operation needed
            } else {
                dp[j] = 1 + std::min({
                    prev,          // replace
                    dp[j - 1],     // insert (into word1)
                    temp           // delete (from word1)
                });
            }

            prev = temp; // update prev to dp[i-1][j] for next iteration
        }
    }

    return dp[m];
}

#include <cassert>
#include <string>

// Forward declaration of the function to test
int editDistance(const std::string& word1, const std::string& word2);

int main() {
    // Basic examples
    assert(editDistance("horse", "ros") == 3);
    assert(editDistance("intention", "execution") == 5);

    // Identical strings
    assert(editDistance("abc", "abc") == 0);

    // One empty string
    assert(editDistance("", "") == 0);
    assert(editDistance("abc", "") == 3);
    assert(editDistance("", "xyz") == 3);

    // String where only insertions/deletions needed
    assert(editDistance("abc", "ab") == 1);
    assert(editDistance("ab", "abc") == 1);

    // Single character strings
    assert(editDistance("a", "a") == 0);
    assert(editDistance("a", "b") == 1);

    // Other edge cases
    assert(editDistance("kitten", "sitting") == 3);
    assert(editDistance("flaw", "lawn") == 2);
    assert(editDistance("park", "spake") == 3);

    return 0;
}
