Write a C++ function named `alphabetSquare` that takes a positive integer `n` as input and returns a `std::vector<std::string>` representing an `n` by `n` grid where characters are filled in row-major order starting from 'A' and continuing through the alphabet (A, B, C, ...). Each row should be a string of length `n` containing consecutive uppercase letters separated by nothing (just concatenated letters). The function should handle `n` such that the total number of characters `n*n` does not exceed 26 (i.e., `n` <= 5, since 5*5=25, and 6 would require 36 characters). If `n` is not positive or `n*n > 26`, return an empty vector. For example, for `n=3`, return `{"ABC", "DEF", "GHI"}`. The function should be pure and not read from standard input or print to output.

#include <cassert>
#include <vector>
#include <string>

// The solution function is included above in the section.
// Tests:

int main() {
    // n=1
    assert(alphabetSquare(1) == std::vector<std::string>{"A"});
    // n=2
    assert(alphabetSquare(2) == std::vector<std::string>{"AB", "CD"});
    // n=3
    assert(alphabetSquare(3) == std::vector<std::string>{"ABC", "DEF", "GHI"});
    // n=4
    assert(alphabetSquare(4) == std::vector<std::string>{"ABCD", "EFGH", "IJKL", "MNOP"});
    // n=5
    assert(alphabetSquare(5) == std::vector<std::string>{"ABCDE", "FGHIJ", "KLMNO", "PQRST", "UVWXY"});
    // n=6 -> 36 > 26, empty
    assert(alphabetSquare(6).empty());
    // n=0 -> empty
    assert(alphabetSquare(0).empty());
    // n=-1 -> empty
    assert(alphabetSquare(-1).empty());
    // Verify the last character for n=5 is 'Y' (since 25 letters)
    auto grid5 = alphabetSquare(5);
    assert(grid5.back().back() == 'Y');
    // Verify that the first character is always 'A'
    assert(alphabetSquare(4).front().front() == 'A');
}

#include <vector>
#include <string>

// Build an n x n grid filled with consecutive uppercase letters.
// Returns empty vector if n is not positive or n*n exceeds 26.
std::vector<std::string> alphabetSquare(int n) {
    if (n <= 0 || n * n > 26) {
        return {};
    }

    std::vector<std::string> grid;
    grid.reserve(n);
    char current = 'A';

    for (int i = 0; i < n; ++i) {
        std::string row;
        row.reserve(n);
        for (int j = 0; j < n; ++j) {
            row.push_back(current);
            ++current;
        }
        grid.push_back(row);
    }

    return grid;
}

// The core algorithm is straightforward: iterate over each row from 0 to `n-1`, and for each row, iterate over each column from 0 to `n-1`. Maintain a running character counter starting at 'A'. For each position, append the current character to a row string, then increment the character counter using `++ch`. After building the row string of length `n`, push it into the result vector. Important edge cases: if `n` is less than 1, or if `n*n` exceeds 26 (since 'Z' is the last uppercase letter), return an empty vector to avoid invalid character overflow. Time complexity is O(n^2) because we generate exactly `n*n` characters, and space complexity is O(n^2) for the stored output. No extra significant space is used besides the result.
