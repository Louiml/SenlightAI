/*
Write a C++ function that takes a single line of text as input (a string containing only printable ASCII characters, with no spaces) and returns the first non-repeating character in that string. If every character in the string appears more than once, the function should return a null character (`'\0'`). The function must be case-sensitive (e.g., `'A'` and `'a'` are different characters) and must consider only the first occurrence of each character when determining the "first" non-repeating character. The function should handle empty strings, single-character strings, and strings with all unique or all repeated characters. You may assume the input string contains only lowercase letters, uppercase letters, digits, and punctuation (no spaces). The function signature must be: `char firstUniqueChar(const std::string& s);`.
*/
#include <string>
#include <set>
#include <map>
#include <climits>

// Return the first non-repeating character in the input string.
// If all characters repeat or the string is empty, return '\0'.
char firstUniqueChar(const std::string& s) {
    std::set<char> repeated;
    std::map<char, int> firstOccurrence;

    // First pass: record first index for each character, track repeats.
    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (repeated.find(c) != repeated.end()) {
            continue; // already marked as repeated
        }
        if (firstOccurrence.find(c) == firstOccurrence.end()) {
            // First time seeing this character
            firstOccurrence[c] = static_cast<int>(i);
        } else {
            // Second time: mark as repeated and remove from unique map
            repeated.insert(c);
            firstOccurrence.erase(c);
        }
    }

    // Second pass: find the character with the smallest first index.
    char result = '\0';
    int minIndex = INT_MAX;
    for (std::map<char, int>::const_iterator it = firstOccurrence.begin();
         it != firstOccurrence.end(); ++it) {
        if (it->second < minIndex) {
            minIndex = it->second;
            result = it->first;
        }
    }
    return result;
}
#include <cassert>

int main() {
    // Basic cases
    assert(firstUniqueChar("leetcode") == 'l');        // 'l' is first non-repeating
    assert(firstUniqueChar("loveleetcode") == 'v');    // 'v' is first non-repeating
    assert(firstUniqueChar("aabb") == '\0');           // all repeated
    assert(firstUniqueChar("abc") == 'a');             // all unique
    assert(firstUniqueChar("") == '\0');               // empty string
    assert(firstUniqueChar("a") == 'a');               // single character

    // Case sensitivity
    assert(firstUniqueChar("aA") == 'a');              // 'a' and 'A' are distinct

    // Digits and punctuation
    assert(firstUniqueChar("12123") == '3');           // '3' appears once
    assert(firstUniqueChar("abca") == 'b');            // 'b' is first non-repeating
    assert(firstUniqueChar("zz") == '\0');             // repeated

    // Longer mixed string
    assert(firstUniqueChar("abcdabcde") == 'e');       // 'e' is the only unique

    return 0;
}
// The algorithm processes the input string in two passes. In the first pass, we iterate through the string character by character. For each character, we maintain two pieces of information: whether it has been seen more than once (tracked in a `std::set<char>`), and the index of its first occurrence (tracked in a `std::map<char, int>` for characters seen exactly once so far). When a character is encountered for the first time, we store its index in the map. If the same character appears again, we remove it from the map and add it to the set of repeated characters. After the first pass, the map contains only characters that appear exactly once, each associated with the position of its first occurrence. In the second pass, we iterate through the map to find the character with the smallest index. If the map is empty, we return `'\0'`.  
// Edge cases: empty string returns `'\0'`; single-character string returns that character if it appears once (which it always does by definition); strings with all repeated characters return `'\0'`; case sensitivity is preserved by using `char` as the map key.  
// Time complexity is \(O(n + k)\), where \(n\) is the length of the string and \(k\) is the number of distinct characters (at most 95 for printable ASCII), because each character is processed in constant time for set/map operations. Space complexity is \(O(k)\), which is bounded by a constant (max distinct printable characters), so effectively \(O(1)\) for fixed ASCII.
