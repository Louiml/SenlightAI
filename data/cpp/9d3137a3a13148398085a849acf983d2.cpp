/*
Write a C++ function `fahrenheitToCelsiusTable(int start, int end, int step)` that takes three integer parameters representing a start Fahrenheit value, an end Fahrenheit value, and a positive step size. The function must generate and return a string containing the Fahrenheit-to-Celsius conversion table for all Fahrenheit values from `start` to `end` (inclusive), incrementing by `step`. Each line of the table must be formatted as `F C` (Fahrenheit value, a single space, then the corresponding Celsius value) followed by a newline character. The Celsius conversion must use integer arithmetic with the formula `(F - 32) * 5 / 9` (truncated toward zero, as in C++ integer division). The function should handle the case where `start > end` by returning an empty string, and it must not rely on any global variables or input/output operations. The output string must exactly match the expected format with no extra spaces or trailing characters except the final newline.
*/

#include <string>

// Build a Fahrenheit-to-Celsius conversion table as a formatted string.
// start, end: Fahrenheit range (inclusive); step: positive increment.
// Returns lines "F C\n" for each F from start to end by step.
std::string fahrenheitToCelsiusTable(int start, int end, int step) {
    std::string result;
    for (int f = start; f <= end; f += step) {
        const int c = ((f - 32) * 5) / 9;  // integer division truncates
        result += std::to_string(f) + " " + std::to_string(c) + "\n";
    }
    return result;
}

#include <cassert>

int main() {
    // Basic range
    assert(fahrenheitToCelsiusTable(32, 50, 10) == "32 0\n42 5\n50 10\n");
    // Negative Fahrenheit values (truncation toward zero)
    assert(fahrenheitToCelsiusTable(-40, -20, 10) == "-40 -40\n-30 -34\n-20 -28\n");
    // Start equals end
    assert(fahrenheitToCelsiusTable(100, 100, 1) == "100 37\n");
    // Start greater than end -> empty string
    assert(fahrenheitToCelsiusTable(50, 40, 5) == "");
    // Larger step skips values; end not exactly hit
    assert(fahrenheitToCelsiusTable(0, 100, 30) == "0 -17\n30 -1\n60 15\n90 32\n");
    // Step of 1 over a small range
    assert(fahrenheitToCelsiusTable(32, 35, 1) == "32 0\n33 0\n34 1\n35 1\n");
    // Negative start, positive end
    assert(fahrenheitToCelsiusTable(-10, 20, 15) == "-10 -23\n5 -15\n20 -6\n");
    // Larger numbers
    assert(fahrenheitToCelsiusTable(212, 212, 1) == "212 100\n");
    // Step larger than range, start<=end
    assert(fahrenheitToCelsiusTable(10, 20, 100) == "10 -12\n");
    return 0;
}

// The solution iterates from the starting Fahrenheit value `f` upward to the ending value, incrementing by the given step each iteration. For each `f`, we compute `c = ((f - 32) * 5) / 9` using integer arithmetic, which truncates any fractional part toward zero (this matches the original snippet's behavior). We append the formatted line to a `std::string` using `std::to_string` for both numbers, separating them by a space and following with a newline. The loop continues while `f <= end`. Edge cases: if `start > end`, the loop never executes and we return an empty string; if `step` is zero or negative, the loop would be infinite, but the task specification states the step is positive, so we can assume that. For a table with `n` entries where `n = max(0, floor((end - start) / step) + 1)`, each iteration does constant work (arithmetic, string concatenation), so the time complexity is `O(n)`. The space complexity is `O(total length of output)` because we build and return the full string; the auxiliary space besides the output string is `O(1)`.
