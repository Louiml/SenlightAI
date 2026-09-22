Write a C++ function `int mostFrequentNGramLength(const std::string& text, int n)` that takes a string `text` (containing only printable ASCII characters and spaces, with no newline) and a positive integer `n` (the gram length). The function should count how many times each distinct contiguous substring of exactly `n` characters appears in `text` (the n-grams). It should then return the maximum frequency count among all n-grams. If no n-gram occurs more than once (i.e., every n-gram is unique), return 1 (not 0) to indicate there is no repeated n-gram. The input string will have length at least `n`. For example, for `text="abcabc"` and `n=3`, the n-grams are `"abc"` (twice), `"bca"` (once), `"cab"` (once), so the maximum frequency is 2. For `text="abcd"` and `n=2`, all n-grams are unique, so return 1. The function must handle overlapping n-grams (i.e., all contiguous substrings of length `n`).

The core algorithm is to slide a window of length `n` over the input string. For each starting index from 0 to `len - n`, extract the substring of length `n` and store it in an associative container (e.g., `std::unordered_map<std::string, int>`) that maps each distinct n-gram to its occurrence count. After processing all windows, iterate over the map to find the maximum count. If the maximum count is 1, return 1 as specified; otherwise, return the maximum count. Edge cases: if the input string length equals `n`, there is exactly one n-gram, so the maximum count is 1 (no repetition). If `n` is larger than the string length, the problem statement guarantees this will not happen. The time complexity is O(L * n) where L is the length of the input string, because for each of the (L-n+1) windows we create a substring of length n (which takes O(n) time) and insert/update in the unordered map (average O(1) per operation). The space complexity is O((L-n+1) * n) in the worst case if all n-grams are distinct, storing all substrings in the map.

#include <string>
#include <unordered_map>
#include <algorithm>

// Returns the maximum frequency count of any contiguous substring of length n.
// Returns 1 if every n-gram is unique.
int mostFrequentNGramLength(const std::string& text, int n) {
    if (text.length() < n) {
        return 0; // Should not happen per constraints, but safe guard.
    }

    std::unordered_map<std::string, int> frequency;
    int maxCount = 0;

    // Iterate over all starting positions of n-grams.
    for (size_t i = 0; i <= text.length() - n; ++i) {
        std::string gram = text.substr(i, n);
        int count = ++frequency[gram];
        maxCount = std::max(maxCount, count);
    }

    // If all grams are unique, return 1 as specified.
    return (maxCount == 1) ? 1 : maxCount;
}

#include <cassert>
#include <string>

// Declare the solution function (would be in a header in practice)
int mostFrequentNGramLength(const std::string& text, int n);

int main() {
    // Basic example from prompt
    assert(mostFrequentNGramLength("abcabc", 3) == 2);
    assert(mostFrequentNGramLength("abcd", 2) == 1);

    // Overlapping grams case
    assert(mostFrequentNGramLength("aaaa", 2) == 3); // "aa" appears at positions 0,1,2

    // Single character n=1
    assert(mostFrequentNGramLength("hello", 1) == 2); // 'l' appears twice

    // n equals string length -> one gram
    assert(mostFrequentNGramLength("xyz", 3) == 1);

    // Mixed characters, spaces count
    assert(mostFrequentNGramLength("ab ab ab", 3) == 3); // "ab " appears three times (with space)

    // Longer string with repeated pattern
    assert(mostFrequentNGramLength("data data data data", 5) == 4); // "data " appears 4 times

    // All unique longer length
    assert(mostFrequentNGramLength("abcdefgh", 3) == 1);

    // n larger than string (should not happen, but test safe guard)
    assert(mostFrequentNGramLength("ab", 5) == 0);

    return 0;
}
