/*
Write a standalone C++ function named `generatePatternFourSections` that takes a positive integer `n` and returns a `std::string` containing the exact pattern produced by the given code snippet, with each row separated by a newline character (`\n`). The pattern for each row `row` (1-indexed) consists of four groups of asterisks separated by a single space: first a group of `row` asterisks, then a group of `n - row + 1` asterisks, then another group of `n - row + 1` asterisks, and finally a group of `row` asterisks. The function must reproduce the output exactly, including spaces between groups, with no trailing spaces at the end of any line, and the final newline after the last row must be omitted (i.e., the returned string should end with the last asterisk of the last row, not a newline). For example, for `n = 3`, the expected output string is: `"* *** *** *\n** ** ** **\n*** * * ***"` (note: for row 1, groups are 1,3,3,1; row 2: 2,2,2,2; row 3: 3,1,1,3). Handle edge cases: if `n` is 0 or negative, return an empty string. Ensure the function uses `const` where appropriate and does not print to standard output; it must only return the string.
*/

#include <string>

// Generate the pattern described by the original code snippet.
// Returns an empty string for n <= 0.
std::string generatePatternFourSections(int n) {
    if (n <= 0) {
        return "";
    }
    
    std::string result;
    // Reserve approximate space to reduce reallocations:
    // Each row has 2n + 2 asterisks and 3 spaces = 2n + 5 characters, plus newline except last.
    // Total ~ n * (2n + 5) characters.
    result.reserve(static_cast<size_t>(n) * (2 * n + 5));
    
    for (int row = 1; row <= n; ++row) {
        int leftGroup = row;
        int rightGroup = n - row + 1;
        
        // Build the four groups
        result.append(leftGroup, '*');
        result += ' ';
        result.append(rightGroup, '*');
        result += ' ';
        result.append(rightGroup, '*');
        result += ' ';
        result.append(leftGroup, '*');
        
        // Add newline except after the last row
        if (row != n) {
            result += '\n';
        }
    }
    
    return result;
}

#include <cassert>
#include <string>

// Function declaration (from solution)
std::string generatePatternFourSections(int n);

int main() {
    // Test n=1: single row with groups 1,1,1,1
    assert(generatePatternFourSections(1) == "* * * *");
    
    // Test n=2
    // Row1: * ** ** *  (1,2,2,1)
    // Row2: ** * * **  (2,1,1,2)
    assert(generatePatternFourSections(2) == "* ** ** *\n** * * **");
    
    // Test n=3 as given
    assert(generatePatternFourSections(3) == "* *** *** *\n** ** ** **\n*** * * ***");
    
    // Test n=4
    // Row1: * **** **** *
    // Row2: ** *** *** **
    // Row3: *** ** ** ***
    // Row4: **** * * ****
    std::string expected4 = 
        "* **** **** *\n"
        "** *** *** **\n"
        "*** ** ** ***\n"
        "**** * * ****";
    assert(generatePatternFourSections(4) == expected4);
    
    // Test n=0 and negative
    assert(generatePatternFourSections(0) == "");
    assert(generatePatternFourSections(-5) == "");
    
    // Test large n to ensure no crash; just check length pattern
    std::string large = generatePatternFourSections(10);
    assert(large.size() > 0);
    // Spot check: first row should be "* ********** ********** *" 
    // actually n=10: row1 groups 1,10,10,1. So starts with "* ********** ********** *"
    size_t firstNewline = large.find('\n');
    assert(firstNewline != std::string::npos);
    assert(large.substr(0, firstNewline) == "* ********** ********** *");
    
    // Verify no trailing newline
    assert(large.back() == '*');
    
    return 0;
}

// The solution is straightforward: iterate over each row from 1 to `n` inclusive. For each row, construct four substrings using string repetition: `std::string(row, '*')`, `std::string(n - row + 1, '*')`, `std::string(n - row + 1, '*')`, and `std::string(row, '*')`. Concatenate them with single spaces between, and append a newline except after the last row. The complexity is O(n^2) in time and space because the total number of asterisks across all rows is proportional to n^2 (each row has 2n+2 asterisks, and there are n rows, so total characters ~ 2n^2 + 3n). Edge cases: if `n <= 0`, return an empty string immediately. Also, ensure that `n` is handled without overflow; for typical int ranges, this is fine. For clarity, use a helper to build each line: for row from 1 to n, compute left = row, right = n - row + 1, then line = string(left, '*') + " " + string(right, '*') + " " + string(right, '*') + " " + string(left, '*'). Accumulate lines with '\n' between but not after the last. The function is `generatePatternFourSections(int n) const`? Actually, a free function, not a member, so no const on the function itself, but we can apply `const` to any local variables. Use `std::ostringstream` for efficient concatenation or simply build with `std::string` and `+=`. Time complexity: O(n^2) for building the string. Space complexity: O(n^2) for the returned string.
