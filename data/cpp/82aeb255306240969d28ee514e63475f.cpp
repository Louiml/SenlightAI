Write a C++ function named `classifyEvenOdd` that takes a single integer parameter and returns a `std::string` containing either `"Even"` or `"Odd"` (without quotes, with exact capitalization). The function must handle all possible `int` values including negative numbers, zero, and both minimum (`INT_MIN`) and maximum (`INT_MAX`) representable integers. The returned string should exactly match `"Even"` for even numbers and `"Odd"` for odd numbers. The function must not produce any console output; it should purely return the classification as a string.
#include <cassert>
#include <climits>
#include <string>

// Assume the solution function is declared above, or include the header here.
std::string classifyEvenOdd(int number); // declaration for test

int main() {
    // Basic positive cases
    assert(classifyEvenOdd(0) == "Even");
    assert(classifyEvenOdd(2) == "Even");
    assert(classifyEvenOdd(4) == "Even");
    assert(classifyEvenOdd(7) == "Odd");
    assert(classifyEvenOdd(1) == "Odd");

    // Negative numbers
    assert(classifyEvenOdd(-2) == "Even");
    assert(classifyEvenOdd(-3) == "Odd");

    // Extreme values
    assert(classifyEvenOdd(INT_MAX) == "Odd");
    assert(classifyEvenOdd(INT_MIN) == "Even");

    // Additional random checks
    assert(classifyEvenOdd(100) == "Even");
    assert(classifyEvenOdd(-101) == "Odd");

    return 0;
}
#include <string>

// Classify an integer as even or odd and return the result as a string.
std::string classifyEvenOdd(int number) {
    if (number % 2 == 0) {
        return "Even";
    } else {
        return "Odd";
    }
}
// The core algorithm is straightforward: check if the input integer is divisible by 2 using the modulo operator (`%`). If `number % 2 == 0`, the integer is even; otherwise, it is odd. This works correctly for negative numbers in C++ because the remainder of a negative number divided by 2 is either 0 (even) or -1 (odd), and both negative and positive odd numbers yield a non-zero remainder, so the condition `number % 2 != 0` correctly identifies odd numbers. Zero is even since `0 % 2 == 0`. For `INT_MIN`, the modulo operation is well-defined and returns 0, so it is correctly classified as even; `INT_MAX` is odd. The time complexity is O(1) because only a single arithmetic operation and comparison are performed. The space complexity is O(1) for the computation, though the returned string requires O(5) or O(4) bytes depending on word length (since "Even" has 4 characters and "Odd" has 3, plus null terminator). No edge cases require special handling beyond the simple condition.
