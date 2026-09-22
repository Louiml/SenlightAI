Write a C++ function `generateNumberTriangle` that takes a positive integer `n` as input and returns a `std::string` containing a triangular number pattern. The pattern consists of `n` lines, where line `i` (1-indexed) contains `i` numbers. The numbers are consecutive integers starting from 1, and between consecutive numbers within the same line, place a single asterisk `*`. Numbers on the same line are not separated by spaces, and each line ends with a newline character (`\n`). For example, for `n = 3`, the output string should be `"1\n2*3\n4*5*6\n"`. The function must handle edge cases: if `n <= 0`, return an empty string. Ensure the function is efficient and uses appropriate `const` correctness where applicable.
The solution generates the pattern line by line. Maintain a counter `current` starting at 1. For each line `i` from 1 to `n`, append numbers to a line string: for each position `j` from 1 to `i`, if `j != 1`, append a `*` first, then append the current counter value as a decimal string, and increment the counter. After completing the line, append a newline character. If `n <= 0`, return an empty string. The main algorithm is straightforward nested loops: outer loop over lines (runs `n` times), inner loop over numbers per line (total numbers across all lines is `n*(n+1)/2`). Time complexity is O(n²) due to the total number of characters appended (each number requires at least one character plus the asterisks). Auxiliary space is O(n²) for the output string itself. Edge cases: `n=1` produces `"1\n"`; `n=0` or negative returns `""`. No special handling for large numbers except using `std::to_string` for conversion.
#include <string>
#include <sstream>

std::string generateNumberTriangle(int n) {
    if (n <= 0) return "";
    std::ostringstream output;
    int current = 1;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= i; ++j) {
            if (j != 1) output << '*';
            output << current++;
        }
        output << '\n';
    }
    return output.str();
}
#include <cassert>
#include <string>

// The solution function is assumed to be declared above.
int main() {
    assert(generateNumberTriangle(0) == "");
    assert(generateNumberTriangle(-3) == "");
    assert(generateNumberTriangle(1) == "1\n");
    assert(generateNumberTriangle(2) == "1\n2*3\n");
    assert(generateNumberTriangle(3) == "1\n2*3\n4*5*6\n");
    assert(generateNumberTriangle(4) == "1\n2*3\n4*5*6\n7*8*9*10\n");
    // Check a larger case quickly via substring properties
    std::string result = generateNumberTriangle(5);
    assert(result.length() == 5 + 1 + 10); // 5 lines + 1 newline each, 10 numbers (at least one digit each) plus asterisks
    // Actually more robust: count lines and total numbers
    int newlines = 0;
    for (char c : result) if (c == '\n') newlines++;
    assert(newlines == 5);
    assert(result.back() == '\n');
    return 0;
}
