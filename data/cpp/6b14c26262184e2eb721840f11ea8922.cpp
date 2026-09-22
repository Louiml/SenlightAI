/*
Write a C++ function named `capitalizeFirstLetter` that takes a non-empty string consisting only of lowercase English letters (`'a'` through `'z'`) as input and returns a new string with the first character converted to its uppercase equivalent (e.g., `'a'` becomes `'A'`, `'b'` becomes `'B'`), leaving all other characters unchanged. The function must not modify the input string, must handle single-character strings correctly, and must work correctly regardless of how the string is passed. The input is guaranteed to be non-empty and consist only of lowercase letters, so no validation is required.
*/
#include <string>

// Returns a new string identical to input but with the first character
// converted from lowercase to uppercase. Assumes input is non-empty and
// consists only of lowercase letters.
std::string capitalizeFirstLetter(const std::string& input) {
    std::string result = input;
    if (!result.empty()) {
        result[0] = result[0] - 'a' + 'A';
    }
    return result;
}
#include <cassert>
#include <string>

// Declaration of the function under test.
std::string capitalizeFirstLetter(const std::string& input);

int main() {
    assert(capitalizeFirstLetter("hello") == "Hello");
    assert(capitalizeFirstLetter("a") == "A");
    assert(capitalizeFirstLetter("zebra") == "Zebra");
    assert(capitalizeFirstLetter("world") == "World");
    assert(capitalizeFirstLetter("single") == "Single");
    assert(capitalizeFirstLetter("c") == "C");
    assert(capitalizeFirstLetter("computer") == "Computer");
    assert(capitalizeFirstLetter("programming") == "Programming");
    assert(capitalizeFirstLetter("test") == "Test");
    assert(capitalizeFirstLetter("function") == "Function");
    return 0;
}
// The solution is straightforward: access the first character of the input string using `s[0]` (since the string is guaranteed non-empty, this is safe). To convert a lowercase letter to uppercase without relying on locale-specific functions, we can use the ASCII arithmetic: subtract `'a'` and add `'A'` (i.e., `s[0] - 'a' + 'A'`), because lowercase and uppercase letters are contiguous in ASCII with a fixed offset of 32. Since the input is guaranteed to be lowercase, no check is needed, but for robustness the reference solution will still include a conditional or use `std::toupper` with `<cctype>`. However, to match the spirit of the original snippet, we use the arithmetic approach. We create a copy of the input string, modify the copy’s first character, and return it. Edge cases: a single-character string (e.g., `"a"` → `"A"`) works fine; strings of length > 1 keep the rest unchanged. Time complexity is O(n) because copying the string is O(n), where n is the string length. Space complexity is O(n) for the returned string, plus O(1) auxiliary space.
