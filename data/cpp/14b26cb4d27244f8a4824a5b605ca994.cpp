// Write a C++ function that takes a C-style string (null-terminated `char*`) as input and converts all lowercase English letters (`'a'` through `'z'`) in-place to their uppercase equivalents, leaving all other characters unchanged. The function should then print the modified string followed by a newline to `std::cout`. The input string is guaranteed to be non-null, non-empty, and may contain spaces, digits, punctuation, and mixed-case letters. The function should return `void` and must not use any standard library string functions (e.g., `strlen`, `toupper`) or allocate new memory; it must operate directly on the given character array.

#include <iostream>
#include <cassert>
#include <cstring>

// The solution function is already defined above (or included separately).
// Test function directly.
int main() {
    // Test basic all-lowercase input
    char test1[] = "hello";
    convertLowerToUpperInPlaceAndPrint(test1);
    assert(std::strcmp(test1, "HELLO") == 0);

    // Test mixed case and punctuation
    char test2[] = "Hello, World! 123";
    convertLowerToUpperInPlaceAndPrint(test2);
    assert(std::strcmp(test2, "HELLO, WORLD! 123") == 0);

    // Test no lowercase letters
    char test3[] = "ABC123 !@#";
    convertLowerToUpperInPlaceAndPrint(test3);
    assert(std::strcmp(test3, "ABC123 !@#") == 0);

    // Test single character lowercase
    char test4[] = "z";
    convertLowerToUpperInPlaceAndPrint(test4);
    assert(std::strcmp(test4, "Z") == 0);

    // Test string with spaces only
    char test5[] = "  a  b  ";
    convertLowerToUpperInPlaceAndPrint(test5);
    assert(std::strcmp(test5, "  A  B  ") == 0);

    std::cout << "All tests passed." << std::endl;
    return 0;
}

#include <iostream>

// Converts all lowercase English letters in a C-string to uppercase in-place,
// then prints the resulting string followed by a newline.
// No standard library string functions are used; operates directly on array.
void convertLowerToUpperInPlaceAndPrint(char* text) {
    for (int index = 0; text[index] != '\0'; ++index) {
        // Check if current character is a lowercase letter
        if ((text[index] >= 'a') && (text[index] <= 'z')) {
            // ASCII offset between lowercase and uppercase is 32
            text[index] -= ('a' - 'A');
        }
    }
    std::cout << text << std::endl;
}

// The main algorithm is straightforward: iterate over the character array starting at index 0 and continue until the null terminator (`'\0'`) is encountered. For each character, check if it lies in the range `'a'` to `'z'` using ASCII codes (since `'a'` is 97 and `'z'` is 122). If so, convert it to uppercase by subtracting the constant `('a' - 'A')`, which equals 32, because ASCII uppercase letters are 32 positions lower than their lowercase counterparts. This works because the input is guaranteed printable ASCII. Edge cases include empty strings (though task says non-empty, the loop handles it by stopping immediately), already uppercase letters (they are unaffected because they fail the range check), non-letter characters like digits or spaces (also unaffected), and a string that ends before the null terminator (not possible in valid C-strings). The function must not modify the null terminator itself, which is guaranteed by the loop condition. Time complexity is `O(n)` where `n` is the length of the string (number of characters before the null terminator), and space complexity is `O(1)` because no extra storage is used beyond a loop counter.
