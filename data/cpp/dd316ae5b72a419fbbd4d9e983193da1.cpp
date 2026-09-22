/*
Write a C++ function named `countValidPasswords` that takes a single string containing multiple lines of password policy data, where each line has the format `min-max letter: password` (e.g., `"1-3 a: abcde"`), and returns the number of passwords that satisfy their policy. A password is valid if the count of the specified letter in the password is between `min` and `max` inclusive. The input uses `-` to separate the min and max bounds, a space, the letter, a colon and space, then the password (which may contain any characters including spaces, but the password itself is the substring after the last space on the line). Lines are separated by `\n`. The input string may be empty (return 0). Assume all lines are well-formed and numeric bounds are non-negative integers.
*/

#include <algorithm>
#include <string>

// Count how many password entries in the multi-line input satisfy their policy.
// Each line format: "min-max letter: password" (bounds are non-negative integers).
int countValidPasswords(const std::string& input) {
    int valid = 0;
    std::size_t start = 0;

    while (start < input.size()) {
        // Find the end of the current line (either newline or end of string).
        std::size_t end = input.find('\n', start);
        if (end == std::string::npos) end = input.size();

        // Extract the current line (without newline).
        const std::string line = input.substr(start, end - start);
        if (!line.empty()) {
            // Parse min: characters before the first '-'.
            const std::size_t dashPos = line.find('-');
            const int min = std::stoi(line.substr(0, dashPos));

            // Parse max: after '-' up to the first space.
            const std::size_t spacePos = line.find(' ', dashPos + 1);
            const int max = std::stoi(line.substr(dashPos + 1, spacePos - dashPos - 1));

            // The required letter is the character immediately before the colon.
            const std::size_t colonPos = line.find(':');
            const char letter = line[colonPos - 1];

            // Password is everything after the last space in the line.
            const std::size_t lastSpacePos = line.rfind(' ');
            const std::string password = line.substr(lastSpacePos + 1);

            const int count = std::count(password.begin(), password.end(), letter);
            if (count >= min && count <= max) {
                ++valid;
            }
        }

        // Move to the next line (skip the newline character).
        start = end + (end < input.size() ? 1 : 0);
    }

    return valid;
}

#include <cassert>

int main() {
    // Basic valid and invalid cases.
    assert(countValidPasswords("1-3 a: abcde") == 1);          // one 'a' in [1,3] -> valid
    assert(countValidPasswords("1-3 b: cdefg") == 0);          // zero 'b' in [1,3] -> invalid
    assert(countValidPasswords("2-9 c: ccccccccc") == 1);      // nine 'c' in [2,9] -> valid

    // Multiple lines.
    assert(countValidPasswords("1-3 a: abcde\n1-3 b: cdefg\n2-9 c: ccccccccc") == 2);

    // Password contains spaces (but policy uses last space to separate).
    assert(countValidPasswords("1-2 x: hello world xx") == 1); // password "xx" has two x's in [1,2] -> valid
    assert(countValidPasswords("1-2 x: hello world x") == 1);

    // Empty input.
    assert(countValidPasswords("") == 0);

    // Lines with no newline at the end.
    assert(countValidPasswords("1-1 a: a") == 1);

    // Bounds with multiple digits.
    assert(countValidPasswords("10-12 a: aaaaaaaaaaa") == 1);  // 11 a's in [10,12] -> valid

    // Password with more than max occurrences.
    assert(countValidPasswords("1-2 a: aaa") == 0);

    // Password with fewer than min occurrences.
    assert(countValidPasswords("3-5 a: aa") == 0);

    // Exactly at the boundary.
    assert(countValidPasswords("2-2 b: bb") == 1);
}

// The solution processes the input string line by line. For each line, parse the minimum bound (characters before the first `-`), the maximum bound (characters between `-` and the first space after it), the required letter (character immediately before the colon), and the password (substring after the last space on the line). Use `std::count` to count occurrences of the letter in the password, then check if the count falls within the inclusive range. Edge cases: an empty input string should yield 0; lines may have passwords containing spaces, so using `rfind(' ')` correctly extracts the full password; numeric conversion via `std::stoi` handles single- and multi-digit bounds. Time complexity is O(L * P) where L is the number of lines and P is the average password length (due to counting), and space complexity is O(1) beyond the input string (only storing parsed values per line; no extra storage scaling with input length).
