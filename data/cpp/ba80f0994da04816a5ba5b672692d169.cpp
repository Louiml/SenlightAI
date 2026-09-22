Write a C++ function that takes a C-style string (null-terminated character array) containing only uppercase letters and no spaces, and returns a new C++ `std::string` where each character is printed to standard output *in reverse order* using `printf("%c", ...)`, and the function also returns that reversed string. The input string is guaranteed to be non-empty and contain only characters from `'A'` to `'Z'`. The function must not modify the input string, must use `const` correctly, and must handle the automatic null terminator added by string literals. The output should be a single line with the reversed characters, no extra spaces or newlines added by the function itself.

The solution must traverse the input C-string from the last character before the null terminator back to the first character. First, compute the length of the string by scanning until `'\0'`, or use `strlen` from `<cstring>`. Then iterate from `length-1` down to `0`, printing each character with `printf("%c", str[i])` and simultaneously appending it to a `std::string` result. Edge cases: non-empty input—so at least one character exists; null terminator is automatically appended by the compiler when using a string literal, so no manual `'\0'` needed in output; only uppercase letters, so no special handling for lowercase or digits. Time complexity is O(n) for scanning plus O(n) for the reverse loop, total O(n). Space complexity is O(n) for the returned string, plus O(1) auxiliary for counters. The function must be `const`-correct by taking `const char* str` as a parameter.

#include <cstdio>
#include <cstring>
#include <string>

// Print the characters of a C-string in reverse order to stdout and return the reversed string.
std::string reverseAndPrint(const char* str) {
    std::size_t len = std::strlen(str);
    std::string reversed;
    reversed.reserve(len);
    
    for (std::size_t i = len; i > 0; --i) {
        char ch = str[i - 1];
        std::printf("%c", ch);
        reversed.push_back(ch);
    }
    
    return reversed;
}

#include <cassert>
#include <cstdio>
#include <string>

// Forward declaration of the solution function.
std::string reverseAndPrint(const char* str);

int main() {
    // Test 1: Simple reversal and output capture (we can't capture printf easily, so just check return)
    assert(reverseAndPrint("ABC") == "CBA");
    
    // Test 2: Single character
    assert(reverseAndPrint("Z") == "Z");
    
    // Test 3: Longer string with repeated characters
    assert(reverseAndPrint("HELLO") == "OLLEH");
    
    // Test 4: All same letters
    assert(reverseAndPrint("AAA") == "AAA");
    
    // Test 5: Full alphabet
    assert(reverseAndPrint("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == "ZYXWVUTSRQPONMLKJIHGFEDCBA");
    
    // Test 6: Verify that the input string is not modified
    const char* input = "TEST";
    reverseAndPrint(input);
    assert(std::string(input) == "TEST");
    
    // Test 7: Ensure function works with string literal (const correctness)
    assert(reverseAndPrint("WORLD") == "DLROW");
    
    return 0;
}
