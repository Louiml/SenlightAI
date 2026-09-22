/*
Given a string `s` consisting only of lowercase English letters `'a'`, `'b'`, and `'c'`, write a C++ function `countSubstringsWithAllThree` that returns the number of substrings that contain at least one occurrence of each of the three characters `'a'`, `'b'`, and `'c'`. The function must be efficient for strings up to length 10^5. For example, for `s = "abc"`, the answer is 1 (the whole substring); for `s = "aaacb"`, valid substrings include `"aaacb"`, `"aacb"`, `"acb"`, and `"cb"` but not `"aaac"` (missing `'b'`) — the total is 4; for `s = "abcabc"`, all substrings containing at least one of each character number 10.
*/

#include <string>
#include <algorithm>

// Counts substrings of s containing at least one 'a', one 'b', and one 'c'.
// s consists only of lowercase 'a', 'b', 'c'.
int countSubstringsWithAllThree(const std::string& s) {
    int lastA = -1, lastB = -1, lastC = -1;
    int total = 0;
    const int n = static_cast<int>(s.size());

    for (int i = 0; i < n; ++i) {
        if (s[i] == 'a') lastA = i;
        else if (s[i] == 'b') lastB = i;
        else lastC = i; // s[i] == 'c'

        int earliest = std::min(lastA, std::min(lastB, lastC));
        total += earliest + 1; // if any is -1, this adds 0
    }

    return total;
}

#include <cassert>

// Test driver for countSubstringsWithAllThree
int main() {
    // Basic examples
    assert(countSubstringsWithAllThree("abc") == 1);
    assert(countSubstringsWithAllThree("aaacb") == 4);
    assert(countSubstringsWithAllThree("abcabc") == 10);

    // Strings missing one or more characters
    assert(countSubstringsWithAllThree("a") == 0);
    assert(countSubstringsWithAllThree("ab") == 0);
    assert(countSubstringsWithAllThree("cc") == 0);

    // Single repeated character
    assert(countSubstringsWithAllThree("aaaa") == 0);

    // All permutations of "abc"
    assert(countSubstringsWithAllThree("acb") == 1);
    assert(countSubstringsWithAllThree("bac") == 1);
    assert(countSubstringsWithAllThree("bca") == 1);
    assert(countSubstringsWithAllThree("cab") == 1);
    assert(countSubstringsWithAllThree("cba") == 1);

    // Mixed long string
    assert(countSubstringsWithAllThree("abca") == 2); // "abc", "abca" (with last 'a' extends)
    assert(countSubstringsWithAllThree("cbaabc") == 9);

    // Empty string (if allowed by problem statement, but here we assume non-empty; still test)
    assert(countSubstringsWithAllThree("") == 0);

    // Stress-like test: all a's then b then c
    assert(countSubstringsWithAllThree("aaabc") == 3); // substrings: "abc", "aabc", "aaabc"
}

// The key observation is that for any right endpoint `i`, the number of valid substrings ending at `i` equals the number of choices for the left endpoint `L` such that the substring `s[L..i]` contains all three characters. To compute this efficiently, maintain the most recent occurrence index of each character `'a'`, `'b'`, and `'c'` as we scan left to right. Let `d[0]`, `d[1]`, `d[2]` be the last seen positions of `'a'`, `'b'`, `'c'` respectively, initialized to `-1` (meaning not yet seen). At each step `i`, update the position for the current character. The earliest position among these three last occurrences, `min(d[0], d[1], d[2])`, is the smallest left index such that the substring from that index to `i` contains all three characters. Any left index from 0 up to that minimum will also work, so the number of valid substrings ending at `i` is exactly `min(d[0], d[1], d[2]) + 1`. Sum this over all `i`. Edge cases: if any character has not appeared yet, the minimum will be `-1`, so the contribution is 0 (since `-1 + 1 = 0`), correctly handling strings that lack one or more characters. Time complexity is O(n) where n is the string length, and space complexity is O(1) beyond the input string.
