/*
Write a C++ function that takes two strings of equal length (containing only lowercase English letters) and returns `true` if the first string is the reverse of the second string, otherwise returns `false`. The function must compare characters one by one, starting from the last character of the first string and the first character of the second string, moving inward (left index decreasing, right index increasing) until all positions are checked. The function should handle empty strings as valid reversals (both empty → true). Do not use any STL algorithms like `reverse` or `equal`; implement the comparison manually with a loop.
*/

#include <string>

// Return true if s1 is the reverse of s2.
bool isReverse(const std::string& s1, const std::string& s2) {
    // If lengths differ, they cannot be reverses.
    if (s1.size() != s2.size()) return false;

    int i = static_cast<int>(s1.size()) - 1; // last index of s1
    int j = 0;                              // first index of s2

    // Compare from the outside inward.
    while (i >= 0 && j < static_cast<int>(s2.size())) {
        if (s1[i] != s2[j]) {
            return false;
        }
        --i;
        ++j;
    }

    return true;
}

#include <cassert>
#include <string>

// Declaration of the function under test.
bool isReverse(const std::string& s1, const std::string& s2);

int main() {
    // Basic reversal.
    assert(isReverse("abc", "cba") == true);
    // Palindrome.
    assert(isReverse("racecar", "racecar") == true);
    // Single character.
    assert(isReverse("a", "a") == true);
    // Empty strings.
    assert(isReverse("", "") == true);
    // Mismatch in the middle.
    assert(isReverse("abc", "cbd") == false);
    // Unequal lengths (should be false).
    assert(isReverse("ab", "cba") == false);
    // Second string is not reverse.
    assert(isReverse("hello", "olleh") == true);
    assert(isReverse("hello", "ollem") == false);
    return 0;
}

// The main algorithm compares the last character of `s1` with the first character of `s2`, then the second-to-last with the second, and so on, until either a mismatch is found or all characters are processed. This is equivalent to checking `s1[i] == s2[j]` where `i` starts at `s1.size()-1` and decrements, while `j` starts at 0 and increments. The loop condition `(i >= 0 && j < s2.size())` ensures we stop when either index goes out of bounds; if lengths are equal, both will reach their boundary at the same time. Edge cases: empty strings both result in no loop iterations → true; strings of unequal length (if the task allowed, but we assume equal length) would still work because the loop stops early and returns false if any mismatch occurs before the shorter string ends, but if the shorter string is a prefix-reverse, it would incorrectly return true — however, since the task specifies equal length, this is not an issue. Time complexity is O(n) where n is the length of the strings. Space complexity is O(1) as only indices and a flag are used.
