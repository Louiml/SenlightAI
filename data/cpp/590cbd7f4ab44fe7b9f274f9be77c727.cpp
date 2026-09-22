Write a C++ function `capitalizeFirstLetter` that takes a non‑empty string `input` (containing only lowercase and uppercase English letters as well as digits, and no spaces) and returns a new string where the first character is converted to uppercase if it is a lowercase letter (i.e., between `'a'` and `'z'`). All other characters must remain unchanged. The function must preserve the original string (i.e., it must not modify the input parameter). Assume the input will always have at least one character.
#include <cassert>
#include <string>

// Solution function declaration (as provided above)
std::string capitalizeFirstLetter(const std::string& input);

int main() {
    // Basic lowercase conversion
    assert(capitalizeFirstLetter("hello") == "Hello");
    // Single character
    assert(capitalizeFirstLetter("a") == "A");
    // Already uppercase
    assert(capitalizeFirstLetter("World") == "World");
    // Starts with digit
    assert(capitalizeFirstLetter("1abc") == "1abc");
    // Mixed content after first character
    assert(capitalizeFirstLetter("xYz") == "XYz");
    // Long string
    assert(capitalizeFirstLetter("the quick brown fox") == "The quick brown fox");
    // First character is uppercase, rest lowercase
    assert(capitalizeFirstLetter("Zebra") == "Zebra");
    // Empty string edge case (though task says non-empty, but robustly handled)
    assert(capitalizeFirstLetter("") == "");
    return 0;
}
#include <string>
#include <cctype> // for std::islower (alternative to manual range check)

// Return a copy of the input string with its first character converted to uppercase.
// If the first character is not a lowercase letter, the function returns the input unchanged.
std::string capitalizeFirstLetter(const std::string& input) {
    std::string result = input; // copy the input

    // Check if the first character is a lowercase letter using std::islower (safer than ASCII arithmetic)
    if (!result.empty() && std::islower(static_cast<unsigned char>(result[0]))) {
        result[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(result[0])));
    }

    return result;
}
// The solution is straightforward: access the first character of the string using `input[0]`. If that character falls within the ASCII range for lowercase letters (`'a'`–`'z'`), convert it to uppercase by subtracting 32 from its ASCII code (since uppercase letters are exactly 32 less than their lowercase counterparts). We then create and return a copy of the input string with the first character replaced by the uppercase version. For correctness, we must consider the edge case where the string length is 1 (e.g., `"a"` becomes `"A"`), and where the first character is already uppercase or a digit (`"Z"` stays `"Z"`, `"5"` stays `"5"`). We can also handle the case where the first character is a non‑letter symbol if needed, but the task restricts input to letters and digits, so no special handling is required beyond the lowercase check. Time complexity is O(1) since we only inspect and modify the first character, and space complexity is O(n) for the returned copy (where n is the length of the input), though we could also use auxiliary O(1) space if we modify a copy in place—but creating a new string is clearer.
