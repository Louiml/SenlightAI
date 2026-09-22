// Write a C++ function named `computeArithmeticResults` that takes two integer parameters `a` and `b` (where `b` is not zero) and returns a `std::string` containing the results of the following operations, each on a new line and in this exact order: the sum, difference, product, integer quotient (truncated toward zero), integer remainder (as given by the `%` operator), and floating-point division (computed as `double` with 10 decimal places fixed formatting). For example, for `a=5`, `b=2`, the output string should be exactly:
// ```
// Soma: 7
// Subtracao: 3
// Multiplicacao: 10
// Quociente: 2
// Resto: 1
// Divisao: 2.5000000000
// ```
// The function must handle negative numbers correctly (e.g., integer division truncates toward zero, and the remainder has the sign of the dividend in C++). Do not include any power, increment/decrement, or assignment operations. The function should be `const`-correct (parameters passed by value, no mutation) and must not use `std::cout`—it should build and return a `std::string`.
#include <cassert>
#include <string>

// Declaration from solution
std::string computeArithmeticResults(int a, int b);

int main() {
    // Test with positive numbers
    std::string result1 = computeArithmeticResults(5, 2);
    assert(result1 == "Soma: 7\nSubtracao: 3\nMultiplicacao: 10\nQuociente: 2\nResto: 1\nDivisao: 2.5000000000\n");

    // Test with negative dividend
    std::string result2 = computeArithmeticResults(-7, 3);
    assert(result2 == "Soma: -4\nSubtracao: -10\nMultiplicacao: -21\nQuociente: -2\nResto: -1\nDivisao: -2.3333333333\n");

    // Test with negative divisor
    std::string result3 = computeArithmeticResults(7, -3);
    assert(result3 == "Soma: 4\nSubtracao: 10\nMultiplicacao: -21\nQuociente: -2\nResto: 1\nDivisao: -2.3333333333\n");

    // Test with both negative
    std::string result4 = computeArithmeticResults(-7, -2);
    assert(result4 == "Soma: -9\nSubtracao: -5\nMultiplicacao: 14\nQuociente: 3\nResto: -1\nDivisao: 3.5000000000\n");

    // Test with zero dividend
    std::string result5 = computeArithmeticResults(0, 5);
    assert(result5 == "Soma: 0\nSubtracao: -5\nMultiplicacao: 0\nQuociente: 0\nResto: 0\nDivisao: 0.0000000000\n");

    // Test with equal numbers
    std::string result6 = computeArithmeticResults(4, 4);
    assert(result6 == "Soma: 8\nSubtracao: 0\nMultiplicacao: 16\nQuociente: 1\nResto: 0\nDivisao: 1.0000000000\n");

    // Test with large numbers
    std::string result7 = computeArithmeticResults(1000000, 100);
    assert(result7 == "Soma: 1000100\nSubtracao: 999900\nMultiplicacao: 100000000\nQuociente: 10000\nResto: 0\nDivisao: 10000.0000000000\n");

    return 0;
}
#include <string>
#include <sstream>
#include <iomanip>

// Compute and return arithmetic results as a formatted string.
// Precondition: b must not be zero.
std::string computeArithmeticResults(int a, int b) {
    // Compute all results
    int sum = a + b;
    int diff = a - b;
    int prod = a * b;
    int quotient = a / b;   // truncates toward zero
    int remainder = a % b;  // sign follows dividend
    double division = static_cast<double>(a) / static_cast<double>(b);

    // Build the output string using stringstream
    std::ostringstream oss;
    oss << "Soma: " << sum << "\n";
    oss << "Subtracao: " << diff << "\n";
    oss << "Multiplicacao: " << prod << "\n";
    oss << "Quociente: " << quotient << "\n";
    oss << "Resto: " << remainder << "\n";
    // Fixed precision for the division
    oss << std::fixed << std::setprecision(10);
    oss << "Divisao: " << division << "\n";
    return oss.str();
}
// The solution involves performing six arithmetic operations on two integers and formatting the results into a single string. The main steps: (1) compute `a+b`, `a-b`, `a*b`, `a/b`, `a%b`, and `(double)a / (double)b`; (2) convert each numeric result to a string using `std::to_string` for the integer results and a helper for formatting the double with exactly 10 decimal places (using `std::ostringstream` with `std::fixed` and `std::setprecision(10)`); (3) concatenate each operation label with the result and a newline. Edge cases include negative inputs (C++ guarantees truncation toward zero for integer division and that `a%b` has the sign of `a`), zero `b` (the problem specification says `b` is not zero, so no special handling needed, but we can add a comment). Time complexity is O(1) because we perform a constant number of operations. Space complexity is O(1) for the computation itself, but the returned string uses O(L) where L is the total length of the formatted output (limited to a few dozen characters). The function must be `const`-correct by taking parameters by value (no modification) and not altering any external state.
