// Write a C++ function `size_t spanLength(const char* str, const char* accept)` that returns the length of the initial portion of `str` consisting only of characters that appear in the string `accept`. The function must treat characters as unsigned bytes, so high-bit and non-ASCII bytes are handled correctly. The function should stop at the first character in `str` that is not found in `accept`, and should return 0 if `accept` is empty or if the first character of `str` is not accepted. The input strings are null-terminated C-style strings.

// The solution uses a `std::bitset<256>` to mark which byte values are present in the `accept` string. Iterate over `accept` and set each byte’s bit in the bitset. Then iterate over `str` while the current byte’s bit is set, incrementing a counter. The main loop stops when hitting a null terminator (end of `str`) or a character not in the bitset. Edge cases include empty `accept` (bitset all false, so loop stops immediately and returns 0), empty `str` (returns 0), and characters with values 128–255 (must be cast to `unsigned char` before indexing the bitset to avoid negative indices). Time complexity is O(|accept| + |matched prefix of str|) in the worst case, and O(1) auxiliary space because the bitset is fixed-size.

#include <cstddef>
#include <bitset>

// Returns the length of the initial segment of str consisting only of
// characters found in the accept string. Characters are treated as unsigned bytes.
size_t spanLength(const char* str, const char* accept) {
    std::bitset<256> accepted;
    for (const unsigned char* p = reinterpret_cast<const unsigned char*>(accept);
         *p != '\0'; ++p) {
        accepted.set(*p);
    }

    const unsigned char* s = reinterpret_cast<const unsigned char*>(str);
    size_t length = 0;
    while (*s != '\0' && accepted.test(*s)) {
        ++length;
        ++s;
    }
    return length;
}

#include <cassert>

int main() {
    // Basic matching
    assert(spanLength("hello", "hel") == 4); // 'h','e','l','l' all in "hel", stops at 'o'
    assert(spanLength("hello", "olh") == 1); // only 'h' is in "olh"
    assert(spanLength("hello", "a") == 0);   // first char not accepted

    // Empty accept or empty str
    assert(spanLength("hello", "") == 0);
    assert(spanLength("", "abc") == 0);
    assert(spanLength("", "") == 0);

    // High-bit and non-ASCII bytes
    const char highA[] = {static_cast<char>(0xE9), 'x', '\0'}; // é then 'x'
    const char acceptHigh[] = {static_cast<char>(0xE9), '\0'};
    assert(spanLength(highA, acceptHigh) == 1);

    // Accept contains all tested chars, full length
    assert(spanLength("abc", "cba") == 3);
    assert(spanLength("aaab", "a") == 3); // stops at 'b'

    // Space and punctuation
    assert(spanLength("  hello", " ") == 2);
    assert(spanLength("12345", "0123456789") == 5);
}
