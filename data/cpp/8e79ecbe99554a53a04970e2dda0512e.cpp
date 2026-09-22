// Given a string `s` consisting of lowercase English letters, write a C++ function `int maxProduct(const string& s)` that splits the characters of `s` (each character can be used at most once) into two disjoint subsequences `s1` and `s2` (some characters may be unused), such that both `s1` and `s2` are palindromes. The goal is to maximize the product of the lengths of `s1` and `s2`. Return that maximum possible product. For example, for `s = "leetcodecom"`, a valid split yields `s1 = "ete"`, `s2 = "cdc"` giving lengths 3 and 3 with product 9. The length of `s` will be between 1 and 12 inclusive, so a brute-force recursion is acceptable, but your solution must be efficient enough for this constraint.
#include <cassert>
#include <string>

int maxProduct(const std::string& s); // declaration

int main() {
    // Basic cases
    assert(maxProduct("a") == 0); // single char, can only form one palindrome, other empty => product 0
    assert(maxProduct("ab") == 0); // no way to form two non-empty palindromes
    assert(maxProduct("aa") == 1); // one 'a' in each substring => 1*1
    assert(maxProduct("aba") == 1); // "a" and "a" => 1*1, or "aba" and "" => 0
    assert(maxProduct("abba") == 4); // "abba" and "" gives 0, but "ab" isn't palindrome; actually "abba" can't split, but "a" and "bb" => 1*2=2; best is "aba"? wait, let's think: for "abba", can we have "aa" and "bb"? In order: index0 a to s1, index1 b to s2, index2 b to s2, index3 a to s1 => s1="aa", s2="bb", both palindromes, product = 2*2=4)
    assert(maxProduct("abcba") == 4); // "aca" and "bb"? but order: a(0) to s1, b(1) to s2, c(2) skip, b(3) to s2, a(4) to s1 => s1="aa", s2="bb" product 4)
    assert(maxProduct("leetcodecom") == 9); // from problem example
    // Edge: empty string? Not in constraints but if given, returns 0
    assert(maxProduct("") == 0);
    // All same characters
    assert(maxProduct("aaaa") == 4); // can split 2 and 2 => 4
    assert(maxProduct("aaaaa") == 6); // 2 and 3 => 6
    return 0;
}
#include <string>
#include <algorithm>
#include <climits>

// Helper to check if a string is a palindrome.
static bool isPalindrome(const std::string& str) {
    int left = 0;
    int right = (int)str.size() - 1;
    while (left < right) {
        if (str[left] != str[right]) return false;
        ++left;
        --right;
    }
    return true;
}

// Recursive helper to explore all assignments.
static void explore(int index, const std::string& s, std::string& s1, std::string& s2, int& bestProduct) {
    if (index == (int)s.size()) {
        if (isPalindrome(s1) && isPalindrome(s2)) {
            int product = (int)s1.size() * (int)s2.size();
            bestProduct = std::max(bestProduct, product);
        }
        return;
    }
    // Option 1: skip current character
    explore(index + 1, s, s1, s2, bestProduct);
    
    // Option 2: add character to s1
    s1.push_back(s[index]);
    explore(index + 1, s, s1, s2, bestProduct);
    s1.pop_back();
    
    // Option 3: add character to s2
    s2.push_back(s[index]);
    explore(index + 1, s, s1, s2, bestProduct);
    s2.pop_back();
}

// Find the maximum product of lengths of two disjoint palindromic subsequences.
int maxProduct(const std::string& s) {
    std::string s1, s2;
    int bestProduct = 0;  // Minimum possible product is 0 (empty strings)
    explore(0, s, s1, s2, bestProduct);
    return bestProduct;
}
// The problem is a combinatorial optimization: we need to assign each character of the original string to one of three states: (a) not used, (b) added to `s1`, or (c) added to `s2`. The order of characters in each subsequence must follow the original relative order, so we cannot reorder them. We recursively explore all assignments. At each index `i` from 0 to `n-1`, we have three branches: skip the character, append it to `s1`, or append it to `s2`. At the base case (index equals `n`), we check if both `s1` and `s2` are palindromes. If they are, we compute the product of their lengths and update the maximum. Because the string length is at most 12, the total number of assignments is `3^n` (up to 531,441), which is feasible. An important edge case is that empty strings are palindromes (length 0), so we may use no characters at all, but that yields product 0, so effectively we only care about non-trivial palindromes. We must also consider that a character can be assigned to only one of `s1` or `s2`, not both—this is enforced in the recursion by not pushing the same character twice. The time complexity is `O(3^n * (|s1| + |s2|))` for palindrome checks, but for `n ≤ 12` this is fine. Space complexity is `O(n)` for recursion depth plus `O(n)` for the two subsequence strings.
