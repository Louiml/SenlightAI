/*
Write a C++ function that takes a non-empty string and returns a new string with all characters in reverse order. The function must preserve all characters exactly as they appear, including spaces, punctuation, digits, and special characters. The input string will never be empty, but it may contain leading/trailing spaces, multiple consecutive spaces, and any Unicode or ASCII characters. The function should not modify the input string and should work correctly for strings of any length, including very short strings like "A" or very long strings.
*/

#include <string>

// Return a new string with the characters of the input string in reverse order.
std::string reverseString(const std::string& str) {
    std::string reversed;
    reversed.reserve(str.size()); // optional optimization to avoid reallocations

    for (std::size_t i = str.size(); i > 0; --i) {
        reversed.push_back(str[i - 1]);
    }

    return reversed;
}

#include <cassert>
#include <string>

// Function under test (solution function from above)
std::string reverseString(const std::string& str) {
    std::string reversed;
    reversed.reserve(str.size());
    for (std::size_t i = str.size(); i > 0; --i) {
        reversed.push_back(str[i - 1]);
    }
    return reversed;
}

int main() {
    // Basic cases
    assert(reverseString("hello") == "olleh");
    assert(reverseString("A") == "A");
    assert(reverseString("ab") == "ba");

    // Cases with spaces and punctuation
    assert(reverseString("hello world") == "dlrow olleh");
    assert(reverseString("  leading and trailing  ") == "  gniliart dna gnidael  ");
    assert(reverseString("a b c") == "c b a");

    // Case with digits and special characters
    assert(reverseString("12345!@#") == "#@!54321");

    // Case with only spaces
    assert(reverseString("   ") == "   ");

    // Case with single character repeated
    assert(reverseString("xxxx") == "xxxx");

    // Case with mixed case
    assert(reverseString("Hello World!") == "!dlroW olleH");

    return 0;
}

// The solution reverses the input string by iterating from the last character to the first, appending each character to a result string. This is done using a simple loop that starts at `str.length() - 1` and decrements the index until it reaches 0. Edge cases include: a single-character string (loop runs once), strings with only spaces (spaces are preserved), and strings with length 1 or 2 where indexing is straightforward. There is no need to handle empty input per the specification, but the function should still be robust. Time complexity is O(n) because each character is visited exactly once, and space complexity is O(n) for the resulting string, on top of the input string which is not modified. Using `push_back` is efficient and avoids reallocation concerns for typical inputs.
