Write a C++ function named `printHollowTriangle` that takes a single integer `size` (where `1 <= size <= 20`) and returns a `std::string` containing the pattern shown below, with each line separated by a newline character (`\n`). For `size = 5`, the exact output (including trailing spaces on each line) must be:
```
* * * * * * * * *
* * * *   * * * * 
* * *       * * *
* *           * *
*               *
```
Note that the first line has 9 stars (i.e., `2*size - 1` stars) separated by spaces. Each subsequent line removes exactly one star from the left side and one star from the right side, and replaces the removed stars' positions with two spaces (i.e., the gap grows by 2 spaces per line). The last line has exactly `size` stars (all on the left side) followed by `2*(size-1)` spaces and then nothing else (no trailing spaces after the final line). No leading spaces before the first star on any line. The function must validate the input: if `size < 1` or `size > 20`, return the string `"Invalid size!"` (without a newline). The function should be `const`-correct (take input by value, return `std::string` by value). Do not include any input/output statements in the function; it only builds and returns the string.
The core algorithm builds each of the `size` rows. For row index `i` (starting from 0), the number of stars on the left side is `size - i`, and the number of stars on the right side is also `size - i`. Between the left and right stars, there is a gap of `2*i` spaces. However, the pattern requires stars and spaces to be separated by a single space. For each star, after writing it we add a space except after the very last star on that row. To construct a row: first write `size - i` stars separated by spaces, then write `2*i` spaces, then if `i > 0` write `size - i` stars separated by spaces (the first right star should be preceded by a space? Actually from the given pattern, after the left part ends with a space and then the gap spaces, then immediately the right stars start with a star, but we must ensure separation). The easiest way is to build each row as a vector of tokens: first `size - i` tokens of `"*"`, then `2*i` tokens of `" "` (each space is a token), then `size - i` tokens of `"*"`. Then join all tokens with a single space. This automatically produces the correct spacing. For the last line, there are no right stars, so the row is just `size` stars separated by spaces. Edge cases: `size=1` returns a single star (no trailing spaces). The function must validate `size` and return `"Invalid size!"` otherwise. Time complexity is \(O(size^2)\) because each row has length proportional to `size`, and there are `size` rows. Space complexity is \(O(size^2)\) for the returned string, plus \(O(size)\) auxiliary for the token vector per row.
#include <string>
#include <vector>

// Returns the hollow triangle pattern for the given size.
// For size in [1,20], returns a multi-line string. For invalid size, returns "Invalid size!".
std::string printHollowTriangle(int size) {
    if (size < 1 || size > 20) {
        return "Invalid size!";
    }

    std::string result;
    for (int i = 0; i < size; ++i) {
        int stars = size - i;
        int gapSpaces = 2 * i;

        std::vector<std::string> tokens;
        for (int s = 0; s < stars; ++s) {
            tokens.push_back("*");
        }
        for (int g = 0; g < gapSpaces; ++g) {
            tokens.push_back(" ");
        }
        if (stars > 0 && i < size - 1) { // only add right stars if not last line
            for (int s = 0; s < stars; ++s) {
                tokens.push_back("*");
            }
        }

        // Join tokens with a single space
        for (size_t t = 0; t < tokens.size(); ++t) {
            if (t > 0) {
                result += ' ';
            }
            result += tokens[t];
        }

        if (i != size - 1) {
            result += '\n';
        }
    }

    return result;
}
#include <string>
#include <cassert>

// Declare the function (in the same translation unit)
std::string printHollowTriangle(int size);

int main() {
    // Test invalid size
    assert(printHollowTriangle(0) == "Invalid size!");
    assert(printHollowTriangle(21) == "Invalid size!");

    // Test size 1: single star, no newline
    assert(printHollowTriangle(1) == "*");

    // Test size 2: expected pattern
    assert(printHollowTriangle(2) == "* * *\n*   *");

    // Test size 3
    assert(printHollowTriangle(3) == "* * * * *\n* *   * *\n*       *");

    // Test size 5 (from problem statement)
    assert(printHollowTriangle(5) == 
        "* * * * * * * * *\n"
        "* * * *   * * * * \n"
        "* * *       * * *\n"
        "* *           * *\n"
        "*               *");

    // Test size 20 to ensure no crash and valid string length pattern
    std::string big = printHollowTriangle(20);
    assert(big.size() > 0);
    // Ensure last line has exactly 20 stars separated by spaces (i.e., 39 characters)
    assert(big.find_last_of('\n') == std::string::npos || 
           big.substr(big.find_last_of('\n')+1).size() == 39);
}
