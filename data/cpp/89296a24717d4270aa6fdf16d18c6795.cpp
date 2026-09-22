Write a C++ function that takes a string consisting of lowercase English letters and returns the minimum number of distinct characters that must be removed so that no character appears more than twice in the remaining string. For example, for the string `"aabbb"`, the character `'a'` appears 2 times and `'b'` appears 3 times; since `'b'` exceeds the limit of 2, one `'b'` must be removed, so the answer is 1. The function should handle empty strings and strings where all characters already appear at most twice, in which case the result is 0. The function must be case-sensitive and only consider lowercase letters.
The solution counts the frequency of each character in the input string. Since the alphabet is limited to 26 lowercase letters, we can use a fixed-size array or a hash map. For each character, if its frequency is greater than 2, the excess beyond 2 must be removed. The total number of removals is the sum of `max(0, frequency - 2)` over all distinct characters. This is because we can keep at most two occurrences of each character and discard the rest. Edge cases include an empty string (answer 0) and strings where all frequencies are ≤2 (answer 0). The time complexity is O(n) where n is the length of the string, and space complexity is O(1) since the frequency storage is constant (26 slots).
#include <string>
#include <vector>

// Returns the minimum number of characters to remove so that no character appears more than twice.
int minRemovalsForMaxTwo(const std::string& s) {
    std::vector<int> freq(26, 0);
    for (char c : s) {
        freq[c - 'a']++;
    }
    int removals = 0;
    for (int count : freq) {
        if (count > 2) {
            removals += count - 2;
        }
    }
    return removals;
}
#include <cassert>
#include <string>
#include "solution.h"

int main() {
    assert(minRemovalsForMaxTwo("") == 0);
    assert(minRemovalsForMaxTwo("a") == 0);
    assert(minRemovalsForMaxTwo("aa") == 0);
    assert(minRemovalsForMaxTwo("aaa") == 1);
    assert(minRemovalsForMaxTwo("aaaa") == 2);
    assert(minRemovalsForMaxTwo("aabbb") == 1);
    assert(minRemovalsForMaxTwo("abc") == 0);
    assert(minRemovalsForMaxTwo("aaabbbccc") == 3); // each appears 3 times, remove 1 each
    assert(minRemovalsForMaxTwo("abracadabra") == 3); // a:5->3 extra, b:2, r:2, c:1, d:1
    assert(minRemovalsForMaxTwo("zzzzzz") == 4);
    return 0;
}
