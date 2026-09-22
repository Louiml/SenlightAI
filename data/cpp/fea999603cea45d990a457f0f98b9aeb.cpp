// Write a C++ function that takes three whitespace-free strings `s1`, `s2`, and `s3` as input and returns a boolean indicating whether the concatenated string `s1 + s2 + s3` is a palindrome. A palindrome is a string that reads the same forward and backward. The function must check the entire concatenated string, not just individual parts. You may assume the input strings consist only of lowercase English letters and their total length is exactly 9 (i.e., `s1.size() + s2.size() + s3.size() == 9`), although your function should still handle any lengths correctly if given different inputs. The main challenge is to correctly combine the three strings and compare characters from both ends symmetrically.

// The main algorithm is straightforward: concatenate the three input strings into one, then use two pointers (or a simple index-based loop) to compare the first character with the last, the second with the second-to-last, etc., up to the middle. If any pair of compared characters differs, the function immediately returns `false`; if all comparisons succeed, return `true`. Edge cases: (1) If the concatenated string is empty (all three inputs empty), it is vacuously a palindrome, so return `true`. (2) A single-character string is always a palindrome. (3) The length can be odd or even; the loop should run `length / 2` times, which correctly handles both. Time complexity is `O(n)` where `n` is the total length of the concatenated string; since the loop runs at most `n/2` comparisons, it is linear. Space complexity is `O(n)` for the concatenated string itself, plus constant auxiliary space. Even though the prompt guarantees length exactly 9, the function is written generically.

#include <string>

// Return true if s1+s2+s3 forms a palindrome; false otherwise.
bool isConcatenatedPalindrome(const std::string& s1,
                             const std::string& s2,
                             const std::string& s3) {
    const std::string combined = s1 + s2 + s3;
    const std::size_t length = combined.size();

    for (std::size_t i = 0; i < length / 2; ++i) {
        if (combined[i] != combined[length - 1 - i]) {
            return false;
        }
    }
    return true;
}

#include <cassert>
#include <string>

// Declaration (as given in solution, but must match exact signature)
bool isConcatenatedPalindrome(const std::string& s1,
                              const std::string& s2,
                              const std::string& s3);

int main() {
    // Test basic palindrome from three parts
    assert(isConcatenatedPalindrome("ab", "c", "ba") == true);
    // Test non-palindrome
    assert(isConcatenatedPalindrome("ab", "c", "de") == false);
    // Test when one part has no letters
    assert(isConcatenatedPalindrome("", "race", "car") == true); // "racecar"
    assert(isConcatenatedPalindrome("", "", "") == true);        // empty string
    // Test single character total
    assert(isConcatenatedPalindrome("a", "", "") == true);
    // Test case where first and last differ
    assert(isConcatenatedPalindrome("xyz", "", "xy") == false); // "xyzxy" → not palindrome
    // Test even length palindrome
    assert(isConcatenatedPalindrome("ab", "cd", "dcba") == true); // "abcddcba"
    // Test odd length palindrome with middle char
    assert(isConcatenatedPalindrome("ab", "c", "ba") == true);
    // Test all same characters
    assert(isConcatenatedPalindrome("aaa", "aaa", "aaa") == true); // length 9 all 'a'
    // Test tricky split: "hello"+"o"+"lleh" → true
    assert(isConcatenatedPalindrome("hello", "o", "lleh") == true);
    return 0;
}
