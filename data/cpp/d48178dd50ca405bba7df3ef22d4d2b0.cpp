Write a standalone C++ function that, given a string `s` (containing only lowercase English letters) and a character `ch`, returns a `bool` indicating whether `ch` appears at any even index in `s` (i.e., at index 0, 2, 4, …). The function must not mutate the input string or use any global state. The input string will always be non-empty (length ≥ 1). The function should handle strings of any length, including those where the last even index is beyond the last character (but that won’t happen since we only iterate valid indices). For example, for `s = "abc"` and `ch = 'a'`, the answer is `true` because `'a'` is at index 0 (even). For `s = "abc"` and `ch = 'c'`, the answer is `false` because `'c'` is at index 2? Wait, index 2 is even? Actually index 2 is even, so that would be true. Let’s correct: For `s = "abc"` and `ch = 'c'`, index 2 is even, so answer is `true`. For `s = "abcd"` and `ch = 'b'`, index 1 is odd, so answer is `false`. The function should be named `hasCharAtEvenIndex`.

The solution is straightforward: iterate over even indices `i = 0, 2, 4, ...` while `i < s.length()`. At each step, compare `s[i]` to the target character. If a match is found, return `true` immediately without checking the rest. If no match is found after the loop, return `false`. This is efficient because we only check roughly half the characters, and we stop early on success. Edge cases: if the target character appears at multiple even indices, the first one suffices; if the string is of length 1, we only check index 0; if the character does not appear at any even index, we return `false`. For a string of length `n`, the loop runs at most `⌈n/2⌉` times, so time complexity is `O(n)` (specifically `O(n/2)` but asymptotically `O(n)`). Space complexity is `O(1)` since we only use a loop counter and no extra data structures.

#include <string>

// Returns true if the given character appears at any even index (0-based) in the string.
bool hasCharAtEvenIndex(const std::string& s, char ch) {
    for (std::size_t i = 0; i < s.size(); i += 2) {
        if (s[i] == ch) {
            return true;
        }
    }
    return false;
}

#include <cassert>
#include <string>

// The solution function is assumed to be defined above.
bool hasCharAtEvenIndex(const std::string& s, char ch);

int main() {
    // Basic cases
    assert(hasCharAtEvenIndex("a", 'a') == true);
    assert(hasCharAtEvenIndex("a", 'b') == false);
    assert(hasCharAtEvenIndex("ab", 'a') == true);
    assert(hasCharAtEvenIndex("ab", 'b') == false);
    // Multiple even indices, first match
    assert(hasCharAtEvenIndex("abcabc", 'a') == true);
    assert(hasCharAtEvenIndex("abcabc", 'c') == true);  // index 2
    // Character at odd index only
    assert(hasCharAtEvenIndex("abcd", 'b') == false);
    // Character not present at all
    assert(hasCharAtEvenIndex("hello", 'z') == false);
    // Longer string, matches at later even index
    assert(hasCharAtEvenIndex("abcdefghijklmnop", 'k') == true); // index 10
    // Ensure no out-of-bounds and correct for odd length
    assert(hasCharAtEvenIndex("xyzxyzxyz", 'x') == true);
    assert(hasCharAtEvenIndex("xyzxyzxyz", 'z') == true); // index 2,4,8
    return 0;
}
