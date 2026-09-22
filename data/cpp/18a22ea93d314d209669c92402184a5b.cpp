/*
Write a C++ function that takes a vector of strings and a vector of queries, where each query is a pair of indices [l, r] inclusive, and returns for each query the number of strings in the range [l, r] that both start and end with a vowel (a, e, i, o, u — case-sensitive lowercase only). The input words are guaranteed non-empty and contain only lowercase English letters. The function should return a vector of integers, one per query, in the same order as the queries. Assume the vector of strings can be empty, in which case every query result is 0 (though queries themselves will be valid). Implement the function with a descriptive name and make it `const`-correct where appropriate.
*/

#include <vector>
#include <string>
#include <unordered_set>

// Count the number of strings in each inclusive range [l, r] that
// start and end with a lowercase vowel (a, e, i, o, u).
std::vector<int> countVowelBoundedWords(
    const std::vector<std::string>& words,
    const std::vector<std::vector<int>>& queries)
{
    const std::unordered_set<char> vowels = {'a', 'e', 'i', 'o', 'u'};
    const int n = static_cast<int>(words.size());
    
    // prefix[i] = number of valid words in words[0..i-1]
    std::vector<int> prefix(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        const bool isVowelBounded = 
            vowels.count(words[i].front()) && vowels.count(words[i].back());
        prefix[i + 1] = prefix[i] + (isVowelBounded ? 1 : 0);
    }
    
    std::vector<int> results;
    results.reserve(queries.size());
    for (const auto& q : queries) {
        const int l = q[0];
        const int r = q[1];
        // Valid indices: l and r are guaranteed within [0, n-1]
        results.push_back(prefix[r + 1] - prefix[l]);
    }
    return results;
}

#include <cassert>
#include <vector>
#include <string>

// Function declaration (the solution is assumed to be included above)
std::vector<int> countVowelBoundedWords(
    const std::vector<std::string>& words,
    const std::vector<std::vector<int>>& queries);

int main() {
    // Example from the snippet: {"aba", "bcb", "ece", "aa", "e"}
    std::vector<std::string> words1 = {"aba", "bcb", "ece", "aa", "e"};
    std::vector<std::vector<int>> queries1 = {{0, 2}, {1, 4}, {1, 1}};
    std::vector<int> result1 = countVowelBoundedWords(words1, queries1);
    assert((result1 == std::vector<int>{2, 3, 0}));
    
    // Empty words vector
    std::vector<std::string> words2;
    std::vector<std::vector<int>> queries2 = {{0, 0}, {0, 0}};
    std::vector<int> result2 = countVowelBoundedWords(words2, queries2);
    assert((result2 == std::vector<int>{0, 0}));
    
    // Single word that is valid
    std::vector<std::string> words3 = {"a"};
    std::vector<std::vector<int>> queries3 = {{0, 0}};
    std::vector<int> result3 = countVowelBoundedWords(words3, queries3);
    assert((result3 == std::vector<int>{1}));
    
    // Single word not valid
    std::vector<std::string> words4 = {"bc"};
    std::vector<std::vector<int>> queries4 = {{0, 0}};
    std::vector<int> result4 = countVowelBoundedWords(words4, queries4);
    assert((result4 == std::vector<int>{0}));
    
    // Non-vowel but starts and ends with same vowel? edge: "abca" starts with 'a' ends with 'a' valid
    std::vector<std::string> words5 = {"abca", "b"};
    std::vector<std::vector<int>> queries5 = {{0, 1}, {1, 1}, {0, 0}};
    std::vector<int> result5 = countVowelBoundedWords(words5, queries5);
    // "abca" valid, "b" invalid, so total valid in [0,1] is 1
    assert((result5 == std::vector<int>{1, 0, 1}));
    
    // Case sensitivity: only lowercase vowels count
    std::vector<std::string> words6 = {"Aba", "eE"};
    std::vector<std::vector<int>> queries6 = {{0, 1}};
    std::vector<int> result6 = countVowelBoundedWords(words6, queries6);
    // "Aba" starts with 'A' not vowel, "eE" starts with 'e' but ends with 'E' not vowel => both invalid
    assert((result6 == std::vector<int>{0}));
    
    // Larger range covering all
    std::vector<std::string> words7 = {"ae", "io", "u", "xy", "ba"};
    std::vector<std::vector<int>> queries7 = {{0, 4}};
    std::vector<int> result7 = countVowelBoundedWords(words7, queries7);
    // "ae" valid, "io" valid, "u" valid, "xy" invalid, "ba" invalid => 3
    assert((result7 == std::vector<int>{3}));
    
    // Query with l > 0 and r < n-1
    std::vector<std::string> words8 = {"a", "b", "c", "d", "e"};
    std::vector<std::vector<int>> queries8 = {{1, 3}, {0, 4}, {2, 2}};
    std::vector<int> result8 = countVowelBoundedWords(words8, queries8);
    // Indices: 0 valid, 1 invalid, 2 invalid, 3 invalid, 4 valid
    // [1,3] = 0, [0,4] = 2, [2,2] = 0
    assert((result8 == std::vector<int>{0, 2, 0}));
    
    return 0;
}

// The problem is a classic range-query counting problem. A direct approach would scan each query’s subarray, giving O(n·q) time, which is inefficient for large inputs. The optimal solution uses a prefix sum array. First, precompute a boolean indicator for each word whether it is "vowel-bounded" (first and last characters are vowels). Then build a prefix sum array `prefix` where `prefix[i]` stores the count of such valid words in indices `[0, i-1]` (so `prefix[0] = 0`). For a query `[l, r]`, the answer is `prefix[r+1] - prefix[l]`. This works because prefix sums allow O(1) range queries after O(n) preprocessing. Edge cases: empty words vector (prefix array is just `{0}`, all queries return 0), queries with `l == r`, and words of length 1 (first and last character are the same). Time complexity: O(n + q) where n is the number of words and q is the number of queries. Space complexity: O(n) for the prefix array, plus O(q) for the result.
