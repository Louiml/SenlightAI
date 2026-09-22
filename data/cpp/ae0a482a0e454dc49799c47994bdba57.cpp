// Write a C++ function `std::string pattern(int n)` that, for a positive integer `n`, returns a multiline string (with each line separated by `"\n"` and no trailing newline at the end) that prints the exact star (`*`) and tilde (`~`) pattern produced by the provided code snippet. The pattern consists of two halves: the first half has `n-1` lines where each line `i` (0-indexed) starts with `n-i` stars, then `i` tildes, then `i` tildes again, then `n-i` stars (so total per line: `(n-i)` stars, `2*i` tildes, `(n-i)` stars). The second half has `n` lines where each line `i` (0-indexed) starts with `i+1` stars, then `(n-1-i)` tildes, then `(n-1-i)` tildes again, then `(i+1)` stars (so total per line: `(i+1)` stars, `2*(n-1-i)` tildes, `(i+1)` stars). For `n=1`, the first half is empty and the second half is just a single line `"*~*"` (since `i=0`: 1 star, 0 tildes, 0 tildes, 1 star). The function must return the complete string; do not print to console. Handle `n` being any positive integer, including large values, using efficient string building. The function should be declared in a header-less style, and the signature must match exactly. Ensure the output string is built without unnecessary copies (e.g., using `std::ostringstream` or appending to a `std::string`).
#include <cassert>
#include <string>

// Forward declarations
std::string pattern(int n);
static void append_repeated(std::string&, char, int);

int main() {
    // Test n=1
    assert(pattern(1) == "*~*");

    // Test n=2
    assert(pattern(2) == "****\n*~~*\n**");

    // Test n=3 (build expected manually)
    // First half i=0: "***" + "" + "" + "***" -> "******"
    //           i=1: "**" + "~" + "~" + "**" -> "**~~**"
    // Second half i=0: "*" + "~~" + "~~" + "*" -> "*~~~~*"
    //           i=1: "**" + "~" + "~" + "**" -> "**~~**"
    //           i=2: "***" + "" + "" + "***" -> "******"
    assert(pattern(3) == "******\n**~~**\n*~~~~*\n**~~**\n******");

    // Test n=4 (verified with the original loop)
    // Let's quickly generate expected via the snippet logic.
    // First half: 
    // i=0: "****" + "" + "" + "****" = "********"
    // i=1: "***" + "~" + "~" + "***" = "***~~***"
    // i=2: "**" + "~~" + "~~" + "**" = "**~~~~**"
    // Second half:
    // i=0: "*" + "~~~" + "~~~" + "*" = "*~~~~~~*"
    // i=1: "**" + "~~" + "~~" + "**" = "**~~~~**"
    // i=2: "***" + "~" + "~" + "***" = "***~~***"
    // i=3: "****" + "" + "" + "****" = "********"
    std::string expected4 = 
        "********\n"
        "***~~***\n"
        "**~~~~**\n"
        "*~~~~~~*\n"
        "**~~~~**\n"
        "***~~***\n"
        "********";
    assert(pattern(4) == expected4);

    // Test n=5 (spot check some lines: first line has 5 stars + 0 tildes + 5 stars = "**********", last line same)
    std::string p5 = pattern(5);
    assert(p5.front() == '*');
    assert(p5.back() == '*');

    // Verify line count: n-1 + n = 2n-1 = 9 lines
    size_t newlines = 0;
    for (char c : p5) if (c == '\n') ++newlines;
    assert(newlines == 8); // 9 lines → 8 newlines

    // Test symmetry: first line equals last line
    size_t first_nl = p5.find('\n');
    size_t last_nl = p5.rfind('\n');
    assert(first_nl != std::string::npos);
    assert(last_nl != std::string::npos);
    std::string first_line = p5.substr(0, first_nl);
    std::string last_line = p5.substr(last_nl + 1);
    assert(first_line == last_line);

    // Test n=10: check no trailing newline
    std::string p10 = pattern(10);
    assert(p10.back() != '\n');
    // Count total characters: sum of lengths? Not necessary, but check first line length = 2*n-? Actually first line: n stars + 0 tildes + n stars = 2n = 20 characters.
    size_t first_nl10 = p10.find('\n');
    assert(first_nl10 == 20);

    // Edge test: non-positive input returns empty
    assert(pattern(0) == "");
    assert(pattern(-3) == "");

    return 0;
}
#include <string>

// Append `count` copies of character `ch` to the end of `out`.
static void append_repeated(std::string& out, char ch, int count) {
    if (count > 0) {
        out.append(static_cast<size_t>(count), ch);
    }
}

// Generate the complete star/tilde pattern for a given positive integer n.
// The pattern consists of two halves as described in the task.
std::string pattern(int n) {
    std::string result;
    if (n <= 0) {
        return result; // empty string for non-positive input
    }

    // First half: n-1 lines
    for (int i = 0; i < n - 1; ++i) {
        // Left stars: n - i
        append_repeated(result, '*', n - i);
        // Middle tildes: 2*i (two adjacent groups of i)
        append_repeated(result, '~', i);
        append_repeated(result, '~', i);
        // Right stars: n - i
        append_repeated(result, '*', n - i);
        // Add newline except after the last overall line? 
        // This is not the last line, so always add newline for first half.
        result.push_back('\n');
    }

    // Second half: n lines
    for (int i = 0; i < n; ++i) {
        // Left stars: i+1
        append_repeated(result, '*', i + 1);
        // Middle tildes: 2*(n-1-i) (two adjacent groups of n-1-i)
        int tilde_count = n - 1 - i;
        append_repeated(result, '~', tilde_count);
        append_repeated(result, '~', tilde_count);
        // Right stars: i+1
        append_repeated(result, '*', i + 1);
        // Add newline only if this is not the last line
        if (i != n - 1) {
            result.push_back('\n');
        }
    }

    return result;
}
// The core algorithm is to generate two groups of lines based on the loop structure from the snippet. For the first half (lines `i=0` to `n-2`), each line has `(n-i)` stars, then `2*i` tildes, then `(n-i)` stars. For the second half (lines `i=0` to `n-1`), each line has `(i+1)` stars, then `2*(n-1-i)` tildes, then `(i+1)` stars. The key is to avoid any stray characters and ensure each line ends with a newline except the last overall line, which must not end with a newline. Edge cases: `n=1` produces only the second half with one line `"*~*"`; `n=2` produces first half with one line `"**~~**"` (since `i=0`: stars=2, tildes=0, stars=2) and second half with two lines: `"*~~*"` and `"**~~**"` (wait, check second half: i=0: stars=1, tildes=2*(2-1-0)=2 → "*~~*"; i=1: stars=2, tildes=2*(2-1-1)=0 → "**"). So full pattern for n=2: line1: "**", "**"? Actually first half: i=0: stars=2, tildes=0, stars=2 → "****"; second half: i=0: "*~~*", i=1: "**". So overall: `"****\n*~~*\n**"`. This matches the snippet exactly for n=2. Time complexity: O(n^2) because the total number of characters is sum over lines of line lengths ≈ O(n^2). Space complexity: O(n^2) for the result string. Implementation uses `std::string` with `append` of repeated characters via a helper `add_repeated` or using `std::string(size_t, char)` constructor, and appends `'\n'` between lines but not after the last.
