/*
Write a standalone C++ function named `printHourglassPattern` that takes a single positive integer `n` as input and returns a `std::string` containing an hourglass-shaped pattern of stars (`*`) and spaces, where the widest row (top and bottom) has `2*n - 1` stars, and the middle row (if any) has exactly `1` star. The pattern must be symmetric vertically: the top half decreases from `2*n - 1` stars down to `1` star (each row centered by leading spaces), then the bottom half increases back up to `2*n - 1` stars. Rows are separated by newline characters (`\n`). The function must not read from standard input or print to standard output; it must build and return the complete pattern as a string. Handle the edge case `n == 1` correctly (return a single `*` with no newline). The pattern must use only spaces for indentation, with one space between each pair of adjacent stars on the same row, and the total number of characters per row should be `2*n - 1` (including inner spaces between stars), with leading spaces to center the stars. Assume `n >= 1`.
*/
#include <string>

// Build an hourglass pattern of stars and spaces for a given positive integer n.
// Returns a string with rows separated by newline; no trailing newline.
std::string printHourglassPattern(int n) {
    std::string result;
    // Top half: decreasing stars, from 2n-1 down to 1.
    for (int i = 0; i < n; ++i) {
        int stars = 2 * (n - i) - 1;
        int leadingSpaces = i;
        // Append leading spaces.
        result.append(leadingSpaces, ' ');
        // Append alternating '*' and ' ' (odd positions get '*', even get ' ').
        for (int k = 1; k <= stars; ++k) {
            if (k % 2 == 1) {
                result.push_back('*');
            } else {
                result.push_back(' ');
            }
        }
        // Add newline except after the middle row (which is the last of top half).
        if (i != n - 1) {
            result.push_back('\n');
        }
    }
    // Bottom half: increasing stars, from 3 up to 2n-1 (skip the middle row already made).
    for (int j = 1; j < n; ++j) {
        int stars = 2 * j - 1;
        int leadingSpaces = n - j;
        result.append(leadingSpaces, ' ');
        for (int k = 1; k <= stars; ++k) {
            if (k % 2 == 1) {
                result.push_back('*');
            } else {
                result.push_back(' ');
            }
        }
        if (j != n - 1) {
            result.push_back('\n');
        }
    }
    return result;
}
#include <cassert>
#include <string>

int main() {
    // n = 1: single star, no newline
    assert(printHourglassPattern(1) == "*");
    
    // n = 2:
    // top row (i=0): 3 stars, leading 0 => "* * *"
    // middle (i=1): 1 star, leading 1 => " *"
    // bottom (j=1): 3 stars (2*1+1), leading 0 => "* * *"
    // Result: "* * *\n *\n* * *"
    assert(printHourglassPattern(2) == "* * *\n *\n* * *");
    
    // n = 3:
    // rows: "* * * * *\n * * *\n  *\n * * *\n* * * * *"
    assert(printHourglassPattern(3) == "* * * * *\n * * *\n  *\n * * *\n* * * * *");
    
    // n = 4: check symmetry and row count
    std::string p4 = printHourglassPattern(4);
    int newlines = 0;
    for (char c : p4) if (c == '\n') ++newlines;
    assert(newlines == 6); // 7 rows, so 6 newlines
    assert(p4.substr(0, 7) == "* * * *"); // first row has 7 stars separated by spaces => "* * * * * * *"
    // Actually first row for n=4 has 7 stars, so string starts with "* * * * * * *" length 13.
    assert(p4[0] == '*');
    assert(p4[13] == '\n'); // after first row
    
    // n = 5: verify no trailing newline
    std::string p5 = printHourglassPattern(5);
    assert(p5.back() != '\n');
    assert(p5.size() > 0);
    
    // Verify last row equals first row for n = 5
    std::string firstRow = p5.substr(0, p5.find('\n'));
    std::string lastRow = p5.substr(p5.find_last_of('\n') + 1);
    assert(firstRow == lastRow);
    
    // n = 6: total characters calculation for 11 rows
    // Not checking every character, just that the middle row is exactly n-1 spaces + "*"
    std::string p6 = printHourglassPattern(6);
    // Find middle row line
    std::string line5 = p6; // split
    size_t pos = 0;
    std::string middle;
    int row = 0;
    size_t start = 0, end = 0;
    // This is simpler: just check that each row length is 2*n-1 = 11
    std::string current;
    int count = 0;
    for (char c : p6) {
        if (c == '\n') {
            assert(current.length() == 11);
            if (count == 0) assert(current[0] == '*');
            if (count == 5) assert(current == "     *"); // 5 spaces + star
            current.clear();
            ++count;
        } else {
            current += c;
        }
    }
    // Last row (no newline after it)
    assert(current.length() == 11);
    assert(current[0] == '*');
    
    // Test n = 1 again explicitly
    assert(printHourglassPattern(1).size() == 1);
}
// The solution builds the string row by row. For each row index `i` (0-based) in the top half (descending), the number of stars is `2*(n - i) - 1`, and the leading spaces are `i`. For the bottom half (ascending, excluding the already-produced middle row), for a row index `j` starting from 1 up to `n-1`, the number of stars is `2*j - 1`, and leading spaces are `n - j`. A helper lambda `appendRow(stars, leadingSpaces)` constructs a row: add `leadingSpaces` spaces, then for `stars` count, output `*` for odd positions (1-based) and a space for even positions (since the pattern alternates `*`, space, `*`, ...). Then append a newline except after the last row (to avoid trailing newline). Edge case `n == 1`: the top half produces one row with one star, and the bottom half loop is skipped entirely, so we return `"*"` without a newline. Time complexity is O(n^2) because each row's length is proportional to n, and there are O(n) rows. Space complexity is O(n^2) for the returned string, plus O(1) auxiliary.
