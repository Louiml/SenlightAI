// Write a C++ function `int firstUniqueCharacterIndex(const std::string& s)` that returns the 0-based index of the first non-repeating character in the input string. If no such character exists, return `-1`. The string contains only lowercase English letters (`'a'` to `'z'`). Your function must be `const`-correct (take the string by `const` reference) and must scan the string at most twice: once to count frequencies and once to find the first character with frequency 1. Ensure your code is self-contained and includes all necessary standard headers.

The core idea is to use a hash map (or array, since only 26 letters are possible) to count the frequency of each character in the input string. First, we iterate through the string and increment the count for each character. Second, we iterate through the string again in the original order and return the index of the first character whose frequency is exactly 1; if no such character is found, we return -1.  
**Edge cases:**  
- Empty string → return -1 (no characters).  
- Single character string → return 0 (that character is unique).  
- All characters repeated → return -1.  
- Multiple unique characters → return the smallest index among them.  
**Complexity:**  
- Time: \(O(n)\) because we make two passes over the string (each pass is linear).  
- Space: \(O(1)\) auxiliary if using a fixed-size array of 26 integers, or \(O(k)\) where \(k\) is the alphabet size if using a hash map; for lowercase letters, we can use a fixed array. Since the alphabet size is constant, space is effectively \(O(1)\).

#include <string>
#include <vector>

// Returns the index of the first non-repeating character in s, or -1 if none exists.
int firstUniqueCharacterIndex(const std::string& s) {
    // Frequency table for lowercase English letters.
    std::vector<int> charCount(26, 0);
    
    // First pass: count occurrences of each character.
    for (char c : s) {
        charCount[c - 'a']++;
    }
    
    // Second pass: find the first character with count == 1.
    for (int i = 0; i < static_cast<int>(s.size()); ++i) {
        if (charCount[s[i] - 'a'] == 1) {
            return i;
        }
    }
    
    return -1;
}

#include <cassert>
#include <string>

// Function declaration from the solution (already defined above).
int firstUniqueCharacterIndex(const std::string& s);

int main() {
    // Basic cases
    assert(firstUniqueCharacterIndex("leetcode") == 0);       // 'l' is first unique
    assert(firstUniqueCharacterIndex("loveleetcode") == 2);   // 'v' is first unique
    assert(firstUniqueCharacterIndex("aabb") == -1);          // all repeated
    assert(firstUniqueCharacterIndex("a") == 0);              // single character
    assert(firstUniqueCharacterIndex("") == -1);              // empty string
    // Cases with repeated first character but unique later
    assert(firstUniqueCharacterIndex("aabbccd") == 6);        // 'd' is unique
    assert(firstUniqueCharacterIndex("ababab") == -1);        // all repeated
    // Multiple unique characters: pick smallest index
    assert(firstUniqueCharacterIndex("abcabcxyz") == 6);      // 'x' at index 6 (first unique)
    // Edge: unique character appears at the end
    assert(firstUniqueCharacterIndex("zzza") == 3);
    return 0;
}
