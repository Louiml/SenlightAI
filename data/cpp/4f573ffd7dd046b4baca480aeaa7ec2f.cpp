/*
Write a C++ function that takes a single string as input and returns `true` if the string is a palindrome (reads the same forward and backward, case-sensitive, including spaces and punctuation), and `false` otherwise. The function must handle empty strings (which are considered palindromes) and strings of any length. Implement the function with a descriptive name and const-correct parameters.
*/

#include <string>

// Returns true if the input string is a palindrome (case-sensitive), false otherwise.
// An empty string is considered a palindrome.
bool isPalindrome(const std::string& text) {
    int left = 0;
    int right = static_cast<int>(text.length()) - 1;

    while (left < right) {
        if (text[left] != text[right]) {
            return false;
        }
        ++left;
        --right;
    }
    return true;
}

#include <cassert>

int main() {
    assert(isPalindrome("racecar") == true);
    assert(isPalindrome("hello") == false);
    assert(isPalindrome("A") == true);
    assert(isPalindrome("") == true);
    assert(isPalindrome("abccba") == true);
    assert(isPalindrome("abcba") == true);
    assert(isPalindrome("abca") == false);
    assert(isPalindrome("A man a plan a canal Panama") == false); // case and spaces matter
    assert(isPalindrome("tacocat") == true);
    assert(isPalindrome("12321") == true);
    return 0;
}

// The solution approach is straightforward: compare characters from the two ends of the string moving inward. For a string of length `n`, compare `s[i]` with `s[n-1-i]` for all `i` from `0` to `n/2` (or `n-1`, but early exit when a mismatch is found). If any mismatch occurs, return `false` immediately. If all comparisons pass, return `true`. Edge cases: an empty string is a palindrome (loop doesn't execute), a single character is trivially a palindrome, and the function is case-sensitive so `"AbA"` is a palindrome but `"Aba"` is not. The time complexity is O(n) because in the worst case we examine about n/2 characters; the space complexity is O(1) since no additional storage is used (except for the input string itself).
