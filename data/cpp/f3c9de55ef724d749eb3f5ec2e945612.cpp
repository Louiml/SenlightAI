/*
Write a C++ function named `isPalindromeRecursive` that takes a `std::string` by const reference and returns a `bool` indicating whether the string is a palindrome (reads the same forward and backward). The function must be implemented recursively without using loops, and it should handle both even and odd-length strings, as well as strings containing uppercase and lowercase letters (case-sensitive comparison). The function must not modify the original string. Also consider edge cases like empty strings (return true) and single-character strings (return true).
*/

#include <string>

// Recursively checks if the substring of str from start to end is a palindrome.
bool isPalindromeHelper(const std::string& str, int start, int end) {
    if (start >= end) {
        return true; // Base case: empty or crossed the middle
    }
    if (str[start] != str[end]) {
        return false; // Mismatch found
    }
    return isPalindromeHelper(str, start + 1, end - 1);
}

// Public function: returns true if the entire string is a palindrome.
bool isPalindromeRecursive(const std::string& str) {
    if (str.empty()) {
        return true; // Empty string is trivially a palindrome
    }
    return isPalindromeHelper(str, 0, static_cast<int>(str.length()) - 1);
}

#include <cassert>
#include <string>

// The solution function is declared above; include its definition here.

int main() {
    assert(isPalindromeRecursive("") == true);               // empty
    assert(isPalindromeRecursive("a") == true);              // single char
    assert(isPalindromeRecursive("aa") == true);             // even length
    assert(isPalindromeRecursive("ab") == false);            // simple false
    assert(isPalindromeRecursive("racecar") == true);        // classic odd length
    assert(isPalindromeRecursive("BookkooB") == true);       // case-sensitive palindrome
    assert(isPalindromeRecursive("Bookkoob") == false);      // case differs
    assert(isPalindromeRecursive("abccba") == true);         // even palindrome
    assert(isPalindromeRecursive("abcba") == true);          // odd palindrome
    assert(isPalindromeRecursive("abca") == false);          // last char differs
    return 0;
}

// The solution uses a recursive helper that compares characters from the two ends of the string. The recursion stops when the start index becomes greater than or equal to the end index (meaning we’ve crossed the middle, or the string is empty or has one character). At each recursive step, compare `str[start]` and `str[end]`; if they differ, return false immediately; otherwise recurse with `start+1` and `end-1`. Since the string is passed by const reference, no copy is made, and `const` correctness ensures the original is untouched. Time complexity is O(n) in the worst case, where n is the string length, because each character is compared at most once. Space complexity is O(n) due to the recursion stack depth (worst-case for a palindrome of length n, the recursion depth is roughly n/2). Edge cases: empty string → start=0, end=-1 → start>end → true. Single character → start=0, end=0 → start>end is false, but since start==end, the recursive call with (1,-1) triggers start>end → true. Non-palindromic returns early at first mismatch, but worst-case still O(n).
