/*
Write a C++ function `bool isBeautifulString(const std::string& s)` that determines whether a string is "beautiful" according to this rule: for every index `i` from `0` to `n-2`, the character at position `i` must not appear later in the string at any position `j > i` unless the character at position `i+1` is the same as the character at position `i`. In other words, if a character repeats somewhere later, then it must be immediately followed by the same character (forming a block of identical characters, like "aa" or "ccc"). The string contains only lowercase English letters and has length at least 1. The function should return `true` if the string satisfies this condition for all indices, and `false` otherwise. For example, `"aab"` is beautiful because for `i=0`, `'a'` is found later at `pos=1`, and `s[1]=='a'` equals `s[0]`, and for `i=1`, no later occurrence exists; `"aba"` is not beautiful because at `i=0`, `'a'` appears later at `pos=2`, but `s[1]` is `'b'` not `'a'`. Also handle the case where the repeated character appears more than once: e.g., `"aaa"` is beautiful because for `i=0`, `'a'` appears later at `pos=1` and `s[1]=='a'`; for `i=1`, it appears later at `pos=2` and `s[2]=='a'`. However, `"aabaa"` is not beautiful because at `i=2`, `'b'` appears later at `pos=4`, but `s[3]` is `'a'` not `'b'`. The function must be const-correct and include necessary headers.
*/
#include <string>

// Returns true if for every index i, any later occurrence of s[i]
// must be immediately preceded by the same character (i.e., s[i+1] == s[i]).
bool isBeautifulString(const std::string& s) {
    for (std::size_t i = 0; i < s.size(); ++i) {
        // Find the first occurrence of s[i] after position i
        std::size_t pos = s.find(s[i], i + 1);
        if (pos != std::string::npos) {
            // A later occurrence exists; check the immediate next character
            // Since pos > i, i+1 is valid (i < s.size()-1)
            if (s[i + 1] != s[i]) {
                return false;
            }
        }
    }
    return true;
}
#include <cassert>
#include <string>

// Function under test (declared here for clarity; in practice include the header)
bool isBeautifulString(const std::string& s);

int main() {
    // Single character strings are always beautiful
    assert(isBeautifulString("a") == true);
    // Repeating character in a block
    assert(isBeautifulString("aaa") == true);
    // Two identical characters followed by a different one
    assert(isBeautifulString("aab") == true);
    // Alternate characters where repetition is not adjacent
    assert(isBeautifulString("aba") == false);
    // Repetition of a character later but not immediately after the first occurrence
    assert(isBeautifulString("abca") == false);
    // Mixed: "aabb" is beautiful because each repeated char is adjacent
    assert(isBeautifulString("aabb") == true);
    // "aabaa" fails because 'b' repeats later not adjacent
    assert(isBeautifulString("aabaa") == false);
    // All distinct characters
    assert(isBeautifulString("abcd") == true);
    // Long block of same character
    assert(isBeautifulString("ccccc") == true);
    // Edge with multiple blocks and a later non-adjacent repeat
    assert(isBeautifulString("aabbaa") == false); // at i=2, 'b' appears later at pos=3, s[3]=='b' okay, but at i=3 no later; at i=0 'a' appears later at pos=4, s[1]=='a' okay, but at i=4 'a' appears later at pos=5, s[5]='a'? actually s[4]='a', s[5]='a'? Let's correct: "aabbaa" length 6: indices 0:a,1:a,2:b,3:b,4:a,5:a. At i=0, find 'a' at pos=1, s[1]=='a' ok. At i=1, find 'a' at pos=4, s[2]=='b' != 'a' → false.
    assert(isBeautifulString("aabbaa") == false);
    return 0;
}
// The core idea is to simulate the original snippet's logic but extract it into a clean, reusable function. For each index `i` from 0 to `n-1`, we search for the first occurrence of `s[i]` in the substring starting at `i+1`. If such a later position `pos` exists (i.e., `pos != std::string::npos`), then we must check whether the character immediately following `i` (i.e., `s[i+1]`) is equal to `s[i]`. If it is not, then the string is not beautiful, and we return `false` immediately. If it is equal, we continue to the next `i`. If no later occurrence exists, we also continue. The loop runs at most `n` times, and each `find` call scans up to `n` characters, so the worst-case time complexity is \(O(n^2)\) (e.g., when the string consists of all distinct characters, each find scans the entire remaining suffix). Space complexity is \(O(1)\) auxiliary, not counting the input string. Edge cases: a string of length 1 always returns `true` because there is no `i+1` to compare; a string with repeated characters that form blocks like `"aa"` or `"aaa"` passes because each later occurrence is immediately preceded by the same character; a string like `"abca"` fails at `i=0` because `'a'` appears later at position 3 but `s[1]` is `'b'`. The function must handle `std::string::npos` correctly and avoid out-of-bounds access when `i` is the last index, because `i+1` would be out of range; however, for the last index, no later occurrence can exist since `find` starts at `i+1` which is beyond the end, so the inner condition never triggers, but we must be careful not to access `s[i+1]` unless we know a later occurrence exists.
