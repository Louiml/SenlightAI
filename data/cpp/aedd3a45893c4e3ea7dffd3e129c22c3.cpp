// Write a C++ function named `compareCStrings` that takes two null-terminated C-style string arrays (`const char*` or `const char[]`) and returns an integer according to lexicographic order: `0` if the strings are equal, `1` if the first string is greater than the second, and `-1` if the first string is less than the second. The comparison must be case-sensitive and based on ASCII values character by character. The function should handle strings of different lengths correctly (e.g., `"abc"` is less than `"abcd"`, and `"abcd"` is greater than `"abc"`). The function must not use any standard library string comparison functions (like `strcmp`); it should manually iterate through the arrays. Additionally, the function must be `const`-correct (i.e., take parameters as `const char*` or `const char[]` and not modify them) and must properly handle edge cases such as both strings being empty or one being empty.
#include <cassert>

int main() {
    // Test empty strings
    assert(compareCStrings("", "") == 0);
    assert(compareCStrings("", "a") == -1);
    assert(compareCStrings("a", "") == 1);

    // Test identical strings
    assert(compareCStrings("hello", "hello") == 0);

    // Test prefix cases
    assert(compareCStrings("abc", "abcd") == -1);
    assert(compareCStrings("abcd", "abc") == 1);
    assert(compareCStrings("abc", "abc") == 0);

    // Test different characters
    assert(compareCStrings("apple", "banana") == -1);
    assert(compareCStrings("banana", "apple") == 1);

    // Test case sensitivity (ASCII: 'A'=65, 'a'=97)
    assert(compareCStrings("Apple", "apple") == -1);
    assert(compareCStrings("apple", "Apple") == 1);

    // Test numbers and punctuation
    assert(compareCStrings("123", "124") == -1);
    assert(compareCStrings("!a", "a") == -1);
    assert(compareCStrings("a!", "a") == 1);

    // Test long strings and null characters inside (note: cannot have null inside C string, so skip)
    // Test single character equality
    assert(compareCStrings("z", "z") == 0);
    assert(compareCStrings("z", "y") == 1);
    assert(compareCStrings("y", "z") == -1);

    // Test repeated characters of differing lengths
    assert(compareCStrings("aaa", "aaaa") == -1);
    assert(compareCStrings("aaaa", "aaa") == 1);
}
#include <cstddef>  // for size_t

// Compare two null-terminated C strings lexicographically.
// Returns: 0 if equal, 1 if str1 > str2, -1 if str1 < str2.
int compareCStrings(const char* str1, const char* str2) {
    // Compute lengths manually
    size_t len1 = 0;
    while (str1[len1] != '\0') {
        ++len1;
    }
    size_t len2 = 0;
    while (str2[len2] != '\0') {
        ++len2;
    }

    // Compare character by character up to the maximum length
    for (size_t i = 0; i < len1 || i < len2; ++i) {
        if (str1[i] > str2[i]) {
            return 1;
        } else if (str1[i] < str2[i]) {
            return -1;
        }
    }

    // If all compared characters equal, decide by length
    if (len1 == len2) {
        return 0;
    } else if (len1 < len2) {
        return -1;
    } else {
        return 1;
    }
}
// The solution manually compares two null-terminated C strings. First, compute the length of each string by iterating until the null character `'\0'` is found. Then, iterate through both strings up to the maximum of the two lengths. At each index `i`, compare `str1[i]` and `str2[i]`: if `str1[i] > str2[i]` return `1`, if `<` return `-1`. After the loop, if both lengths are equal return `0`; if `str1` is shorter return `-1`; otherwise return `1`. This correctly handles cases where one string is a prefix of the other, because when the loop ends, the shorter string will have a `'\0'` (ASCII 0) compared against the other string's non-null character, but the loop condition uses the max length, so the comparison at the index where one ends will naturally resolve (e.g., `str1[i]` is `'\0'` = 0, `str2[i]` is `'a'` = 97, so `str1 < str2`). Edge cases: both empty strings return `0`; empty vs non-empty returns `-1` or `1` based on which is empty. Time complexity is \(O(n + m)\) for computing lengths plus \(O(\max(n,m))\) for comparison, so overall \(O(n + m)\). Auxiliary space is \(O(1)\) as only integer counters are used.
