Write a C++ function `computeDiscountedTotal(long int quantity, long int price)` that calculates and returns the total cost for a bulk purchase as a `double`. The total cost is simply `quantity * price`, but if the quantity exceeds 1000, a 10% discount is applied to the entire total. The function must handle large values of `quantity` and `price` (up to `long int` range) without overflow in intermediate calculations, so use `double` for the multiplication. Return the result with full precision so that when printed with 6 decimal places, it matches the format from the original snippet. The function should be self-contained and not read from standard input.
#include <iostream>
#include <iomanip>
#include <cassert>
#include <cmath>

// Forward declaration of the function under test.
double computeDiscountedTotal(long int quantity, long int price);

int main() {
    // Test cases with expected outputs (matching original snippet behavior).
    // Case 1: quantity <= 1000, no discount.
    assert(computeDiscountedTotal(1000, 10) == 10000.0);
    
    // Case 2: quantity > 1000, 10% discount applied.
    assert(computeDiscountedTotal(1001, 10) == 9009.0);
    
    // Case 3: large quantity and price, discount applied.
    long int q = 123456789L;
    long int p = 987654321L;
    double expected = static_cast<double>(q) * p;
    expected -= 0.1 * expected;
    assert(std::abs(computeDiscountedTotal(q, p) - expected) < 1e-6);
    
    // Case 4: quantity exactly 1001, price 0 -> zero total.
    assert(computeDiscountedTotal(1001, 0) == 0.0);
    
    // Case 5: quantity 0, any price -> zero total.
    assert(computeDiscountedTotal(0, 500) == 0.0);
    
    // Case 6: quantity 999, price 1 -> no discount.
    assert(computeDiscountedTotal(999, 1) == 999.0);
    
    // Case 7: quantity 2000, price 1 -> discount: 2000 * 0.9 = 1800.
    assert(std::abs(computeDiscountedTotal(2000, 1) - 1800.0) < 1e-6);
    
    // Case 8: quantity 1001, price 100 -> discount = 100100 * 0.9 = 90090.
    assert(std::abs(computeDiscountedTotal(1001, 100) - 90090.0) < 1e-6);
    
    // Print one result to show it matches the original format.
    std::cout << std::fixed << std::setprecision(6)
              << computeDiscountedTotal(1500, 500) << std::endl; // Should be 675000.000000
    
    return 0;
}
#include <cstddef>

// Compute the total cost for a bulk purchase.
// Applies a 10% discount if the quantity exceeds 1000.
// Returns the total as a double to avoid overflow and preserve precision.
double computeDiscountedTotal(long int quantity, long int price) {
    // Use double for the product to handle large long int values safely.
    double total = static_cast<double>(quantity) * price;
    
    // Apply a 10% discount when quantity exceeds 1000.
    if (quantity > 1000) {
        total -= 0.1 * total;
    }
    
    return total;
}
// The solution is straightforward arithmetic: compute `total = static_cast<double>(quantity) * price` to avoid integer overflow (since `long int * long int` could overflow for values near the maximum). Then, if `quantity > 1000`, subtract 10% of the total: `total -= 0.1 * total`. An important edge case is when `quantity` is exactly 1000 — no discount applies. Another edge case is when `price` or `quantity` is zero, in which case the total is zero regardless of discount (the discount on zero is still zero). Since the discount is applied after computing the full total, the order of operations matters only for floating-point rounding, but using a single `double` ensures consistency. Time complexity is O(1), space complexity is O(1). The returned `double` should be printed with `fixed << setprecision(6)` in the test to verify exact match, but the function itself returns the computed value.
