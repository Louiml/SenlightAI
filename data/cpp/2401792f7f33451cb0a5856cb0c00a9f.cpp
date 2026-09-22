// Write a C++ function named `generateLetterTriangle` that takes an integer `n` (where `1 <= n <= 26`) and returns a `std::string` containing a right-angled triangle pattern of uppercase letters. For each row `i` from 1 to `n`, the row contains exactly `i` repetitions of the `i`-th letter of the alphabet (i.e., 'A' for row 1, 'B' for row 2, …, 'Z' for row 26). Each row is terminated by a newline character `\n`. The function must handle the full valid range of input, and you may assume the input is always within the specified range. The returned string should have exactly `n` lines, with no extra spaces or trailing characters other than the newline at the end of each line.
// The solution directly maps the problem into nested loops: an outer loop iterates from `i = 1` to `n` for each row, and an inner loop iterates from `j = 1` to `i` to print the required number of characters. The character for row `i` is obtained by computing `'A' + (i - 1)` (since `'A'` corresponds to the integer 65 in ASCII, and `i` is 1-based). The code builds a `std::string` result by appending the character `i` times using `std::string(count, ch)` and then appends a newline `\n`. Edge cases: the minimum input `n = 1` produces a single line with one 'A'; the maximum `n = 26` produces 26 lines, the last having 26 'Z's. No special handling is needed for invalid inputs because the problem guarantees valid input. Time complexity is \(O(n^2)\) because the total number of characters printed is the sum of 1 through `n`, which equals \(n(n+1)/2\). Space complexity is \(O(n^2)\) for the returned string (or \(O(1)\) if we were printing directly, but we must return a string).
#include <string>

// Returns a right-angled triangle pattern of uppercase letters for rows 1..n.
// Row i contains i copies of the i-th letter of the alphabet, followed by '\n'.
std::string generateLetterTriangle(int n) {
    std::string result;
    result.reserve(static_cast<size_t>(n * (n + 1) / 2 + n)); // total chars + newlines

    for (int i = 1; i <= n; ++i) {
        char currentChar = static_cast<char>('A' + (i - 1)); // 'A' for i=1, 'B' for i=2, ...
        result.append(static_cast<size_t>(i), currentChar);  // i copies
        result.push_back('\n');
    }

    return result;
}
#include <cassert>
#include <string>

// Declaration of the function under test
std::string generateLetterTriangle(int n);

int main() {
    // Minimum case: n = 1
    assert(generateLetterTriangle(1) == "A\n");

    // n = 2
    assert(generateLetterTriangle(2) == "A\nBB\n");

    // n = 3
    assert(generateLetterTriangle(3) == "A\nBB\nCCC\n");

    // n = 4
    assert(generateLetterTriangle(4) == "A\nBB\nCCC\nDDDD\n");

    // n = 5
    assert(generateLetterTriangle(5) == "A\nBB\nCCC\nDDDD\nEEEEE\n");

    // Check that each row has correct number of characters (spot check with size)
    std::string result26 = generateLetterTriangle(26);
    // For n=26: total characters = sum(1..26) + 26 newlines = 351 + 26 = 377
    assert(result26.size() == 377);
    // Last line should be 26 'Z's followed by newline
    assert(result26.substr(result26.size() - 27) == std::string(26, 'Z') + "\n");

    // Verify first character of each row increments correctly for n=6
    std::string result6 = generateLetterTriangle(6);
    // Expected rows: A, BB, CCC, DDDD, EEEEE, FFFFFF
    assert(result6.find("A\n") == 0);
    assert(result6.find("FF\n") != std::string::npos);
    assert(result6.find("ZZZ\n") == std::string::npos); // should not appear

    // Ensure no extra characters like spaces
    for (char c : generateLetterTriangle(3)) {
        if (c != '\n') {
            assert(c >= 'A' && c <= 'Z');
        }
    }

    return 0;
}
