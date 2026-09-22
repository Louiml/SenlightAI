/*
Write a C++ function `int firstNonRepeatingCharIndex(const std::string& s)` that returns the index of the first non-repeating character in the given lowercase English string. If no such character exists, return −1. The function must be efficient for strings up to length 10^5, and must handle empty strings and strings where every character repeats. Assume the input contains only lowercase letters 'a' to 'z'. The function should be standalone (no `main`), use appropriate `const` correctness, and be placed in a header-ready form.
*/
#include <string>
#include <array>

// Returns the index of the first character that appears exactly once in s.
// If no such character exists, returns -1. Assumes s contains only lowercase letters.
int firstNonRepeatingCharIndex(const std::string& s) {
    std::array<int, 26> counts{};
    for (char ch : s) {
        ++counts[ch - 'a'];
    }
    for (int i = 0; i < static_cast<int>(s.size()); ++i) {
        if (counts[s[i] - 'a'] == 1) {
            return i;
        }
    }
    return -1;
}
#include <cassert>
#include <string>
#include <iostream>

// Declaration of the function under test (as if from header)
int firstNonRepeatingCharIndex(const std::string& s);

int main() {
    // Basic cases
    assert(firstNonRepeatingCharIndex("leetcode") == 0);          // 'l' is first unique
    assert(firstNonRepeatingCharIndex("loveleetcode") == 2);      // 'v' is first unique
    assert(firstNonRepeatingCharIndex("aabb") == -1);             // all repeat
    assert(firstNonRepeatingCharIndex("") == -1);                 // empty string
    assert(firstNonRepeatingCharIndex("abcabc") == -1);           // all repeat
    // Single character
    assert(firstNonRepeatingCharIndex("z") == 0);
    // Unique appears later
    assert(firstNonRepeatingCharIndex("aaab") == 3);
    // Multiple unique, pick first
    assert(firstNonRepeatingCharIndex("abac") == 0);              // 'a' repeats, 'b' is first unique
    assert(firstNonRepeatingCharIndex("ccddc") == 3);             // 'd' first unique
    // Longer repeated pattern with one unique at end
    assert(firstNonRepeatingCharIndex("xxyyzzq") == 6);
    // Large stress test (simulate 100k chars, all same except one unique at end)
    std::string large(100000, 'a');
    large[99999] = 'b';
    assert(firstNonRepeatingCharIndex(large) == 99999);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The problem requires finding the first character that appears exactly once in the string. A direct nested-loop comparison would be O(n²) and too slow for large inputs. Instead, we first scan the string once to count the frequency of each character using a fixed-size array of 26 integers (since the input is guaranteed lowercase letters). This avoids the overhead and potential collisions of a hash map, and uses constant auxiliary space. After counting, we scan the string a second time, and for each character at position `i`, check if its frequency is exactly 1; the first such `i` is returned. If the loop completes without finding any, return −1. Edge cases: an empty string returns −1 immediately, and a string with all repeats (e.g., "aabb") also returns −1. Time complexity is O(n) with two passes; space complexity is O(1) because the array size is fixed at 26 (or O(k) where k ≤ 26). The solution is robust and does not rely on any standard library containers beyond the basic string.
