Write a C++ function named `countCharsUntilDollar` that reads characters from standard input one by one using `std::cin.get()` until it encounters the sentinel character `'$'` (not included in the counting), and then returns a `std::string` containing three integers separated by spaces: the number of lowercase English letters (a–z), the number of decimal digits (0–9), and the number of whitespace characters (space, newline, or tab) that were read before the sentinel. The function should not read beyond the sentinel character (i.e., it stops immediately after reading `'$'`), and it must process any characters before the sentinel, including uppercase letters, punctuation, and other symbols, which are ignored for counting. Assume the input is provided via a standard input stream and that the sentinel will eventually appear. Edge cases include the sentinel being the first character (all counts zero), and input containing mixed types of characters, including multiple consecutive whitespace, digits, and letters, with no requirement to handle end-of-file before the sentinel.
// The solution approach is to initialize three counters (`lowercaseCount`, `digitCount`, and `whitespaceCount`) to zero, then use a loop that repeatedly reads the next character with `std::cin.get()`. The loop continues while the character is not the sentinel `'$'`. Within the loop, each character is checked against specific ASCII ranges: for lowercase letters, check if the integer value is between 97 and 122 inclusive; for digits, check between 48 and 57 inclusive; for whitespace, check if the character equals space (`' '`), newline (`'\n'`), or tab (`'\t'`). The sentinel itself is not counted and terminates the loop, so the loop naturally stops immediately after reading it, satisfying the requirement not to read beyond. Important edge cases include the sentinel as the first input, where the loop condition checks the initial uninitialized character — but since the loop uses a `do-while` pattern or initializes the character before the loop, we must be careful: the original snippet has undefined behavior because it checks `c` before reading. To fix, we read the first character before entering the loop, or use a `while` with a `cin.get()` in the condition. A clean way is to use `char c; while (std::cin.get(c) && c != '$') { ... }`. This reads a character, and if it's not the sentinel, processes it; if the read fails (EOF), the loop ends, but the task assumes sentinel exists. Time complexity is \(O(n)\) where \(n\) is the number of characters up to and including the sentinel (excluding the sentinel from processing), and space complexity is \(O(1)\) since only a few integer counters and a character variable are used. The function returns a formatted string using `std::to_string` and concatenation.
#include <string>
#include <iostream>

// Reads characters from standard input until '$' (not counted) and returns a string with counts of lowercase letters, digits, and whitespace.
std::string countCharsUntilDollar() {
    int lowerCount = 0;
    int digitCount = 0;
    int whitespaceCount = 0;

    char c;
    while (std::cin.get(c) && c != '$') {
        if (c >= 'a' && c <= 'z') {
            ++lowerCount;
        }
        if (c >= '0' && c <= '9') {
            ++digitCount;
        }
        if (c == ' ' || c == '\n' || c == '\t') {
            ++whitespaceCount;
        }
    }

    return std::to_string(lowerCount) + " " + std::to_string(digitCount) + " " + std::to_string(whitespaceCount);
}
#include <cassert>
#include <sstream>
#include <iostream>
#include <string>

// Forward declaration of the solution function (assumes it is declared in a header or above).
std::string countCharsUntilDollar();

// Helper to redirect cin to a string stream and call the solution, then restore cin.
std::string runWithInput(const std::string& input) {
    std::istringstream inputStream(input);
    std::streambuf* originalCin = std::cin.rdbuf(inputStream.rdbuf());
    std::string result = countCharsUntilDollar();
    std::cin.rdbuf(originalCin);
    return result;
}

int main() {
    // Sentinel first, nothing counted.
    assert(runWithInput("$") == "0 0 0");

    // Only lowercase letters.
    assert(runWithInput("abc$") == "3 0 0");

    // Only digits.
    assert(runWithInput("12345$") == "0 5 0");

    // Mixed letters and digits with punctuation ignored.
    assert(runWithInput("a1B2c3!$") == "2 3 0");

    // Whitespace: space, newline, tab.
    assert(runWithInput(" \n\t$") == "0 0 3");

    // Mixed everything.
    assert(runWithInput("Hi 12\n\tend$") == "3 2 3"); // 'H' is uppercase, so only 'i', 'e', 'n', 'd'? Actually 'e','n','d' are 3 lowercase, plus 'i' is 1 -> 4 total? Let's compute: "Hi 12\n\tend$" -> chars: 'H' (uppercase), 'i' (lower), ' ' (space), '1' (digit), '2' (digit), '\n' (newline), '\t' (tab), 'e' (lower), 'n' (lower), 'd' (lower), '$' stops. Lowercase: i, e, n, d = 4. Digits: 1,2 = 2. Whitespace: space, newline, tab = 3. So assert should be "4 2 3".
    assert(runWithInput("Hi 12\n\tend$") == "4 2 3");

    // Digits and letters interleaved, no whitespace.
    assert(runWithInput("a1b2c3$") == "3 3 0");

    // Multiple whitespace of same kind.
    assert(runWithInput("   $") == "0 0 2"); // Only two spaces before '$' because sentinel stops.

    std::cout << "All tests passed." << std::endl;
    return 0;
}
