// Write a C++ function `reverseCString` that takes a null-terminated character array (C-string) and its length as parameters, reverses the order of characters in place, and returns `void`. The function must handle arrays containing only lowercase and uppercase letters, digits, and spaces (no embedded nulls except the terminator). If the length is 0 or 1, the array should remain unchanged. Use `std::swap` for swapping, and ensure the function does not rely on any standard library string classes—only raw `char` arrays are allowed. The solution must be self-contained and not use `std::reverse` or similar algorithms.
// The algorithm uses two indices: a start index `s` initialized to 0 and an end index `e` initialized to `n-1`. A `while` loop runs while `s < e`, swapping the characters at positions `s` and `e` using `std::swap`, then incrementing `s` and decrementing `e`. This effectively mirrors the string around its center. Edge cases: if `n` is 0 or 1, the loop condition fails immediately, leaving the array unchanged (a single character reversed is itself). The function assumes the input is a valid null-terminated C-string and that the provided `n` matches the actual length (excluding the null terminator). The null terminator is not part of the reversal—only the `n` characters before it are reversed. Since we swap in place using constant extra space, the time complexity is O(n) with n = number of characters, and auxiliary space complexity is O(1).
#include <cstddef>  // for size_t
#include <utility>  // for std::swap

// Reverses a C-string (character array) of given length in place.
// The null terminator at position n is not touched.
void reverseCString(char* str, std::size_t n) {
    if (str == nullptr || n < 2) return;  // nothing to reverse for empty or single-char

    std::size_t start = 0;
    std::size_t end = n - 1;
    while (start < end) {
        std::swap(str[start], str[end]);
        ++start;
        --end;
    }
}
#include <cassert>
#include <cstring>
#include <cstddef>

int main() {
    // Test 1: basic reversal
    char s1[] = "hello";
    reverseCString(s1, std::strlen(s1));
    assert(std::strcmp(s1, "olleh") == 0);

    // Test 2: empty string (length 0)
    char s2[] = "";
    reverseCString(s2, 0);
    assert(std::strcmp(s2, "") == 0);

    // Test 3: single character
    char s3[] = "x";
    reverseCString(s3, 1);
    assert(std::strcmp(s3, "x") == 0);

    // Test 4: two characters
    char s4[] = "ab";
    reverseCString(s4, 2);
    assert(std::strcmp(s4, "ba") == 0);

    // Test 5: palindrome remains same
    char s5[] = "radar";
    reverseCString(s5, 5);
    assert(std::strcmp(s5, "radar") == 0);

    // Test 6: string with spaces and numbers
    char s6[] = "a1 b2";
    reverseCString(s6, 5);
    assert(std::strcmp(s6, "2b 1a") == 0);

    // Test 7: even length
    char s7[] = "test";
    reverseCString(s7, 4);
    assert(std::strcmp(s7, "tset") == 0);

    // Test 8: null pointer (should not crash)
    reverseCString(nullptr, 10);  // no assertion needed, just ensure it's safe

    // Test 9: uppercase and lowercase
    char s9[] = "AbC";
    reverseCString(s9, 3);
    assert(std::strcmp(s9, "CbA") == 0);

    // Test 10: longer string
    char s10[] = "C++ is fun";
    reverseCString(s10, 11);
    assert(std::strcmp(s10, "nuf si ++C") == 0);
}
