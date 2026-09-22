Write a C++ function `std::string patternRows(int n)` that, given a positive integer `n` (with `n` between 2 and 9 for simplicity, though it works for any positive int), returns a multi-line string containing `n` rows. For each row `i` (1-indexed), the output should consist of exactly `i-1` leading spaces followed by the digit `i` repeated `(n - i + 1)` times, then a newline character. The rows are printed in increasing order of `i` from 1 to `n`. For example, for `n=4`, the returned string (including trailing newline after the last row) should be:
```
1111
 222
  33
   4
```
(where each row ends with a newline). The function must build and return the complete string, not print to the console. The input is guaranteed to be a positive integer.

#include <cassert>
#include <string>

// The solution function is defined above (declared here for test).
std::string patternRows(int n);

int main() {
    // n = 1: single row "1\n"
    assert(patternRows(1) == "1\n");

    // n = 2: "11\n 2\n" (note second row has one leading space)
    assert(patternRows(2) == "11\n 2\n");

    // n = 3: "111\n 22\n  3\n"
    assert(patternRows(3) == "111\n 22\n  3\n");

    // n = 4: from the snippet
    assert(patternRows(4) == "1111\n 222\n  33\n   4\n");

    // n = 5: check first, middle, and last rows
    std::string five = patternRows(5);
    assert(five == "11111\n 2222\n  333\n   44\n    5\n");

    // n = 6: ensures pattern extends correctly
    assert(patternRows(6) == "111111\n 22222\n  3333\n   444\n    55\n     6\n");

    // n = 7: slightly larger
    assert(patternRows(7) == "1111111\n 222222\n  33333\n   4444\n    555\n     66\n      7\n");

    // n = 8: check length of string (sum of 1..n *2 + n)
    std::string eight = patternRows(8);
    assert(eight.length() == 8 * 9 + 8); // spaces+digits: 2*(1+...+8)=72, newlines=8 -> 80

    // n = 9: last row has 8 spaces + '9' + newline
    std::string nine = patternRows(9);
    assert(nine.substr(nine.length() - 3) == "        9\n");

    return 0;
}

#include <string>

// Return a multi-line string where for each row i (1..n),
// there are (i-1) leading spaces and the digit i repeated (n-i+1) times.
std::string patternRows(int n) {
    std::string result;
    result.reserve(n * (n + 1)); // upper bound approximation

    for (int i = 1; i <= n; ++i) {
        // Append (i-1) spaces
        result.append(i - 1, ' ');

        // Append digit i repeated (n - i + 1) times
        // Use char conversion since i is 1..9 in typical usage.
        const char digit = static_cast<char>('0' + i);
        result.append(n - i + 1, digit);

        // Newline after each row
        result.push_back('\n');
    }

    return result;
}

// The problem is a direct generalization of the nested-loop pattern in the snippet. The main algorithm iterates `i` from 1 to `n`. For each `i`, it appends `(i-1)` spaces to a `std::string` result, then appends the character `char('0' + i)` exactly `(n - i + 1)` times, and finally appends a newline character. Edge cases: for the minimal input `n=1`, the output is a single row with 0 spaces and the character `'1'` once, followed by a newline. The digit `i` is a single decimal digit because we restrict `n` to 2–9 for readability, but the code uses `std::to_string` to be safe for larger `n`, though that would break the pattern for multi-digit numbers. Time complexity: the total number of characters appended is `(n + (n-1) + ... + 1)` spaces plus the same number of digits plus `n` newlines, i.e., `O(n^2)` time. Auxiliary space: the result string itself is `O(n^2)`, and we only use a few loop variables.
