/*
Write a C++ function `compareCaseInsensitive(const std::string& a, const std::string& b)` that takes two strings of equal length (you may assume they are always equal length, but handle it gracefully by comparing up to the shorter length if not) and returns an integer: `-1` if `a` is lexicographically less than `b` when compared case-insensitively (both uppercase and lowercase letters are treated the same, with lowercase used for comparison), `1` if `a` is greater than `b`, and `0` if they are equal ignoring case. The comparison must be done character-by-character from left to right, stopping at the first differing character. The input strings consist only of printable ASCII characters (letters, digits, punctuation, spaces). You must not use any standard library string comparison functions like `std::tolower` or `std::lexicographical_compare`; instead, manually convert uppercase letters to lowercase using arithmetic (e.g., `'A'` to `'a'`) and then compare.
*/
#include <string>

// Compare two strings case-insensitively.
// Return -1 if a < b, 1 if a > b, 0 if equal ignoring case.
// Uppercase letters are treated as lowercase.
int compareCaseInsensitive(const std::string& a, const std::string& b) {
    // Determine the number of characters to compare.
    const size_t commonLength = (a.size() < b.size()) ? a.size() : b.size();

    for (size_t i = 0; i < commonLength; ++i) {
        // Normalize characters to lowercase manually.
        char ca = a[i];
        char cb = b[i];
        if (ca >= 'A' && ca <= 'Z') ca = ca - 'A' + 'a';
        if (cb >= 'A' && cb <= 'Z') cb = cb - 'A' + 'a';

        if (ca < cb) return -1;
        if (ca > cb) return 1;
    }

    // All compared characters are equal; longer string is greater.
    if (a.size() < b.size()) return -1;
    if (a.size() > b.size()) return 1;
    return 0;
}
#include <cassert>
#include <string>

// Assume the solution function is declared above.
// The following main tests the compareCaseInsensitive function.

int main() {
    // Identical strings
    assert(compareCaseInsensitive("hello", "hello") == 0);
    // Different case only
    assert(compareCaseInsensitive("Hello", "hello") == 0);
    assert(compareCaseInsensitive("ABC", "abc") == 0);
    // First is lexicographically smaller
    assert(compareCaseInsensitive("apple", "Banana") == -1); // 'a' < 'b'
    assert(compareCaseInsensitive("Apple", "banana") == -1);
    // First is lexicographically larger
    assert(compareCaseInsensitive("Zebra", "apple") == 1); // 'z' > 'a'
    // Non-letter characters unaffected
    assert(compareCaseInsensitive("a1B", "A1b") == 0);
    assert(compareCaseInsensitive("a1B", "A1c") == -1);
    // Different lengths
    assert(compareCaseInsensitive("abc", "abcd") == -1);
    assert(compareCaseInsensitive("abcd", "abc") == 1);
    // Empty strings
    assert(compareCaseInsensitive("", "") == 0);
    assert(compareCaseInsensitive("", "a") == -1);
    // Punctuation and spaces
    assert(compareCaseInsensitive("Hello, World", "hello, world") == 0);
    assert(compareCaseInsensitive("Hello", "hello!") == -1);
    return 0;
}
// The solution iterates over both strings simultaneously up to the length of the shorter string (since the problem guarantees equal length, but to be safe and robust, we handle unequal lengths by only comparing the overlapping portion; if all overlapping characters are equal, the shorter string is considered smaller—this is consistent with typical lexicographic ordering). For each character position, we first normalize both characters to lowercase if they are uppercase letters (`'A'`–`'Z'`), by adding the offset `'a' - 'A'` (which equals 32) to convert to lowercase. Then we compare the normalized characters using standard `<` and `>` operators. If a difference is found, we immediately return `-1` or `1` accordingly. If the loop completes without finding a difference, we then compare the lengths of the original strings; if they are equal, return `0`, otherwise return `-1` if first string is shorter, `1` if longer. This approach runs in `O(n)` time where `n` is the length of the shorter string, and uses `O(1)` extra space since we only modify local copies of characters (or we can avoid modifying the strings by comparing on the fly). Edge cases include strings that differ only in case (return `0`), strings that are identical (return `0`), strings that are equal up to the shorter length but differ in length, and strings containing non-letter characters where case conversion is a no-op.
