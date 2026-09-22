// You are given a string `s` of length `n` consisting of lowercase English letters, and a series of `q` queries. Each query provides two indices `l` and `r` (1-based). For each query, determine whether the substring from `l` to `r` can be "extended" to a longer substring that is still a palindrome of length at least 2. More precisely, output `"YES"` if there exists at least one character outside the interval `[l, r]` (i.e., at position `< l` or `> r`, 1-based) that is equal to `s[l]` or `s[r]`, and `"NO"` otherwise. In other words, check if the first character of the interval appears somewhere to its left, or the last character of the interval appears somewhere to its right. If either is true, the answer is `"YES"`; else `"NO"`. Write a C++ function that takes the string and the list of queries (as a vector of pairs) and returns a vector of strings (`"YES"`/`"NO"`) in order.

#include <cassert>
#include <string>
#include <vector>

// Include the solution function here

int main() {
    // Test 1: Basic case
    std::string s1 = "ababa";
    std::vector<std::pair<int,int>> q1 = {{1, 1}, {2, 3}, {1, 5}, {3, 4}};
    auto res1 = checkPalindromicExtensions(s1, q1);
    std::vector<std::string> exp1 = {"NO", "YES", "NO", "YES"};
    assert(res1 == exp1);

    // Test 2: All same characters
    std::string s2 = "aaa";
    std::vector<std::pair<int,int>> q2 = {{1, 2}, {2, 3}, {1, 3}};
    auto res2 = checkPalindromicExtensions(s2, q2);
    std::vector<std::string> exp2 = {"YES", "YES", "YES"};
    assert(res2 == exp2);

    // Test 3: Single character string, single query
    std::string s3 = "x";
    std::vector<std::pair<int,int>> q3 = {{1, 1}};
    auto res3 = checkPalindromicExtensions(s3, q3);
    std::vector<std::string> exp3 = {"NO"};
    assert(res3 == exp3);

    // Test 4: No matches
    std::string s4 = "abc";
    std::vector<std::pair<int,int>> q4 = {{1, 1}, {2, 2}, {3, 3}, {1, 3}};
    auto res4 = checkPalindromicExtensions(s4, q4);
    std::vector<std::string> exp4 = {"NO", "NO", "NO", "NO"};
    assert(res4 == exp4);

    // Test 5: Mixed queries
    std::string s5 = "aab";
    std::vector<std::pair<int,int>> q5 = {{1, 1}, {2, 3}, {1, 2}};
    auto res5 = checkPalindromicExtensions(s5, q5);
    std::vector<std::string> exp5 = {"NO", "YES", "YES"};
    assert(res5 == exp5);

    // Test 6: Edge case with repeated characters in middle
    std::string s6 = "abbc";
    std::vector<std::pair<int,int>> q6 = {{2, 2}, {1, 4}, {2, 3}};
    auto res6 = checkPalindromicExtensions(s6, q6);
    std::vector<std::string> exp6 = {"YES", "NO", "YES"};
    assert(res6 == exp6);

    // Test 7: Large interval covering whole string
    std::string s7 = "xyx";
    std::vector<std::pair<int,int>> q7 = {{1, 3}, {2, 2}};
    auto res7 = checkPalindromicExtensions(s7, q7);
    std::vector<std::string> exp7 = {"NO", "YES"};
    assert(res7 == exp7);

    // Test 8: Query with l==r and character appears right
    std::string s8 = "aba";
    std::vector<std::pair<int,int>> q8 = {{2, 2}, {1, 2}};
    auto res8 = checkPalindromicExtensions(s8, q8);
    std::vector<std::string> exp8 = {"YES", "YES"};
    assert(res8 == exp8);

    // Test 9: Empty string not allowed by constraints, but test with length 2
    std::string s9 = "ab";
    std::vector<std::pair<int,int>> q9 = {{1, 1}, {2, 2}, {1, 2}};
    auto res9 = checkPalindromicExtensions(s9, q9);
    std::vector<std::string> exp9 = {"NO", "NO", "NO"};
    assert(res9 == exp9);

    // Test 10: Repeated queries
    std::string s10 = "zaz";
    std::vector<std::pair<int,int>> q10 = {{1, 2}, {2, 3}, {1, 3}};
    auto res10 = checkPalindromicExtensions(s10, q10);
    std::vector<std::string> exp10 = {"YES", "YES", "NO"};
    assert(res10 == exp10);

    return 0;
}

#include <string>
#include <vector>
#include <array>

// Given a string s and queries as pairs of 1-based indices [l, r],
// return "YES" if s[l] appears left of l or s[r] appears right of r, else "NO".
std::vector<std::string> checkPalindromicExtensions(const std::string& s,
                                                    const std::vector<std::pair<int,int>>& queries) {
    int n = (int)s.size();
    // hasLeft[i] = true if s[i] occurs in s[0..i-1]
    // hasRight[i] = true if s[i] occurs in s[i+1..n-1]
    std::vector<bool> hasLeft(n, false), hasRight(n, false);
    
    // Compute hasLeft
    std::array<bool, 26> seenLeft{};
    for (int i = 0; i < n; ++i) {
        hasLeft[i] = seenLeft[s[i] - 'a'];
        seenLeft[s[i] - 'a'] = true;
    }
    
    // Compute hasRight
    std::array<bool, 26> seenRight{};
    for (int i = n - 1; i >= 0; --i) {
        hasRight[i] = seenRight[s[i] - 'a'];
        seenRight[s[i] - 'a'] = true;
    }
    
    std::vector<std::string> result;
    result.reserve(queries.size());
    for (const auto& [l, r] : queries) {
        // Convert to 0-based
        int L = l - 1;
        int R = r - 1;
        bool good = (hasLeft[L] || hasRight[R]);
        result.push_back(good ? "YES" : "NO");
    }
    return result;
}

// The problem is straightforward: for each query `[l, r]`, we need to know if there exists any occurrence of `s[l]` strictly before index `l`, or any occurrence of `s[r]` strictly after index `r`. The naive implementation would scan left from `l-1` down to 0 and right from `r+1` to `n-1` for each query, giving O(n) per query and O(nq) overall. This is acceptable for small constraints but can be optimized with prefix/suffix arrays. We can precompute for every position `i` whether the character `s[i]` appears somewhere to its left (i.e., in `s[0..i-1]`) and whether it appears somewhere to its right (in `s[i+1..n-1]`). This can be done with a boolean array `hasLeft[i]` and `hasRight[i]` by maintaining frequency counts or last-seen positions. Then for each query, simply check `hasLeft[l] || hasRight[r]`. Edge cases: when `l == r`, the condition still applies (look left of `l` and right of `r`). If `n` is 1, all queries yield "NO" because no outside character exists. Time complexity: O(n + q) preprocessing and O(q) query answering, space O(n).
