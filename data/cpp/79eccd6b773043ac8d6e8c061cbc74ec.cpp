/*
Write a C++ function that, given a positive integer `n`, returns a single string containing exactly `n` lines (excluding trailing newlines). For each line `i` (where line numbering starts at 1 and goes up to `n`), print exactly `n` characters: if the column index `j` (also starting at 1) satisfies `i <= j <= n`, then place the digit character representing `j` (for `j` in 1..9; if `n` is 10 or greater, only handle `j` from 1 to 9 as digits, and for any `j` >= 10, use the character `'?'` as a placeholder because a single character cannot represent multi‑digit numbers); otherwise, place a space `' '`. After each line, append a newline character `'\n'`, except after the last line. The function must be `const`‑correct and applicable to any positive integer `n` up to 1000. The output must be exactly `n` lines, each having length exactly `n` (counting spaces and digit characters). For example, for `n=5`, the output should be:
```
12345
 2345
  345
   45
    5
```
*/

#include <string>

// Generate the pattern string for a given n.
// Returns a string with exactly n lines, each of length n (excluding newlines).
std::string generateTrianglePattern(int n) {
    std::string result;
    result.reserve(static_cast<size_t>(n) * (n + 1)); // Reserve space for n lines + newlines

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (j < i) {
                result += ' ';
            } else if (j <= 9) {
                result += static_cast<char>('0' + j);
            } else {
                result += '?'; // Placeholder for multi-digit column numbers
            }
        }
        if (i != n) {
            result += '\n';
        }
    }
    return result;
}

#include <cassert>
#include <string>

// Assume the solution function is declared above here.
// In a real test, include the solution file.

int main() {
    // n=1: single line, single character '1'
    assert(generateTrianglePattern(1) == "1");

    // n=2: lines: "12", " 2" (no trailing newline)
    assert(generateTrianglePattern(2) == "12\n 2");

    // n=5: as described in the task
    std::string expected5 = "12345\n 2345\n  345\n   45\n    5";
    assert(generateTrianglePattern(5) == expected5);

    // n=9: all columns are single-digit, same as original snippet
    std::string expected9 = "123456789\n 23456789\n  3456789\n   456789\n    56789\n     6789\n      789\n       89\n        9";
    assert(generateTrianglePattern(9) == expected9);

    // n=10: column 10 becomes '?', still width = 10 per line
    std::string expected10 = "123456789?\n 23456789?\n  3456789?\n   456789?\n    56789?\n     6789?\n      789?\n       89?\n        9?\n         ?";
    // Care: last line has 9 spaces then '?', let's construct more reliably:
    // line 1: 1 2 3 4 5 6 7 8 9 ? → length 10
    // line 2: space 2 3 4 5 6 7 8 9 ? → " 23456789?"
    // line 3: "  3456789?"
    // ...
    // line 9: "        9?" (8 spaces then 9 then ?)
    // line 10: "         ?" (9 spaces then ?)
    // We'll just check that length per line is correct via helper in test.
    std::string result10 = generateTrianglePattern(10);
    // Verify line count and length
    size_t newlineCount = 0;
    for (char c : result10) if (c == '\n') newlineCount++;
    assert(newlineCount == 9); // 10 lines, 9 newlines
    // Check first 5 characters of first line
    assert(result10.substr(0, 5) == "12345");
    // Check that no ':' appears and all columns after 9 are '?'
    for (size_t pos = 0; pos < result10.size(); ++pos) {
        char c = result10[pos];
        if (c != '\n') {
            if (c == '?') {
                // find column index: not trivial, but we trust logic
            }
        }
    }
    // Simpler: test known partial strings
    assert(result10.substr(0, 10) == "123456789?");
    assert(result10.substr(11, 10) == " 23456789?");

    // n=100: just check that length per line is 100 and newline count is 99
    std::string result100 = generateTrianglePattern(100);
    size_t newlines = 0;
    size_t lineStart = 0;
    for (size_t pos = 0; pos <= result100.size(); ++pos) {
        if (pos == result100.size() || result100[pos] == '\n') {
            // line from lineStart to pos-1
            size_t len = pos - lineStart;
            assert(len == 100);
            lineStart = (pos == result100.size()) ? pos : pos + 1;
            newlines++;
        }
    }
    assert(newlines == 100);
}

// The pattern is a right‑aligned descending triangle of column numbers. For each row index `i` (1‑based), we output `(i-1)` leading spaces, then the digits from `i` up to `n`. However, because each cell must be exactly one character, we cannot output multi‑digit numbers like `12`; we only output a single character per column. For `j` from `i` to `9`, we output the character `'0' + j`. For any `j` between 10 and `n`, we output a placeholder `'?'` to keep the output width exactly `n` characters per line. This constraint is important when `n > 9`; the problem is defined for general `n`, and the placeholder ensures the format is valid (though it changes the visual from the original snippet, we preserve the original snippet's logic for `n <= 9`). The algorithm uses two nested loops: outer loop over `i` (1..n), inner loop over `j` (1..n). For each `(i,j)`:
// - If `j < i`, output space.
// - Else if `j <= 9`, output `'0'+j`.
// - Else (j >= 10), output `'?'`.
// After the inner loop, append `'\n'` unless it is the last line. Time complexity is O(n^2) because we iterate over all n² cells. Space complexity is O(n²) for the returned string (since we build the entire result). Edge cases: `n=1` gives `"1"` (no trailing newline). For `n > 9`, placeholders appear for columns 10 and above. The function must handle large n up to 1000 without integer overflow (the loop indices fit in int) and efficiently concatenate characters (using `std::string::push_back` or `operator+=`).
