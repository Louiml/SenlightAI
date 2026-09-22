Write a C++ function named `canBePalindromeAfterOneRemoval` that takes a non-empty string `s` and returns `true` if the string can become a palindrome by removing at most one character, otherwise `false`. The function must handle strings containing lowercase English letters only, and must consider that removing zero characters is also allowed (i.e., an already-palindromic string is valid). The input string is passed by const reference, and the function must be `const`-correct (no modification of the input). Do not include a `main` function in your solution; only provide the function definition with necessary headers.
// The algorithm uses a two-pointer approach. Start with `i = 0` and `j = s.size() - 1`. While `i <= j`, check if `s[i] == s[j]`. If they are equal, move both pointers inward. If they differ, then to fix the mismatch we have exactly two options: either remove `s[i]` (which means checking if the substring from `i+1` to `j` is a palindrome) or remove `s[j]` (checking substring from `i` to `j-1`). If either of those substrings is a palindrome, the whole string can be made into a palindrome by removing that one character. If both fail, return `false`. If the loop completes without ever needing a removal, the string is already a palindrome, so return `true`. Edge cases: an empty string (though task says non-empty, handle safely), a single character (already palindrome), strings where mismatch occurs at the middle (e.g., "abca" – remove 'c' or 'b' works). Time complexity is O(n) because each of the two palindrome checks runs in O(n) in the worst case, but they are sequential at most once. Space complexity is O(1) auxiliary (excluding input storage).
#include <string>

// Returns true if the given string can become a palindrome by removing at most one character.
bool canBePalindromeAfterOneRemoval(const std::string& s) {
    // Helper lambda to check if a substring is a palindrome.
    auto isPalindrome = [](const std::string& str, int left, int right) -> bool {
        while (left < right) {
            if (str[left] != str[right]) {
                return false;
            }
            ++left;
            --right;
        }
        return true;
    };

    int i = 0;
    int j = static_cast<int>(s.size()) - 1;

    while (i < j) {
        if (s[i] == s[j]) {
            ++i;
            --j;
        } else {
            // Try removing s[i] or removing s[j].
            return isPalindrome(s, i + 1, j) || isPalindrome(s, i, j - 1);
        }
    }

    // No mismatch found, already a palindrome.
    return true;
}
#include <cassert>
#include <string>

// Declaration of the function under test (assume from the solution above).
bool canBePalindromeAfterOneRemoval(const std::string& s);

int main() {
    // Already palindrome (zero removals needed)
    assert(canBePalindromeAfterOneRemoval("racecar") == true);
    assert(canBePalindromeAfterOneRemoval("a") == true);
    assert(canBePalindromeAfterOneRemoval("aa") == true);

    // One removal needed
    assert(canBePalindromeAfterOneRemoval("abca") == true);      // remove 'b' or 'c'
    assert(canBePalindromeAfterOneRemoval("abcda") == true);     // remove 'd'
    assert(canBePalindromeAfterOneRemoval("abccba") == true);    // already palindrome

    // Cannot be fixed with one removal
    assert(canBePalindromeAfterOneRemoval("abc") == false);      // need two removals
    assert(canBePalindromeAfterOneRemoval("abcd") == false);
    assert(canBePalindromeAfterOneRemoval("abcaa") == false);    // removing one leaves non-palindrome

    // Edge: longer strings
    assert(canBePalindromeAfterOneRemoval("deeee") == true);     // remove 'd'
    assert(canBePalindromeAfterOneRemoval("eeeed") == true);     // remove 'd'
    assert(canBePalindromeAfterOneRemoval("abcdecba") == true);  // remove 'd'

    // Single character and two characters
    assert(canBePalindromeAfterOneRemoval("ab") == true);        // remove one

    return 0;
}
