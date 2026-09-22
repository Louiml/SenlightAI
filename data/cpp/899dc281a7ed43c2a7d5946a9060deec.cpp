// Write a C++ function that takes an integer `N` (representing a price before tax) and a tax rate `taxRate` (an integer percentage, e.g., 8 means 8%) and returns a string describing the result of applying the tax and comparing the after-tax price against a fixed threshold of 206. Specifically, after computing `taxed = (N * (100 + taxRate)) / 100` using integer arithmetic, if `taxed < 206` return `"Yay!"`, if `taxed == 206` return `"so-so"`, otherwise return `":("`. The function must handle negative `N` and negative tax rates correctly (e.g., a negative tax rate effectively reduces the price). Also, because integer division truncates toward zero in C++, ensure that the calculation is consistent with the behavior of the original snippet, which uses integer arithmetic. For simplicity, you may assume `N` is within the range of a 32-bit signed integer, but note that `(N * (100 + taxRate))` could overflow for large `N` and tax rates; use a 64-bit intermediate type to avoid overflow. The function signature should be `std::string priceCategory(int N, int taxRate)`.
// The solution involves computing the after-tax price using integer arithmetic: `taxed = (N * (100 + taxRate)) / 100`. Since `N` and `taxRate` are both integers, we perform a multiplication that could potentially overflow if both are large; therefore, we cast `N` to `long long` (or use `int64_t`) before multiplication, and also cast `(100 + taxRate)` to `long long` to ensure the multiplication is done in 64-bit. The division by 100 is integer division, which truncates toward zero in C++ (since C++11). This means for negative values, the result is truncated toward zero, e.g., `-1 / 100 = 0`. This is consistent with the original snippet's behavior if `N` were negative, though the original doesn't explicitly handle negatives. Edge cases: when `taxRate` is negative, say `-8`, then `100 + taxRate = 92`, effectively a discount. The result is compared against 206 using strict `<`, `==`, and `>`. The function returns a `std::string` exactly as specified. Time complexity is O(1) (constant arithmetic), space complexity is O(1) (only a few local variables). The solution must be robust against overflow, handle negative inputs, and produce the exact same strings as the original snippet for positive inputs and typical tax rates.
#include <string>
#include <cstdint>

// Given a pre-tax price N (integer) and a tax rate as a percentage (integer),
// compute the after-tax price using integer arithmetic (truncation toward zero)
// and compare it against a fixed threshold of 206.
// Return "Yay!" if after-tax < 206, "so-so" if == 206, ":(" otherwise.
std::string priceCategory(int N, int taxRate) {
    // Use 64-bit to avoid overflow during multiplication.
    const int64_t multiplier = static_cast<int64_t>(100) + taxRate;
    const int64_t afterTax = (static_cast<int64_t>(N) * multiplier) / 100;

    if (afterTax < 206) {
        return "Yay!";
    } else if (afterTax == 206) {
        return "so-so";
    } else {
        return ":(";
    }
}
#include <cassert>
#include <string>

// The function declaration (or include header) is expected above.
// This is a test harness that calls the solution function directly.
int main() {
    // Original snippet examples with 8% tax.
    assert(priceCategory(100, 8) == "Yay!");    // 108 < 206
    assert(priceCategory(190, 8) == "Yay!");    // 205 < 206
    assert(priceCategory(191, 8) == "so-so");   // 206 == 206
    assert(priceCategory(192, 8) == ":(");      // 207 > 206

    // Edge case: zero tax.
    assert(priceCategory(206, 0) == "so-so");   // 206 == 206
    assert(priceCategory(205, 0) == "Yay!");    // 205 < 206
    assert(priceCategory(207, 0) == ":(");      // 207 > 206

    // Negative tax rate (discount).
    assert(priceCategory(250, -8) == "Yay!");   // 230 < 206? No, 230 > 206, so check: -8% gives 230, so ":("
    // Let's compute: 250 * 92 / 100 = 230, so ":("
    assert(priceCategory(250, -8) == ":(");
    assert(priceCategory(200, -8) == "Yay!");   // 200 * 92 / 100 = 184 < 206

    // Negative N: integer division truncates toward zero.
    assert(priceCategory(-206, 0) == ":(");     // -206 < 206? Actually -206 < 206, so "Yay!"
    // Let's check: -206 < 206 => true, so "Yay!" but wait: original snippet computes N = (108*N)/100, then if N < 206. For -206, after-tax = -206, which is < 206 -> "Yay!"
    assert(priceCategory(-206, 0) == "Yay!");
    assert(priceCategory(-207, 0) == "Yay!");   // -207 < 206 -> "Yay!"

    // Large N to test overflow prevention.
    assert(priceCategory(2000000000, 50) == ":("); // 3,000,000,000 > 206, no overflow.

    // Boundary around threshold.
    assert(priceCategory(191, 8) == "so-so");   // 206
    assert(priceCategory(190, 8) == "Yay!");    // 205
    assert(priceCategory(192, 8) == ":(");      // 207

    return 0;
}
