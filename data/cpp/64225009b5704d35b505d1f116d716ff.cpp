// Write a C++ function named `classifyParity` that takes a single integer input and returns a string describing whether the integer is even or odd. The returned string must exactly match the format `"The entered number X is even"` or `"The entered number X is odd"`, where `X` is the original integer value in decimal form. The function must handle negative integers correctly (e.g., -4 is even, -3 is odd), and must not read from standard input or write to standard output. The function should be `const`-correct for any constant parameters and must not modify its input.

// The solution uses the modulo operator `%` to check divisibility by 2. In C++, for negative numbers, `n % 2` yields either `0` or `-1` (since the sign follows the dividend). Thus, checking `n % 2 == 0` correctly identifies even numbers including negative evens, while any non-zero remainder (which could be `1` or `-1`) indicates an odd number. For edge cases: zero is even (0 % 2 == 0), the most negative representable integer (e.g., INT_MIN) is even because it's divisible by 2 without overflow, and the function must construct the string using `std::to_string` to correctly handle any integer including negatives. Time complexity is O(log10(n)) due to the number of digits in the decimal conversion, and space complexity is O(log10(n)) for the resulting string. No additional auxiliary data structures are used.

#include <string>

// Returns a descriptive string indicating whether the given integer is even or odd.
std::string classifyParity(int number) {
    const bool isEven = (number % 2 == 0);
    const std::string parity = isEven ? "even" : "odd";
    return "The entered number " + std::to_string(number) + " is " + parity;
}

#include <cassert>

// Forward declaration for testing (normally comes from the solution header)
std::string classifyParity(int number);

int main() {
    assert(classifyParity(0) == "The entered number 0 is even");
    assert(classifyParity(2) == "The entered number 2 is even");
    assert(classifyParity(-2) == "The entered number -2 is even");
    assert(classifyParity(1) == "The entered number 1 is odd");
    assert(classifyParity(-1) == "The entered number -1 is odd");
    assert(classifyParity(1000000) == "The entered number 1000000 is even");
    assert(classifyParity(-2147483647) == "The entered number -2147483647 is odd");
    assert(classifyParity(2147483646) == "The entered number 2147483646 is even");
    return 0;
}
