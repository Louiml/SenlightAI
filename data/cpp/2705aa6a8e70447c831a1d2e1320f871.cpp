// Given two C-style strings, `src` and `segment`, write a C++ function named `span_initial_segment` that computes the length of the initial portion of `src` which consists entirely of characters that appear in `segment`. The function should treat characters as unsigned bytes, so all 256 possible values (including null, though null terminates the string) are valid members of the segment. It must return the number of characters from the beginning of `src` that belong to the segment set. For example, if `src` is `"hello world"` and `segment` is `"hel "`, the function should return 5 because `"hello"` is the longest prefix using only `'h'`, `'e'`, `'l'`, `'o'`, and space. If `src` starts with a character not in `segment`, return 0. Note that the implementation must not modify the input strings and must only use facilities from the C++ standard library (no external dependencies). The strings are null-terminated, and the input may be empty. The function signature must be `size_t span_initial_segment(const char* src, const char* segment)`.
// The core idea is to use a fixed-size bitset (or a boolean array) of 256 entries to represent the set of characters present in `segment`. Since we only need to answer membership queries for characters in `src`, we can build the set in one pass over `segment`. Then we traverse `src` from the beginning, checking each character against the set, and stop at the first character not in the set or when we reach the null terminator. The result is the distance (number of characters) from the start of `src` to that stopping point, which is the pointer difference. Edge cases include: empty `src` (returns 0), empty `segment` (returns 0 because no characters match), `segment` containing the null character (but the loop stops at `*segment` so null is never added; however, `src` is also terminated by null, so we never test null as a character from `src`), and `src` that is entirely contained in the segment (returns full length). The solution uses `unsigned char` for indexing to avoid sign-extension issues with negative `char` values. Time complexity is O(len(segment) + len(src)) in the worst case, and space complexity is O(1) fixed (the bitset is constant size).
#include <cstddef>   // for size_t
#include <array>     // for std::array (optional, can use plain bool array)

// Compute the length of the initial prefix of src that consists only of
// characters present in segment. Characters are treated as unsigned bytes.
size_t span_initial_segment(const char* src, const char* segment) {
    // A boolean array of size 256 to mark which characters are in segment.
    bool in_segment[256] = {false};

    // Mark all characters from segment (excluding the null terminator).
    for (const unsigned char* p = reinterpret_cast<const unsigned char*>(segment);
         *p != 0; ++p) {
        in_segment[*p] = true;
    }

    // Walk through src from the beginning while characters are in segment.
    const char* initial = src;
    for (const unsigned char* p = reinterpret_cast<const unsigned char*>(src);
         *p != 0 && in_segment[*p]; ++p) {
        // Continue while conditions hold.
    }

    // Return the number of matched characters.
    return reinterpret_cast<const char*>(p) - initial;
}
#include <cassert>
#include <cstddef>

// Function declaration (must match the solution).
size_t span_initial_segment(const char* src, const char* segment);

int main() {
    // Basic case: prefix matches.
    assert(span_initial_segment("hello world", "hel ") == 5);
    // No match at start.
    assert(span_initial_segment("abc", "b") == 0);
    // Entire src matches.
    assert(span_initial_segment("aaaa", "a") == 4);
    // Empty src.
    assert(span_initial_segment("", "abc") == 0);
    // Empty segment.
    assert(span_initial_segment("xyz", "") == 0);
    // Segment contains characters beyond letters (digits, punctuation).
    assert(span_initial_segment("123!@#abc", "123!@#") == 7);
    // Non-ASCII high characters (e.g., 0xFF) are treated as unsigned.
    char src[] = {static_cast<char>(200), 'a', '\0'};
    char seg[] = {static_cast<char>(200), '\0'};
    assert(span_initial_segment(src, seg) == 1);
    // Stop at the first character not in segment.
    assert(span_initial_segment("abcXYZ", "abc") == 3);
    return 0;
}
