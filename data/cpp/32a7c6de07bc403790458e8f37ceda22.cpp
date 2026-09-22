// Write a C++ function `convertToUpperCase(char* text)` that modifies a null-terminated C-style string in place so that every lowercase letter (from `'a'` through `'z'`) becomes uppercase, while leaving all other characters unchanged. The function must not use any standard library string functions (like `toupper`, `strlen`, etc.) — you may only use pointer arithmetic to find the end of the string. Do not output anything from the function; just modify the array. For safety, assume the input is always a valid null-terminated array.

The solution works by iterating over the characters of the string using a pointer that advances until the null terminator is encountered. For each character, we check whether it is a lowercase ASCII letter: the condition `ch >= 'a' && ch <= 'z'`. If true, we replace it with the corresponding uppercase letter by using the arithmetic `ch - 'a' + 'A'` (this works because in ASCII, lowercase letters and uppercase letters are separated by a constant offset). The loop stops when the current character is `'\0'`. We do not need to handle any edge cases beyond typical ASCII input: non-letter characters (digits, punctuation, spaces, uppercase letters) are left untouched. The time complexity is O(n) where n is the length of the string, and space complexity is O(1) since we modify in place and use only a pointer and a temporary character variable.

#include <cstddef>

// Converts all lowercase ASCII letters in a null-terminated string to uppercase.
// Modifies the array in place. Does not use any standard library string functions.
void convertToUpperCase(char* text) {
    if (text == nullptr) return; // defensive: no-op for null pointer

    for (char* p = text; *p != '\0'; ++p) {
        char ch = *p;
        if (ch >= 'a' && ch <= 'z') {
            *p = ch - 'a' + 'A';
        }
    }
}

#include <cassert>
#include <cstring>

int main() {
    // Test with mixed case string
    char s1[] = "ApPle";
    convertToUpperCase(s1);
    assert(strcmp(s1, "APPLE") == 0);

    // Test with all lower case
    char s2[] = "hello world";
    convertToUpperCase(s2);
    assert(strcmp(s2, "HELLO WORLD") == 0);

    // Test with no lower case letters
    char s3[] = "ABC123!@#";
    convertToUpperCase(s3);
    assert(strcmp(s3, "ABC123!@#") == 0);

    // Test with digits, spaces, punctuation mixed
    char s4[] = "a1 b2 c3";
    convertToUpperCase(s4);
    assert(strcmp(s4, "A1 B2 C3") == 0);

    // Test with empty string
    char s5[] = "";
    convertToUpperCase(s5);
    assert(strcmp(s5, "") == 0);

    // Test with single lowercase character
    char s6[] = "z";
    convertToUpperCase(s6);
    assert(strcmp(s6, "Z") == 0);

    // Test with single uppercase character
    char s7[] = "Z";
    convertToUpperCase(s7);
    assert(strcmp(s7, "Z") == 0);

    // Test with multiple lowercase repeated
    char s8[] = "aaa";
    convertToUpperCase(s8);
    assert(strcmp(s8, "AAA") == 0);

    // Test with long string containing only 'a' and spaces
    char s9[] = "a a a";
    convertToUpperCase(s9);
    assert(strcmp(s9, "A A A") == 0);

    // Test with null pointer (should not crash)
    convertToUpperCase(nullptr);
    // No assertion needed; if it reaches here, it's fine.

    return 0;
}
