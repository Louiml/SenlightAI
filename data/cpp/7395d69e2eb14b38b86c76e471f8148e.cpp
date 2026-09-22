// Write a standalone C++ function that, given four integers (two product codes, two quantities) and two double-precision prices, computes and returns the total amount to be paid for two items, formatted as a string with exactly two decimal places and prefixed by `"VALOR A PAGAR: R$ "`. The code and quantity arguments are integers, and prices are floating-point values. The function must handle any non-negative quantities and prices, including zero values, and must not read from standard input or print to standard output; it only computes and returns the formatted result.

#include <cassert>
#include <string>

// Forward declaration of the solution function
std::string computeTotalValue(int code1, int qty1, double price1,
                              int code2, int qty2, double price2);

int main() {
    // Test from original snippet: 12 1 4.56 and 16 2 5.00
    assert(computeTotalValue(12, 1, 4.56, 16, 2, 5.00) == "VALOR A PAGAR: R$ 14.56");

    // Zero quantities
    assert(computeTotalValue(1, 0, 10.0, 2, 0, 20.0) == "VALOR A PAGAR: R$ 0.00");

    // One item only, second zero price
    assert(computeTotalValue(1, 3, 1.99, 2, 0, 0.0) == "VALOR A PAGAR: R$ 5.97");

    // Large quantities and decimal prices
    assert(computeTotalValue(1, 100, 0.01, 2, 200, 0.02) == "VALOR A PAGAR: R$ 5.00");

    // Floating point imprecision: 0.1 + 0.2 should format as 0.30
    assert(computeTotalValue(1, 1, 0.1, 2, 1, 0.2) == "VALOR A PAGAR: R$ 0.30");

    // Mixed values with rounding to two decimals
    assert(computeTotalValue(1, 2, 3.335, 2, 1, 1.005) == "VALOR A PAGAR: R$ 7.68"); // 6.67 + 1.01 = 7.68

    // Codes are ignored but can be any integers
    assert(computeTotalValue(12345, 1, 100.0, -6789, 1, 0.0) == "VALOR A PAGAR: R$ 100.00");
}

#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

// Computes total price for two products and returns formatted string.
std::string computeTotalValue(int code1, int qty1, double price1,
                              int code2, int qty2, double price2) {
    // Code parameters are not used in calculation but kept for signature completeness.
    double total = (qty1 * price1) + (qty2 * price2);

    // Round to nearest cent to avoid floating-point representation errors.
    total = std::round(total * 100.0) / 100.0;

    std::ostringstream out;
    out << "VALOR A PAGAR: R$ " << std::fixed << std::setprecision(2) << total;
    return out.str();
}

// The solution is straightforward: compute the total value as `(quantity1 * price1) + (quantity2 * price2)`. Since the original problem uses `printf` with `%.2lf`, the output must be rounded to two decimal places. However, floating-point arithmetic can introduce tiny errors (e.g., 0.1 + 0.2 = 0.30000000000000004), so to ensure correct rounding for typical monetary values, we can use `std::round` on the total multiplied by 100 and then divide back by 100 before formatting, or use `std::ostringstream` with `std::fixed` and `std::setprecision(2)` which rounds to nearest, handling most cases correctly (though for exactness, the rounding function is safer). Edge cases: zero quantities or zero prices should still produce "0.00". Negative values are not expected per task, but if present, they would be handled naturally. Time complexity is O(1), space complexity is O(1) for computation plus the returned string length.
