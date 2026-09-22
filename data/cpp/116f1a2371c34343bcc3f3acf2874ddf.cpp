Write a C++ function named `capitalizeFirstLetter` that takes a non-empty string as input and returns a new string where the first character is converted to uppercase (using the ASCII/standard `toupper` behavior) and all remaining characters are preserved exactly as they appear. The function should work for strings that begin with letters, digits, punctuation, or spaces; if the first character is not a lowercase letter, it should remain unchanged. The function must not modify the original string and should be `const`-correct. The input string may contain any printable ASCII characters, including spaces and special symbols, and may be of any length (but will always be non-empty for the tests).
// The solution iterates over the input string once. We can directly construct the output string by taking the first character, applying `std::toupper` (which safely returns the original character if it is not a lowercase letter or if the locale's rules differ), and then appending the rest of the string using a substring operation or a loop. Edge cases include: a single-character string (so the loop or substring handles it correctly), a first character that is already uppercase or non-alphabetic (no change), and strings with embedded spaces or punctuation (no effect on these). The algorithm runs in \(O(n)\) time, where \(n\) is the length of the string, and uses \(O(n)\) auxiliary space for the returned string (which is unavoidable). We use `std::toupper` which requires casting to `unsigned char` in general to avoid undefined behavior for negative values, but since input is printable ASCII, it's safe; for robustness we cast anyway.
#include<string>
#include<cctype>

// Return a copy of the input with the first character converted to uppercase.
// Non-letter first characters are left unchanged.
std::string capitalizeFirstLetter(const std::string& input) {
    if (input.empty()) {
        return input;
    }
    std::string result;
    result.reserve(input.size());
    // Use static_cast to unsigned char to avoid undefined behavior in std::toupper
    result.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(input[0]))));
    result.append(input, 1, std::string::npos);
    return result;
}
#include<assert.h>

// Note: the solution function is defined in the Solution section; this main is for testing.
int main() {
    assert(capitalizeFirstLetter("hello") == "Hello");
    assert(capitalizeFirstLetter("world") == "World");
    assert(capitalizeFirstLetter("already") == "Already");
    assert(capitalizeFirstLetter("123abc") == "123abc");
    assert(capitalizeFirstLetter("!test") == "!test");
    assert(capitalizeFirstLetter("a") == "A");
    assert(capitalizeFirstLetter("Z") == "Z");
    assert(capitalizeFirstLetter("hello world") == "Hello world");
    assert(capitalizeFirstLetter("  leading space") == "  leading space");
    assert(capitalizeFirstLetter("xYz") == "XYz");
}
