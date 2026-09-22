// Write a C++ function `normalizeCase` that takes a non-empty string consisting only of lowercase and uppercase English letters (no spaces, digits, or other characters). The function must compare the number of lowercase letters against the number of uppercase letters. If there are more uppercase letters (strictly greater), return the entire string converted to uppercase; otherwise (if lowercase count is greater than or equal to uppercase count), return the entire string converted to lowercase. The input may be any length from 1 to 1000 characters. Assume only valid characters are provided, so no validation is needed. The function must return a new `std::string` and must not modify the original input.

The solution requires a single pass through the input string to count the occurrences of lowercase letters (characters in the range `'a'` to `'z'`). Since the input is guaranteed to contain only letters, any character that is not lowercase is necessarily uppercase, so the uppercase count can be inferred as `word.length() - lowercaseCount`. After counting, compare `lowercaseCount` with `uppercaseCount`. If uppercase is strictly greater, build and return a string where each character is converted to uppercase using `std::toupper` (casting the result to `char`); otherwise, build and return a string converted to lowercase using `std::tolower`. Edge cases include strings with all lowercase, all uppercase, equal counts, and a single character. The time complexity is O(n) because we iterate over the string once to count, and then possibly iterate again to build the result (or we could build both strings in the same pass, but the simpler approach is two passes). Space complexity is O(n) to store the result, plus input itself; auxiliary space beyond the output is O(1) if we use a simple loop.

#include <string>
#include <cctype>

// Return a copy of the input string with case normalized:
// all uppercase if there are more uppercase letters,
// otherwise all lowercase.
std::string normalizeCase(const std::string& word) {
    int lowercaseCount = 0;
    const int length = static_cast<int>(word.length());

    // Count lowercase letters; the rest are uppercase.
    for (char ch : word) {
        if (ch >= 'a' && ch <= 'z') {
            ++lowercaseCount;
        }
    }

    const int uppercaseCount = length - lowercaseCount;
    std::string result;
    result.reserve(length);

    if (uppercaseCount > lowercaseCount) {
        // More uppercase: convert entire string to uppercase.
        for (char ch : word) {
            result.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(ch))));
        }
    } else {
        // Lowercase count >= uppercase: convert entire string to lowercase.
        for (char ch : word) {
            result.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(ch))));
        }
    }

    return result;
}

#include <cassert>

int main() {
    // Mixed case, more lowercase
    assert(normalizeCase("aBc") == "abc");
    // Mixed case, more uppercase
    assert(normalizeCase("AbC") == "ABC");
    // All lowercase
    assert(normalizeCase("hello") == "hello");
    // All uppercase
    assert(normalizeCase("WORLD") == "world");
    // Equal counts → lowercase (since not strictly greater)
    assert(normalizeCase("AbCd") == "abcd");
    // Single lowercase
    assert(normalizeCase("z") == "z");
    // Single uppercase
    assert(normalizeCase("Z") == "z");
    // Long string with exactly 500 lowercase and 500 uppercase
    std::string mixed(500, 'a');
    mixed += std::string(500, 'A');
    assert(normalizeCase(mixed) == std::string(1000, 'a'));
    // Another long string with 501 uppercase and 499 lowercase
    std::string mixed2(499, 'a');
    mixed2 += std::string(501, 'A');
    assert(normalizeCase(mixed2) == std::string(1000, 'A'));
}
