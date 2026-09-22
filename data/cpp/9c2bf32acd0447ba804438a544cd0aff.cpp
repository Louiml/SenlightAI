Write a C++ function named `decideGirlOrBoy` that takes a non-empty string `username` consisting only of lowercase English letters (a–z) and returns a string indicating the result: if the number of distinct characters in the username is even, return `"CHAT WITH HER!"`; if odd, return `"IGNORE HIM!"`. The function must not modify the input string and must use constant space regardless of string length. For example, for `"abca"`, the distinct characters are `a`, `b`, `c` (3), so the result is `"IGNORE HIM!"`.

The solution counts the number of unique characters in the input string. This is done by using a fixed-size boolean or integer array of 26 elements (one per lowercase letter). Iterate through each character, compute its index by subtracting `'a'`, and mark that index as seen. After processing the entire string, count how many indices are marked. If the count is even, return the "CHAT WITH HER!" message; otherwise return "IGNORE HIM!". Edge cases include a single-character string (odd count → "IGNORE HIM!"), strings with all same characters (count = 1 → odd), and strings containing all 26 letters (even → "CHAT WITH HER!"). The algorithm runs in O(n) time (where n is the length of the string) and O(1) space since the array size is fixed at 26.

#include <string>
#include <array>

// Return "CHAT WITH HER!" if the number of distinct characters is even,
// otherwise return "IGNORE HIM!".
std::string decideGirlOrBoy(const std::string& username) {
    std::array<bool, 26> seen{};
    for (char ch : username) {
        seen[ch - 'a'] = true;
    }
    int distinctCount = 0;
    for (bool value : seen) {
        if (value) {
            ++distinctCount;
        }
    }
    return (distinctCount % 2 == 0) ? "CHAT WITH HER!" : "IGNORE HIM!";
}

#include <cassert>
#include <string>

// Solution function declared above (assume included)

int main() {
    assert(decideGirlOrBoy("a") == "IGNORE HIM!");
    assert(decideGirlOrBoy("ab") == "CHAT WITH HER!");
    assert(decideGirlOrBoy("abc") == "IGNORE HIM!");
    assert(decideGirlOrBoy("abca") == "IGNORE HIM!");
    assert(decideGirlOrBoy("abcabc") == "CHAT WITH HER!");
    assert(decideGirlOrBoy("zzz") == "IGNORE HIM!");
    assert(decideGirlOrBoy("abcdefghijklmnopqrstuvwxyz") == "CHAT WITH HER!");
    assert(decideGirlOrBoy("aaaaabbbbb") == "CHAT WITH HER!");
}
