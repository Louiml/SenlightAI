Write a C++ function that takes two integer arguments, `a` and `b`, and returns a `std::string` describing the relationship between them. The returned string must follow this exact format: if `a > b`, return `"<a> is greater than <b>"`; if `a < b`, return `"<b> is greater than <a>"`; if they are equal, return `"Both numbers are equal."`. Use the integer values directly in the string, without additional formatting. The function must be pure, meaning it has no side effects (no console I/O), and the result must be determined solely by the two input values. The function should handle any valid `int` values, including negative numbers, zero, and extremes like `INT_MAX` and `INT_MIN`.

// The solution is straightforward: compare the two integers using relational operators. Since the function must return a formatted string, we can construct it using `std::to_string` to convert each integer to its decimal representation, then concatenate with the appropriate literal text. There are three cases: strictly greater, strictly less, and equal. The comparison `a > b`, `a < b`, and the else branch for equality covers all possibilities without overlooking any value. Edge cases include large or negative numbers, which `std::to_string` handles correctly by producing a minus sign where needed. The time complexity is O(1) because we perform a constant number of arithmetic and string operations. The space complexity is O(1) for the returned string’s content length (which is bounded by the number of digits in the integers, at most ~11 chars for 32-bit int plus surrounding text, but this is constant for a fixed integer size). No special handling is required for `INT_MIN` because comparison works on the raw values, and `std::to_string(INT_MIN)` produces the correct textual representation.

#include <string>

// Returns a string describing the relationship between two integers.
// Format: "a is greater than b", "b is greater than a", or "Both numbers are equal."
std::string compareIntegers(int a, int b) {
    if (a > b) {
        return std::to_string(a) + " is greater than " + std::to_string(b);
    }
    if (a < b) {
        return std::to_string(b) + " is greater than " + std::to_string(a);
    }
    return "Both numbers are equal.";
}

#include <cassert>
#include <string>
#include <climits>

// Declaration of the solution function
std::string compareIntegers(int a, int b);

int main() {
    // Basic cases
    assert(compareIntegers(5, 3) == "5 is greater than 3");
    assert(compareIntegers(3, 5) == "5 is greater than 3");
    assert(compareIntegers(4, 4) == "Both numbers are equal.");

    // Negative numbers
    assert(compareIntegers(-2, -5) == "-2 is greater than -5");
    assert(compareIntegers(-7, -1) == "-1 is greater than -7");
    assert(compareIntegers(-3, -3) == "Both numbers are equal.");

    // Zero
    assert(compareIntegers(0, 10) == "10 is greater than 0");
    assert(compareIntegers(0, -1) == "0 is greater than -1");

    // Extremes
    assert(compareIntegers(INT_MAX, INT_MIN) == "2147483647 is greater than -2147483648");
    assert(compareIntegers(INT_MIN, INT_MAX) == "2147483647 is greater than -2147483648");
    assert(compareIntegers(INT_MAX, INT_MAX) == "Both numbers are equal.");

    return 0;
}
