// Write a C++ function that takes two integer parameters, `a` and `b`, and returns a string containing the results of the five basic arithmetic operations — addition, subtraction, multiplication, integer division, and modulus — in that order, each separated by a space. The function must handle division by zero gracefully: if `b` is zero, skip the division and modulus results and instead include the word "error" for each, while still outputting the valid operations. The function should also demonstrate correct handling of post-increment and pre-increment behavior by returning a second string that shows the values of `a` after both increment operations and the values of `b` after both decrement operations, with each result separated by a space. The task is to implement this as a pure function that does not modify the original inputs, so you will work with copies internally.
The solution requires computing the arithmetic results on copies of the input integers to avoid modifying the original parameters. For the first part, compute `a + b`, `a - b`, `a * b`, `a / b`, and `a % b`; if `b == 0`, the division and modulus are undefined, so replace those two results with the word "error". For the second part, create local copies of `a` and `b`. Demonstrate post-increment by evaluating `copyA++` (which returns the old value) and then pre-increment on the already-incremented copy (which returns the new value after incrementing again). Similarly, for `b`, use post-decrement and then pre-decrement. Collect all values into strings separated by spaces. Edge cases include zero divisor, negative numbers (integer division truncates toward zero, modulus follows C++ rules), and the fact that post-increment returns the old value while pre-increment returns the new value. Time complexity is O(1) and space complexity is O(1) for the computation, though the returned strings require linear space relative to the number of digits.
#include <string>
#include <sstream>

// Computes arithmetic results and increment/decrement demonstrations without modifying inputs.
std::string arithmeticAndIncrementDemo(int a, int b) {
    std::ostringstream result;

    // Part 1: Basic arithmetic
    result << (a + b) << " "
           << (a - b) << " "
           << (a * b) << " ";

    if (b == 0) {
        result << "error error";
    } else {
        result << (a / b) << " " << (a % b);
    }

    // Part 2: Increment/decrement behavior using copies
    int copyA = a;
    int copyB = b;

    result << " | "
           << copyA++ << " "   // returns old value
           << ++copyA << " "   // increments again, returns new value
           << copyB-- << " "   // returns old value
           << --copyB;         // decrements again, returns new value

    return result.str();
}
#include <cassert>
#include <string>

// Function declaration from the solution
std::string arithmeticAndIncrementDemo(int a, int b);

int main() {
    // Standard case
    assert(arithmeticAndIncrementDemo(14, 3) == "17 11 42 4 2 | 14 16 3 1");

    // Division by zero
    assert(arithmeticAndIncrementDemo(10, 0) == "10 10 0 error error | 10 12 0 -2");

    // Negative numbers
    assert(arithmeticAndIncrementDemo(-14, 3) == "-11 -17 -42 -4 -2 | -14 -12 3 1");

    // Both negative
    assert(arithmeticAndIncrementDemo(-14, -3) == "-17 -11 42 4 -2 | -14 -12 -3 -5");

    // Zero dividend
    assert(arithmeticAndIncrementDemo(0, 5) == "5 -5 0 0 0 | 0 2 5 3");

    // One and one
    assert(arithmeticAndIncrementDemo(1, 1) == "2 0 1 1 0 | 1 3 1 -1");

    // Large values
    assert(arithmeticAndIncrementDemo(100000, 99999) == "199999 1 9999900000 1 1 | 100000 100002 99999 99997");
}
