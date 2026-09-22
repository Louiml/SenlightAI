Write a C++ function named `reverseCString` that takes a C-style string (a null-terminated character array) and reverses its contents in place, modifying the original array. The function must not use any standard library functions for reversing (such as `std::reverse`, `strrev`, etc.), and must work correctly for strings of any length, including empty strings and single-character strings. After the function returns, the same array should contain the reversed characters followed by a null terminator. The function should return nothing (void). As a bonus, ensure it handles strings with spaces correctly (e.g., `"hello world"` becomes `"dlrow olleh"`).

#include <cassert>
#include <cstring>

// Forward declaration of the function under test
void reverseCString(char* str);

int main() {
    // Test 1: normal word
    char s1[] = "hello";
    reverseCString(s1);
    assert(std::strcmp(s1, "olleh") == 0);
    
    // Test 2: phrase with spaces
    char s2[] = "hello world";
    reverseCString(s2);
    assert(std::strcmp(s2, "dlrow olleh") == 0);
    
    // Test 3: single character
    char s3[] = "a";
    reverseCString(s3);
    assert(std::strcmp(s3, "a") == 0);
    
    // Test 4: empty string
    char s4[] = "";
    reverseCString(s4);
    assert(std::strcmp(s4, "") == 0);
    
    // Test 5: even length palindrome
    char s5[] = "abba";
    reverseCString(s5);
    assert(std::strcmp(s5, "abba") == 0);
    
    // Test 6: odd length
    char s6[] = "abcde";
    reverseCString(s6);
    assert(std::strcmp(s6, "edcba") == 0);
    
    // Test 7: numeric characters
    char s7[] = "12345";
    reverseCString(s7);
    assert(std::strcmp(s7, "54321") == 0);
    
    // Test 8: special characters
    char s8[] = "!@#$%";
    reverseCString(s8);
    assert(std::strcmp(s8, "%$#@!") == 0);
    
    // Test 9: longer phrase
    char s9[] = "C++ is fun";
    reverseCString(s9);
    assert(std::strcmp(s9, "nuf si ++C") == 0);
    
    // Test 10: two characters
    char s10[] = "xy";
    reverseCString(s10);
    assert(std::strcmp(s10, "yx") == 0);
    
    return 0;
}

#include <cstddef>

// Reverses a C-style string in place.
// The input must be a null-terminated character array that is modifiable.
void reverseCString(char* str) {
    if (str == nullptr) return; // safety: nothing to do for null pointer
    
    // Find the length (number of characters before null terminator)
    std::size_t length = 0;
    while (str[length] != '\0') {
        ++length;
    }
    
    // Two-pointer approach: swap characters from both ends inward
    std::size_t left = 0;
    std::size_t right = length > 0 ? length - 1 : 0;
    
    while (left < right) {
        // Swap characters
        char temp = str[left];
        str[left] = str[right];
        str[right] = temp;
        
        ++left;
        --right;
    }
}

// The core idea is to use two pointers: one starting at the beginning of the string (index 0) and one starting at the character just before the null terminator. We swap the characters at these positions and then move the left pointer forward and the right pointer backward, continuing until the left pointer meets or passes the right pointer. This ensures each character is swapped exactly once, giving an in-place reversal. For an empty string (first character is `'\0'`), the loop does not execute. For a single-character string, left and right pointers point to the same character, so the swap is a no-op. For odd-length strings the middle character stays in place. Edge cases: input may contain spaces, which are treated as ordinary characters; the null terminator must remain at the position after the last character after reversal. The time complexity is O(n) where n is the length of the string (we scan half the string and do constant work per swap), and space complexity is O(1) because we only use a few integer variables for indices or pointers. The algorithm is straightforward and does not require additional storage.
