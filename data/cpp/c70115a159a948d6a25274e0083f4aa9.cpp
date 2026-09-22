/*
Write a C++ function `readNames(int n)` that reads exactly `n` full names from standard input, where each name may contain multiple words (e.g., "akshay gupta"), and returns a `std::vector<std::string>` containing the names in the order they were read. The input will be provided on separate lines, one name per line, and may include leading or trailing spaces. The function must handle the newline character left in the input buffer after reading an integer `n` (which will be read separately in the caller). Ensure that empty lines (if any) are skipped, and only non-empty names are stored. Assume `n` is a positive integer and the input is well-formed with no extra blank lines between names, but there may be extra spaces around each name.
*/
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <limits>

// Reads n names from standard input, trimming leading/trailing whitespace.
// Skips empty lines (if any) and returns the list of non-empty names.
std::vector<std::string> readNames(int n) {
    std::vector<std::string> names;
    // Consume the newline that follows the integer n.
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for (int i = 0; i < n; ++i) {
        std::string line;
        std::getline(std::cin, line);

        // Trim leading and trailing whitespace (spaces, tabs, etc.)
        auto first = line.find_first_not_of(" \t\r\n");
        auto last = line.find_last_not_of(" \t\r\n");
        if (first == std::string::npos) {
            // Line is all whitespace, skip it.
            continue;
        }

        std::string trimmed = line.substr(first, last - first + 1);
        names.push_back(trimmed);
    }
    return names;
}
#include <cassert>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

// Declare the function (already defined above, but for standalone test we include its definition here)
std::vector<std::string> readNames(int n);

int main() {
    // Test 1: Simple two names, one with two words.
    {
        std::istringstream input("2\nakshay gupta\nabhishek\n");
        std::cin.rdbuf(input.rdbuf());
        std::vector<std::string> result = readNames(2);
        assert(result.size() == 2);
        assert(result[0] == "akshay gupta");
        assert(result[1] == "abhishek");
    }

    // Test 2: Names with leading/trailing spaces.
    {
        std::istringstream input("3\n  Alice Smith  \nBob\n  Charlie Brown\n");
        std::cin.rdbuf(input.rdbuf());
        std::vector<std::string> result = readNames(3);
        assert(result.size() == 3);
        assert(result[0] == "Alice Smith");
        assert(result[1] == "Bob");
        assert(result[2] == "Charlie Brown");
    }

    // Test 3: Only one name, and extra newline after n.
    {
        std::istringstream input("1\nJohn Doe\n");
        std::cin.rdbuf(input.rdbuf());
        std::vector<std::string> result = readNames(1);
        assert(result.size() == 1);
        assert(result[0] == "John Doe");
    }

    // Test 4: All names are single words, no spaces.
    {
        std::istringstream input("3\nRiya\nAman\nSara\n");
        std::cin.rdbuf(input.rdbuf());
        std::vector<std::string> result = readNames(3);
        assert(result.size() == 3);
        assert(result[0] == "Riya");
        assert(result[1] == "Aman");
        assert(result[2] == "Sara");
    }

    // Test 5: Input with tabs and multiple spaces inside name.
    {
        std::istringstream input("2\n\t   First   Last\t\n  Second   \n");
        std::cin.rdbuf(input.rdbuf());
        std::vector<std::string> result = readNames(2);
        assert(result.size() == 2);
        // Note: internal spaces are preserved, only leading/trailing trimmed.
        assert(result[0] == "First   Last");
        assert(result[1] == "Second");
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The main challenge is correctly consuming the newline after reading an integer `n` before reading the first name with `getline`. A common approach is to use `cin.ignore()` to discard the newline, but we need to be careful about potential extra whitespace. Since the snippet uses `getline(cin, temp)` to absorb the leftover newline, we can emulate that: after reading `n`, call `cin.ignore(numeric_limits<streamsize>::max(), '\n')` to ignore the rest of the current line (which likely contains only the newline). Then, for each of the `n` iterations, use `getline(cin, line)`, trim leading and trailing whitespace from `line`, and if the trimmed line is not empty, push it into the result vector. Trimming is important because the input may have extra spaces around names, and we want to store only the actual name content. If a line is empty (e.g., due to an accidental blank line), we skip it and continue reading, but since the problem states well-formed input, this is just a safety measure. The time complexity is O(total input characters) and space complexity O(total length of names), which is proportional to the input size.
