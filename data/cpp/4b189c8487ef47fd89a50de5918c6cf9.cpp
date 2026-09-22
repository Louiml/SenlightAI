/*
Write a C++ function `classifyNumber(int value)` that takes a single integer and returns a `std::string` containing exactly one of `"Positive"`, `"Zero"`, or `"Negative"` based on the sign of the number. The function must not print to the console; instead it must return the classification as a string so it can be used in other contexts (e.g., testing, further processing). The input integer can be any valid `int` value, including `INT_MIN`, `INT_MAX`, and zero. The output string must match exactly: `"Positive"` for values greater than zero, `"Zero"` for zero, and `"Negative"` for values less than zero. Ensure the function is `const`-correct and uses appropriate standard library utilities.
*/
#include <string>

// Return "Positive", "Zero", or "Negative" based on the sign of the input value.
std::string classifyNumber(int value) {
    if (value > 0) {
        return "Positive";
    } else if (value == 0) {
        return "Zero";
    } else {
        return "Negative";
    }
}
#include <cassert>
#include <string>
#include <climits>

// The solution function is declared above the test main.
std::string classifyNumber(int value);

int main() {
    assert(classifyNumber(5) == "Positive");
    assert(classifyNumber(0) == "Zero");
    assert(classifyNumber(-7) == "Negative");
    assert(classifyNumber(INT_MAX) == "Positive");
    assert(classifyNumber(INT_MIN) == "Negative");
    assert(classifyNumber(1) == "Positive");
    assert(classifyNumber(-1) == "Negative");
    return 0;
}
// The solution is straightforward: use an `if`-`else if`-`else` chain to check the sign of the integer. The primary algorithm is a constant-time comparison: if `value > 0`, return `"Positive"`; else if `value == 0`, return `"Zero"`; else return `"Negative"`. Edge cases include the boundary value `0` (handled by the equality check) and extreme values like `INT_MIN` and `INT_MAX` — the comparisons work correctly because they are simple integer comparisons with no overflow risk. Time complexity is O(1) and space complexity is O(1) (only the returned string is allocated, which is trivial). There are no loops or recursion, so the function is trivially correct for all valid integer inputs.
