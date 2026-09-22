Write a C++ function named `printIJSequence` that takes no parameters and returns a `std::string` containing the exact output produced by the given nested loop snippet. The snippet iterates a double `i` from 0.0 to 2.0 inclusive in steps of 0.2, and for each `i`, iterates a double `j` from `i+1` to `i+3` inclusive in steps of 1.0 (since the inner loop uses `j++`). For every `j`, it appends a line `"I=<i> J=<j>"` (where `<i>` and `<j>` are the current values printed by `cout` default formatting) followed by a newline to the result string. The function must replicate the exact formatting, including the default floating-point output precision (6 significant digits) and the fact that due to floating-point representation, values like `0.2`, `0.4`, etc., may appear as `0.2`, `0.4`, etc., but some iterations may produce values like `0.600000` or `1.2` depending on compiler/library. However, the accepted solution must match the logical output exactly as the original code would produce with standard `cout` default formatting. The function should be deterministic and not depend on external input.
// The solution must mimic the original nested loop exactly. The outer loop runs `double i = 0; i <= 2; i += 0.2`. Due to floating-point accumulation, `i` will take values approximately 0, 0.2, 0.4, 0.6000000000000001, 0.8, 1.0, 1.2, 1.4, 1.5999999999999999, 1.8, 1.9999999999999998. The condition `i <= 2` is true for all these because the last value is slightly less than 2.0, so the loop runs 11 times. The inner loop initializes `j = i + 1` and runs while `j <= i + 3`, with `j++` (increment by 1.0). Since `i + 3` is exactly 3 more than `i`, the inner loop runs exactly 3 times for each `i` (j = i+1, i+2, i+3). The output lines are produced by `cout` with default precision (6 significant digits), which prints numbers like `0`, `0.2`, `0.4`, `0.6`, `0.8`, `1`, `1.2`, `1.4`, `1.6`, `1.8`, `2` for `i` (after rounding), and for `j` it prints `1`, `2`, `3`, `1.2`, `2.2`, `3.2`, etc. To replicate exactly, the solution can use a `std::ostringstream` with default formatting, and set the precision to 6 (the default for `cout` is actually 6 significant digits, not fixed), and use `std::defaultfloat`. The main edge case is that floating-point equality (`i <= 2`) might behave differently if the step is not exact; but for 0.2, the loop runs as described. The time complexity is O(1) because the number of iterations is fixed (11 * 3 = 33 lines regardless of input), and space complexity is O(1) auxiliary storage (the result string grows to a constant size).
#include <string>
#include <sstream>
#include <iomanip>

/**
 * Reproduces the exact output of the given nested loop snippet.
 * Returns a string with each line "I=<i> J=<j>" separated by a newline.
 */
std::string printIJSequence() {
    std::ostringstream oss;
    // Use default formatting (same as std::cout default: 6 significant digits)
    oss << std::defaultfloat << std::setprecision(6);

    for (double i = 0; i <= 2; i += 0.2) {
        for (double j = i + 1; j <= (i + 3); j++) {
            oss << "I=" << i << " J=" << j << "\n";
        }
    }
    return oss.str();
}
#include <cassert>
#include <string>

// Declaration of the function under test
std::string printIJSequence();

int main() {
    // Get the full output
    std::string output = printIJSequence();

    // Count the number of lines (should be 11 * 3 = 33)
    int lineCount = 0;
    size_t pos = 0;
    while ((pos = output.find('\n', pos)) != std::string::npos) {
        lineCount++;
        pos++;
    }
    assert(lineCount == 33);

    // Check the first line (i=0, j=1)
    assert(output.find("I=0 J=1\n") == 0);

    // Check a middle line (i=0.4, j=2.4 appears as "I=0.4 J=1.4" first, then "I=0.4 J=2.4")
    // Verify that the sequence contains expected substrings
    assert(output.find("I=0.4 J=1.4") != std::string::npos);
    assert(output.find("I=0.4 J=2.4") != std::string::npos);
    assert(output.find("I=0.4 J=3.4") != std::string::npos);

    // Check the last line (i should be ~1.9999999999999998, printed as "2" by default format, j=5)
    // The last line should be "I=2 J=5\n" (since i prints as "2" and j as "5")
    size_t lastNewline = output.rfind('\n');
    assert(lastNewline != std::string::npos);
    std::string lastLine = output.substr(0, lastNewline); // remove trailing newline? Actually output ends with newline, so last line is before final newline
    // Better: find the position after the last newline
    size_t lastLineStart = output.rfind('\n', output.size() - 2); // find second last newline
    if (lastLineStart == std::string::npos) lastLineStart = 0; else lastLineStart++;
    std::string lastLine = output.substr(lastLineStart);
    assert(lastLine == "I=2 J=5\n");

    // Also verify the second line (i=0, j=2)
    assert(output.find("I=0 J=2\n") != std::string::npos);

    return 0;
}
