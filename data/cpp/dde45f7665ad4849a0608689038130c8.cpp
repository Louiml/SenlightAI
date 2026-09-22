Write a C++ function `calculateOperations(int a, int b)` that takes two integer values and returns a `std::string` containing the results of five arithmetic operations—addition, subtraction, multiplication, integer division (truncated toward zero), and the modulo remainder—formatted exactly as shown in the sample output format: `"Soma: <sum>." << newline << "Subtracao: <difference>." << newline << "Multiplicacao: <product>." << newline << "Divisao: <quotient>." << newline << "Resto: <remainder>."`. The function must handle the edge case where division and modulo are performed with a divisor of zero: in that case, instead of performing the division and modulo, output `"Divisao: erro."` and `"Resto: erro."` respectively, while the other three operations (addition, subtraction, multiplication) still produce their normal results. The inputs are integers (positive, negative, or zero), and you must ensure integer division follows C++ truncation toward zero semantics. The function should be `const`-correct and return the complete multi-line string with a trailing newline after the last line.

#include <cassert>
#include <string>

// Forward declaration of the solution function (already defined above).
std::string calculateOperations(int a, int b);

int main() {
    // Basic positive numbers
    assert(calculateOperations(10, 2) == "Soma: 12.\nSubtracao: 8.\nMultiplicacao: 20.\nDivisao: 5.\nResto: 0.\n");

    // Negative dividend with modulo (C++ truncates toward zero)
    assert(calculateOperations(-7, 3) == "Soma: -4.\nSubtracao: -10.\nMultiplicacao: -21.\nDivisao: -2.\nResto: -1.\n");

    // Zero divisor: only division/modulo are errors
    assert(calculateOperations(5, 0) == "Soma: 5.\nSubtracao: 5.\nMultiplicacao: 0.\nDivisao: erro.\nResto: erro.\n");

    // Zero dividend, non-zero divisor
    assert(calculateOperations(0, 4) == "Soma: 4.\nSubtracao: -4.\nMultiplicacao: 0.\nDivisao: 0.\nResto: 0.\n");

    // Both negative with modulo
    assert(calculateOperations(-10, -3) == "Soma: -13.\nSubtracao: -7.\nMultiplicacao: 30.\nDivisao: 3.\nResto: -1.\n");

    // Zero divided by zero (both erroneous)
    assert(calculateOperations(0, 0) == "Soma: 0.\nSubtracao: 0.\nMultiplicacao: 0.\nDivisao: erro.\nResto: erro.\n");

    // One positive and one negative, division truncates toward zero
    assert(calculateOperations(7, -2) == "Soma: 5.\nSubtracao: 9.\nMultiplicacao: -14.\nDivisao: -3.\nResto: 1.\n");

    // Large values (no overflow here)
    assert(calculateOperations(100000, 1000) == "Soma: 101000.\nSubtracao: 99000.\nMultiplicacao: 100000000.\nDivisao: 100.\nResto: 0.\n");

    // Negative divisor with positive dividend
    assert(calculateOperations(9, -4) == "Soma: 5.\nSubtracao: 13.\nMultiplicacao: -36.\nDivisao: -2.\nResto: 1.\n");

    // Same positive value for both
    assert(calculateOperations(3, 3) == "Soma: 6.\nSubtracao: 0.\nMultiplicacao: 9.\nDivisao: 1.\nResto: 0.\n");
}

#include <string>
#include <sstream>

// Compute and format the five arithmetic results for two integers.
// Returns a multi-line string like:
// "Soma: 7.\nSubtracao: 1.\nMultiplicacao: 12.\nDivisao: 3.\nResto: 0.\n"
// If b == 0, division and modulo are reported as "erro" instead.
std::string calculateOperations(int a, int b) {
    const int soma = a + b;
    const int sub = a - b;
    const int mult = a * b;

    std::ostringstream out;
    out << "Soma: " << soma << ".\n";
    out << "Subtracao: " << sub << ".\n";
    out << "Multiplicacao: " << mult << ".\n";

    if (b != 0) {
        const int div = a / b;
        const int rest = a % b;
        out << "Divisao: " << div << ".\n";
        out << "Resto: " << rest << ".\n";
    } else {
        out << "Divisao: erro.\n";
        out << "Resto: erro.\n";
    }

    return out.str();
}

// The solution uses straightforward arithmetic operators for the three safe operations. For division and modulo, we must first check if `b == 0`; if so, we replace those results with the literal `"erro"` strings. For non-zero `b`, we use the `/` and `%` operators directly, which in C++ perform truncation toward zero for integers. The output is assembled using `std::string` concatenation or `std::ostringstream` to build the multi-line result. The dominant work is the constant-time arithmetic operations, so time complexity is \(O(1)\) and auxiliary space is \(O(1)\) beyond the returned string (whose length is proportional to the number of digits in the numbers, but that is inherent to the output). Edge cases to consider: both `a` and `b` being zero (division and modulo error), `b` zero while `a` non-zero, negative operands with modulo (C++ yields a result with the sign of the dividend), and large integers that may overflow—these are not tested here, but the function uses standard `int` semantics as specified.
