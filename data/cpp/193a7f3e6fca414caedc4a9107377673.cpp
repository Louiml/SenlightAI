Write a C++ function `bool buildMapping(const std::string& source, const std::string& target, std::vector<std::pair<char,char>>& mapping)` that determines whether `source` can be transformed into `target` by replacing each distinct character in `source` with exactly one distinct character in `target` (a one-to-one mapping). The function must return `true` if such a mapping exists, and in that case fill `mapping` with all pairs `(source_char, target_char)` for characters that actually differ between the two strings, sorted by `source_char` in alphabetical order. If the mapping is impossible (i.e., the same source character maps to two different target characters, or the strings have unequal lengths), return `false` and leave `mapping` empty. The input strings contain only lowercase English letters and may be empty. Example: for `source="aba"` and `target="cdc"`, the function returns `true` and `mapping` contains `{('a','c'),('b','d')}`; for `source="ab"` and `target="aa"` it returns `false` because `a` must map to both `a` and `b`.

// The problem reduces to checking consistency of a mapping from characters in `source` to characters in `target`. Since the transformation must be one-to-one, each character in `source` may map to at most one character in `target`. The main algorithm: first, if lengths differ, return `false` immediately. Then iterate through each position `i`, and for each pair `(source[i], target[i])`, if `source[i]` has not been seen before, record it in an array `mapFrom` (size 26, initialized to 0) with value `target[i]-'a'+1` (using +1 to distinguish from "unused"). If it has been seen and the stored mapping differs from the current target character, return `false`. After the loop, if no conflict, collect all entries where `mapFrom` is nonzero, convert back to characters, and sort by source character (which is already naturally ordered if we iterate through the array from 0 to 25). Edge cases: empty strings are handled correctly (empty mapping, returns `true`); self-mapping (source char equals target char) is allowed but should not be included in `mapping` because only differing pairs are needed; duplicates and all-identical strings produce empty mapping. Time complexity is O(n + 26) where n is the string length, and space complexity is O(1) auxiliary (fixed-size array).

#include <string>
#include <vector>
#include <utility>
#include <algorithm>

// Determines if source can be transformed to target by a one-to-one character mapping.
// If possible, fills 'mapping' with all differing character pairs sorted by source char.
bool buildMapping(const std::string& source, const std::string& target,
                  std::vector<std::pair<char,char>>& mapping) {
    mapping.clear();
    if (source.size() != target.size()) {
        return false;
    }

    const int alphabetSize = 26;
    int mapFrom[alphabetSize] = {0}; // 0 means unused, else stores target_char - 'a' + 1

    for (std::size_t i = 0; i < source.size(); ++i) {
        int srcIdx = source[i] - 'a';
        int tgtVal = target[i] - 'a' + 1;
        if (mapFrom[srcIdx] == 0) {
            mapFrom[srcIdx] = tgtVal;
        } else if (mapFrom[srcIdx] != tgtVal) {
            return false;
        }
    }

    for (int i = 0; i < alphabetSize; ++i) {
        if (mapFrom[i] != 0) {
            char srcChar = static_cast<char>('a' + i);
            char tgtChar = static_cast<char>('a' + mapFrom[i] - 1);
            if (srcChar != tgtChar) {
                mapping.emplace_back(srcChar, tgtChar);
            }
        }
    }
    // Pairs are already sorted by source character due to array iteration order.
    return true;
}

#include <cassert>
#include <vector>
#include <utility>
#include <string>

// The solution function is assumed to be defined above this test.
// For completeness, include the declaration here.
bool buildMapping(const std::string& source, const std::string& target,
                  std::vector<std::pair<char,char>>& mapping);

int main() {
    std::vector<std::pair<char,char>> mapping;

    // Basic case with differing pairs
    assert(buildMapping("aba", "cdc", mapping));
    assert((mapping == std::vector<std::pair<char,char>>{{'a','c'}, {'b','d'}}));

    // Self-mapping characters are not included
    assert(buildMapping("abc", "abc", mapping));
    assert(mapping.empty());

    // Conflict: same source char maps to two different target chars
    assert(!buildMapping("ab", "aa", mapping));
    assert(mapping.empty());

    // Unequal lengths
    assert(!buildMapping("abc", "ab", mapping));
    assert(mapping.empty());

    // Empty strings
    assert(buildMapping("", "", mapping));
    assert(mapping.empty());

    // Different lengths but first chars match conflict check must come after length check
    assert(!buildMapping("aa", "a", mapping));

    // One-to-one mapping with all differing chars
    assert(buildMapping("abc", "xyz", mapping));
    assert((mapping == std::vector<std::pair<char,char>>{{'a','x'}, {'b','y'}, {'c','z'}}));

    // Mapping to same target char from different sources is allowed
    // (target chars can repeat, only source chars must be consistent)
    assert(buildMapping("ab", "cc", mapping));
    assert((mapping == std::vector<std::pair<char,char>>{{'a','c'}, {'b','c'}}));

    // Long string with mixed conflicts and valid parts
    assert(!buildMapping("hello", "world", mapping));
    assert(mapping.empty());

    // All source chars map to themselves except one
    assert(buildMapping("abx", "aby", mapping));
    assert((mapping == std::vector<std::pair<char,char>>{{'x','y'}}));

    return 0;
}
