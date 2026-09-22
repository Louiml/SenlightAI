// Write a C++ function that takes a positive integer `n` and returns a string representing a triangular pattern of asterisks. The pattern is built by printing exactly one `*` per line for `n` lines, where the `i`-th line (1-indexed) has spaces before the asterisk determined by the formula `2 * ((n - 1) - (2 * i))`. Your function must generate this exact string, with each line ending in a newline character. If `n` is less than 1, return an empty string. The pattern will only be valid (non-negative spaces) for `n` values where the formula yields a non-negative number for all `i`; however, you should still handle any input gracefully without errors (i.e., if the space count is negative, treat it as 0). The function must be pure and independent—no input/output inside, just return the string.

// The given code snippet reads an integer `n`, computes `m = n - 1`, and for each `i` from 1 to `n`, it prints spaces equal to `2 * (m - (2 * i))` followed by an asterisk and newline. Note that for typical values of `n`, this formula can produce negative space counts for large `i` (e.g., when `2*i > m`), but the original loop simply prints no spaces (since `for` with negative count runs zero times). Our function must replicate that behavior: if the computed space count is negative, we treat it as 0. We build the string by iterating `i` from 1 to `n`, appending `spaceCount` spaces (where `spaceCount = max(0, 2 * (m - 2*i))`), then one `*` and a newline. For `n < 1`, return empty string. Edge cases: `n = 1` gives `m = 0`, `spaceCount = max(0, 2*(0-2)) = 0`, so output is just `*\n`. For `n = 2`, `m=1`, `i=1` gives `2*(1-2)=-2`→0 spaces; `i=2` gives `2*(1-4)=-6`→0 spaces; output is two lines each `*`. Time complexity is O(n^2) in the worst case because the total number of spaces across all lines is roughly sum of `2*(m - 2*i)` for i where positive, which is O(n^2) for small n, but practically it's O(n * max_spaces) which is O(n^2). Space complexity is O(n^2) because we build and return a string of that size. However, for the problem typical constraints, it's fine.

#include <string>
#include <algorithm>

// Generate a triangular pattern of asterisks with spaces before each asterisk
// according to the formula: spaces = 2 * ((n-1) - 2*i) for i = 1..n.
// Negative space counts are treated as 0. Returns empty string for n < 1.
std::string generateAsteriskPattern(int n) {
    if (n < 1) return "";

    std::string result;
    const int m = n - 1;

    for (int i = 1; i <= n; ++i) {
        int spaces = 2 * (m - 2 * i);
        if (spaces < 0) spaces = 0;
        result.append(spaces, ' ');
        result.push_back('*');
        result.push_back('\n');
    }

    return result;
}

#include <cassert>

int main() {
    // n = 1: one line with no leading spaces
    assert(generateAsteriskPattern(1) == "*\n");

    // n = 2: two lines, both with zero spaces because formula yields negative
    assert(generateAsteriskPattern(2) == "*\n*\n");

    // n = 3: i=1: spaces = 2*(2-2)=0 → * ; i=2: spaces = 2*(2-4)=-4→0 ; i=3: spaces = 2*(2-6)=-8→0
    assert(generateAsteriskPattern(3) == "*\n*\n*\n");

    // n = 4: i=1: spaces=2*(3-2)=2 → "  *"; i=2: spaces=2*(3-4)=-2→0; i=3:0; i=4:0
    assert(generateAsteriskPattern(4) == "  *\n*\n*\n*\n");

    // n = 5: i=1: spaces=2*(4-2)=4; i=2: spaces=2*(4-4)=0; i=3: negative→0; i=4:0; i=5:0
    assert(generateAsteriskPattern(5) == "    *\n*\n*\n*\n*\n");

    // n = 6: i=1: 2*(5-2)=6; i=2: 2*(5-4)=2; i=3: 2*(5-6)=-2→0; etc.
    assert(generateAsteriskPattern(6) == "      *\n  *\n*\n*\n*\n*\n");

    // n = 0 or negative returns empty
    assert(generateAsteriskPattern(0) == "");
    assert(generateAsteriskPattern(-1) == "");

    // Verify length for n=10: sum of spaces + asterisks + newlines
    // total chars = sum(max(0, 2*(9-2*i))) + 10*2
    std::string big = generateAsteriskPattern(10);
    int expected_length = 0;
    for (int i = 1; i <= 10; ++i) {
        int sp = 2 * (9 - 2 * i);
        if (sp < 0) sp = 0;
        expected_length += sp + 2; // spaces + '*' + '\n'
    }
    assert(static_cast<int>(big.length()) == expected_length);

    return 0;
}
