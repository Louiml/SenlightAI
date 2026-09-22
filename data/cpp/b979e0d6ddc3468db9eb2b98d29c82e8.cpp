Write a C++ function named `classifyInteger` that takes a single integer value as input and returns a `std::string` containing either `"even"` or `"odd"` based on whether the integer is even or odd. The function must handle negative numbers correctly, treat zero as even, and avoid any side effects such as printing to the console. The solution should be a pure function that can be reused in different programs, and it must not rely on any global state.

// The core algorithm uses the modulo operator `%` to determine the remainder when dividing the integer by 2. In C++, the result of `n % 2` is always either `0` or `1` for non‑negative numbers, but for negative numbers it can be `-1` (e.g., `-3 % 2` equals `-1`). Therefore, a direct comparison with `== 1` fails for negative odd numbers. The robust solution is to compare the remainder with `0`: if `n % 2 == 0`, the number is even, otherwise it is odd. This works for all integers, including zero (even), positive and negative odd numbers. An alternative is to use bitwise AND: `(n & 1) == 0` checks the least significant bit, but this is less clear for readability. The time complexity is O(1) constant, and the auxiliary space is O(1) since only a string is returned.

#include <string>

// Returns "even" if the input integer is even, otherwise "odd".
// Handles negative numbers correctly by checking remainder against zero.
std::string classifyInteger(const int value) {
    if (value % 2 == 0) {
        return "even";
    }
    return "odd";
}

#include <cassert>
#include <string>

// Declare the function being tested (already defined above in the solution).
std::string classifyInteger(const int value);

int main() {
    // Zero is even.
    assert(classifyInteger(0) == "even");
    // Positive even.
    assert(classifyInteger(4) == "even");
    // Positive odd.
    assert(classifyInteger(7) == "odd");
    // Negative even.
    assert(classifyInteger(-8) == "even");
    // Negative odd (catches the common modulo pitfall).
    assert(classifyInteger(-3) == "odd");
    // Large magnitudes.
    assert(classifyInteger(1000000) == "even");
    assert(classifyInteger(-1000001) == "odd");
    // Edge case for int minimum (typically -2147483648, even).
    assert(classifyInteger(-2147483647 - 1) == "even");
    // Edge case for int maximum (2147483647, odd).
    assert(classifyInteger(2147483647) == "odd");
    return 0;
}
