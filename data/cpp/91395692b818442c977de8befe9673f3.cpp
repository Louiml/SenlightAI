// Given a string `s` consisting of lowercase English letters, and three integers `maxLetters`, `minSize`, and `maxSize`, write a C++ function that returns the maximum number of occurrences of any substring of `s` whose length is between `minSize` and `maxSize` inclusive, and which contains at most `maxLetters` distinct characters. If no such substring exists, return 0. For example, for `s = "aababcaab"`, `maxLetters = 2`, `minSize = 3`, `maxSize = 4`, the answer is 2 because substring `"aab"` occurs twice, while longer substrings with at most 2 distinct letters occur at most once.

// A direct approach enumerating all substrings and counting their occurrences would be too slow for large inputs. The key observation is that if a substring of length between `minSize` and `maxSize` satisfies the distinct-character constraint, then any prefix of it of length `minSize` also satisfies the constraint (since removing characters cannot increase the number of distinct characters). Therefore, to maximize the occurrence count, we only need to consider substrings of exactly length `minSize`. This reduces the problem to counting occurrences of length-`minSize` substrings that have at most `maxLetters` distinct characters.
//
// To efficiently slide a fixed-length window of size `minSize` across the string, we maintain a frequency array for characters in the current window and a counter of distinct characters. We also use a Rolling Hash (Rabin-Karp) to uniquely identify each length-`minSize` substring, avoiding the overhead of storing strings as keys. The hash is computed incrementally: when the window slides by one character, we remove the leftmost character (updating its frequency and distinct count) and add a new character on the right, adjusting the hash modulo a large prime to avoid collisions. For each valid window (distinct count ≤ `maxLetters`), we increment a count for that hash in an unordered map and track the maximum count.
//
// Edge cases: if `minSize` is 1, the base multiplier logic still works (it is never used for removal when the window size is 1). If the string is shorter than `minSize`, no valid substrings exist and the function returns 0. Collisions in the hash are possible but with a modulo prime and base 26, the probability is extremely low; for a production solution one could use double hashing or a string key, but this solution is sufficient for the task.
//
// Time complexity is O(n) where n is the length of the string, because we scan each character once. Space complexity is O(n) in the worst case for the hash map that stores counts of distinct substrings, plus O(1) for the frequency array (fixed 26 letters).

#include <string>
#include <unordered_map>
#include <vector>

// Returns the maximum number of occurrences of any substring of s with length
// between minSize and maxSize (inclusive) that has at most maxLetters distinct characters.
int maxOccurrences(const std::string& s, int maxLetters, int minSize, int maxSize) {
    const int n = static_cast<int>(s.size());
    if (n < minSize) return 0;

    const long long MOD = 1000000007LL;
    const long long BASE = 26LL;

    // Frequency of characters in the current window of length minSize
    std::vector<int> freq(26, 0);
    int distinct = 0;

    // Rolling hash value for current window
    long long hash = 0;
    long long baseMul = 1; // BASE^(minSize-1) mod MOD, used for removing the leftmost char

    // Precompute baseMul for minSize-1 multiplications
    for (int i = 0; i < minSize - 1; ++i) {
        baseMul = (baseMul * BASE) % MOD;
    }

    std::unordered_map<long long, int> counts; // hash -> occurrence count
    int result = 0;

    for (int i = 0; i < n; ++i) {
        // Add new character to the right
        int addIdx = s[i] - 'a';
        freq[addIdx]++;
        if (freq[addIdx] == 1) distinct++;
        hash = (hash * BASE + addIdx) % MOD;

        // Remove the character that falls out of the window
        if (i >= minSize) {
            int removeIdx = s[i - minSize] - 'a';
            freq[removeIdx]--;
            if (freq[removeIdx] == 0) distinct--;
            // Subtract the contribution of the removed character
            hash = (hash - (baseMul * removeIdx) % MOD + MOD) % MOD;
        }

        // If we have a full window of length minSize and valid distinct count
        if (i >= minSize - 1 && distinct <= maxLetters) {
            int newCount = ++counts[hash];
            if (newCount > result) result = newCount;
        }
    }

    return result;
}

#include <cassert>
#include <string>

// The function under test is declared above; include the header or paste here.
// For standalone test, assume maxOccurrences is available.

int main() {
    // Basic example from the problem statement
    assert(maxOccurrences("aababcaab", 2, 3, 4) == 2); // "aab" appears twice

    // String too short to contain any substring of minSize
    assert(maxOccurrences("a", 1, 2, 3) == 0);

    // All substrings of length 1 with at most 1 distinct char
    assert(maxOccurrences("abc", 1, 1, 1) == 1); // each single char appears once

    // Repeated pattern with enough letters allowed
    assert(maxOccurrences("aaaa", 1, 2, 2) == 3); // "aa" appears 3 times

    // More letters than allowed
    assert(maxOccurrences("abcabcabc", 2, 3, 3) == 0); // any 3-char window has 3 distinct letters

    // Larger maxSize still uses minSize because of the key observation
    assert(maxOccurrences("abcabcabc", 3, 2, 5) == 3); // "ab" appears 3 times (each window of length 2 has ≤3 distinct)

    // Single character string with minSize=1
    assert(maxOccurrences("x", 1, 1, 1) == 1);

    // Empty string
    assert(maxOccurrences("", 1, 1, 1) == 0);

    // Edge with minSize=1 and maxLetters=1
    assert(maxOccurrences("aaabbb", 1, 1, 2) == 3); // 'a' appears 3 times, 'b' appears 3 times

    // Ensure hash collisions don't break count (if they occur, this may fail but probability is low)
    assert(maxOccurrences("zzzzz", 1, 2, 3) == 4); // "zz" appears 4 times

    return 0;
}
