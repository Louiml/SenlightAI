/*
Write a C++ function that takes a string as input and returns a new string formed by removing all space characters and converting any uppercase alphabetic letters (`'A'` through `'Z'`) to their lowercase equivalents. The function should preserve the order of all other characters, including digits, punctuation, and lowercase letters. The input string may be empty, may contain leading or trailing spaces, multiple consecutive spaces, or only uppercase/lowercase letters. The function must not modify the input; it should return the transformed string directly. For example, `"Hello World"` should produce `"helloworld"`, and `"A B C"` should produce `"abc"`.
*/
#include <string>

// Remove spaces and convert uppercase letters to lowercase in a string.
std::string normalizeString(const std::string& input) {
    std::string result;
    result.reserve(input.size()); // Reserve to avoid reallocations

    for (char ch : input) {
        if (ch != ' ') {
            if (ch >= 'A' && ch <= 'Z') {
                result += ch + 32; // ASCII offset to lowercase
            } else {
                result += ch;
            }
        }
    }
    return result;
}
#include <cassert>
#include <string>

// Declare the function (in practice it would be in a header)
std::string normalizeString(const std::string& input);

int main() {
    // Basic mixed-case and spaces
    assert(normalizeString("Hello World") == "helloworld");
    // All uppercase with spaces
    assert(normalizeString("ABC DEF") == "abcdef");
    // All lowercase with spaces
    assert(normalizeString("abc def") == "abcdef");
    // Empty string
    assert(normalizeString("") == "");
    // Only spaces
    assert(normalizeString("   ") == "");
    // Leading/trailing spaces and multiple consecutive spaces
    assert(normalizeString("  A  B  c  ") == "abc");
    // Punctuation and digits preserved
    assert(normalizeString("Hello, World! 123") == "hello,world!123");
    // Single uppercase letter
    assert(normalizeString("Z") == "z");
    // Mixed case with no spaces
    assert(normalizeString("AbCdEf") == "abcdef");
    // Numbers and special characters unaffected
    assert(normalizeString("1 A @ B 2") == "1a@b2");
    return 0;
}
// The task is a straightforward character-by-character transformation. The main algorithm iterates through each character in the input string using a range-based `for` loop. For each character, we ignore it if it equals a space (`' '`). Otherwise, if the character is an uppercase letter (ASCII range 65–90, or equivalently between `'A'` and `'Z'`), we convert it to lowercase by adding 32 to its ASCII value (or using `std::tolower` from `<cctype>`). If it is not uppercase, we append it unchanged. The complexity is linear in the length of the input: \(O(n)\) time and \(O(1)\) auxiliary space (not counting the returned string). Edge cases: empty string returns empty string; all spaces return empty string; mixed case and punctuation are handled correctly. The conversion using `ch + 32` relies on ASCII encoding, which is standard for C++. For robustness, using `std::tolower` is preferable, but the explicit arithmetic is acceptable for this task. The returned string accumulates the result, so space usage is proportional to the size of the output, which is at most the input size.
