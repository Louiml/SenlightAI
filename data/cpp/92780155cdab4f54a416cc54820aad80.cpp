// Write a C++ function named `countAsterisksForInput` that reads a sequence of non-negative integers from standard input until the sentinel value `-1` is encountered. For each valid integer `x` read (including `0`), the function should compute and output the value `x - 1`. However, if the input value is exactly `0`, the function must output `0` instead (not `-1`). The output for each input value should be on its own line, formatted without any decimal places (i.e., as an integer). The function should handle arbitrary precision, meaning the input and output values may exceed typical 32-bit integer range. The function returns nothing (`void`), and it terminates only when `-1` is read. If `-1` appears as the first input, the function should produce no output.

The solution directly models the behavior described: repeatedly read a value from `std::cin` into a `long double` (to support large magnitudes and avoid integer overflow). For each value, check if it equals `-1`; if so, break out of the loop. Otherwise, if the value equals `0`, output `0`; else output `x - 1` with `fixed` and `setprecision(0)` to ensure integer-style rendering (no decimal point). Important edge cases: `0` must be treated specially to avoid printing `-1`; negative values other than `-1` are not expected but the subtraction still works. The sentinel `-1` must not be processed. Time complexity is \(O(n)\) where \(n\) is the number of input numbers read, and space complexity is \(O(1)\) because we only store the current value.

#include <iostream>
#include <iomanip>

// Reads numbers until -1 is encountered. For each input x:
// if x == 0, prints 0; otherwise prints x - 1 (no decimals).
void countAsterisksForInput() {
    long double x;
    while (std::cin >> x) {
        if (x == -1) {
            break;
        }

        if (x == 0) {
            std::cout << 0 << '\n';
        } else {
            std::cout << std::fixed << std::setprecision(0) << (x - 1) << '\n';
        }
    }
}

#include <cassert>
#include <sstream>
#include <iostream>
#include <iomanip>

// Forward declaration of the solution function (would normally be in the same file).
void countAsterisksForInput();

// Helper to capture output from the function given an input stream.
std::string runWithInput(const std::string& input) {
    std::istringstream in(input);
    std::streambuf* old_cin = std::cin.rdbuf(in.rdbuf());

    std::ostringstream out;
    std::streambuf* old_cout = std::cout.rdbuf(out.rdbuf());

    countAsterisksForInput();

    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);

    return out.str();
}

int main() {
    // Test cases: input string -> expected output string
    assert(runWithInput("5 -1") == "4\n");
    assert(runWithInput("0 0 -1") == "0\n0\n");
    assert(runWithInput("10 0 3 -1") == "9\n0\n2\n");
    assert(runWithInput("-1") == "");
    assert(runWithInput("1 2 3 4 -1") == "0\n1\n2\n3\n");
    assert(runWithInput("100000000000000000000 -1") == "99999999999999999999\n");
    assert(runWithInput("0 -1") == "0\n");
    assert(runWithInput("2 0 -1") == "1\n0\n");

    return 0;
}
