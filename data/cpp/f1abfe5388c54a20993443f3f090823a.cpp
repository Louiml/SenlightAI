// Write a C++ function named `sumAndProduct` that takes two integers `a` and `b` as input and returns a `std::string` containing the sum and the product of the two numbers, separated by a single space. The output must exactly match the format: `<sum> <product>`. The function should handle negative numbers, zero, and any valid integer range within the C++ `int` type. It must be `const`-correct (parameters passed by value, no state modification) and include appropriate headers. The function should not print anything to the console; it only returns the formatted result.

// The solution is straightforward: compute the sum as `a + b` and the product as `a * b`, then convert both integers to strings using `std::to_string` and concatenate them with a space in between. The main edge cases are when the sum or product is negative (since `std::to_string` correctly handles the minus sign) and when either input is zero (the product becomes zero, which is handled naturally). Overflow is not a concern since the problem specifies valid `int` range; however, if the inputs are large, the multiplication could overflow, but that is out of scope. Time complexity is \(O(1)\) for arithmetic and string conversion, and space complexity is \(O(1)\) for the temporary strings and the result (the length of the result is proportional to the number of digits, but that's constant for typical `int` values). The function is pure and does not depend on any external state.

#include <string>

// Return the sum and product of two integers as "<sum> <product>".
std::string sumAndProduct(int a, int b) {
    int sum = a + b;
    int product = a * b;
    return std::to_string(sum) + " " + std::to_string(product);
}

#include <cassert>
#include <string>

std::string sumAndProduct(int a, int b); // declaration for testing

int main() {
    assert(sumAndProduct(5, 7) == "12 35");
    assert(sumAndProduct(-5, 7) == "2 -35");
    assert(sumAndProduct(0, 0) == "0 0");
    assert(sumAndProduct(-3, -4) == "-7 12");
    assert(sumAndProduct(100, -1) == "99 -100");
    assert(sumAndProduct(1, 1) == "2 1");
    assert(sumAndProduct(-1, 1) == "0 -1");
    assert(sumAndProduct(10, 0) == "10 0");
    return 0;
}
