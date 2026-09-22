// Write a C++ function named `firstNonRepeatingCharacter` that takes a string `S` containing only lowercase English letters (no spaces or other characters) and returns the first character in the string that appears exactly once (i.e., the first non-repeating character). If every character in the string repeats at least once, the function should return the special character `'$'` (dollar sign) to indicate that no such character exists. The function must preserve the original order of characters when determining which non-repeating character comes first, meaning you must scan the string from left to right and return the earliest occurring character whose total frequency in the entire string is exactly one. The input string is guaranteed to be non-empty, but its length can be up to 10^5, so an efficient solution is required.

// The solution uses a hash map (unordered_map) to count the frequency of every character in the string in a first pass. Then, in a second pass through the original string, we check each character in order and return the first one whose frequency count is exactly 1, since that character is the first non-repeating character by definition. If no character has a frequency of 1 after scanning the entire string, we return `'$'`. This approach handles all edge cases: a string with only one character (which is always non-repeating), a string where all characters repeat (returns `'$'`), and a string with multiple non-repeating characters (returns the earliest one). The time complexity is O(n) because we traverse the string twice and each map operation is O(1) on average. The space complexity is O(k) where k is the number of distinct characters, which is at most 26 for lowercase letters, so effectively O(1). Since the problem guarantees lowercase letters only, the map size is bounded, but the approach works for any character set.

#include <string>
#include <unordered_map>

// Returns the first character in s that appears exactly once.
// Returns '$' if no such character exists.
char firstNonRepeatingCharacter(const std::string& s) {
    std::unordered_map<char, int> frequency;
    
    // First pass: count occurrences of each character
    for (char c : s) {
        ++frequency[c];
    }
    
    // Second pass: find the first character with count 1
    for (char c : s) {
        if (frequency[c] == 1) {
            return c;
        }
    }
    
    return '$';
}

#include <cassert>
#include <string>

// Declaration of the function under test (assumed to be included from the solution)
char firstNonRepeatingCharacter(const std::string& s);

int main() {
    // Single character
    assert(firstNonRepeatingCharacter("a") == 'a');
    
    // All characters repeat
    assert(firstNonRepeatingCharacter("aabbcc") == '$');
    
    // First non-repeating is at the end
    assert(firstNonRepeatingCharacter("aabbc") == 'c');
    
    // First non-repeating is in the middle
    assert(firstNonRepeatingCharacter("abacabad") == 'c');
    
    // Multiple non-repeating, earliest one is selected
    assert(firstNonRepeatingCharacter("loveleetcode") == 'v');
    
    // String with all distinct characters, first one returned
    assert(firstNonRepeatingCharacter("abc") == 'a');
    
    // Repeats at the start, then a unique one appears later
    assert(firstNonRepeatingCharacter("aabcde") == 'b');
    
    // Only one character repeats, others are unique
    assert(firstNonRepeatingCharacter("abca") == 'b');
    
    // Longer string with a unique at the end
    assert(firstNonRepeatingCharacter("xxttyyuuiiooppllkkjjhhggffdds") == 's');
    
    return 0;
}
