// Write a C++ function `char classifyLetter(char c)` that takes a single character as input and returns `'A'` if the character is an uppercase English letter (`'A'`–`'Z'`), otherwise returns `'a'` (representing lowercase). The function must handle all ASCII characters, including digits, punctuation, and lowercase letters, and must work correctly for edge cases like `'Z'`, `'z'`, `'0'`, `' '`, and the null character. The function should not modify the input and must use proper `const` correctness.

// The solution is straightforward: check if the character lies within the ASCII range for uppercase letters using `c >= 'A' && c <= 'Z'`. If true, return `'A'`; otherwise, return `'a'`. This works because in ASCII, uppercase letters are contiguous from 65 to 90, and all other characters (lowercase, digits, symbols, control characters) fail the condition. There is no need to handle locale-specific behavior since the task specifies exact ASCII ranges. Edge cases include boundary characters `'A'` and `'Z'` (which should return `'A'`), lowercase `'a'` and `'z'`, digits, spaces, and characters outside printable range. Time complexity is O(1) and space complexity is O(1) as the function only performs a constant-time comparison and returns a single character.

#include <cctype> // not strictly needed, but for clarity

// Return 'A' if c is an uppercase ASCII letter, otherwise return 'a'.
char classifyLetter(const char c) {
    if (c >= 'A' && c <= 'Z') {
        return 'A';
    }
    return 'a';
}

#include <cassert>

int main() {
    assert(classifyLetter('A') == 'A');
    assert(classifyLetter('Z') == 'A');
    assert(classifyLetter('a') == 'a');
    assert(classifyLetter('z') == 'a');
    assert(classifyLetter('0') == 'a');
    assert(classifyLetter(' ') == 'a');
    assert(classifyLetter('!') == 'a');
    assert(classifyLetter('[') == 'a');
    assert(classifyLetter('@') == 'a');
    assert(classifyLetter('\0') == 'a');
    return 0;
}
