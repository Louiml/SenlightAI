/*
Write a C++ function `countUniqueLines` that takes no parameters and reads from standard input: first an integer `n` (number of lines to follow), then `n` lines of arbitrary text (each possibly containing spaces). The function should return the number of distinct lines among the `n` input lines, ignoring case differences (i.e., "Hello" and "hello" are considered the same). Lines may be empty, may contain leading/trailing spaces, and duplicate lines should be counted only once. The function must read input using `getline` after consuming the integer `n` (handling the newline left in the buffer) and return the count as an `int`.
*/

#include <iostream>
#include <string>
#include <set>
#include <cctype>

// Reads an integer n, then n lines from standard input.
// Returns the number of case-insensitive distinct lines.
int countUniqueLines() {
    int n;
    std::cin >> n;
    std::cin.ignore(); // discard newline after n

    std::set<std::string> uniqueLines;
    for (int i = 0; i < n; ++i) {
        std::string line;
        std::getline(std::cin, line);
        // Convert to lowercase for case-insensitive comparison
        for (char& c : line) {
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        }
        uniqueLines.insert(line);
    }
    return static_cast<int>(uniqueLines.size());
}

#include <iostream>
#include <sstream>
#include <cassert>

// Declaration of the function under test (already provided above)
int countUniqueLines();

int main() {
    // Test 1: basic distinct lines
    {
        std::istringstream input("3\napple\nbanana\napple\n");
        std::cin.rdbuf(input.rdbuf());
        assert(countUniqueLines() == 2);
    }

    // Test 2: case-insensitive duplicates
    {
        std::istringstream input("4\nHello\nhello\nHELLO\nWorld\n");
        std::cin.rdbuf(input.rdbuf());
        assert(countUniqueLines() == 2);
    }

    // Test 3: empty lines count as distinct
    {
        std::istringstream input("3\n\n\n\n");
        std::cin.rdbuf(input.rdbuf());
        assert(countUniqueLines() == 1);
    }

    // Test 4: zero lines
    {
        std::istringstream input("0\n");
        std::cin.rdbuf(input.rdbuf());
        assert(countUniqueLines() == 0);
    }

    // Test 5: lines with spaces are distinct from trimmed version
    {
        std::istringstream input("2\n hello\nhello\n");
        std::cin.rdbuf(input.rdbuf());
        assert(countUniqueLines() == 2);
    }

    // Test 6: multiple spaces and mixed case
    {
        std::istringstream input("4\nA B\n a b\nA B\nAB\n");
        std::cin.rdbuf(input.rdbuf());
        assert(countUniqueLines() == 3); // "a b", " a b", "ab" (case-insensitive)
    }

    // Test 7: all same after case normalization
    {
        std::istringstream input("3\nHi\nhI\nHI\n");
        std::cin.rdbuf(input.rdbuf());
        assert(countUniqueLines() == 1);
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

// The main challenge is reading the integer `n` and then reading full lines including empty ones. After `cin >> n`, the newline remains in the input buffer, so we must call `cin.ignore()` to discard it before using `getline`. For case-insensitive comparison, convert each line to lowercase (or uppercase) before inserting it into a `std::set<std::string>`. The set automatically eliminates duplicates, so the result is simply the size of the set. Edge cases: empty lines are valid and should be counted as a distinct line if they appear; lines with leading/trailing spaces are considered distinct from ones without them (since we only normalize case, not whitespace); `n` may be 0, in which case the function returns 0. Time complexity is O(total characters across all lines) because each line is converted and inserted once, with set insertion O(log k) where k is the number of distinct lines. Space complexity is O(total characters stored in the set).
