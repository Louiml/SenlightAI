Write a C++ function that takes a non-empty string and returns the minimum number of insertions needed to make it a palindrome. The function should be named `minInsertionsToPalindrome` and should accept the string as a `const std::string&` parameter. The result must be non-negative and the function must handle strings up to length 1000 efficiently. For example, given `"abca"`, the minimum insertions is 1 (insert `'b'` before `'a'` to form `"abcba"`). Given `"racecar"`, the result is 0. Given `"gbbc"`, the result is 2. The function should work for lowercase English letters only.
#include <cassert>
#include <string>

// Assume the solution function is defined above.

int main() {
    assert(minInsertionsToPalindrome("") == 0);
    assert(minInsertionsToPalindrome("a") == 0);
    assert(minInsertionsToPalindrome("ab") == 1);
    assert(minInsertionsToPalindrome("abc") == 2);
    assert(minInsertionsToPalindrome("racecar") == 0);
    assert(minInsertionsToPalindrome("abca") == 1);
    assert(minInsertionsToPalindrome("gbbc") == 2);
    assert(minInsertionsToPalindrome("aba") == 0);
    assert(minInsertionsToPalindrome("xyz") == 2);
    assert(minInsertionsToPalindrome("bbbab") == 0);
    return 0;
}
#include <string>
#include <vector>
#include <algorithm>

// Returns the minimum number of insertions needed to make the input string a palindrome.
int minInsertionsToPalindrome(const std::string& str) {
    if (str.empty()) return 0;
    
    std::string rev = str;
    std::reverse(rev.begin(), rev.end());
    
    int n = str.size();
    std::vector<int> prev(n + 1, 0);
    std::vector<int> curr(n + 1, 0);
    
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (str[i - 1] == rev[j - 1]) {
                curr[j] = 1 + prev[j - 1];
            } else {
                curr[j] = std::max(prev[j], curr[j - 1]);
            }
        }
        prev = curr;
    }
    
    return n - prev[n];
}
// The problem is the classic "minimum number of insertions to make a string palindrome" which is equivalent to `str.length() - length_of_longest_palindromic_subsequence`. The longest palindromic subsequence (LPS) can be found by taking the LCS (longest common subsequence) of the original string and its reverse. The LCS is computed using dynamic programming with two 1D arrays (prev and curr) to reduce space complexity from O(n*m) to O(m) where m is the length of the second string (the reversed string). Base cases: when either string is empty, LCS length is 0. Transitions: if characters match, add 1 to the diagonal; otherwise take the max of the top and left. After computing the LCS length, the answer is `str.size() - lcs`. Edge cases: empty string (though task says non-empty, we still handle it as 0), single character (answer 0), already palindrome (answer 0), and strings with all distinct characters (answer length-1). Time complexity is O(n*m) where n and m are the lengths of the two strings (here both equal to the input length, so O(n^2)). Space complexity is O(m) = O(n). Since the string length is at most 1000, this is efficient.
