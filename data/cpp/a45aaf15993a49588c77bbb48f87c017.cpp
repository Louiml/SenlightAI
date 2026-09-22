Write a C++ function named `applyDiscount` that takes a single `float` parameter representing the original price of an item and returns a `float` representing the final price after applying a tiered discount. The discount rules are: if the price is strictly less than 5000, no discount is applied; if the price is between 5000 and 9999 inclusive, a 5% discount is applied; if the price is between 10000 and 29999 inclusive, a 7% discount is applied; otherwise (price 30000 or more), a 10% discount is applied. The function must be `const`-correct where appropriate, handle edge cases such as zero and negative prices gracefully (treating them as no discount, since negative prices are nonsensical but should not crash), and return the computed final price as a `float`. Note: use exact floating-point arithmetic; do not worry about rounding to cents unless explicitly required, but ensure the logic is correct for all real-valued inputs.

// The solution uses a simple if-else chain to determine the discount rate based on the input price. First, handle the edge case where the price is less than 5000 (including negative and zero values) by returning the price unchanged. Then check the mid-range thresholds exactly as specified, applying the corresponding percentage discount. For any price 30000 or above, apply the 10% discount. The main algorithm is straightforward: compute `discounted = price * (1 - rate)` where `rate` is 0.0, 0.05, 0.07, or 0.10. Edge cases include negative prices (should be returned unchanged, though logically invalid) and boundary values like 5000, 9999, 10000, 29999, and 30000, which must be included in the correct bracket. Time complexity is O(1) since only a few comparisons and arithmetic operations are performed, and space complexity is O(1) as no additional data structures are used.

#include <algorithm> // not actually needed, but included for completeness if needed

// Apply tiered discount based on original price.
// Returns the final price after discount.
// Prices below 5000 (including non-positive) have no discount.
float applyDiscount(float price) {
    if (price < 5000.0f) {
        return price;
    } else if (price <= 9999.0f) { // 5000 <= price <= 9999
        return price * (1.0f - 0.05f);
    } else if (price <= 29999.0f) { // 10000 <= price <= 29999
        return price * (1.0f - 0.07f);
    } else { // price >= 30000
        return price * (1.0f - 0.10f);
    }
}

#include <cassert>
#include <cmath>

// Global main function containing assert checks for the solution function.
int main() {
    // Edge cases with zero and negative
    assert(applyDiscount(0.0f) == 0.0f);
    assert(applyDiscount(-100.0f) == -100.0f);
    
    // Below 5000
    assert(applyDiscount(4999.99f) == 4999.99f);
    assert(applyDiscount(1.0f) == 1.0f);
    
    // Between 5000 and 9999 inclusive
    assert(applyDiscount(5000.0f) == 5000.0f * 0.95f);
    assert(applyDiscount(9999.0f) == 9999.0f * 0.95f);
    assert(applyDiscount(7500.0f) == 7500.0f * 0.95f);
    
    // Between 10000 and 29999 inclusive
    assert(applyDiscount(10000.0f) == 10000.0f * 0.93f);
    assert(applyDiscount(29999.0f) == 29999.0f * 0.93f);
    assert(applyDiscount(15000.0f) == 15000.0f * 0.93f);
    
    // 30000 or above
    assert(applyDiscount(30000.0f) == 30000.0f * 0.90f);
    assert(applyDiscount(100000.0f) == 100000.0f * 0.90f);
    
    // Exact boundary check for 30000 (inclusive in higher bracket)
    assert(applyDiscount(29999.99f) == 29999.99f * 0.93f);
    assert(applyDiscount(30000.01f) == 30000.01f * 0.90f);
    
    return 0;
}
