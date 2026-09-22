/*
Write a C++ function named `reverseCString` that takes a C-string (a null-terminated character array) and reverses the characters in place. The function should accept the character array and its length as parameters, and it must not rely on any standard library string or algorithm functions other than `swap`. After calling the function, the original array should contain the characters in reverse order, and the string must remain properly null-terminated. The function should work correctly for both even-length and odd-length strings, including a single-character string, and it should handle an empty string (length 0) gracefully without accessing invalid memory. Do not use a `main` function in the solution; provide only the function implementation.
*/
#include <algorithm> // for std::swap

// Reverse the characters of a C-string in place.
// The string must be null-terminated, and the length must not include the null terminator.
void reverseCString(char* str, const int length) {
    int start = 0;
    int end = length - 1;
    while (start <= end) {
        std::swap(str[start], str[end]);
        ++start;
        --end;
    }
}
#include <cassert>
#include <cstring>

int main() {
    // Test 1: Even length string
    char s1[] = "abcd";
    reverseCString(s1, 4);
    assert(std::strcmp(s1, "dcba") == 0);

    // Test 2: Odd length string
    char s2[] = "abcde";
    reverseCString(s2, 5);
    assert(std::strcmp(s2, "edcba") == 0);

    // Test 3: Single character
    char s3[] = "x";
    reverseCString(s3, 1);
    assert(std::strcmp(s3, "x") == 0);

    // Test 4: Empty string (length 0)
    char s4[] = "";
    reverseCString(s4, 0);
    assert(std::strcmp(s4, "") == 0);

    // Test 5: Two characters
    char s5[] = "ab";
    reverseCString(s5, 2);
    assert(std::strcmp(s5, "ba") == 0);

    // Test 6: String with repeated characters
    char s6[] = "aabb";
    reverseCString(s6, 4);
    assert(std::strcmp(s6, "bbaa") == 0);

    // Test 7: Palindrome string (should remain same)
    char s7[] = "racecar";
    reverseCString(s7, 7);
    assert(std::strcmp(s7, "racecar") == 0);

    // Test 8: All same characters
    char s8[] = "zzzz";
    reverseCString(s8, 4);
    assert(std::strcmp(s8, "zzzz") == 0);
}
// The solution uses a two-pointer technique: initialize a start index at 0 and an end index at `length - 1`. Swap the characters at these positions, then increment start and decrement end until start is greater than or equal to end. For even-length strings, the pointers will cross or meet exactly; for odd-length strings, they meet at the middle character, which does not need swapping. Edge cases: length 0 (the loop condition `start <= end` would be false immediately since `start=0` and `end=-1`, so no access occurs), length 1 (start and end both 0, swap is harmless but unnecessary; the loop still works). The algorithm runs in O(n) time for a string of length n, performing about n/2 swaps, and uses O(1) auxiliary space. The function should use `const` for the length parameter but not for the character array since it modifies it.
