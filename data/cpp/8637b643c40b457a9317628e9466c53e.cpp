// Write a C++ function named `classifyEvenOdd` that takes a single integer argument and returns a `std::string` containing either `"Even"` or `"Odd"` based on whether the input number is divisible by 2. The function must handle negative numbers correctly (e.g., `-4` is even, `-3` is odd), and zero must be considered even. The returned string should be exactly `"Even"` or `"Odd"` with capitalization as shown. The function should be `const`-correct—it must not modify any external state, and the input parameter should be passed by value. Your implementation must not include a `main` function or any global variables; only the function definition with necessary headers.

// The solution is straightforward: test divisibility by 2 using the modulo operator (`%`). For any integer `n`, `n % 2` returns `0` if `n` is even, and `1` or `-1` for odd numbers (depending on sign). Since C++ treats any non-zero value as `true` in a boolean context, we can simply check `if (n % 2 == 0)` to return `"Even"`, otherwise `"Odd"`. This works correctly for negative numbers because modulo preserves the sign of the dividend, but the comparison with zero is unaffected. Zero itself yields `0 % 2 == 0`, so it is correctly classified as even. Edge cases: very large integers (beyond `int` range) are not handled since the parameter is `int`; but within the `int` domain, no overflow occurs in the modulo operation. Time complexity is O(1) constant time, and space complexity is O(1) auxiliary space (the returned string is a temporary, but no dynamic memory is used beyond that).

#include <string>

// Classify an integer as "Even" or "Odd".
// Returns "Even" for numbers divisible by 2, "Odd" otherwise.
// Handles negative numbers correctly: -4 -> "Even", -3 -> "Odd".
// Zero is even.
std::string classifyEvenOdd(const int number) {
    if (number % 2 == 0) {
        return "Even";
    }
    return "Odd";
}

#include <cassert>
#include <string>

// Declaration of the function under test (prototype).
std::string classifyEvenOdd(int number);

int main() {
    // Basic positive even and odd cases
    assert(classifyEvenOdd(4) == "Even");
    assert(classifyEvenOdd(7) == "Odd");
    // Zero is even
    assert(classifyEvenOdd(0) == "Even");
    // Negative numbers
    assert(classifyEvenOdd(-2) == "Even");
    assert(classifyEvenOdd(-5) == "Odd");
    // Large values within int range
    assert(classifyEvenOdd(1000000) == "Even");
    assert(classifyEvenOdd(999999) == "Odd");
    // Boundary values for int
    assert(classifyEvenOdd(2147483646) == "Even");
    assert(classifyEvenOdd(-2147483647) == "Odd");
    // Repeated call (const correctness)
    const int testNumber = 10;
    assert(classifyEvenOdd(testNumber) == "Even");
    return 0;
}
