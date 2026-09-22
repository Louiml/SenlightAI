// Given a lowercase English string `s` of length at least 1, write a C++ function that rearranges the characters so that no two adjacent characters are the same. If such a rearrangement is impossible, return an empty string. Otherwise, return any valid rearrangement. For example, for `"aab"` a valid output is `"aba"`, and for `"aaab"` the output must be `""` because the most frequent character `'a'` appears 3 times, which exceeds `(n+1)/2` for `n=4`, making a valid arrangement impossible.
// The key observation is that a valid rearrangement exists if and only if the most frequent character’s count does not exceed `(n+1)/2` where `n` is the string length. The algorithm first counts frequencies of all 26 lowercase letters. Then it identifies the character with the maximum frequency. If that maximum frequency is greater than `(n+1)/2`, return empty string. Otherwise, place that most frequent character at even indices (0, 2, 4, ...). After placing all copies of that character, fill the remaining characters into the remaining even slots (if any) and then odd slots, always advancing by 2 and wrapping from even to odd when the even positions are exhausted. This guarantees no two adjacent characters are identical because the most frequent character is spaced apart by at least one position, and all other characters have lower frequencies and are placed after. Edge cases include strings with a single distinct character (possible only if its count is 1, e.g., `"a"` → `"a"`, or if count exceeds 1 then impossible), and strings where the most frequent character count is exactly `(n+1)/2` (works because the even slots are exactly enough). Time complexity is `O(n)` because we iterate through the string once and then through the 26 counts, and the placement loops total `n` assignments. Space complexity is `O(1)` auxiliary, ignoring the returned string.
#include <string>
#include <vector>
#include <algorithm>
#include <cstdint>

// Rearrange a lowercase string so no two adjacent characters are equal.
// Returns an empty string if impossible; otherwise returns any valid arrangement.
std::string reorganizeString(const std::string& input) {
    const int ALPHABET_SIZE = 26;
    std::vector<int> freq(ALPHABET_SIZE, 0);
    for (char c : input) {
        freq[c - 'a']++;
    }
    
    int n = input.size();
    char maxChar = 'a';
    int maxFreq = 0;
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        if (freq[i] > maxFreq) {
            maxFreq = freq[i];
            maxChar = static_cast<char>('a' + i);
        }
    }
    
    // If the most frequent character appears too many times, impossible.
    if (maxFreq > (n + 1) / 2) {
        return "";
    }
    
    std::string result(n, ' ');
    int index = 0;
    
    // Place the most frequent character at even indices.
    while (freq[maxChar - 'a'] > 0) {
        result[index] = maxChar;
        freq[maxChar - 'a']--;
        index += 2;
    }
    
    // Place the remaining characters.
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        while (freq[i] > 0) {
            if (index >= n) {
                index = 1;
            }
            result[index] = static_cast<char>('a' + i);
            freq[i]--;
            index += 2;
        }
    }
    
    return result;
}
#include <cassert>
#include <string>

// declartion from solution (assume included in same translation unit)
std::string reorganizeString(const std::string& input);

int main() {
    // Simple rearrangement
    std::string res1 = reorganizeString("aab");
    assert(res1 == "aba");
    
    // Impossible case
    assert(reorganizeString("aaab") == "");
    
    // Single character
    assert(reorganizeString("a") == "a");
    
    // Two different characters with equal count
    assert(reorganizeString("ab") == "ab");
    
    // Many repeated characters but still possible
    std::string res2 = reorganizeString("aaabbc");
    // Verify no adjacent equal characters
    for (size_t i = 1; i < res2.size(); ++i) {
        assert(res2[i] != res2[i-1]);
    }
    
    // Already valid input
    assert(reorganizeString("abc") == "abc");
    
    // All same characters impossible
    assert(reorganizeString("aaaa") == "");
    
    // Case where max freq equals (n+1)/2 exactly
    std::string res3 = reorganizeString("aaabbb");
    for (size_t i = 1; i < res3.size(); ++i) {
        assert(res3[i] != res3[i-1]);
    }
    
    // Test edge with length 2 and same chars impossible
    assert(reorganizeString("aa") == "");
    
    return 0;
}
