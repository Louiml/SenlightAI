/*
Write a C++ function named `replaceSpacesWithAtNine` that takes a non-const `std::string` by reference and replaces every space character (`' '`) with the two-character sequence `"@9"` (i.e., the at-sign followed by the digit 9). The function should modify the string in place and return the same reference to allow chaining. The input may contain leading, trailing, or multiple consecutive spaces. Do not modify any other characters. The function must handle an empty string correctly. After the call, the string will be longer than the original unless no spaces were present. For example, `"a b"` becomes `"a@9b"`, and `"   "` (three spaces) becomes `"@9@9@9"`.
*/
#include <string>

// Replace every space ' ' with the two-character sequence "@9" in the input string.
// Modifies the string in place and returns the same reference for chaining.
std::string& replaceSpacesWithAtNine(std::string& s) {
    // Build a new string to avoid repeated insertions.
    std::string result;
    result.reserve(s.size()); // At least original size; may need more if spaces exist.
    for (char ch : s) {
        if (ch == ' ') {
            result += "@9";
        } else {
            result += ch;
        }
    }
    // Assign the new content back to the original string.
    s = std::move(result);
    return s;
}
#include <cassert>
#include <string>

// Declaration of the function under test (in a real setup include the header)
std::string& replaceSpacesWithAtNine(std::string& s);

int main() {
    // Basic case with a single space
    std::string s1 = "a b";
    assert(replaceSpacesWithAtNine(s1) == "a@9b");

    // Multiple consecutive spaces
    std::string s2 = "a  b";
    assert(replaceSpacesWithAtNine(s2) == "a@9@9b");

    // Leading and trailing spaces
    std::string s3 = " hello ";
    assert(replaceSpacesWithAtNine(s3) == "@9hello@9");

    // No spaces
    std::string s4 = "hello";
    assert(replaceSpacesWithAtNine(s4) == "hello");

    // Empty string
    std::string s5 = "";
    assert(replaceSpacesWithAtNine(s5) == "");

    // All spaces
    std::string s6 = "   ";
    assert(replaceSpacesWithAtNine(s6) == "@9@9@9");

    // Mixed content with digits and symbols
    std::string s7 = "a@9 b c";
    assert(replaceSpacesWithAtNine(s7) == "a@9@9b@9c");

    // Long string with spaces at various positions
    std::string s8 = "one two three";
    assert(replaceSpacesWithAtNine(s8) == "one@9two@9three");

    // Verify the returned reference is the same object
    std::string s9 = "x y";
    std::string& ref = replaceSpacesWithAtNine(s9);
    assert(&ref == &s9);

    return 0;
}
// The solution iterates through the string from left to right, and each time a space is found, it replaces that single character with the two-character substring `"@9"`. Since the string grows in place, we can use the standard library's `std::string::replace` method, which handles reallocation automatically. However, a more efficient and straightforward approach is to build a new string by appending characters: for each character in the original string, if it is a space, append `"@9"`, otherwise append the character itself. Then assign the newly built string back to the input reference. This avoids costly repeated insertions. Care must be taken to use `const` access to the original string while reading (though the original is non-const, we can copy it first to avoid invalidation). Edge cases include: empty string (returns unchanged), no spaces (returns unchanged), multiple consecutive spaces (each becomes `"@9"`), and spaces at the boundaries. The time complexity is O(n) where n is the length of the original string (since we build a new string of at most 2n characters), and the auxiliary space is O(n) for the temporary string. The function correctly returns the reference to the modified input.
