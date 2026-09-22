/*
Write a C++ function `std::string printHourglass(int totalSymbols, char symbol)` that, given a positive integer `totalSymbols` (1 ≤ totalSymbols ≤ 1000) and a printable character `symbol`, produces a string representing the largest possible hourglass shape made entirely of that character, using at most `totalSymbols` copies of the character. The hourglass is built from concentric lines: the top row has an odd number of characters (the maximum width), each subsequent row decreases by 2 characters until reaching 1 (or if the maximum width is 1, only that single row), then increases back by 2 until the maximum width is reached again. All rows are centered with spaces on the left (no trailing spaces). After the shape, the output string must end with a newline and then the number of unused characters (i.e., `totalSymbols` minus the number actually used). If `totalSymbols` is less than 1, return an empty string. The function must not print anything; it returns the full formatted string.
*/

#include <string>
#include <cmath>
#include <algorithm>

// Build the largest hourglass shape using at most totalSymbols copies of symbol.
// Returns a string with the shape followed by a newline and the unused count.
std::string printHourglass(int totalSymbols, char symbol) {
    if (totalSymbols < 1) {
        return "";
    }

    // Find largest k such that 2*k^2 - 1 <= totalSymbols
    int k = 1;
    while (2 * k * k - 1 <= totalSymbols) {
        ++k;
    }
    --k;  // last valid k

    int used = 2 * k * k - 1;
    int remainder = totalSymbols - used;
    int maxWidth = 2 * k - 1;

    std::string result;
    result.reserve(used + maxWidth * maxWidth / 2 + 10);  // rough estimate

    // Top half: widths maxWidth, maxWidth-2, ..., 1
    for (int width = maxWidth; width >= 1; width -= 2) {
        int spaces = (maxWidth - width) / 2;
        result.append(spaces, ' ');
        result.append(width, symbol);
        result.push_back('\n');
    }

    // Bottom half: widths 3, 5, ..., maxWidth (if maxWidth > 1)
    if (maxWidth > 1) {
        for (int width = 3; width <= maxWidth; width += 2) {
            int spaces = (maxWidth - width) / 2;
            result.append(spaces, ' ');
            result.append(width, symbol);
            result.push_back('\n');
        }
    }

    // Append the remainder count
    result += std::to_string(remainder);

    return result;
}

#include <cassert>
#include <string>

// Forward declaration of the function under test (in real usage, include the header)
std::string printHourglass(int totalSymbols, char symbol);

int main() {
    // Basic case: totalSymbols = 7 gives a 3-wide hourglass (uses 7, remainder 0)
    // shape: " * \n***\n * \n0" with spaces? Actually maxWidth=3: rows: 1,3,1? wait k=2 => W=3, used=7
    // Top: width 3 -> "***", width 1 -> " * ", bottom: width 3 -> "***" (but we skip duplicated middle)
    // Our code: top: width 3, width 1; bottom: width 3 (since maxWidth>1). That yields 4 lines total? Actually top has 2 rows, bottom has 1 row (width 3). Total rows = 3. Correct.
    std::string out7 = printHourglass(7, '*');
    // Expected: "***\n * \n***\n0"
    assert(out7 == "***\n * \n***\n0");

    // totalSymbols = 1 => single symbol, remainder 0
    assert(printHourglass(1, '#') == "#\n0");

    // totalSymbols = 0 => empty string
    assert(printHourglass(0, 'x') == "");

    // totalSymbols = 18: largest k? k=3 => 2*9-1=17 used, remainder 1, maxWidth=5
    std::string out18 = printHourglass(18, 'a');
    // Expected shape: top widths 5,3,1; bottom widths 3,5; then newline and "1"
    std::string expected18 = "aaaaa\n aaa \n  a  \n aaa \naaaaa\n1";
    assert(out18 == expected18);

    // totalSymbols = 2: largest k=1 => used=1, remainder=1, maxWidth=1
    assert(printHourglass(2, 'b') == "b\n1");

    // Large input: ensure no crash and remainder is non-negative
    std::string big = printHourglass(1000, 'z');
    assert(!big.empty());
    assert(big.back() != '\n');  // ends with remainder digit

    // Check that the used symbols count plus remainder equals totalSymbols
    // We can't directly parse, but verify the total characters (excluding newlines and spaces) equals used.
    // This is just a sanity check for one case.
    std::string test17 = printHourglass(17, 'o');
    // For k=3, used=17, remainder=0, shape is same as out18 without remainder
    assert(test17 == "ooooo\n ooo \n  o  \n ooo \nooooo\n0");

    // Verify that for totalSymbols=3, k=1 (since 2*1^2-1=1 <=3, k=2 gives 7>3), so maxWidth=1, used=1, remainder=2
    assert(printHourglass(3, 'p') == "p\n2");

    // Verify that for totalSymbols=5, k=1? 2*1-1=1 <=5, k=2 gives 7>5 so used=1, remainder=4
    assert(printHourglass(5, 'q') == "q\n4");

    // Verify totalSymbols=6: k=1 still, remainder=5
    assert(printHourglass(6, 'r') == "r\n5");

    // Verify totalSymbols=7: k=2 as before
    assert(printHourglass(7, 's') == "sss\n s \nsss\n0");

    return 0;
}

// The core is to determine the largest odd maximum width `W` such that the total number of symbols in the full hourglass (top half + bottom half minus the duplicated middle row) does not exceed `totalSymbols`. For a maximum width `W = 2k - 1` (where `k` is the number of rows in the top half including the middle), the total symbols used is `2k^2 - 1`. We need the largest integer `k` with `2k^2 - 1 <= totalSymbols`. This can be found by iterating `k` from 1 upward until the condition fails, then taking the last valid `k`. Alternatively, solve `k = floor(sqrt((totalSymbols+1)/2))` and adjust if necessary. Then `W = 2k - 1`. The actual used symbols is `2k^2 - 1`, and the remainder is `totalSymbols - (2k^2 - 1)`. Build the shape: for the top half, rows go from width `W` down to 1 (step -2), each row prefixed by `(W - width)/2` spaces. The bottom half (if `W > 1`) goes from width 3 up to `W` (step +2), with the same left padding. Append a newline after the shape, then the remainder as an integer (without extra spaces). Edge case: when `totalSymbols` is 1, `k=1`, `W=1`, the shape is a single symbol, remainder 0. For `totalSymbols` less than 1, return empty string. Time complexity O(W) for building the string, which is O(sqrt(totalSymbols)) because W ≈ 2*sqrt(totalSymbols/2). Space complexity O(W * average width) for the output string, which is O(totalSymbols) in the worst case because the number of symbols used is at most totalSymbols.
