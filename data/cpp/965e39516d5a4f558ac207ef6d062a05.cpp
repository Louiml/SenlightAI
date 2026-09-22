Write a C++ function named `classifyInterval` that takes a single `double` value and returns a `std::string` describing which interval it belongs to, based on the following rules: if the value is between 0 and 25 inclusive, return `"Intervalo [0,25]"`; if it is greater than 25 and up to 50 inclusive, return `"Intervalo (25,50]"`; if greater than 50 and up to 75 inclusive, return `"Intervalo (50,75]"`; if greater than 75 and up to 100 inclusive, return `"Intervalo (75,100]"`; and if the value is less than 0 or greater than 100, return `"Fora de intervalo"`. The function must handle negative values, zero, and values exactly at boundaries (e.g., 25, 50, 75, 100) correctly, and must not rely on global variables or user input inside the function itself.
// The solution uses a simple chain of conditional checks that mirrors the original snippet but returns a string instead of printing. The value `a` is compared against interval endpoints in increasing order: first check if `a` is in `[0,25]` (inclusive both ends), then `(25,50]`, then `(50,75]`, then `(75,100]`, and finally if none match (i.e., `a < 0` or `a > 100`), return `"Fora de intervalo"`. Because the ranges are disjoint and cover all real numbers, exactly one condition will be true, so no ambiguity exists. Edge cases include `a = 0` (first interval), `a = 25` (first interval, not second), `a = 25.0001` (second), and `a = 100` (fourth), plus negative values and values above 100. Since the input is a single double, the algorithm runs in constant time \(O(1)\) and uses constant extra space \(O(1)\). The only subtlety is that floating-point comparisons with `>` and `<=` are used exactly as in the original, which is acceptable for this task because the boundaries are representable exactly in binary floating point.
#include <string>

// Classify a double value into one of five intervals and return the label.
// The intervals are: [0,25], (25,50], (50,75], (75,100], or out-of-range.
std::string classifyInterval(double value) {
    if (value >= 0.0 && value <= 25.0) {
        return "Intervalo [0,25]";
    }
    if (value > 25.0 && value <= 50.0) {
        return "Intervalo (25,50]";
    }
    if (value > 50.0 && value <= 75.0) {
        return "Intervalo (50,75]";
    }
    if (value > 75.0 && value <= 100.0) {
        return "Intervalo (75,100]";
    }
    // value < 0.0 or value > 100.0
    return "Fora de intervalo";
}
#include <cassert>
#include <string>

// Solution function declaration (must match the provided implementation)
std::string classifyInterval(double value);

int main() {
    // Boundary and edge cases
    assert(classifyInterval(0.0) == "Intervalo [0,25]");
    assert(classifyInterval(25.0) == "Intervalo [0,25]");
    assert(classifyInterval(25.1) == "Intervalo (25,50]");
    assert(classifyInterval(50.0) == "Intervalo (25,50]");
    assert(classifyInterval(50.0001) == "Intervalo (50,75]");
    assert(classifyInterval(75.0) == "Intervalo (50,75]");
    assert(classifyInterval(75.5) == "Intervalo (75,100]");
    assert(classifyInterval(100.0) == "Intervalo (75,100]");
    assert(classifyInterval(-0.1) == "Fora de intervalo");
    assert(classifyInterval(100.1) == "Fora de intervalo");
}
