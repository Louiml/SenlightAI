Write a C++ function `vector<vector<string>> palindromePartitions(const string& s)` that returns all possible ways to partition a given non-empty string `s` into substrings such that every substring is a palindrome. A palindrome is a string that reads the same forward and backward. The result should contain every distinct partition as a vector of substrings in the order they appear in the original string. For example, for input `"aab"`, the output should be `{{"a","a","b"}, {"aa","b"}}`. The order of partitions within the outer vector does not matter, but the substrings within each partition must appear in left-to-right order. The input string may contain uppercase and lowercase letters, digits, and special characters; treat characters case-sensitively (e.g., `"Aa"` is not a palindrome). The function should work efficiently for strings of length up to 16.

The problem is a classic backtracking task. We explore all possible ways to split the string by choosing a valid palindrome prefix at each recursion step. At a given start index `start`, we iterate over every end index `end` from `start` to `n-1`. For each `end`, we check if the substring `s[start..end]` is a palindrome using a helper function that uses two pointers moving inward. If it is, we add that substring to the current path and recursively process the remainder starting at `end+1`. When `start` reaches the string length, we have a complete valid partition and we push a copy of the path into the answer vector. After the recursive call, we pop the last added substring to backtrack and try the next possible palindrome. This ensures we explore all combinations. Edge cases include an empty input (but the problem guarantees non-empty), single-character strings (every single character is a palindrome, so the result is just one partition with that character), and strings where the only palindromes are individual characters (e.g., `"abc"` yields `{{"a","b","c"}}`). Time complexity is exponential in the worst case—specifically \(O(n \cdot 2^n)\) for generating all possible palindrome partitions, as each of the up to \(2^{n-1}\) ways to split may require checking palindromes of average length \(O(n)\). Space complexity is \(O(n)\) for the recursion stack and the current path, plus \(O(n \cdot 2^n)\) for the output in the worst case.

#include <string>
#include <vector>

// Check if the substring s[start..end] is a palindrome.
bool isPalindrome(const std::string& s, int start, int end) {
    while (start < end) {
        if (s[start] != s[end]) {
            return false;
        }
        ++start;
        --end;
    }
    return true;
}

// Backtracking helper to build all palindrome partitions starting at 'start'.
void backtrack(const std::string& s, int start, std::vector<std::string>& current,
               std::vector<std::vector<std::string>>& result) {
    if (start == static_cast<int>(s.size())) {
        result.push_back(current);
        return;
    }
    for (int end = start; end < static_cast<int>(s.size()); ++end) {
        if (isPalindrome(s, start, end)) {
            current.push_back(s.substr(start, end - start + 1));
            backtrack(s, end + 1, current, result);
            current.pop_back();
        }
    }
}

// Return all palindrome partitions of the input string s.
std::vector<std::vector<std::string>> palindromePartitions(const std::string& s) {
    std::vector<std::vector<std::string>> result;
    std::vector<std::string> current;
    backtrack(s, 0, current, result);
    return result;
}

#include <cassert>
#include <string>
#include <vector>

// Include the solution function definition here (or link appropriately).

int main() {
    // Test single character
    auto p1 = palindromePartitions("a");
    assert(p1.size() == 1 && p1[0] == std::vector<std::string>{"a"});

    // Test two identical characters
    auto p2 = palindromePartitions("aa");
    assert(p2.size() == 2);
    assert((p2[0] == std::vector<std::string>{"a","a"} || p2[0] == std::vector<std::string>{"aa"}));
    assert((p2[1] == std::vector<std::string>{"a","a"} || p2[1] == std::vector<std::string>{"aa"}));

    // Test "aab"
    auto p3 = palindromePartitions("aab");
    assert(p3.size() == 2);
    bool hasSingle = false, hasPair = false;
    for (const auto& part : p3) {
        if (part == std::vector<std::string>{"a","a","b"}) hasSingle = true;
        if (part == std::vector<std::string>{"aa","b"}) hasPair = true;
    }
    assert(hasSingle && hasPair);

    // Test "aba"
    auto p4 = palindromePartitions("aba");
    assert(p4.size() == 2);
    bool hasAllSingle = false, hasFull = false;
    for (const auto& part : p4) {
        if (part == std::vector<std::string>{"a","b","a"}) hasAllSingle = true;
        if (part == std::vector<std::string>{"aba"}) hasFull = true;
    }
    assert(hasAllSingle && hasFull);

    // Test "abc" (no multi-character palindrome)
    auto p5 = palindromePartitions("abc");
    assert(p5.size() == 1 && p5[0] == std::vector<std::string>{"a","b","c"});

    // Test "aabb"
    auto p6 = palindromePartitions("aabb");
    assert(p6.size() == 4);
    // Expected partitions: {"a","a","b","b"}, {"aa","b","b"}, {"a","a","bb"}, {"aa","bb"}
    bool p6a=false,p6b=false,p6c=false,p6d=false;
    for (const auto& part : p6) {
        if (part == std::vector<std::string>{"a","a","b","b"}) p6a=true;
        if (part == std::vector<std::string>{"aa","b","b"}) p6b=true;
        if (part == std::vector<std::string>{"a","a","bb"}) p6c=true;
        if (part == std::vector<std::string>{"aa","bb"}) p6d=true;
    }
    assert(p6a && p6b && p6c && p6d);

    // Test case sensitivity: "Aa" is not a palindrome
    auto p7 = palindromePartitions("Aa");
    assert(p7.size() == 1 && p7[0] == std::vector<std::string>{"A","a"});

    // Test with digits and special chars: "1a1"
    auto p8 = palindromePartitions("1a1");
    assert(p8.size() == 2);
    bool p8a=false, p8b=false;
    for (const auto& part : p8) {
        if (part == std::vector<std::string>{"1","a","1"}) p8a=true;
        if (part == std::vector<std::string>{"1a1"}) p8b=true;
    }
    assert(p8a && p8b);

    return 0;
}
