Write a C++ function that determines whether a given string is "Funny" or "Not Funny" according to the following rule: a string is Funny if, for every adjacent pair of characters (from index 1 to length-1), the absolute difference between the ASCII values of the characters at those positions in the original string is exactly equal to the absolute difference between the ASCII values of the corresponding characters in the reversed string. The function should accept a single `std::string` parameter (non-empty, consisting of printable ASCII characters) and return a `bool` (`true` for Funny, `false` for Not Funny). You may assume the input contains only lowercase/uppercase letters or digits, but handle any printable ASCII characters generically. The function must be const-correct and efficient for strings up to 10^5 characters long.
#include <cassert>
#include <string>

// The solution function is declared above; include it in the same file.

int main() {
    assert(isFunny("acxz") == true);       // abs diff: 2,3,22 vs 2,3,22
    assert(isFunny("bcxz") == false);      // abs diff differ at last pair
    assert(isFunny("a") == true);          // single character, no pairs
    assert(isFunny("ab") == true);         // diff=1 both ways
    assert(isFunny("ba") == true);         // diff=1 both ways
    assert(isFunny("zz") == true);         // diff=0 both ways
    assert(isFunny("abcd") == true);       // all diffs=1
    assert(isFunny("dcba") == true);       // all diffs=1
    assert(isFunny("azby") == false);      // pattern breaks
    assert(isFunny("1234") == true);       // digits, diffs=1
    return 0;
}
#include <string>
#include <algorithm>
#include <cstdlib>

// Returns true if the absolute differences between adjacent characters
// in the original string match those in the reversed string.
bool isFunny(const std::string& s) {
    // Reverse a copy of the string.
    std::string rev = s;
    std::reverse(rev.begin(), rev.end());
    
    // Compare adjacent differences for positions 1..n-1.
    for (size_t i = 1; i < s.length(); ++i) {
        const int diffOriginal = std::abs(static_cast<int>(s[i]) - static_cast<int>(s[i-1]));
        const int diffReversed = std::abs(static_cast<int>(rev[i]) - static_cast<int>(rev[i-1]));
        if (diffOriginal != diffReversed) {
            return false;
        }
    }
    return true;
}
// The solution involves constructing the reverse of the input string and then comparing adjacent absolute differences between the original and reversed strings. For each index `i` from 1 to `n-1` (where `n` is the string length), compute `abs(s[i] - s[i-1])` and `abs(rev[i] - rev[i-1])`. If any pair differs, return `false`; otherwise return `true`. Edge cases: strings of length 1 (or 0) have no adjacent pairs, so they are trivially Funny (though the problem states non-empty; length 1 has zero comparisons, so the loop never runs and returns true). The reversed string can be created by copying and using `std::reverse`, or by comparing directly without storing (e.g., compute `s[i]` vs `s[n-1-i]`), but storing is simpler. Time complexity: O(n) for both reversing and the loop. Space complexity: O(n) for the reversed string copy (or O(1) if comparing in-place without storing, but storing is acceptable and more readable). A more space-efficient approach compares `s[i]` with `s[n-1-i]` directly for each pair, but the classic solution stores the reverse.
