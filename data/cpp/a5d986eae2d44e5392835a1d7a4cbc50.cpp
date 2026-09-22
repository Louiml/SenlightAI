Write a C++ function `bool isValidMenuOption(char choice)` that takes a single character representing a user's menu selection and returns `true` if and only if the character is one of the digits `'1'`, `'2'`, `'3'`, `'4'`, or `'5'`, matching the valid options in the provided numeric-methods menu. The function must handle uppercase/lowercase letters (e.g., `'A'`, `'a'`), punctuation, whitespace, and any other characters as invalid, returning `false`. It must not modify any external state, must not use any I/O, and must be `const`-correct. Additionally, the function must treat the character `'0'` and `'6'` through `'9'` as also invalid, since only options 1–5 exist.
The solution is straightforward: check if the input character is exactly one of `'1'`, `'2'`, `'3'`, `'4'`, or `'5'` using a direct comparison or a set. Since the valid set is small and fixed, the simplest and most efficient approach is to use a logical OR of five equality comparisons: `choice == '1' || choice == '2' || ...`. This avoids any overhead of character conversion (like `isdigit`) because `isdigit` would incorrectly accept `'0'` and `'6'`–`'9'`. Edge cases include non-digit characters (letters, symbols, spaces) and the digit `'0'`—all must return `false`. The function has constant time complexity `O(1)` and constant auxiliary space `O(1)`, as no additional storage is required. Since the function only reads the input and returns a `bool`, it is trivially `const`-correct (no mutable state) and safe to call from any context.
#include <cstddef> // not strictly needed, but included for clarity

// Returns true if the given character is one of the menu options '1'..'5'.
// Any other character, including '0' and '6'..'9', returns false.
bool isValidMenuOption(const char choice) {
    // Direct comparison against the five valid digits.
    // This is more precise than isdigit(), which would accept '0' and '6'-'9'.
    return choice == '1' || choice == '2' || choice == '3' || choice == '4' || choice == '5';
}
#include <cassert>

int main() {
    // Valid options
    assert(isValidMenuOption('1') == true);
    assert(isValidMenuOption('2') == true);
    assert(isValidMenuOption('3') == true);
    assert(isValidMenuOption('4') == true);
    assert(isValidMenuOption('5') == true);
    
    // Invalid digits (outside 1-5)
    assert(isValidMenuOption('0') == false);
    assert(isValidMenuOption('6') == false);
    assert(isValidMenuOption('9') == false);
    
    // Invalid non-digit characters
    assert(isValidMenuOption('a') == false);
    assert(isValidMenuOption('A') == false);
    assert(isValidMenuOption('!') == false);
    assert(isValidMenuOption(' ') == false);
    assert(isValidMenuOption('\n') == false);
    
    // Edge: null character
    assert(isValidMenuOption('\0') == false);
    
    return 0;
}
