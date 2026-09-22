// Write a C++ function `char findAddedCharacter(const std::string& original, const std::string& modified)` that, given two strings where `modified` is formed by taking `original` and appending exactly one extra lowercase English letter (the extra letter may appear anywhere, not necessarily at the end, and the original string's characters remain in their original relative order), returns that extra character. The function must handle cases where the extra letter is the same as an existing character, and the input may be empty for `original` (in which case `modified` is a single character). You may not use extra data structures like hashmaps or multisets for the core logic; the solution must be based on a straight‑forward and deterministic comparison approach. The function must be `const`‑correct and include appropriate headers.
// The simplest robust approach is to sort both strings and then compare them character by character. Since `modified` is exactly `original` plus one extra character, after sorting, the two strings will be identical except for the extra character that appears at exactly one index when scanning from the beginning. We iterate through the sorted `modified` string (which is one character longer) and compare each character with the corresponding position in the sorted `original`. The first index where they differ gives us the extra character. If no difference is found (which cannot happen given valid input, but for safety we return `'\0'`), it would be the last character of `modified`. Edge cases: when `original` is empty, the loop runs only once and `s[i]` is compared with `s[i]` — but we need to handle that carefully because `original[i]` is out of bounds; instead, we can loop over indices from 0 to `modified.size() - 1` and check if `i >= original.size()` then return `modified[i]`, or if `original[i] != modified[i]` then return `modified[i]`. Time complexity is O(n log n) due to sorting, where n is the length of `modified`. Space complexity is O(1) extra (ignoring the sort’s internal stack space) because we sort in place. Alternative approaches exist (XOR of all characters), but sorting is straightforward and matches the given snippet’s spirit.
#include <string>
#include <algorithm>

// Return the extra character that was appended to 'original' to form 'modified'.
// Precondition: modified is exactly original plus one extra lowercase letter,
// and the original's characters retain their relative order.
char findAddedCharacter(const std::string& original, const std::string& modified) {
    std::string sorted_original = original;
    std::string sorted_modified = modified;
    std::sort(sorted_original.begin(), sorted_original.end());
    std::sort(sorted_modified.begin(), sorted_modified.end());

    for (std::size_t i = 0; i < sorted_modified.size(); ++i) {
        if (i >= sorted_original.size() || sorted_original[i] != sorted_modified[i]) {
            return sorted_modified[i];
        }
    }
    return '\0'; // Should never reach here with valid input.
}
#include <cassert>
#include <string>

int main() {
    // Basic case: extra letter at the end
    assert(findAddedCharacter("abcd", "abcde") == 'e');
    // Extra letter in the middle
    assert(findAddedCharacter("abc", "abxc") == 'x');
    // Empty original, single character modified
    assert(findAddedCharacter("", "z") == 'z');
    // Duplicate letters: extra letter duplicates an existing one
    assert(findAddedCharacter("aabb", "aabbb") == 'b');
    // Extra letter is the same as an existing character, placed at the front
    assert(findAddedCharacter("hello", "ehello") == 'e');
    // Longer string with multiple duplicates
    assert(findAddedCharacter("mississippi", "mississipqpi") == 'q');
    // Extra letter at the very end, with all letters the same
    assert(findAddedCharacter("zzz", "zzzz") == 'z');
    // Extra letter at the very beginning
    assert(findAddedCharacter("xyz", "axyz") == 'a');
    // One‑character original
    assert(findAddedCharacter("a", "ab") == 'b');
    // Two‑character original, extra letter identical to the original's only distinct character
    assert(findAddedCharacter("ab", "abb") == 'b');
    return 0;
}
