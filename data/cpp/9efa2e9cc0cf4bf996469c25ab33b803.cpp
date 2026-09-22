// Write a C++ function `bool isomorphicStrings(const std::string& first, const std::string& second)` that determines whether two strings are isomorphic. Two strings are isomorphic if the characters in the first string can be replaced to get the second string, with the constraint that each character in the first string must map to exactly one character in the second string, and each character in the second string must map to exactly one character in the first string. This means the mapping must be one-to-one and onto between the set of characters in the first string and the set of characters in the second string. For example, `"egg"` and `"add"` are isomorphic (e→a, g→d), but `"foo"` and `"bar"` are not (because both `f` and `o` would need to map to `b`, and `o` would need to map to both `a` and `r`). Also, `"ab"` and `"aa"` are not isomorphic because `a` and `b` map to the same character. The function should return `true` if the strings are isomorphic and `false` otherwise. Assume both strings have equal non‑zero length, but handle any length gracefully (if lengths differ, return `false` immediately).

#include <cassert>

int main() {
    // Basic isomorphic pairs.
    assert(isomorphicStrings("egg", "add") == true);
    assert(isomorphicStrings("paper", "title") == true);
    assert(isomorphicStrings("ab", "xy") == true);
    assert(isomorphicStrings("", "") == true);
    assert(isomorphicStrings("a", "a") == true);

    // Non-isomorphic pairs.
    assert(isomorphicStrings("foo", "bar") == false);  // f->b, o->a and r conflict.
    assert(isomorphicStrings("ab", "aa") == false);    // a->a and b->a reverse conflict.
    assert(isomorphicStrings("abc", "defg") == false); // different lengths.
    assert(isomorphicStrings("aba", "xyz") == false);  // 'a' maps to 'x' and 'z'.
    assert(isomorphicStrings("aa", "ab") == false);    // 'a' maps to 'a' and 'b' reverse conflict.
}

#include <string>
#include <unordered_map>

// Check if two strings are isomorphic by verifying consistent one-to-one mappings.
bool isomorphicStrings(const std::string& first, const std::string& second) {
    if (first.length() != second.length()) {
        return false;
    }

    // Map characters from first to second and from second to first.
    std::unordered_map<char, char> firstToSecond;
    std::unordered_map<char, char> secondToFirst;

    for (std::size_t i = 0; i < first.length(); ++i) {
        const char c1 = first[i];
        const char c2 = second[i];

        // Check forward mapping.
        auto it1 = firstToSecond.find(c1);
        if (it1 == firstToSecond.end()) {
            firstToSecond[c1] = c2;
        } else {
            if (it1->second != c2) {
                return false;
            }
        }

        // Check reverse mapping.
        auto it2 = secondToFirst.find(c2);
        if (it2 == secondToFirst.end()) {
            secondToFirst[c2] = c1;
        } else {
            if (it2->second != c1) {
                return false;
            }
        }
    }

    return true;
}

// The core idea is to maintain two mappings simultaneously: one from characters of the first string to characters of the second string, and one from characters of the second string back to characters of the first string. We iterate through both strings character by character. For the forward mapping, if a character from the first string has not been seen yet, we record its mapping to the corresponding character in the second string. If it has been seen, we verify that the stored mapping equals the current character in the second string; if not, the strings are not isomorphic. Similarly, we do the reverse mapping to ensure that each character from the second string maps back to exactly one character from the first string. This double-check prevents cases like `"ab"` and `"aa"`, where forward mapping alone would succeed (a→a, b→a) but reverse mapping fails (a would need to map to both `a` and `b`). Edge cases include unequal lengths (return `false` immediately), empty strings (return `true` since there are no mappings to violate), and single-character strings (always isomorphic). The algorithm runs in O(n) time, where n is the length of the strings, and uses O(1) auxiliary space if we use two fixed-size arrays of size 256 (for ASCII) or use `unordered_map` which is O(k) where k is the number of distinct characters, but still O(1) in practical sense for a fixed character set. The time complexity remains linear because each character is processed at most twice (once in forward, once in reverse).
