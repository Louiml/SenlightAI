// Write a C++ function `allPalindromePartitions` that takes a non-empty string `s` and returns a vector of vectors of strings, where each inner vector represents a partitioning of `s` into substrings such that every substring is a palindrome. The order of partitions and the order of substrings within each partition do not matter, but every valid partitioning must be included exactly once. For example, for the input `"aab"`, possible outputs include `{{"a","a","b"}, {"aa","b"}}`. The function must be `const`-correct, handle strings of length up to 16 efficiently, and not use global or static variables.
#include <cassert>
#include <set>
#include <algorithm>

int main() {
    // Helper to compare sets of partitions (since order may vary).
    auto partitionsEqual = [](const std::vector<std::vector<std::string>>& a,
                              const std::vector<std::vector<std::string>>& b) {
        std::set<std::vector<std::string>> setA(a.begin(), a.end());
        std::set<std::vector<std::string>> setB(b.begin(), b.end());
        return setA == setB;
    };

    // Single character
    auto r1 = allPalindromePartitions("a");
    assert(partitionsEqual(r1, {{"a"}}));

    // Two identical characters
    auto r2 = allPalindromePartitions("aa");
    assert(partitionsEqual(r2, {{"a","a"}, {"aa"}}));

    // Example from problem statement
    auto r3 = allPalindromePartitions("aab");
    assert(partitionsEqual(r3, {{"a","a","b"}, {"aa","b"}}));

    // All distinct characters: only single-character partitions
    auto r4 = allPalindromePartitions("abc");
    assert(partitionsEqual(r4, {{"a","b","c"}}));

    // Longer all same: should have 2^(n-1) partitions
    auto r5 = allPalindromePartitions("aaaa");
    assert(r5.size() == 8);
    // Check one representative partition
    bool hasAllSingles = std::find(r5.begin(), r5.end(), std::vector<std::string>{"a","a","a","a"}) != r5.end();
    bool hasFull = std::find(r5.begin(), r5.end(), std::vector<std::string>{"aaaa"}) != r5.end();
    assert(hasAllSingles && hasFull);

    // Palindrome with mixed characters: "aba"
    auto r6 = allPalindromePartitions("aba");
    assert(partitionsEqual(r6, {{"a","b","a"}, {"aba"}}));

    // Empty string is not required, but if passed, would yield one empty partition.
    // Not testing since task says non-empty.

    // Palindrome "abba"
    auto r7 = allPalindromePartitions("abba");
    assert(partitionsEqual(r7, {
        {"a","b","b","a"},
        {"a","bb","a"},
        {"abba"}
    }));

    // String that has overlapping palindromes: "aabaa"
    auto r8 = allPalindromePartitions("aabaa");
    // Expected partitions (order-insensitive)
    std::vector<std::vector<std::string>> expected8 = {
        {"a","a","b","a","a"},
        {"a","a","b","aa"},
        {"a","aba","a"},
        {"aa","b","a","a"},
        {"aa","b","aa"},
        {"aabaa"}
    };
    assert(partitionsEqual(r8, expected8));

    // Number of partitions for "aab" is 2, check count
    assert(r3.size() == 2);

    return 0;
}
#include <vector>
#include <string>

// Returns all possible palindrome partitions of the input string.
// Each inner vector is a valid partition where every substring is a palindrome.
std::vector<std::vector<std::string>> allPalindromePartitions(const std::string& s) {
    std::vector<std::vector<std::string>> result;
    std::vector<std::string> current;
    
    // Helper to check if s[low..high] is a palindrome.
    auto isPalindrome = [&s](int low, int high) -> bool {
        while (low < high) {
            if (s[low] != s[high]) return false;
            ++low;
            --high;
        }
        return true;
    };
    
    // Depth-first search over possible partition boundaries.
    std::function<void(int)> dfs = [&](int start) {
        if (start >= s.length()) {
            result.push_back(current);
            return;
        }
        for (int end = start; end < s.length(); ++end) {
            if (isPalindrome(start, end)) {
                current.push_back(s.substr(start, end - start + 1));
                dfs(end + 1);
                current.pop_back();
            }
        }
    };
    
    dfs(0);
    return result;
}
// The solution uses depth-first search (backtracking). Starting at index `start`, we iterate over every possible end index `end` from `start` to the end of the string. If the substring `s[start..end]` is a palindrome, we append it to a temporary list, recursively call the function for the remaining suffix starting at `end+1`, and then backtrack by removing that substring. When `start` reaches the string’s length, we have found a complete partition, so we copy the current list into the result. A helper `isPalindrome` checks palindrome status in O(length) using two pointers. 
//   
//   For the DP-enhanced version, we precompute a 2D boolean table `dp[i][j]` where `dp[i][j]` is true if the substring `s[i..j]` is a palindrome. We fill it using the recurrence: `dp[i][j] = (s[i] == s[j] && (j - i <= 2 || dp[i+1][j-1]))`. During backtracking, we only recurse when `dp[start][end]` is true, avoiding repeated palindrome checks. 
//
//   **Edge cases:** Empty string is not allowed per task (input is non-empty). A single character is always a palindrome. The string could consist of identical characters, in which case every substring is a palindrome, leading to many partitions. Substrings are created using `substr`, which is O(length).
//
//   **Time complexity:** Without DP, each palindrome check is O(n) and there are up to O(2^n) partitions in the worst case (e.g., all same characters), so the worst-case time is O(n * 2^n). With DP, palindrome checks become O(1) per substring, so the total time is O(2^n) for the recursion plus O(n^2) for DP construction. In practice, for n ≤ 16, both are acceptable. 
//
//   **Space complexity:** The depth-first recursion uses O(n) call stack, and the DP table uses O(n^2) space. The result vector can hold up to O(2^n) partitions, each of which has up to n substrings, so the output space is O(n * 2^n) in the worst case.
