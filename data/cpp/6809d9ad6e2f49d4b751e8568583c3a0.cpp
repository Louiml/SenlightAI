/*
Write a C++ function named `minimumWindowSubstring` that takes two strings, `s` and `t`, and returns the smallest contiguous substring of `s` that contains all characters from `t` (including duplicates), or an empty string if no such window exists. If there are multiple minimal windows of the same length, any one can be returned. The strings consist only of uppercase and lowercase English letters, with lengths between 1 and 10^5 inclusive. The function must handle cases where `t` is longer than `s`, where the window is the entire string `s`, and where characters in `t` appear multiple times.
*/

#include <string>
#include <unordered_map>
#include <climits>

// Returns the smallest substring of s that contains all characters of t (including duplicates).
std::string minimumWindowSubstring(const std::string& s, const std::string& t) {
    if (t.empty() || s.length() < t.length()) return "";

    // Count of required characters in t
    std::unordered_map<char, int> needed;
    for (char c : t) needed[c]++;

    const int required = needed.size(); // number of distinct characters needed
    int formed = 0;
    int left = 0, right = 0;
    int minLen = INT_MAX;
    int minStart = 0;

    std::unordered_map<char, int> windowCounts;

    while (right < s.length()) {
        char c = s[right];
        windowCounts[c]++;

        // Check if this character meets its required count
        if (needed.count(c) && windowCounts[c] == needed[c]) {
            formed++;
        }

        // Try to shrink the window from the left
        while (left <= right && formed == required) {
            char leftChar = s[left];
            int currentLen = right - left + 1;
            if (currentLen < minLen) {
                minLen = currentLen;
                minStart = left;
            }

            windowCounts[leftChar]--;
            if (needed.count(leftChar) && windowCounts[leftChar] < needed[leftChar]) {
                formed--;
            }
            left++;
        }

        right++;
    }

    return minLen == INT_MAX ? "" : s.substr(minStart, minLen);
}

#include <cassert>
#include <string>

int main() {
    using std::string;

    // Basic examples
    assert(minimumWindowSubstring("ADOBECODEBANC", "ABC") == "BANC");
    assert(minimumWindowSubstring("a", "a") == "a");

    // No valid window
    assert(minimumWindowSubstring("a", "aa") == "");
    assert(minimumWindowSubstring("abc", "d") == "");

    // Window is the whole string
    assert(minimumWindowSubstring("ab", "ab") == "ab");
    assert(minimumWindowSubstring("abc", "cba") == "abc"); // order doesn't matter

    // Duplicates in t
    assert(minimumWindowSubstring("aabbbcd", "aabb") == "aabb");
    assert(minimumWindowSubstring("bba", "ab") == "ba"); // any valid minimal window

    // t longer than s
    assert(minimumWindowSubstring("abc", "abcd") == "");

    // Multiple windows, pick any minimal length
    string result = minimumWindowSubstring("aaflslf", "afl");
    assert(result.length() == 3 && (result == "afl" || result == "fls" || result == "lsl"));

    // Single character match
    assert(minimumWindowSubstring("xyz", "y") == "y");

    // Case sensitivity
    assert(minimumWindowSubstring("aBcdef", "B") == "B");
    assert(minimumWindowSubstring("abcABC", "Ab") == "bA" || minimumWindowSubstring("abcABC", "Ab") == "abA");

    // Large t but present at the beginning
    assert(minimumWindowSubstring("abcdef", "abc") == "abc");

    return 0;
}

// The solution uses a sliding window with two pointers (`left` and `right`) to maintain a valid window containing all required characters. First, count each character's frequency in `t` in an unordered map (or array of size 128 for ASCII). Then expand the right pointer to include characters, decrementing their required count when a needed character is found. A counter `formed` tracks how many distinct required characters currently meet their frequency requirement. When `formed` equals the number of distinct characters in `t`, shrink the window from the left to find the minimal length. While shrinking, if removing a character breaks the requirement, expand again. Update the minimal window whenever a valid one is found. Edge cases: if `t` is empty or longer than `s`, return empty string; if `s` itself is the only valid window, handle it naturally. Time complexity: O(|s| + |t|), each character visited at most twice. Space complexity: O(1) since the alphabet is fixed (128).
