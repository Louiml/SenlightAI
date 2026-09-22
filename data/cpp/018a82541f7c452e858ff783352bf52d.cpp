Write a C++ function named `findSmallestSubstringWithAllCharacters` that takes a `std::set<char>` of unique required characters and a `std::string` as input, and returns the smallest contiguous substring of the input string that contains every character from the set at least once. If no such substring exists, return an empty string. The function must handle duplicate characters in the input string, case-sensitively, and should assume the set is non-empty. For example, with set `{ 'a', 'b', 'c' }` and string `"abbcbcba"`, the function should return `"cba"`. The solution should be self-contained (only standard headers), efficient for moderately long strings, and must not use any global variables or mutate the input.

The solution uses a sliding window approach with two pointers: `left` (start of window) and `right` (end of window). As we expand `right` through the string, we count occurrences of each required character inside the current window. When all required characters are present (i.e., the number of distinct required characters with count > 0 equals the set size), we attempt to shrink the window from the left while still keeping all required characters present, tracking the minimum window length and its start index. The key edge cases: (1) If the input string is shorter than the set size, no solution exists, return empty; (2) If the set has only one character, the smallest substring is just that character if it appears; (3) Characters in the string that are not in the set are ignored for counting but affect window boundaries; (4) The window expansion and contraction must maintain correct counts, and after finding a valid window, we continue expanding `right` to look for even shorter windows. Time complexity is O(n) where n is the string length, because each character is processed at most twice (once when `right` moves, once when `left` moves). Space complexity is O(k) where k is the size of the set, for the count map.

#include <string>
#include <set>
#include <map>
#include <climits>

// Returns the smallest substring of str that contains all characters in requiredChars.
// Returns empty string if no such substring exists.
std::string findSmallestSubstringWithAllCharacters(const std::set<char>& requiredChars, const std::string& str) {
    if (requiredChars.empty() || str.empty()) {
        return "";
    }

    std::map<char, int> charCount;  // counts of each required char in current window
    int requiredSize = requiredChars.size();
    int formed = 0;  // number of distinct required chars currently satisfied

    int left = 0;
    int minLen = INT_MAX;
    int minStart = 0;

    for (int right = 0; right < (int)str.length(); ++right) {
        char c = str[right];
        if (requiredChars.find(c) != requiredChars.end()) {
            charCount[c]++;
            if (charCount[c] == 1) {
                formed++;
            }
        }

        // Shrink window while all required chars are present
        while (formed == requiredSize) {
            int currentLen = right - left + 1;
            if (currentLen < minLen) {
                minLen = currentLen;
                minStart = left;
            }

            char leftChar = str[left];
            if (requiredChars.find(leftChar) != requiredChars.end()) {
                charCount[leftChar]--;
                if (charCount[leftChar] == 0) {
                    formed--;
                }
            }
            left++;
        }
    }

    if (minLen == INT_MAX) {
        return "";
    }
    return str.substr(minStart, minLen);
}

#include <cassert>
#include <string>
#include <set>

int main() {
    std::set<char> setABC = {'a', 'b', 'c'};
    assert(findSmallestSubstringWithAllCharacters(setABC, "abbcbcba") == "cba");
    assert(findSmallestSubstringWithAllCharacters(setABC, "abc") == "abc");
    assert(findSmallestSubstringWithAllCharacters(setABC, "ab") == "");
    assert(findSmallestSubstringWithAllCharacters(setABC, "aabbcc") == "abc");
    assert(findSmallestSubstringWithAllCharacters(setABC, "cba") == "cba");
    assert(findSmallestSubstringWithAllCharacters(setABC, "abac") == "bac");
    assert(findSmallestSubstringWithAllCharacters(setABC, "bcab") == "cab");

    std::set<char> setA = {'a'};
    assert(findSmallestSubstringWithAllCharacters(setA, "bbba") == "a");
    assert(findSmallestSubstringWithAllCharacters(setA, "bbb") == "");
    assert(findSmallestSubstringWithAllCharacters(setABC, "") == "");
    assert(findSmallestSubstringWithAllCharacters(std::set<char>{}, "abc") == "");
    return 0;
}
