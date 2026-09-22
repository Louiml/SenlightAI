// Write a C++ function named `areIsomorphic` that takes two non-empty strings `s` and `t` of equal length and returns a `bool` indicating whether the strings are isomorphic. Two strings are isomorphic if the characters in `s` can be replaced to get `t` by a consistent one-to-one mapping, meaning each character in `s` maps to exactly one character in `t`, and each character in `t` maps to exactly one character in `s`. For example, `"egg"` and `"add"` are isomorphic (e→a, g→d), but `"foo"` and `"bar"` are not (f→b, o→a and o→r conflict). The function must be `const`-correct, handle all printable ASCII characters (including spaces and punctuation), and assume the inputs are non-empty and of equal length.
The solution uses two fixed-size arrays (of size 128, covering all ASCII characters) to track the last seen position index for each character in both strings. The key idea is to map both `s[i]` and `t[i]` to the same numeric "tag" (an increasing index) only when they are paired consistently. During iteration, before updating the tags, we check whether the current tags for `s[i]` and `t[i]` are equal. If they are not equal, that means either `s[i]` was previously mapped to a different character of `t`, or `t[i]` was previously mapped to a different character of `s`, indicating a conflict. If the tags are equal, we then assign both positions the same new tag (`i+1`) to record the pairing. Using `i+1` (instead of `0`) avoids the ambiguity with the initial zero values, which represent "not yet seen." This approach works because if a pair of characters has appeared together before, both will have the same stored tag; if either character appeared with a different partner, the tags will differ. Edge cases include single-character strings (always isomorphic), identical strings (always isomorphic), and strings with repeated characters on only one side (e.g., `"ab"` vs `"aa"` will fail because the second `a` in `t` already has a tag from the first position, while `b` in `s` has tag 0). The time complexity is O(n) where n is the length of the strings (one pass), and space complexity is O(1) since the arrays have fixed size 128.
#include <string>
#include <array>

// Determine if two non-empty strings of equal length are isomorphic.
// A bijective mapping must exist between characters of s and characters of t.
bool areIsomorphic(const std::string& s, const std::string& t) {
    // Arrays to store the "tag" (position index + 1) for each character.
    // Initial value 0 means "not seen yet".
    std::array<int, 128> tagS{};
    std::array<int, 128> tagT{};

    for (std::size_t i = 0; i < s.size(); ++i) {
        // If current characters have mismatched tags, mapping already conflicted.
        if (tagS[static_cast<unsigned char>(s[i])] != tagT[static_cast<unsigned char>(t[i])]) {
            return false;
        }
        // Assign a new unique tag (i+1) to this pair.
        tagS[static_cast<unsigned char>(s[i])] = static_cast<int>(i) + 1;
        tagT[static_cast<unsigned char>(t[i])] = static_cast<int>(i) + 1;
    }
    return true;
}
#include <cassert>
#include <string>

// The solution function is declared above (areIsomorphic).
int main() {
    // Basic true cases
    assert(areIsomorphic("egg", "add") == true);
    assert(areIsomorphic("paper", "title") == true);
    assert(areIsomorphic("ab", "ab") == true);
    assert(areIsomorphic("a", "b") == true);

    // Basic false cases
    assert(areIsomorphic("foo", "bar") == false);
    assert(areIsomorphic("ab", "aa") == false);
    assert(areIsomorphic("aa", "ab") == false);

    // Edge cases: identical strings, different lengths (not expected but still works due to equal-length assumption)
    assert(areIsomorphic("xyz", "xyz") == true);
    assert(areIsomorphic("abc", "def") == true);

    // Space and punctuation characters
    assert(areIsomorphic("a b", "c d") == true);
    assert(areIsomorphic("a b", "c e") == false);

    // Longer repeated patterns
    assert(areIsomorphic("abca", "xeyx") == true);
    assert(areIsomorphic("abca", "xeyz") == false);
    
    return 0;
}
