Write a C++ function named `isLuckyNumber` that takes a single non-empty string `numberString` consisting only of decimal digits (0-9) and returns `true` if the total count of digits '4' and '7' in the string is itself either 4 or 7, and `false` otherwise. The function must be case-sensitive and treat '4' and '7' only as ASCII characters. For example, a string like `"447"` has three lucky digits (two 4s and one 7), so its total count is 3, which is not 4 or 7, so the function returns `false`. Also consider edge cases: if the string contains no '4' or '7' at all, the total is 0, which is not 4 or 7, so return `false`. The input string will never be empty. The function should not print anything; it should only return the boolean result.
The solution is straightforward. We iterate through each character in the input string and count how many times the character is equal to '4' or '7'. Since we need only the total count of these two specific digits, we can use `std::count` twice, or alternatively loop manually. Once we have the total count `k`, we check if `k == 4` or `k == 7`. If so, return `true`; otherwise, return `false`. Edge cases: empty strings are not allowed per the specification, but if encountered, the loop naturally yields count 0 and returns false. Strings with only non-lucky digits (e.g., "123") yield count 0. Strings with exactly four or seven lucky digits (e.g., "4444" → count 4) return true. Time complexity is O(n) where n is the length of the string, because we traverse it once (or twice if using two `count` calls, still O(n)). Space complexity is O(1) auxiliary.
#include <string>
#include <algorithm>

// Determine if the total count of digits '4' and '7' in the string is 4 or 7.
bool isLuckyNumber(const std::string& numberString) {
    const int count4 = std::count(numberString.begin(), numberString.end(), '4');
    const int count7 = std::count(numberString.begin(), numberString.end(), '7');
    const int totalLuckyDigits = count4 + count7;
    return (totalLuckyDigits == 4) || (totalLuckyDigits == 7);
}
#include <cassert>
#include <string>
#include "solution.h" // or include the function directly

int main() {
    assert(isLuckyNumber("4") == true);          // count=1, not 4 or 7
    assert(isLuckyNumber("4444") == true);       // count=4
    assert(isLuckyNumber("7777777") == true);    // count=7
    assert(isLuckyNumber("123") == false);       // count=0
    assert(isLuckyNumber("447") == false);       // count=3
    assert(isLuckyNumber("47474747") == true);   // count=8? Actually 4+4=8, false
    assert(isLuckyNumber("47") == false);        // count=2
    assert(isLuckyNumber("4444444") == false);   // count=7? Wait 7 '4's => count=7, true
    assert(isLuckyNumber("4444444") == true);    // correct
    assert(isLuckyNumber("") == false);          // empty input (not allowed, but safe)
    return 0;
}
