// Write a C++ function named `toLowerCaseString` that takes a `std::string` by const reference and returns a new `std::string` where all uppercase ASCII letters (A–Z) are converted to their lowercase equivalents (a–z). The input may contain digits, punctuation, spaces, and mixed-case characters. The function must not modify the original string and must preserve all non-alphabetic characters exactly as they appear. The solution should use the standard library's `std::transform` algorithm with `::tolower` for conversion. Note that `::tolower` is safe for unsigned char values, so cast each character to `unsigned char` before passing to avoid undefined behavior on negative char values (common on platforms with signed `char`).
The main algorithm uses `std::transform` applied to the input string's iterators, writing the result into a newly created string of the same size. For each character, `::tolower` is called with the character cast to `unsigned char` to prevent potential undefined behavior from negative `char` values. The result is then converted back to `char` for storage. Edge cases include empty strings (returns an empty string), strings already all lowercase (unchanged), strings with no letters (unchanged), and strings with only uppercase letters (all converted). Mixed strings are handled correctly because `::tolower` only affects uppercase letters and returns other characters unchanged. Time complexity is O(n) where n is the string length, and space complexity is O(n) for the returned string (plus temporary storage for the transform output).
#include <string>
#include <algorithm>
#include <cctype>

// Convert all uppercase ASCII letters in the input to lowercase.
// Returns a new string; the original is not modified.
std::string toLowerCaseString(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) -> unsigned char {
                       return static_cast<unsigned char>(::tolower(c));
                   });
    return result;
}
#include <cassert>
#include <string>
#include <cctype>

// Include the solution function declaration here (or link separately).
// For completeness, the function is declared above in the full program.

int main() {
    // Basic uppercase conversion
    assert(toLowerCaseString("HELLO") == "hello");
    // Mixed case and punctuation
    assert(toLowerCaseString("Hello, World!") == "hello, world!");
    // Digits and symbols unaffected
    assert(toLowerCaseString("ABC123!@#") == "abc123!@#");
    // Already lowercase
    assert(toLowerCaseString("already lower") == "already lower");
    // Empty string
    assert(toLowerCaseString("") == "");
    // String with no letters
    assert(toLowerCaseString("12345  ") == "12345  ");
    // Single character
    assert(toLowerCaseString("Z") == "z");
    // Long mixed string with spaces
    assert(toLowerCaseString("MiXeD CaSe TeXt") == "mixed case text");
    // Ensure original input is not modified (optional but good practice)
    std::string original = "ORIGINAL";
    toLowerCaseString(original);
    assert(original == "ORIGINAL");
    return 0;
}
