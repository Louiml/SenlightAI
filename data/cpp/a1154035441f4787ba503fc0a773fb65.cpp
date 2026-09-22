/*
Write a C++ function named `printPattern` that takes no parameters and returns `void`. When called, it must output exactly the pattern shown in the provided code snippet: ten lines where each line consists of five hyphen-space pairs (`"- "` repeated 5 times) followed by a single asterisk and a newline, then nine additional lines where the k-th line (k from 1 to 9) contains `(10 - k)` hyphen-space pairs followed by an asterisk and a newline. The function must not read any input, must not use global variables, and must rely only on standard output streams. The pattern should have a total of 19 lines. Your function should be self-contained, and the output must match exactly, including spaces and newlines.
*/

#include <iostream>

// Prints a fixed rectangle of 5 hyphens followed by asterisk, repeated 10 times,
// then a descending triangle of hyphens followed by asterisk.
void printPattern() {
    // First block: 10 rows, each with 5 "- " pairs then "*"
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 5; ++j) {
            std::cout << "- ";
        }
        std::cout << "*" << '\n';
    }
    
    // Second block: descending rows from 9 down to 0 hyphens before "*"
    for (int i = 10; i > 0; --i) {
        for (int j = 0; j < i - 1; ++j) {
            std::cout << "- ";
        }
        std::cout << "*" << '\n';
    }
}

#include <iostream>
#include <sstream>
#include <cassert>

// Declare the function under test (already implemented in solution)
void printPattern();

int main() {
    // Redirect cout to a stringstream to capture output
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    printPattern();
    
    // Restore original cout
    std::cout.rdbuf(old);
    
    std::string output = buffer.str();
    
    // Expected output as a string
    std::string expected;
    for (int i = 0; i < 10; ++i) {
        expected += "- - - - - *\n";
    }
    for (int i = 10; i > 0; --i) {
        for (int j = 0; j < i - 1; ++j) {
            expected += "- ";
        }
        expected += "*\n";
    }
    
    assert(output == expected);
    
    // Additional checks on line count and content
    std::istringstream lines(output);
    std::string line;
    int lineCount = 0;
    while (std::getline(lines, line)) {
        ++lineCount;
        if (lineCount <= 10) {
            assert(line == "- - - - - *");
        } else {
            int hyphens = (20 - lineCount); // for line 11: 9 hyphens, line 12: 8, ... line 19: 1
            assert(line == std::string(hyphens * 2, '-') + " " + "*");
        }
    }
    assert(lineCount == 19);
    
    std::cout << "All tests passed!\n";
    return 0;
}

// The task requires generating two distinct triangular-like patterns using nested loops. The first block is a fixed rectangle of 10 rows and 5 columns of `"- "` followed by `"*"`. This is straightforward: for each row `i` from 0 to 9, loop `j` from 0 to 4 to print `"- "` (note the space after the hyphen), then print `"*"` and a newline. The second block is a descending triangle: for row index `i` from 10 down to 1, print exactly `i-1` copies of `"- "`, then a `"*"` and newline. This produces 9 rows (i=10 down to i=2) with lengths 9,8,...,1, and finally when i=1, prints just `"*"` (since `i-1=0`). Important edge cases: ensure no extra spaces before the asterisk, ensure the correct number of rows, and ensure each row ends with a newline. The time complexity is O(number of printed characters) = O(10*5 + sum_{i=1}^{9} i) = O(50 + 45) = O(1) since the output size is constant. Space complexity is O(1) auxiliary.
