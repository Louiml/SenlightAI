// Write a C++ function named `isItRated` that takes a single line of input as a `std::string` and returns a `std::string`: if the input line is exactly `"Is it rated?"` (including the question mark and capital letters, no extra spaces), return `"NO"`; otherwise, return `"YES"`. The function must treat the input as an exact whole-line comparison—any leading/trailing whitespace or different casing makes it not match. The function should be pure (no I/O inside) and const-correct.
The solution is a direct string comparison. The main algorithm: compare the input string to the literal `"Is it rated?"` using `==`. Since the task requires an exact match, no trimming or normalization is needed. Edge cases: empty string returns `"YES"`; strings with extra spaces (e.g., `" Is it rated?"` or `"Is it rated? "`) do not match; case differences (e.g., `"is it rated?"`) do not match; strings with the same text but extra characters (e.g., `"Is it rated??"`) do not match. Time complexity is O(n) where n is the length of the input string (due to comparison), and space complexity is O(1) auxiliary (the returned string is constant size).
#include <string>

// Returns "NO" if the input line is exactly "Is it rated?", otherwise "YES".
std::string isItRated(const std::string& line) {
    const std::string target = "Is it rated?";
    return (line == target) ? "NO" : "YES";
}
#include <cassert>
#include <string>

// Include the solution function declaration here or via a header
std::string isItRated(const std::string& line);

int main() {
    // Exact match
    assert(isItRated("Is it rated?") == "NO");
    // Different casing
    assert(isItRated("is it rated?") == "YES");
    // Extra leading/trailing spaces
    assert(isItRated(" Is it rated?") == "YES");
    assert(isItRated("Is it rated? ") == "YES");
    // Missing question mark
    assert(isItRated("Is it rated") == "YES");
    // Completely different text
    assert(isItRated("Hello world") == "YES");
    // Empty string
    assert(isItRated("") == "YES");
    // Extra question mark
    assert(isItRated("Is it rated??") == "YES");
    // Punctuation variation
    assert(isItRated("Is it rated!") == "YES");
    // With a colon
    assert(isItRated("Is it rated?:") == "YES");
    return 0;
}
