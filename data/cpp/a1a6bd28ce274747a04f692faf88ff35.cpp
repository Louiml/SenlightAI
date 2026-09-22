// Write a C++ function named `canBecomePalindrome` that takes a non-empty string `s` and a non-negative integer `maxDeletes`. The function must return `true` if the string can be transformed into a palindrome by deleting at most `maxDeletes` characters, and `false` otherwise. The deletion can remove any characters from anywhere in the string, and the order of the remaining characters must be preserved. Handle edge cases such as empty strings, strings with a single character, and cases where `maxDeletes` is zero. The function should be efficient, using a recursive or iterative two-pointer approach with memoization or pruning, and must not modify the original string.
// The solution uses a two-pointer technique: start with `left = 0` and `right = s.size()-1`. At each step, compare `s[left]` and `s[right]`. If they match, move both pointers inward and continue. If they differ and we still have deletions available, we have two choices: delete `s[left]` (i.e., increment `left`) or delete `s[right]` (i.e., decrement `right`), but after that deletion, we must continue with strictly fewer allowed deletions. Since `maxDeletes` is small, we can explore both branches recursively. The base case is when `left > right` (a palindrome) or when characters differ and no deletions remain (fail). Important edge cases: empty string (trivially true), one character (always true), strings where deleting from the middle or multiple deletions are needed, and `maxDeletes` larger than necessary. Time complexity is O(2^maxDeletes) in the worst case due to branching, but since `maxDeletes` is typically small, it is practical; space complexity is O(maxDeletes) for the recursion stack.
#include <string>

// Returns true if s can become a palindrome by deleting at most maxDeletes characters.
bool canBecomePalindrome(const std::string& s, int maxDeletes) {
    if (maxDeletes < 0) return false;
    
    int left = 0;
    int right = static_cast<int>(s.size()) - 1;
    
    while (left <= right) {
        if (s[left] == s[right]) {
            ++left;
            --right;
        } else if (maxDeletes > 0) {
            // Try deleting left character or right character
            return canBecomePalindrome(s.substr(left + 1, right - left), maxDeletes - 1) ||
                   canBecomePalindrome(s.substr(left, right - left), maxDeletes - 1);
        } else {
            return false;
        }
    }
    
    return true;
}
#include <cassert>

int main() {
    assert(canBecomePalindrome("aba", 0) == true);
    assert(canBecomePalindrome("abca", 1) == true); // delete 'c' or 'b'
    assert(canBecomePalindrome("abc", 1) == false); // need 2 deletions
    assert(canBecomePalindrome("abc", 2) == true); // delete 'a' and 'c' -> "b"
    assert(canBecomePalindrome("", 0) == true);
    assert(canBecomePalindrome("a", 0) == true);
    assert(canBecomePalindrome("ab", 0) == false);
    assert(canBecomePalindrome("ab", 1) == true); // delete one
    assert(canBecomePalindrome("racecar", 0) == true);
    assert(canBecomePalindrome("racecars", 1) == true); // delete 's' at end
    return 0;
}
