/*
Write a C++ function named `generateMultiplicationTable` that takes an integer parameter `n` (where `1 <= n <= 9`) and returns a `std::string` containing the multiplication table for numbers from 1 to `n` for both operands (i.e., `i` from 1 to `n` and `j` from 1 to `n`), formatted exactly as `"i x j = product\n"` for each pair, in row-major order (i.e., increment `i` in outer loop, `j` in inner loop). Each line should be terminated with a newline character `'\n'`. The function should handle invalid input gracefully by throwing an `std::invalid_argument` exception if `n` is outside the valid range.
*/

#include <string>
#include <stdexcept>

// Returns a multiplication table for numbers 1..n formatted as "i x j = product\n".
// Throws std::invalid_argument if n is not in the range [1, 9].
std::string generateMultiplicationTable(int n) {
    if (n < 1 || n > 9) {
        throw std::invalid_argument("n must be between 1 and 9");
    }

    std::string result;
    result.reserve(static_cast<size_t>(n) * n * 8); // rough preallocation

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            result += std::to_string(i) + "x" + std::to_string(j) + "=" + std::to_string(i * j) + "\n";
        }
    }

    return result;
}

#include <cassert>
#include <string>

int main() {
    // Basic table for n=1
    assert(generateMultiplicationTable(1) == "1x1=1\n");

    // Table for n=2
    std::string expected2 = "1x1=1\n1x2=2\n2x1=2\n2x2=4\n";
    assert(generateMultiplicationTable(2) == expected2);

    // Table for n=9 includes all 81 lines; check first and last lines
    std::string table9 = generateMultiplicationTable(9);
    assert(table9.size() > 0);
    assert(table9.substr(0, 8) == "1x1=1\n");        // first line
    assert(table9.substr(table9.size() - 8) == "9x9=81\n"); // last line

    // Verify total number of lines for n=3 (should be 9 lines, each ending with \n)
    std::string table3 = generateMultiplicationTable(3);
    int newline_count = 0;
    for (char c : table3) {
        if (c == '\n') ++newline_count;
    }
    assert(newline_count == 9);

    // Check specific line content for n=4
    std::string table4 = generateMultiplicationTable(4);
    assert(table4.find("3x4=12\n") != std::string::npos);

    // Invalid inputs throw exceptions
    bool threw = false;
    try { generateMultiplicationTable(0); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);
    threw = false;
    try { generateMultiplicationTable(10); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);
}

// The solution uses two nested loops: outer loop iterates `i` from 1 to `n`, inner loop iterates `j` from 1 to `n`. For each pair, a line is appended to an `std::string` using `std::to_string` for numbers and concatenation with the character `'x'`, `'='`, and the product `i*j`, followed by `'\n'`. The function validates `n` at the beginning and throws an exception if `n < 1` or `n > 9`. The time complexity is \(O(n^2)\) because there are exactly \(n \times n\) lines generated. The space complexity is \(O(n^2)\) as well because the output string stores all lines. Edge cases include `n=1` (only one line) and invalid values like 0 or 10, which should throw.
