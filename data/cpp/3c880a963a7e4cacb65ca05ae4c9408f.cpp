Write a C++ function `minInsertionsToMakePalindrome(const std::string& s)` that takes a non-empty string containing lowercase English letters and returns the minimum number of character insertions needed to transform the string into a palindrome. You may insert characters anywhere in the string, including at the beginning and end. For example, for the string `"abca"`, the minimum insertions is 1 (insert `'b'` after `'a'` at the end to get `"abcba"`). The function must compute this efficiently using dynamic programming based on the longest palindromic subsequence (LPS). Note that the input string may have repeated characters and can be of arbitrary length (but assume it fits in memory).

// The key insight is that to make a string a palindrome with minimal insertions, we keep the longest palindromic subsequence (LPS) intact and insert all other characters in their mirrored positions. Thus, the minimum number of insertions equals `length(s) - length(LPS(s))`. The LPS of a string can be found by computing the longest common subsequence (LCS) between the original string and its reverse. This is because a palindrome equals its reverse, so the longest palindromic subsequence corresponds to the longest subsequence that appears in both the string and its reversed version. We solve LCS using dynamic programming with a rolling array to save space: maintain two vectors `prev` and `curr` of size `len+1`, where `prev[j]` represents the LCS length for the first `i-1` characters of the original and the first `j` characters of the reversed string. For each character pair, if equal we add 1 to `prev[j-1]`; otherwise we take the max of `prev[j]` (skip original char) and `curr[j-1]` (skip reversed char). After processing all rows, the answer is `s.size() - prev[rs.size()]`. Edge cases: if the string is already a palindrome, LPS length equals length, so insertions = 0; if the string has length 1, insertions = 0. Time complexity is O(n^2) where n is string length, and space complexity is O(n) due to the two rolling arrays.

#include <string>
#include <vector>
#include <algorithm>

// Compute the minimum number of insertions to make the input string a palindrome.
int minInsertionsToMakePalindrome(const std::string& s) {
    const int n = static_cast<int>(s.size());
    if (n <= 1) return 0;

    // Reverse the string to use LCS on s and its reverse.
    std::string rs = s;
    std::reverse(rs.begin(), rs.end());

    // prev[j] = LCS length for processed part of s and first j chars of rs.
    std::vector<int> prev(n + 1, 0), curr(n + 1, 0);

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (s[i - 1] == rs[j - 1]) {
                curr[j] = 1 + prev[j - 1];
            } else {
                curr[j] = std::max(prev[j], curr[j - 1]);
            }
        }
        std::swap(prev, curr);
        // Reset curr for next iteration by assigning zeros (since swap made prev old curr)
        std::fill(curr.begin(), curr.end(), 0);
    }

    // Minimum insertions = total length - length of LPS.
    return n - prev[n];
}

#include <cassert>
#include <string>

// Function declaration is above; just test it.
int main() {
    assert(minInsertionsToMakePalindrome("abca") == 1);
    assert(minInsertionsToMakePalindrome("racecar") == 0);
    assert(minInsertionsToMakePalindrome("a") == 0);
    assert(minInsertionsToMakePalindrome("ab") == 1);           // insert 'a' at end or 'b' at front
    assert(minInsertionsToMakePalindrome("abc") == 2);          // e.g., "abcba" needs 2 insertions
    assert(minInsertionsToMakePalindrome("abcd") == 3);         // e.g., "abcddcba" needs 3
    assert(minInsertionsToMakePalindrome("zzazz") == 0);        // already palindrome
    assert(minInsertionsToMakePalindrome("leetcode") == 5);     // known result
    assert(minInsertionsToMakePalindrome("g") == 0);
    assert(minInsertionsToMakePalindrome("madaam") == 1);       // insert 'd' to get "madam" or "madaam" -> actually "madam" has 'd' once, but "madaam" -> insert one 'd'? Let's check: "madaam" -> LPS is "maam" length 4, n=6 => 2 inserts. Wait, test carefully: "madaam" -> chars: m a d a a m. LPS? "maam" is subsequence? m(0), a(1), a(4), m(5) => "maam" length 4. Also "madam" is subsequence? m, a, d, a, m => length 5 (positions 0,1,2,3,5) -> actually yes, "madam" is a palindrome and a subsequence (remove one 'a' at position 4). So LPS length is 5, insertions = 6-5 =1. So assert should be 1.
    assert(minInsertionsToMakePalindrome("madaam") == 1);
    return 0;
}
