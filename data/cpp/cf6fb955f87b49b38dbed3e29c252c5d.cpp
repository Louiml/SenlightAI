// Write a C++ function `printStars` that takes an integer `n` (where 1 ≤ n ≤ 100) and returns a string containing exactly `n` lines, each line representing a row of a triangular star pattern. The pattern follows this rule: the first row has 1 star, the second row has 2 stars, and so on up to the `n`-th row which has `n` stars. Each star in a row is separated by a single space, and each row is left-aligned with leading spaces so that the stars are centered (like a right-aligned triangle from the left edge, but with a single space between stars). Specifically, for row `i` (0-indexed), there are `n - i - 1` leading spaces, then `i + 1` stars each followed by a space (except after the last star, do not add a trailing space). Each line ends with a newline character. The function must return the complete pattern as a single string.

// The task is a variation of the classic "printing asterisks" problem but requires constructing and returning a string rather than printing directly. The algorithm is straightforward: use a loop from `i = 0` to `n-1`. For each iteration, append `n - i - 1` spaces to the result string, then append `i + 1` stars, but ensure each star is followed by a space only if it is not the last star in that row; equivalently, append `* ` for all stars except the last, then append `*` for the last one. Finally, append a newline character. Edge cases include `n = 1` (only one row with one star, no leading spaces) and maximum `n = 100`, which results in a string of length roughly `n^2` (about 10,000 characters), well within typical memory limits. Time complexity is O(n²) because the total number of characters printed is proportional to the sum of row lengths, which is about n²/2. Space complexity is O(n²) as well, since we must store the entire output string.

#include <string>

// Returns a string representing a triangular star pattern of height n.
// Each row i (0-indexed) has (n - i - 1) leading spaces, followed by (i + 1) stars,
// separated by single spaces, and ends with a newline.
std::string printStars(int n) {
    std::string result;
    for (int i = 0; i < n; ++i) {
        // Append leading spaces: n - i - 1 spaces
        result.append(n - i - 1, ' ');
        // Append stars: (i + 1) stars separated by single spaces, no trailing space
        for (int j = 0; j < i; ++j) {
            result += "* ";
        }
        result += '*';
        result += '\n';
    }
    return result;
}

#include <cassert>

int main() {
    // n = 1: single star with no leading spaces
    assert(printStars(1) == "*\n");

    // n = 2: row1: one leading space then one star; row2: no leading space then two stars
    assert(printStars(2) == " *\n* *\n");

    // n = 3: typical triangle
    assert(printStars(3) == "  *\n * *\n* * *\n");

    // n = 4: check leading spaces and star grouping
    assert(printStars(4) == "   *\n  * *\n * * *\n* * * *\n");

    // n = 5: larger pattern length check
    std::string res5 = printStars(5);
    // Row 1 should start with 4 spaces then a single '*'
    assert(res5.rfind("    *", 0) == 0);
    // Last row should be "  * * * * *" with one leading space? Actually last row for n=5 has 0 leading spaces and 5 stars.
    // Verify total length: sum of (leading spaces + 2*(i+1) - 1) for i=0..4 = sum(5-i-1 + 2*i+1) = sum(5 + i) for i=0..4 = 5*5 + 10 = 35 plus 5 newlines = 40
    assert(res5.size() == 40);

    // n = 100: ensure function completes and has expected length formula
    int n = 100;
    std::string resBig = printStars(n);
    // Total characters = sum_{i=0}^{n-1} ( (n-i-1) + (i+1)*2 - 1 + 1 ) = sum_{i=0}^{n-1} (n + i) = n^2 + n(n-1)/2 = (3n^2 - n)/2? Let's compute: sum of (n + i) = n*n + n(n-1)/2 = (2n^2 + n^2 - n)/2 = (3n^2 - n)/2. For n=100: (30000 - 100)/2 = 14950. Add newlines: n = 100, so total = 14950 + 100 = 15050
    assert(resBig.size() == static_cast<size_t>(15050));

    // Verify first line of big pattern: 99 spaces then '*'
    assert(resBig.substr(0, 100) == std::string(99, ' ') + "*");

    // Verify second line: 98 spaces, then "* *"
    assert(resBig.substr(101, 100) == std::string(98, ' ') + "* *");
}
