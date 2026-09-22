Write a C++ function named `printNumberTriangle` that takes a positive integer `n` and returns a `std::string` containing a specific pattern of asterisks and spaces. The pattern consists of two parts: first, an ascending part where for each line `i` from 1 to `n`, print `i` leading spaces followed by exactly `n` asterisks, with each line separated by a newline character. Then a descending part where for each line `i` from `n` down to 1, print `i` leading spaces followed by exactly `n` asterisks, again with each line separated by a newline character. The output should contain no trailing spaces on any line, and the final line should end with a newline character. For example, for `n=3`, the returned string should be exactly: `"\n   ***\n  ***\n ***\n  ***\n ***\n***"` (note: the first line is empty because the original code prints a newline before any content, but you may choose to start with the first line containing the pattern without a leading empty line as long as you are consistent — in this task, follow the exact pattern described: each line starts with the spaces, then asterisks, and lines are separated by `\n`). You may assume `n` is at least 1.
#include <cassert>
#include <string>

// The solution function is defined above (assume it's included here).

int main() {
    // Test n=1: ascending "*" with 1 space, then descending "*" with 1 space
    assert(printNumberTriangle(1) == " *\n *\n");
    // Test n=2: ascending: "  **\n", " **\n", descending: " **\n", "**\n"
    assert(printNumberTriangle(2) == "  **\n **\n **\n**\n");
    // Test n=3: ascending 1..3, descending 3..1
    std::string expected3 = "   ***\n  ***\n ***\n  ***\n ***\n***\n";
    assert(printNumberTriangle(3) == expected3);
    // Test n=4
    std::string expected4 = "    ****\n   ****\n  ****\n ****\n  ****\n ****\n****\n";
    assert(printNumberTriangle(4) == expected4);
    // Verify first line starts with spaces, last line has exactly n asterisks and no trailing spaces
    std::string result5 = printNumberTriangle(5);
    // Check it ends with newline and has correct number of newlines (2n = 10)
    int newline_count = 0;
    for (char c : result5) if (c == '\n') ++newline_count;
    assert(newline_count == 10);
    // Check the last non-newline line has exactly 5 asterisks and no leading spaces
    size_t last_newline = result5.rfind('\n');
    std::string last_line = result5.substr(last_newline + 1);
    assert(last_line == "*****");
    return 0;
}
#include <string>

// Returns a string with the number-triangle pattern:
// ascending part: lines 1..n with i spaces then n asterisks
// descending part: lines n..1 with i spaces then n asterisks
// Each line is terminated by a newline.
std::string printNumberTriangle(int n) {
    std::string result;
    // Ascending part
    for (int i = 1; i <= n; ++i) {
        result.append(i, ' ');
        result.append(n, '*');
        result.push_back('\n');
    }
    // Descending part
    for (int i = n; i >= 1; --i) {
        result.append(i, ' ');
        result.append(n, '*');
        result.push_back('\n');
    }
    return result;
}
// The key is to build the output string incrementally. For the ascending part, iterate `i` from 1 to `n` inclusive: append `i` spaces then `n` asterisks, then append a newline (but not after the last line? Actually the task specifies that each line is separated by a newline, so after each line, including the last, you add `\n`). Similarly, for the descending part, iterate `i` from `n` down to 1: append `i` spaces, `n` asterisks, and a newline. Edge cases: `n=1` produces `" *\n *\n"` (if you follow the pattern exactly, ascending gives `" *\n"`, descending gives `" *\n"`, so combined `" *\n *\n"`). The original snippet prints an extra blank line at the very beginning (because of `printf("\n")` before the first loop), but the task description overrides that for clarity: we directly produce lines starting with spaces. Time complexity is O(n^2) because for each of the 2n lines we output n asterisks and up to n spaces, so O(n^2) characters. Space complexity is O(n^2) as well because we build the entire string in memory.
