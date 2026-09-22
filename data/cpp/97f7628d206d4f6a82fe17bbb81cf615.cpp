// Given three strings `a`, `b`, and `c`, write a C++ function `bool isInterleave(const std::string& a, const std::string& b, const std::string& c)` that determines whether `c` can be formed by interleaving the characters of `a` and `b` while preserving the relative order of characters within each original string. An interleaving means we merge `a` and `b` into `c` by taking characters one at a time from either string, in their original order, until both are exhausted. For example, `a = "abc"`, `b = "def"`, `c = "adbecf"` is valid, while `a = "abc"`, `b = "def"`, `c = "abdecf"` is also valid because `c` can be split as `a`'s characters `a,b` then `b`'s `d,e` then `a`'s `c` then `b`'s `f`. But `a = "abc"`, `b = "def"`, `c = "abdec"` should return false because `c` is shorter than the total length. Handle empty strings gracefully: if both `a` and `b` are empty, the only valid `c` is an empty string; if one is empty, `c` must equal the other string exactly. The function must be efficient for strings up to length 300 each, meaning a dynamic programming approach with O(|a| * |b|) time and space is acceptable.
The problem reduces to checking whether we can interleave the characters of `a` and `b` to exactly form `c`. The first condition is that the lengths must match: `len(c) == len(a) + len(b)`. Then we use a 2D boolean DP table `dp[i][j]` that represents whether the first `i` characters of `a` and the first `j` characters of `b` can interleave to form the first `i+j` characters of `c`. The recurrence is: `dp[i][j]` is true if either (i>0 and `a[i-1] == c[i+j-1]` and `dp[i-1][j]` is true) or (j>0 and `b[j-1] == c[i+j-1]` and `dp[i][j-1]` is true). Base case: `dp[0][0] = true`. We iterate over all valid `i` from 0 to `a.length()` and `j` from 0 to `b.length()`, but also need to check that `i+j <= c.length()` since we can't exceed `c`'s length. If at any point we attempt to use a character from `a` or `b` that doesn't match the corresponding position in `c`, the DP cell becomes false. The final answer is `dp[a.length()][b.length()]`. Edge cases: if `c` is empty, only true when both `a` and `b` are empty; if one is empty, the DP reduces to checking equality with `c`. The time complexity is O(|a| * |b|), and space complexity is O(|a| * |b|) for the DP table, which meets the constraints (300*300 = 90k cells).
#include <string>
#include <vector>

// Determine if c can be formed by interleaving a and b preserving order.
bool isInterleave(const std::string& a, const std::string& b, const std::string& c) {
    const int m = static_cast<int>(a.size());
    const int n = static_cast<int>(b.size());
    const int len = static_cast<int>(c.size());

    // Length mismatch means impossible.
    if (m + n != len) {
        return false;
    }

    // dp[i][j] = true if first i chars of a and first j chars of b
    // can interleave to form first (i+j) chars of c.
    std::vector<std::vector<bool>> dp(m + 1, std::vector<bool>(n + 1, false));
    dp[0][0] = true;

    for (int i = 0; i <= m; ++i) {
        for (int j = 0; j <= n; ++j) {
            if (i == 0 && j == 0) continue;
            int k = i + j;
            if (k > len) continue; // can't go beyond c's length

            bool can = false;
            if (i > 0 && a[i - 1] == c[k - 1]) {
                can = can || dp[i - 1][j];
            }
            if (j > 0 && b[j - 1] == c[k - 1]) {
                can = can || dp[i][j - 1];
            }
            dp[i][j] = can;
        }
    }

    return dp[m][n];
}
#include <cassert>
#include <string>

int main() {
    // Basic interleaving examples
    assert(isInterleave("abc", "def", "adbecf") == true);
    assert(isInterleave("abc", "def", "abdecf") == true);
    assert(isInterleave("abc", "def", "abcdef") == true); // all a then b
    assert(isInterleave("abc", "def", "defabc") == true); // all b then a

    // Invalid interleavings
    assert(isInterleave("a", "b", "ab") == true); // trivial
    assert(isInterleave("a", "b", "ba") == false); // order violated
    assert(isInterleave("abc", "def", "abdecfx") == false); // extra char
    assert(isInterleave("abc", "def", "abdec") == false); // missing length

    // Empty string cases
    assert(isInterleave("", "", "") == true);
    assert(isInterleave("", "abc", "abc") == true);
    assert(isInterleave("abc", "", "abc") == true);
    assert(isInterleave("", "abc", "ab") == false);
    assert(isInterleave("abc", "", "") == false);

    // Edge case with repeated characters
    assert(isInterleave("aa", "aa", "aaaa") == true);
    assert(isInterleave("aaa", "aaa", "aaaaaa") == true);
    assert(isInterleave("aab", "aac", "aaabac") == false); // can't reorder within each
    assert(isInterleave("aab", "aac", "aabaac") == true);

    // Larger test to ensure no stack overflow and correct logic
    std::string a(300, 'x');
    std::string b(300, 'y');
    std::string c = a + b;
    assert(isInterleave(a, b, c) == true);
    c[599] = 'z'; // break the end
    assert(isInterleave(a, b, c) == false);

    return 0;
}
