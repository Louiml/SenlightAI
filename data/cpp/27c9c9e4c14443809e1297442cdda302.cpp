/*
Write a C++ function `bool areAnagrams(const std::string& s, const std::string& t)` that returns `true` if the two input strings are anagrams of each other (i.e., they contain exactly the same characters with the same frequencies, ignoring nothing), and `false` otherwise. The function must handle lowercase and uppercase letters as distinct characters (so `'a'` and `'A'` are different), and must also work with any printable ASCII characters, including spaces, digits, punctuation, and empty strings. You may not use sorting; instead, use frequency counting via `std::unordered_map`. The function should be `const`-correct (arguments passed by `const` reference) and include the necessary headers. Do not write a `main` function; that will be provided separately for testing.
*/
#include <string>
#include <unordered_map>

// Returns true if 's' and 't' are anagrams (same characters with same frequencies).
bool areAnagrams(const std::string& s, const std::string& t) {
    if (s.size() != t.size()) {
        return false;
    }

    std::unordered_map<char, int> freq_s;
    std::unordered_map<char, int> freq_t;

    for (std::size_t i = 0; i < s.size(); ++i) {
        ++freq_s[s[i]];
        ++freq_t[t[i]];
    }

    return freq_s == freq_t;
}
#include <cassert>
#include <string>

// Assume the solution function is already declared/defined above.
// Test driver.

int main() {
    // Basic valid anagrams
    assert(areAnagrams("listen", "silent") == true);
    assert(areAnagrams("anagram", "nagaram") == true);
    assert(areAnagrams("abc", "cba") == true);
    
    // Cases that are not anagrams
    assert(areAnagrams("rat", "car") == false);
    assert(areAnagrams("a", "b") == false);
    assert(areAnagrams("ab", "a") == false); // length mismatch
    
    // Empty strings
    assert(areAnagrams("", "") == true);
    assert(areAnagrams("", "a") == false);
    
    // Case-sensitive: uppercase compared differently
    assert(areAnagrams("A", "a") == false);
    
    // Duplicates and spaces/punctuation
    assert(areAnagrams("aabb", "bbaa") == true);
    assert(areAnagrams("aab", "abb") == false);
    assert(areAnagrams("hello world", "world hello") == true);
    assert(areAnagrams("hello world", "world hello!") == false); // punctuation difference
    
    // Strings with digits and symbols
    assert(areAnagrams("123", "321") == true);
    assert(areAnagrams("a1b2", "b2a1") == true);
    assert(areAnagrams("a1b2", "a1b3") == false);
    
    return 0;
}
// The solution uses two `std::unordered_map<char, int>` objects, one per string, to count the frequency of each character. First, we check if the lengths of the two strings differ; if they do, they cannot be anagrams, so we immediately return `false`. Then, in a single loop over the length of the strings, we increment the count for the current character of `s` in the first map and the current character of `t` in the second map. After the loop, we compare the two maps with the `==` operator, which checks that both maps have exactly the same keys and associated values. If equal, the strings are anagrams; otherwise, they are not. Key edge cases: empty strings (both empty → true; one empty and one non-empty → false due to length check), strings of equal length but with different character sets (e.g., `"ab"` vs `"ac"`), strings where the same characters appear in different orders (true), and strings with repeated characters (e.g., `"aab"` vs `"aba"`). The time complexity is \(O(n)\), where \(n\) is the length of the strings (assuming hash operations are average \(O(1)\)), and the space complexity is \(O(u)\) where \(u\) is the number of distinct characters in the strings, which is at most \(O(n)\) but typically much smaller.
