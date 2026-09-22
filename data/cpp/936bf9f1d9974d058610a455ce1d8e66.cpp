/*
Write a C++ function named `checkArithmeticOverflow` that takes four parameters: an integer `limit`, an integer `leftOperand`, a character `operation` (either `'+'` or `'*'`), and an integer `rightOperand`. The function must compute the result of `leftOperand operation rightOperand`. If the computed result is strictly greater than `limit`, return the string `"OVERFLOW"`; otherwise, return `"OK"`. The function must handle the case where the operation is neither `'+'` nor `'*'` by returning `"OK"` (treating it as no operation, result equal to leftOperand, which is assumed ≤ limit for simplicity). The inputs are guaranteed to be positive integers and the arithmetic result will not exceed the range of a 32-bit signed integer, but the function should be correct for all valid inputs within that range. Test with a variety of cases including boundary conditions.
*/

#include <string>

// Checks if leftOperand operation rightOperand exceeds limit.
// Returns "OVERFLOW" if result > limit, else "OK".
std::string checkArithmeticOverflow(int limit, int leftOperand, char operation, int rightOperand) {
    int result = leftOperand; // Default for invalid operation
    if (operation == '+') {
        result = leftOperand + rightOperand;
    } else if (operation == '*') {
        result = leftOperand * rightOperand;
    }
    return (result > limit) ? "OVERFLOW" : "OK";
}

#include <cassert>
#include <string>

// Function declaration (from solution)
std::string checkArithmeticOverflow(int limit, int leftOperand, char operation, int rightOperand);

int main() {
    // Basic addition overflow
    assert(checkArithmeticOverflow(10, 5, '+', 6) == "OVERFLOW");
    // Basic addition OK
    assert(checkArithmeticOverflow(10, 5, '+', 5) == "OK");
    // Boundary: result equals limit -> OK
    assert(checkArithmeticOverflow(10, 5, '+', 5) == "OK");
    // Multiplication overflow
    assert(checkArithmeticOverflow(100, 20, '*', 6) == "OVERFLOW");
    // Multiplication OK
    assert(checkArithmeticOverflow(100, 20, '*', 5) == "OK");
    // Large values within range
    assert(checkArithmeticOverflow(1000000, 999, '*', 1000) == "OK");
    assert(checkArithmeticOverflow(999, 999, '*', 1) == "OK");
    // Invalid operator -> treats as OK
    assert(checkArithmeticOverflow(10, 5, '-', 3) == "OK");
    // Exactly at limit multiplication
    assert(checkArithmeticOverflow(25, 5, '*', 5) == "OK");
    // Zero limit with zero result
    assert(checkArithmeticOverflow(0, 0, '+', 0) == "OK");
    return 0;
}

// The solution requires simple conditional logic: parse the operator character, perform the corresponding arithmetic operation, and compare the result with the limit. Edge cases include when the operator is not `'+'` or `'*'` (here we return `"OK"` to avoid undefined behavior, though the problem statement implies only valid operators are given), and when the result exactly equals the limit (should return `"OK"` because only greater than triggers overflow). The main algorithm is O(1) time and O(1) space, as it only performs a single arithmetic operation and a comparison. No input parsing is needed inside the function since parameters are already provided.
