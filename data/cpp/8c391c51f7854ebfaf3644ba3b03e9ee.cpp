Write a C++ function called `reverseSentence` that takes a string (which may contain spaces, multiple words, and any printable ASCII characters) and returns a new string with all characters in reverse order. The original string should remain unmodified. Handle empty strings by returning an empty string, and preserve all whitespace characters exactly as they appear in the input, including leading, trailing, and multiple consecutive spaces. The function should use standard library facilities and must not modify the input.

#include <cassert>
#include <string>

std::string reverseSentence(const std::string& s); // forward declaration

int main() {
    assert(reverseSentence("") == "");
    assert(reverseSentence("a") == "a");
    assert(reverseSentence("abc") == "cba");
    assert(reverseSentence("hello world") == "dlrow olleh");
    assert(reverseSentence("  leading and trailing  ") == "  gniliart dna gnidael  ");
    assert(reverseSentence("multiple   spaces") == "secaps   elpitlum");
    assert(reverseSentence("123!@#") == "#@!321");
    assert(reverseSentence("palindrome") == "emordnilap");
    assert(reverseSentence("a b c") == "c b a");
    assert(reverseSentence("  ") == "  ");
    return 0;
}

#include <string>

// Return a string with all characters of `s` reversed, preserving whitespace.
std::string reverseSentence(const std::string& s) {
    std::string result;
    result.reserve(s.size());  // Allocate once for efficiency
    for (std::string::size_type i = s.size(); i > 0; --i) {
        result.push_back(s[i - 1]);
    }
    return result;
}

// The solution is straightforward: reverse the characters of the input string. The main algorithm is to create a new string that iterates from the last character of the input to the first, appending each character in that order. This ensures all characters—including spaces, punctuation, and digits—are reversed exactly as they appear. Edge cases include an empty input, which should immediately return an empty string, and inputs with multiple spaces or special characters, which are handled naturally by character-by-character copying. The time complexity is O(n), where n is the length of the input string, since each character is visited once. The space complexity is also O(n) because the function returns a new string of the same length, not modifying the input.
