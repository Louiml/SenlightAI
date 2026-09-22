Write a C++ function `bool hasAllUniqueChars(const std::string& input)` that returns `true` if the input string contains only lowercase English letters (a-z) and every character appears at most once (i.e., no duplicate characters), and `false` otherwise. The function should ignore the actual order of characters and only care about uniqueness. The input string is guaranteed to contain only lowercase letters, but it may be empty (in which case the function should return `true`). You must solve this using a bitmask (a single integer where each bit represents the presence of a letter from 'a' to 'z') to track seen characters, exactly as the given code snippet does, but refactored into a clean, reusable function.
The core idea is to use a 26-bit integer bitmask (e.g., a `long int` or `unsigned int`) where bit `i` (0-indexed) corresponds to whether the character `'a' + i` has been seen. For each character in the string, compute the bit value by shifting `1` left by `(c - 'a')`. If that bit is already set in the mask, a duplicate exists, so return `false`. Otherwise, set the bit and continue. If the loop completes without finding a duplicate, return `true`. An empty string trivially has no duplicates, so return `true`. Edge cases: all 26 letters present (mask becomes `(1<<26)-1`, no overflow if using an integer large enough), repeated letters like "aa", and strings like "abc" which are valid. Time complexity is `O(n)` where `n` is the string length (iterating over each character once), and space complexity is `O(1)` since we only store one integer.
#include <string>

// Returns true if every character in the input is unique.
// Input must contain only lowercase letters; empty string returns true.
bool hasAllUniqueChars(const std::string& input) {
    unsigned int seen = 0; // 26 bits used, one per letter

    for (char c : input) {
        unsigned int bit = 1u << (c - 'a');
        if ((seen & bit) != 0) {
            return false; // duplicate found
        }
        seen |= bit;
    }
    return true;
}
#include <cassert>

int main() {
    // Empty string
    assert(hasAllUniqueChars("") == true);
    
    // Single character
    assert(hasAllUniqueChars("a") == true);
    
    // All unique lowercase letters
    assert(hasAllUniqueChars("abcdefghijklmnopqrstuvwxyz") == true);
    
    // Mixed unique letters (not in order)
    assert(hasAllUniqueChars("zyxwvutsrqponmlkjihgfedcba") == true);
    
    // Simple duplicate
    assert(hasAllUniqueChars("aa") == false);
    
    // Duplicate not at the beginning
    assert(hasAllUniqueChars("abcda") == false);
    
    // Duplicate after many unique letters
    assert(hasAllUniqueChars("abcdefghijklmnopqrstuvwxyza") == false);
    
    // Three characters with one duplicate
    assert(hasAllUniqueChars("aba") == false);
    
    // Long string with a duplicate near the end
    assert(hasAllUniqueChars("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz") == false);
    
    // Unique string with repeated pattern but no duplicates (e.g., "ab" repeated would be false, so test a valid one)
    assert(hasAllUniqueChars("ab") == true);
    
    return 0;
}
