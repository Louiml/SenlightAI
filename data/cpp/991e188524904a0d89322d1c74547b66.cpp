Write a C++ function `countPalindromicSubstrings` that takes a non-empty string `s` (containing only lowercase English letters) and returns the number of distinct substrings of `s` that are palindromes. A substring is defined as a contiguous sequence of characters within `s`; two substrings are considered different if they start or end at different positions, even if they have the same content. The function must be efficient for strings up to length 1000. For example, for input `"aaa"`, the palindromic substrings are `"a"` (positions 0,1,2), `"aa"` (positions 0-1, 1-2), and `"aaa"` (0-2), totaling 6.
// The solution uses dynamic programming with a memoized helper function that checks whether a substring `s[i..j]` is a palindrome. The helper `isPalindrome` first checks if the result for `(i,j)` has been computed (stored in a 2D DP table initialized to -1). If not, it performs a two-pointer comparison from both ends inward. If any characters mismatch, it stores `false`; otherwise, it stores `true`. The main function iterates over all possible start indices `i` and end indices `j` (where `j >= i`), calls the helper for each pair, and adds the boolean result (which converts to 1 or 0) to an accumulator. Edge cases: the empty substring is not considered (since `j >= i` always gives at least one character), and single-character substrings are always palindromes. The DP table has dimensions `(n+1) x (n+1)` but only indices `0..n-1` are used; extra row/column are harmless. The time complexity is O(n²) because each pair `(i,j)` is computed at most once, and the inner two-pointer check costs O(n) worst-case, but overall it's O(n³) in the worst case due to repeated checks; however, with memoization, the total is O(n³) in the worst case if many non-palindromes cause full scans, but since each pair is computed once, total is O(n³). The space complexity is O(n²) for the DP table.
#include <string>
#include <vector>

// Returns the number of palindromic substrings in the given string.
// Uses memoized palindrome checks to avoid redundant work.
int countPalindromicSubstrings(const std::string& s) {
    int n = static_cast<int>(s.size());
    if (n == 0) return 0;

    // dp[i][j] = -1 (unknown), 0 (not palindrome), 1 (palindrome)
    std::vector<std::vector<int>> dp(n, std::vector<int>(n, -1));

    // Helper as a lambda for recursion (captures dp, s, n)
    // Returns 1 if s[i..j] is a palindrome, else 0.
    auto isPal = [&](auto&& self, int i, int j) -> int {
        if (dp[i][j] != -1) {
            return dp[i][j];
        }
        int left = i, right = j;
        while (left < right) {
            if (s[left] != s[right]) {
                dp[i][j] = 0;
                return 0;
            }
            ++left;
            --right;
        }
        dp[i][j] = 1;
        return 1;
    };

    int count = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            count += isPal(isPal, i, j);
        }
    }
    return count;
}
#include <cassert>
#include <string>

// The solution function is defined above; this main tests it.
int main() {
    assert(countPalindromicSubstrings("a") == 1);
    assert(countPalindromicSubstrings("aa") == 3);       // "a","a","aa"
    assert(countPalindromicSubstrings("aaa") == 6);      // 3 singles, 2 doubles, 1 triple
    assert(countPalindromicSubstrings("abc") == 3);      // each single char
    assert(countPalindromicSubstrings("abba") == 6);     // singles:4, "bb", "abba"
    assert(countPalindromicSubstrings("racecar") == 10); // known count
    assert(countPalindromicSubstrings("ababa") == 9);    // 5 singles + "aba"(2) + "bab"(1) + "ababa"
    assert(countPalindromicSubstrings("aaaa") == 10);    // 4+3+2+1
    assert(countPalindromicSubstrings("efe") == 4);     // 3 singles + "efe"
    return 0;
}
