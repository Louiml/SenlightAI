// Write a C++ function named `isAlmostPalindrome` that takes a non-empty string (containing only lowercase English letters) and returns a `bool` indicating whether the string can become a palindrome by deleting **at most one** character. The function must handle strings of any length, including very long ones, and must check both possibilities when a mismatch is found: deleting the character on the left or deleting the character on the right. The solution should be efficient, working in linear time without modifying the input string.

#include <cassert>
#include <string>

// (Function is declared above)

int main() {
    // Already a palindrome
    assert(isAlmostPalindrome("aba") == true);
    assert(isAlmostPalindrome("abba") == true);
    assert(isAlmostPalindrome("racecar") == true);

    // Can be fixed by deleting one character
    assert(isAlmostPalindrome("abca") == true);  // delete 'c' -> "aba"
    assert(isAlmostPalindrome("abc") == true);   // delete 'c' -> "ab"
    assert(isAlmostPalindrome("deeee") == true); // delete 'd' -> "eeee"
    assert(isAlmostPalindrome("ebcbbececabbacecbbcbe") == true); // from LeetCode

    // Single character
    assert(isAlmostPalindrome("a") == true);

    // Cannot be fixed by deleting one character
    assert(isAlmostPalindrome("abcde") == false);
    assert(isAlmostPalindrome("needed") == false); // "neede" is not palindrome either

    // Edge: mismatch at the very center
    assert(isAlmostPalindrome("acxba") == false); // both deletions fail
    assert(isAlmostPalindrome("abxba") == true);  // delete 'x' -> "abba"

    return 0;
}

#include <string>

// Returns true if the string can become a palindrome by deleting at most one character.
bool isAlmostPalindrome(const std::string& s) {
    // Helper to check if a substring (by indices) is a palindrome.
    auto isPalindromeRange = [&s](int left, int right) -> bool {
        while (left < right) {
            if (s[left] != s[right]) {
                return false;
            }
            ++left;
            --right;
        }
        return true;
    };

    int left = 0;
    int right = static_cast<int>(s.size()) - 1;

    while (left < right) {
        if (s[left] != s[right]) {
            // Try deleting left character or deleting right character.
            return isPalindromeRange(left + 1, right) ||
                   isPalindromeRange(left, right - 1);
        }
        ++left;
        --right;
    }
    return true;
}

// The main idea is to use a two-pointer technique scanning from both ends toward the middle. When the characters at the two pointers match, move both inward. When a mismatch is found, the only way to fix it (if possible) is to delete one of the two mismatched characters. Therefore, we test two substrings: one with the left character removed (range `left+1` to `right`) and one with the right character removed (range `left` to `right-1`). Each of these substrings must be a full palindrome; if either is, the original string is an "almost palindrome". If both fail, then no single deletion can fix it. Edge cases include: the whole string already being a palindrome (return `true` immediately), a string with exactly one character (trivially true), and strings where the mismatch occurs right at the center (both deletion paths succeed). Time complexity is O(n) because we scan at most twice: once for the initial pass (worst-case until first mismatch) and then one full scan for each of the two deletion checks (but each check stops early on failure; combined worst-case is O(2n) = O(n)). Space complexity is O(1) beyond the input string.
