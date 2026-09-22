Write a standalone C++ function that takes two integers as input and returns a string summarizing the results of addition, subtraction, multiplication, and integer division of those two numbers. The function must output the four results in the format `"sum: X\\nsub: Y\\nmul: Z\\ndiv: W"` where each result appears on a new line, using `\n` for line breaks and integer division semantics (truncation toward zero for C++11 and later, or implementation-defined for negative operands; assume non-negative inputs for simplicity). The function must be `const`-correct, take arguments by value, and have no side effects other than returning the formatted string. Handle the edge case where the second integer is zero by including the text `"div: undefined"` for the division result rather than performing a division by zero. The function should not read from or write to standard input/output; it must only return the string.

#include <cassert>
#include <string>

int main() {
    // Positive numbers
    assert(arithmeticResults(56, 89) == "sum: 145\nsub: -33\nmul: 4984\ndiv: 0");
    
    // Zero divisor
    assert(arithmeticResults(10, 0) == "sum: 10\nsub: 10\nmul: 0\ndiv: undefined");
    
    // Negative numbers and integer division truncation
    assert(arithmeticResults(-7, 2) == "sum: -5\nsub: -9\nmul: -14\ndiv: -3");
    
    // One positive, one negative
    assert(arithmeticResults(5, -3) == "sum: 2\nsub: 8\nmul: -15\ndiv: -1");
    
    // Both zero
    assert(arithmeticResults(0, 0) == "sum: 0\nsub: 0\nmul: 0\ndiv: undefined");
    
    // Large numbers
    assert(arithmeticResults(1000, 2000) == "sum: 3000\nsub: -1000\nmul: 2000000\ndiv: 0");
    
    // Exactly divisible
    assert(arithmeticResults(100, 25) == "sum: 125\nsub: 75\nmul: 2500\ndiv: 4");
    
    // One negative denominator
    assert(arithmeticResults(9, -2) == "sum: 7\nsub: 11\nmul: -18\ndiv: -4");
    
    // Single-digit values
    assert(arithmeticResults(3, 2) == "sum: 5\nsub: 1\nmul: 6\ndiv: 1");
    
    // Values that yield same div and mul? Not relevant, just edge
    assert(arithmeticResults(1, 1) == "sum: 2\nsub: 0\nmul: 1\ndiv: 1");
    
    return 0;
}

#include <string>

// Return a formatted string with sum, subtraction, multiplication, and division results.
// If divisor is zero, the division result is reported as "undefined".
std::string arithmeticResults(int a, int b) {
    int sum = a + b;
    int sub = a - b;
    int mul = a * b;
    
    std::string divText;
    if (b == 0) {
        divText = "undefined";
    } else {
        int div = a / b;
        divText = std::to_string(div);
    }
    
    return "sum: " + std::to_string(sum) + "\n"
         + "sub: " + std::to_string(sub) + "\n"
         + "mul: " + std::to_string(mul) + "\n"
         + "div: " + divText;
}

// The solution computes four arithmetic operations on two integer parameters. Addition, subtraction, and multiplication are straightforward. Division requires special handling for a zero divisor: instead of dividing, the string `"undefined"` is used to avoid a runtime error. The results are assembled into a single `std::string` using `std::to_string` for numeric conversion and concatenation with `\n` as line separators. Ensure the output format exactly matches the specification, including spaces after colons and no trailing newline beyond the last character. The algorithm takes constant time \(O(1)\) and uses \(O(1)\) auxiliary space for the returned string, which has fixed length (the characters for the numbers plus formatting). Edge cases include negative inputs (handled naturally by arithmetic), and zero divisor (explicit check). The function is pure and `const`-correct because it does not modify its inputs and returns by value.
