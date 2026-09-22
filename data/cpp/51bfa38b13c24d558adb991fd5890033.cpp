/*
Write a C++ function named `removeAdjacentDuplicates` that takes a null-terminated character array (C-string) and removes all but the first character from every run of consecutive equal characters. The function should operate **in-place** and modify the original array so that the resulting string contains no two adjacent characters that are the same. The function should handle empty strings, single-character strings, strings where all characters are identical, and strings with no duplicates at all. You must not use any standard library string functions (like `strlen`, `strcpy`, or `std::string`), but you may use `<iostream>` for testing. The function signature must be: `void removeAdjacentDuplicates(char text[])`. The original size is not passed; you must discover the length by iterating until `'\0'`. The solution should use recursion as the primary control structure (the original snippet uses recursion, and you must preserve that constraint).
*/
// Recursively remove adjacent duplicate characters in-place.
// The input is a null-terminated character array.
void removeAdjacentDuplicates(char text[]) {
    // Base case: empty string or only one character (next is terminator)
    if (text[0] == '\0' || text[1] == '\0') {
        return;
    }

    // If current and next characters are equal, shift the rest left
    if (text[0] == text[1]) {
        // Shift all characters after index 1 one position left
        int i = 1;
        while (text[i] != '\0') {
            text[i] = text[i + 1];  // copy from next position
            i++;
        }
        // After shifting, the terminator has moved; no need to decrement manually
        // Recurse on the same index because new character may also equal current
        removeAdjacentDuplicates(text);
    } else {
        // Different characters: move to next position
        removeAdjacentDuplicates(text + 1);
    }
}
#include <cassert>
#include <iostream>

// Function prototype
void removeAdjacentDuplicates(char text[]);

int main() {
    char test1[] = "aababb";
    removeAdjacentDuplicates(test1);
    assert(std::string(test1) == "abab");

    char test2[] = "";
    removeAdjacentDuplicates(test2);
    assert(std::string(test2) == "");

    char test3[] = "a";
    removeAdjacentDuplicates(test3);
    assert(std::string(test3) == "a");

    char test4[] = "aaaa";
    removeAdjacentDuplicates(test4);
    assert(std::string(test4) == "a");

    char test5[] = "abc";
    removeAdjacentDuplicates(test5);
    assert(std::string(test5) == "abc");

    char test6[] = "aabbcc";
    removeAdjacentDuplicates(test6);
    assert(std::string(test6) == "abc");

    char test7[] = "aaabbbccc";
    removeAdjacentDuplicates(test7);
    assert(std::string(test7) == "abc");

    char test8[] = "abba";
    removeAdjacentDuplicates(test8);
    assert(std::string(test8) == "aba");

    char test9[] = "aabbaa";
    removeAdjacentDuplicates(test9);
    assert(std::string(test9) == "abab");

    char test10[] = "xyzz";
    removeAdjacentDuplicates(test10);
    assert(std::string(test10) == "xyz");

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The core idea is to recursively process the string from left to right. At each step, check whether the current character equals the next character. If they are equal, shift all characters after the current one one position to the left (overwriting the duplicate), which effectively removes the second character of the pair. Then, without advancing the pointer, recursively call the function on the same position (because after shifting, a new character may have moved into the next position that could also equal the current one). If the current and next characters differ, recursively call on the next position. The base case is when we reach the sentinel `'\0'` or when the next character is `'\0'` (meaning we're at the last character). Edge cases include: empty string (immediate return), single character (no adjacent duplicate), all identical characters (e.g., "aaa" → "a"), and no duplicates (no changes). The time complexity is O(n²) in the worst case because each removal requires shifting O(n) characters, and there can be O(n) removals. Space complexity is O(n) due to recursion depth (in worst case, n recursive calls). To handle in-place modification, we rely on the fact that the array is mutable, and we can safely overwrite positions.
