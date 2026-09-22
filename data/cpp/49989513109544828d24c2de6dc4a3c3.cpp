// Write a C++ function named `numberWordOrGreater` that takes a single positive integer `n` as input and returns a `std::string` containing the lowercase English word for numbers from 1 to 9 inclusive (e.g., "one", "two", ..., "nine"), and returns the exact string `"Greater than 9"` for any integer greater than 9. The function must not print anything; it must only return the appropriate string. The function should handle the case where `n` is 0 or negative gracefully by returning an empty string `""` (since the input is specified to be positive, but a defensive check is good practice). The function must use an `if` / `else if` chain rather than a lookup table or switch statement, as a direct translation of the original snippet's logic. The function signature must be `std::string numberWordOrGreater(int n);`. Ensure the function is `const`-correct and uses `const` where appropriate (e.g., no modifying parameters). Time complexity should be O(1) and space complexity O(1), ignoring the returned string.

#include <cassert>
#include <string>

std::string numberWordOrGreater(int n); // declare the solution function

int main() {
    // Test all single-digit positive numbers
    assert(numberWordOrGreater(1) == "one");
    assert(numberWordOrGreater(2) == "two");
    assert(numberWordOrGreater(3) == "three");
    assert(numberWordOrGreater(4) == "four");
    assert(numberWordOrGreater(5) == "five");
    assert(numberWordOrGreater(6) == "six");
    assert(numberWordOrGreater(7) == "seven");
    assert(numberWordOrGreater(8) == "eight");
    assert(numberWordOrGreater(9) == "nine");

    // Test values greater than 9
    assert(numberWordOrGreater(10) == "Greater than 9");
    assert(numberWordOrGreater(100) == "Greater than 9");
    assert(numberWordOrGreater(INT_MAX) == "Greater than 9");

    // Test non-positive defensive handling
    assert(numberWordOrGreater(0) == "");
    assert(numberWordOrGreater(-5) == "");

    return 0;
}

#include <string>

// Given a positive integer n, return the English word for 1-9, or "Greater than 9" for n > 9.
// For n <= 0, return an empty string (defensive handling).
std::string numberWordOrGreater(int n) {
    if (n <= 0) {
        return "";
    }
    if (n == 1) {
        return "one";
    } else if (n == 2) {
        return "two";
    } else if (n == 3) {
        return "three";
    } else if (n == 4) {
        return "four";
    } else if (n == 5) {
        return "five";
    } else if (n == 6) {
        return "six";
    } else if (n == 7) {
        return "seven";
    } else if (n == 8) {
        return "eight";
    } else if (n == 9) {
        return "nine";
    } else {
        return "Greater than 9";
    }
}

// The solution follows the exact branching logic from the original snippet: first check if `n > 0` to guard against non-positive inputs; if not positive, return an empty string. For positive `n`, use a chain of `else if` conditions to check each value from 1 to 9, each returning the corresponding word string literal. If none match (i.e., `n >= 10`), return `"Greater than 9"`. This is a simple O(1) algorithm with at most 10 comparisons, regardless of the magnitude of `n`. The edge cases include `n <= 0` returning an empty string, and `n` being exactly 9 (should return "nine") versus 10 (should return "Greater than 9"). Since the input is guaranteed positive by the problem statement, the defensive check for non-positive inputs is optional but included for robustness. The function uses no extra memory beyond the returned string literals (which are compile-time constants) and the return path.
