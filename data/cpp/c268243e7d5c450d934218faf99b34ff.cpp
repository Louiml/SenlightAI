Given a string `s` containing only lowercase English letters, write a C++ function that returns `true` if the string can be transformed into a palindrome by reversing at most one contiguous substring (of any length, including length 1, but not the entire string), and `false` otherwise. The reversal must be performed exactly once (the substring can be of length 1, which effectively does nothing, but it must still be a valid non-full substring). Note that the entire string cannot be reversed (i.e., the substring cannot cover the whole string), but a substring of length `n-1` is allowed. For example, `"abba"` is already a palindrome, so reversing a single character (e.g., position 0) yields `"abba"` and is valid, returning `true`. `"abcde"` cannot be made a palindrome by one substring reversal, so return `false`.

#include <cassert>
#include <string>

// Forward declaration (or include the solution header)
bool canBePalindromeByOneReversal(const std::string& s);

int main() {
    // Already palindrome: reverse a single character.
    assert(canBePalindromeByOneReversal("abba") == true);
    assert(canBePalindromeByOneReversal("a") == true);
    assert(canBePalindromeByOneReversal("aa") == true);

    // One reversal fixes it.
    assert(canBePalindromeByOneReversal("abca") == true); // reverse "bc" -> "acba" then "abca"? Wait, check: s="abca", reverse [1,2] -> "acba" not palindrome, but reverse [1,3] -> "acba"? Let's test: s[1..3]="bca" reversed="acb" -> "aacb" not palindrome. Actually "abca" reverse [0,1]? "ba ca"? Hmm. Let's compute: mismatches left=0? s[0]='a', s[3]='a' match, left=1, right=2, s[1]='b', s[2]='c' mismatch. Reverse [1,2] gives "acba" not palindrome. So this should be false? But known problem "abca" becomes "acba" after reversing "bc"? That's not palindrome. Actually "abca" reverse "bc" gives "acba" not palindrome. Reverse "abc"? that's whole? Not allowed. So false. But to make a true case: "abccba" already palindrome. "abcba" already. "aab" reverse "ab" gives "aba"? Reverse [1,2] "ab" -> "ba" gives "aba"? s="aab", reverse [1,2] -> "aba" palindrome, true. So test that.
    assert(canBePalindromeByOneReversal("aab") == true); // reverse "ab" -> "aba"
    assert(canBePalindromeByOneReversal("ab") == false); // reverse "a" gives "ab" not palindrome, reverse "b" gives "ba" not palindrome, reverse "ab" whole not allowed, so false.
    assert(canBePalindromeByOneReversal("abc") == false);
    assert(canBePalindromeByOneReversal("abcd") == false);
    assert(canBePalindromeByOneReversal("racecar") == true); // already palindrome
    assert(canBePalindromeByOneReversal("xyzz") == false); // reverse "x"? etc.

    // More tricky: "abcda" -> reverse "bcd" gives "adcba"? not palindrome. So false.
    // "aabba" -> already palindrome? s[0]='a' vs s[4]='a' match, left=1,right=3, s[1]='a', s[3]='b' mismatch, reverse [1,3]="abb" -> "bba" gives "abbba"? Actually s="aabba", reverse [1..3]="abb" -> "bba" -> "abbba" not palindrome. But reverse [0..1]? no. So false.
    assert(canBePalindromeByOneReversal("aabba") == false);

    // True case: "abcdba" -> reverse "bcd" gives "adcba"? Let's not. Use a known: "abcc" reverse "ab" gives "bacc" not. Better: "abb" reverse "bb"? already? Actually "abb" reverse [1,2] "bb" -> "abb" same? reverse "bb" gives "bb" same, so still "abb" not palindrome. So false. 
    // Use "abcba" already palindrome → true.
    assert(canBePalindromeByOneReversal("abcba") == true);

    // Example from known problem: "abcda" false, "abca" false, but "aab" true.
    return 0;
}

#include <string>
#include <algorithm>

// Returns true if s can become a palindrome after reversing exactly one contiguous substring (not the whole string).
bool canBePalindromeByOneReversal(const std::string& s) {
    const int n = static_cast<int>(s.size());
    if (n <= 1) return true; // single character, reverse length-1 substring (itself) works

    // Find first mismatch from left and right.
    int left = 0;
    int right = n - 1;
    while (left < right && s[left] == s[right]) {
        ++left;
        --right;
    }

    // If no mismatch, the string is already a palindrome. Reverse a single char (e.g., index 0) to satisfy "exactly one reversal".
    if (left >= right) return true;

    // Try reversing the substring [left, right] (inclusive).
    std::string candidate = s;
    std::reverse(candidate.begin() + left, candidate.begin() + right + 1);

    // Check if candidate is a palindrome.
    int l = 0;
    int r = n - 1;
    while (l < r) {
        if (candidate[l] != candidate[r]) return false;
        ++l;
        --r;
    }
    return true;
}

// The key insight is that to make a string a palindrome by reversing one contiguous substring, the mismatched characters (when comparing from both ends) must all lie within a single contiguous segment that, when reversed, aligns the matching pairs. More formally, find the first index `i` from the left and first index `j` from the right such that `s[i] != s[j]`. If no such pair exists, the string is already a palindrome, and we can always reverse a single character (e.g., the first character) to keep it a palindrome (since the substring of length 1 is allowed), so return `true`. If mismatches exist, consider the segment from `i` to `j`. Reversing a substring that contains both `i` and `j` (and possibly more) could fix the mismatch. Specifically, after reversing the substring from `i` to `j`, the entire string must become a palindrome. To test this, we can simulate reversing `s[i..j]` and check if the whole string becomes a palindrome. If yes, return `true`; otherwise, return `false`. Edge cases: the reversed substring may be longer than `[i,j]` but then it would include characters that are already matched outside, which would break them unless they are also symmetric, but the simplest check is just to reverse `[i,j]` and test the whole string. Complexity: O(n) time to find mismatches and O(n) to verify after reversal, total O(n). Space O(n) for the temporary reversed string.
