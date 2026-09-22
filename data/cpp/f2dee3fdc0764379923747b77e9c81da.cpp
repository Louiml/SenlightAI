/*
Write a C++ function named `printCatAscii` that takes no arguments and returns a `std::string` containing the exact multi-line ASCII art of a cat as shown in the snippet (including the leading spaces on the first line and all quote characters). The function must not write to the standard output; instead, it should construct the string using newline characters (`\n`) and return it. The returned string must have no trailing newline at the end. A caller can then print the returned string using `std::cout << result;`. The task tests exact character-by-character equality with a reference string.
*/
#include <string>

// Return the exact multi-line ASCII art of a cat as a std::string.
// The string ends with the last line, without a trailing newline.
std::string printCatAscii() {
    // Store each line separately for clarity and to control newlines exactly.
    const std::string line1 = "       _.-;;-._";
    const std::string line2 = "\'-..-\'|   ||   |";
    const std::string line3 = "\'-..-\'|_.-;;-._|";
    const std::string line4 = "\'-..-\'|   ||   |";
    const std::string line5 = "\'-..-\'|_.-\'\'-._|";

    // Concatenate with newline characters, no trailing newline.
    std::string result = line1;
    result += '\n';
    result += line2;
    result += '\n';
    result += line3;
    result += '\n';
    result += line4;
    result += '\n';
    result += line5;

    return result;
}
#include <cassert>
#include <string>

// Function prototype from solution
std::string printCatAscii();

int main() {
    // Build the expected string exactly.
    std::string expected = 
        "       _.-;;-._\n"
        "\'-..-\'|   ||   |\n"
        "\'-..-\'|_.-;;-._|\n"
        "\'-..-\'|   ||   |\n"
        "\'-..-\'|_.-\'\'-._|";

    // Test that the returned string exactly matches.
    assert(printCatAscii() == expected);

    // Test that the string is non-empty and has the correct number of newlines.
    std::string result = printCatAscii();
    assert(!result.empty());
    int newlineCount = 0;
    for (char c : result) {
        if (c == '\n') ++newlineCount;
    }
    assert(newlineCount == 4);  // 5 lines => 4 newlines.

    // Test that the last line is correct.
    assert(result.substr(result.rfind('\n') + 1) == "\'-..-\'|_.-\'\'-._|");

    // Test that the first line has exactly 7 leading spaces.
    assert(result.substr(0, 7) == "       ");

    // Test that there is no trailing newline at the end.
    assert(result.back() != '\n');

    return 0;
}
// The main challenge is to preserve the exact ASCII art, including spaces, single quotes, double quotes, hyphens, underscores, semicolons, and pipe characters. The simplest robust approach is to store the lines in an array of string literals, then join them with `\n` using `std::string` concatenation in a loop. Alternatively, you can directly initialize the `std::string` with the raw literal using `R"( ... )` to avoid escaping quotes, but since the art contains backslashes, double quotes, and newlines, a raw string literal simplifies the task. However, raw string literals may include a leading newline depending on formatting, so it’s safer to use explicit string concatenation. Edge cases: ensure the first line has exactly seven leading spaces, and the last line has no newline at the end. Time complexity is O(total length of output) which is constant (~150 characters), so O(1) with respect to input. Space complexity is O(n) for the returned string where n is the length of the ASCII art. No special error handling is needed since there is no input.
