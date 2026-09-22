// Write a C++ function named `areIsomorphic` that takes two strings `s` and `t` and returns a boolean value indicating whether the two strings are isomorphic. Two strings are isomorphic if the characters in `s` can be replaced to get `t`, with the condition that each character in `s` maps to exactly one unique character in `t`, and each character in `t` maps back to exactly one unique character in `s`. The mapping must be consistent in both directions — no two different characters in `s` may map to the same character in `t`, and no two different characters in `t` may map to the same character in `s`. If the strings have different lengths, they cannot be isomorphic. The function should handle empty strings correctly and be case-sensitive (e.g., `'A'` and `'a'` are considered different characters). The solution must not use any external libraries beyond the standard C++ library.

#include <cassert>
#include <string>

// Function declaration (normally in a header, but here for completeness)
bool areIsomorphic(const std::string& s, const std::string& t);

int main() {
    // Basic test cases
    assert(areIsomorphic("egg", "add") == true);
    assert(areIsomorphic("foo", "bar") == false);
    assert(areIsomorphic("paper", "title") == true);
    assert(areIsomorphic("badc", "baba") == false);

    // Edge cases
    assert(areIsomorphic("", "") == true);
    assert(areIsomorphic("a", "a") == true);
    assert(areIsomorphic("ab", "aa") == false);  // 'a' maps to 'a', 'b' maps to 'a' -> conflict
    assert(areIsomorphic("aa", "ab") == false);  // 'a' maps to 'a' and then 'a' maps to 'b' -> conflict
    assert(areIsomorphic("ab", "cd") == true);   // distinct mappings

    // Case sensitivity
    assert(areIsomorphic("Ab", "ab") == false); // 'A' vs 'a' are different characters

    // Longer strings with duplicate patterns
    assert(areIsomorphic("abcdefghij", "abcdefghij") == true);
    assert(areIsomorphic("abccba", "bcaacb") == true);

    // Different lengths should fail immediately
    assert(areIsomorphic("abc", "ab") == false);
    assert(areIsomorphic("", "a") == false);

    return 0;
}

#include <string>
#include <unordered_map>

bool areIsomorphic(const std::string& s, const std::string& t) {
    if (s.length() != t.length()) {
        return false;
    }

    std::unordered_map<char, char> sToT;
    std::unordered_map<char, char> tToS;

    for (size_t i = 0; i < s.length(); ++i) {
        char charS = s[i];
        char charT = t[i];

        // Check if s[i] already has a conflicting mapping to t[i]
        auto itS = sToT.find(charS);
        if (itS != sToT.end() && itS->second != charT) {
            return false;
        }

        // Check if t[i] already has a conflicting reverse mapping to s[i]
        auto itT = tToS.find(charT);
        if (itT != tToS.end() && itT->second != charS) {
            return false;
        }

        // Establish the mapping in both directions
        sToT[charS] = charT;
        tToS[charT] = charS;
    }

    return true;
}

// The core idea is to enforce a bijective (one-to-one) mapping between characters of the two strings. The algorithm iterates through both strings simultaneously, character by character, using two hash maps (`unordered_map<char, char>`) to track the mappings: one from `s` to `t` and one from `t` to `s`. For each position `i`, the algorithm checks:
// 1. If `s[i]` has already been mapped to a different character in `t`, return `false`.
// 2. If `t[i]` has already been mapped back to a different character in `s`, return `false`.
// 3. Otherwise, record the mappings in both directions.
//
// The two-map approach elegantly handles all edge cases: duplicate characters, conflicting mappings, and different lengths (which can be checked upfront). Since each character in `s` is compared to its mapping in `t` and vice versa, no two different characters can map to the same target without triggering a conflict. The time complexity is **O(n)** where `n` is the length of the strings, because each character is processed once and hash map operations are amortized O(1). The space complexity is **O(k)** where `k` is the number of distinct characters in the input strings, bounded by the alphabet size (e.g., 256 for ASCII, or 128 for lowercase letters, but since any character can appear, the worst case is O(min(n, alphabet size))).
