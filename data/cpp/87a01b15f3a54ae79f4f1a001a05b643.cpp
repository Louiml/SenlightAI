/*
Given an array of \( n \) non-empty strings consisting only of lowercase English letters, write a C++ function that returns a single string by concatenating all the strings in an order that produces the lexicographically smallest possible result. The strings may have different lengths and may contain repeated characters, and the order must be determined by comparing concatenations rather than the strings themselves. For example, given `["b", "ba"]`, the two possible concatenations are `"bba"` and `"bab"`; since `"bab"` is lexicographically smaller, the answer is `"bab"`. The function should handle up to \( n = 10^5 \) strings, each of length up to 50.
*/

#include <string>
#include <vector>
#include <algorithm>

// Returns the lexicographically smallest concatenation of all strings in `arr`.
std::string smallestConcatenation(std::vector<std::string>& arr) {
    // Sort using the comparator that compares concatenations a+b vs b+a.
    std::sort(arr.begin(), arr.end(), [](const std::string& a, const std::string& b) {
        return a + b < b + a;
    });

    // Concatenate all sorted strings into the result.
    std::string result;
    for (const std::string& s : arr) {
        result += s;
    }
    return result;
}

#include <cassert>
#include <string>
#include <vector>

// Include the solution function here (or link appropriately).
std::string smallestConcatenation(std::vector<std::string>& arr);

int main() {
    {
        std::vector<std::string> arr = {"b", "ba"};
        assert(smallestConcatenation(arr) == "bab");
    }
    {
        std::vector<std::string> arr = {"c", "cb", "cba"};
        assert(smallestConcatenation(arr) == "cbacbc");
    }
    {
        std::vector<std::string> arr = {"a", "ab", "aba"};
        assert(smallestConcatenation(arr) == "aabaab");
    }
    {
        std::vector<std::string> arr = {"abc", "def", "ghi"};
        assert(smallestConcatenation(arr) == "abcdefghi");
    }
    {
        std::vector<std::string> arr = {"z", "a", "zz", "aa"};
        std::string result = smallestConcatenation(arr);
        assert(result == "aaaazzzz" || result == "aaaazzzz" || result == "aaaazzzz");
    }
    {
        std::vector<std::string> arr = {"same", "same", "same"};
        assert(smallestConcatenation(arr) == "samesamesame");
    }
    {
        std::vector<std::string> arr = {"ab", "a"};
        assert(smallestConcatenation(arr) == "aab");
    }
    {
        std::vector<std::string> arr = {"ba", "b"};
        assert(smallestConcatenation(arr) == "bab");
    }
    {
        std::vector<std::string> arr = {"x"};
        assert(smallestConcatenation(arr) == "x");
    }
    {
        std::vector<std::string> arr = {"abc", "bca", "cab"};
        // Manually verify: all permutations:
        // abcbcacab, abccabbca, bcabccab? etc. The smallest is "abcbcacab" or "abccabbca"?
        // Let's compute: compare "abc"+"bca"+"cab" = "abcbaccab"? Actually let's compute properly.
        // Correct answer: the order "abc","bca","cab" gives "abcbccab"? No, let's derive:
        // "abc"+"bca" = "abcbca", then + "cab" = "abcbccab". Other order "bca"+"abc"+"cab" = "bcaabccab"? 
        // Compare "abcbccab" vs "bcaabccab": 'a' < 'b' so first is smaller. 
        // But maybe "abc"+"cab"+"bca" = "abccabbca": compare with "abcbccab": at position 4, 'c' vs 'b'? 
        // "abcbccab" vs "abccabbca": positions: a b c b c c a b vs a b c c a b b c a – at position 4, 'b' (from first) < 'c' (second) so first is smaller. So answer is "abcbccab".
        assert(smallestConcatenation(arr) == "abcbccab");
    }
    return 0;
}

// The key observation is that the optimal ordering is not simply sorting by lexicographic order of the strings themselves, because a shorter string can be a prefix of a longer one and the comparison must consider the full concatenated result. For any two adjacent strings \( a \) and \( b \) in the final order, swapping them would change the concatenation, so we need a comparator that checks whether placing \( a \) before \( b \) yields a smaller result than placing \( b \) before \( a \). That is, we sort using the comparator `a+b < b+a`. This comparator is transitive (it induces a strict weak ordering) and works for all cases, including when strings are prefixes of each other. After sorting with this comparator, concatenating all strings in that order gives the globally smallest lexicographic concatenation. The main edge case is when two strings are identical or when the comparator returns false in both directions (which only happens when the strings are identical or concatenations are equal, e.g., `"ab"` and `"aba"` produce `"ababa"` and `"abaab"` — these are different, so tie-breaking is inherent). Time complexity is \( O(n \log n \cdot L) \) where \( L \) is the maximum length of a concatenated pair (up to 100), and space complexity is \( O(nL) \) for storing the input and the returned string.
