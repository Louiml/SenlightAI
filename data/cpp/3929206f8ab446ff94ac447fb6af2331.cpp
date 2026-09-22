/*
Write a C++ function named `classifyNumber` that accepts an integer value and returns a `const char*` (or `std::string`) indicating whether the number is "Even" or "Odd". The function must handle negative numbers, zero, and very large integers (within the `int` range). Negate nothing—simply apply the modulo operation directly to any integer. The function should be `const`-correct, meaning it does not modify its input and is itself a pure function. No input/output is performed inside the function; it only returns the classification.
*/
#include <string>

// Classify an integer as "Even" or "Odd". Returns a std::string.
// Handles all int values, including negatives and zero.
std::string classifyNumber(int n) {
    return (n % 2 == 0) ? "Even" : "Odd";
}
#include <cassert>
#include <string>

// Declare the function under test (assumed to be defined elsewhere).
std::string classifyNumber(int n);

int main() {
    assert(classifyNumber(7) == "Odd");
    assert(classifyNumber(2) == "Even");
    assert(classifyNumber(0) == "Even");
    assert(classifyNumber(-3) == "Odd");
    assert(classifyNumber(-4) == "Even");
    assert(classifyNumber(2147483647) == "Odd");   // max int
    assert(classifyNumber(-2147483647) == "Odd");  // near min int
    assert(classifyNumber(-2147483648) == "Even"); // min int
    return 0;
}
// The core algorithm is trivial: check if `n % 2 == 0` to identify even numbers, otherwise classify as odd. Important edge cases include zero (which is even), negative numbers (e.g., -3 % 2 = -1 in C++, which is not zero, so it correctly maps to "Odd"; -4 % 2 = 0, correctly "Even"), and the largest/smallest `int` values. The modulo operator in C++ for negative dividends yields a negative remainder (or zero), but since we only compare against zero, this works correctly for all integers. Time complexity is O(1) and space complexity is O(1) — just a single comparison and a return of a string literal. To avoid copying, returning `const char*` is lightweight, but a `std::string` return is also acceptable. We'll use `std::string` for type safety and ease of testing in assertions.
