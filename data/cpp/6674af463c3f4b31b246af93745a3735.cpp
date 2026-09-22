Write a standalone C++ function named `areIsomorphicStrings` that takes two `std::string` parameters and returns a `bool`. Two strings are considered isomorphic if the characters in the first string can be replaced to get the second string, preserving the order of characters: each character in the first string maps to exactly one character in the second string, and no two distinct characters in the first string map to the same character in the second string (i.e., the mapping is bijective). Empty strings are isomorphic, and strings of different lengths are never isomorphic. The function must be `const`‑correct with respect to its inputs. Provide a reference solution and a test harness using `assert`.

The core idea is to use two hash maps (or unordered maps) to track the most recent mapping positions for each character in both strings. During a single pass over the strings, we compare the stored "last seen index" (or any consistent marker) for the current character in `s` and the current character in `t`. If they differ, it means either a character in `s` has been mapped to two different characters in `t`, or two different characters in `s` map to the same character in `t` — violating bijectivity. If they match, we update both maps with the current position (using `i+1` to avoid the default value of 0 conflicting with the first index). If the loop completes without a mismatch, the strings are isomorphic. Edge cases: different lengths immediately return `false`; empty strings return `true` because the loop does nothing; strings with length 1 always return `true`. Time complexity is O(n) where n is the length of the strings, and space complexity is O(1) in the sense that the maps are bounded by the alphabet size (constant for typical ASCII), though we can state O(k) where k is the number of distinct characters.

#include <string>
#include <unordered_map>

// Returns true if 's' and 't' are isomorphic (bijective character mapping).
bool areIsomorphicStrings(const std::string& s, const std::string& t) {
    // Different lengths can never be isomorphic.
    if (s.size() != t.size()) {
        return false;
    }

    // Maps: last seen index (i+1) for each character in s and t.
    std::unordered_map<char, int> map_s;
    std::unordered_map<char, int> map_t;

    for (std::size_t i = 0; i < s.size(); ++i) {
        // If the stored indices differ, mapping is inconsistent.
        if (map_s[s[i]] != map_t[t[i]]) {
            return false;
        }
        // Record the current position for both characters.
        map_s[s[i]] = static_cast<int>(i) + 1;
        map_t[t[i]] = static_cast<int>(i) + 1;
    }
    return true;
}

#include <cassert>

int main() {
    // Basic examples
    assert(areIsomorphicStrings("egg", "add") == true);
    assert(areIsomorphicStrings("foo", "bar") == false);
    assert(areIsomorphicStrings("paper", "title") == true);

    // Different lengths
    assert(areIsomorphicStrings("ab", "a") == false);
    assert(areIsomorphicStrings("a", "ab") == false);

    // Empty strings
    assert(areIsomorphicStrings("", "") == true);

    // Single character
    assert(areIsomorphicStrings("x", "y") == true);

    // Same string
    assert(areIsomorphicStrings("abc", "abc") == true);

    // Bijection failure: one-to-many
    assert(areIsomorphicStrings("ab", "aa") == false);

    // Bijection failure: many-to-one
    assert(areIsomorphicStrings("aa", "ab") == false);

    // Repeated patterns with same mapping
    assert(areIsomorphicStrings("abca", "xyzw") == false); // because 'a' maps to 'x' and then 'a' maps to 'w'? Actually 'a' maps to 'x' and 'w'? Wait: s[0]='a'→t[0]='x', s[3]='a'→t[3]='w' → not isomorphic
    assert(areIsomorphicStrings("abca", "xyzw") == false);

    // Correct mapping with repeated chars
    assert(areIsomorphicStrings("abac", "xyxz") == true);

    // All same characters
    assert(areIsomorphicStrings("aaa", "bbb") == true);
    assert(areIsomorphicStrings("aaa", "abc") == false);

    return 0;
}
