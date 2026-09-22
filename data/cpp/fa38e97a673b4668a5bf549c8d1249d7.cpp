/*
Write a C++ function named `lexicographicallyGreater` that takes two C-style strings (null-terminated character arrays) and returns an integer: `1` if the first string is lexicographically greater than the second, `-1` if the first is smaller, and `0` if they are equal. The function must implement lexicographic comparison manually without using `strcmp` or any other standard library comparison function. The comparison should be case-sensitive and follow standard ASCII ordering. The strings may contain any printable characters including spaces, and the function must correctly handle strings of different lengths (e.g., `"abc"` vs `"abcd"` — shorter is considered smaller if it is a prefix). Do not assume any maximum length, and the function must work for empty strings as well.
*/

#include <cstddef>

// Returns 1 if a is lexicographically greater than b,
// -1 if a is smaller, and 0 if equal.
int lexicographicallyGreater(const char* a, const char* b) {
    size_t i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] > b[i]) return 1;
        if (a[i] < b[i]) return -1;
        ++i;
    }
    // One or both strings ended.
    if (a[i] == '\0' && b[i] == '\0') return 0;
    if (a[i] == '\0') return -1; // a is shorter prefix => smaller
    return 1; // b is shorter prefix => b smaller, a greater
}

#include <cassert>

int main() {
    assert(lexicographicallyGreater("apple", "banana") == -1);
    assert(lexicographicallyGreater("banana", "apple") == 1);
    assert(lexicographicallyGreater("same", "same") == 0);
    assert(lexicographicallyGreater("abc", "abcd") == -1);
    assert(lexicographicallyGreater("abcd", "abc") == 1);
    assert(lexicographicallyGreater("", "") == 0);
    assert(lexicographicallyGreater("", "a") == -1);
    assert(lexicographicallyGreater("a", "") == 1);
    assert(lexicographicallyGreater("Zebra", "apple") == -1); // ASCII: 'Z' < 'a'
    assert(lexicographicallyGreater("hello world", "hello world") == 0);
}

// The solution compares character by character from the beginning of both strings until a difference is found or one string ends. If a differing character is encountered, the comparison is based on the ASCII values of those characters. If all characters are equal up to the end of the shorter string, then the shorter string is considered smaller (or equal if both are the same length). We iterate using indices, checking for the null terminator as the end condition. The algorithm runs in O(n) time, where n is the length of the shorter string, because we stop at the first difference or when one string ends. Space complexity is O(1) as we only use a few integer variables. Edge cases include both empty strings (return 0), one empty string and one non-empty (the empty is smaller), and prefix relationships (shorter prefix is smaller). The function must be `const`-correct by taking `const char*` parameters and not modifying the input.
