// Write a C++ function that takes a list of item prices (as a `std::vector<double>`) and a sales tax rate (as a `double`), and returns a `std::pair<double, double>` containing the subtotal (sum of all prices) and the total after tax (subtotal plus tax, where tax = subtotal * rate). The function must handle an empty list by returning {0.0, 0.0}, and it must not modify the input vector. Assume prices and the tax rate are non-negative, and that floating-point rounding is acceptable for comparison (use a small epsilon in tests if needed).
The solution iterates through the input vector once to compute the subtotal by accumulating each price into a running sum. Then it computes the tax amount by multiplying the subtotal by the tax rate, and the total by adding the tax to the subtotal. The empty input case is handled by initializing the subtotal to 0.0, so no special case is needed—an empty loop leaves it at 0, and the tax and total also become 0.0. The time complexity is O(n) for n prices, and space complexity is O(1) for the summation variables (excluding the returned pair). Edge cases include a single item (subtotal equals that item) and a zero tax rate (total equals subtotal). Floating-point precision is acceptable; we can compare results using `std::abs(a - b) < 1e-9` if needed.
#include <vector>
#include <utility>

// Returns {subtotal, total with tax} for a list of prices and a tax rate.
std::pair<double, double> computePurchaseTotals(const std::vector<double>& prices, double taxRate) {
    double subtotal = 0.0;
    for (double price : prices) {
        subtotal += price;
    }
    double taxAmount = subtotal * taxRate;
    double total = subtotal + taxAmount;
    return {subtotal, total};
}
#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// Function under test
std::pair<double, double> computePurchaseTotals(const std::vector<double>& prices, double taxRate) {
    double subtotal = 0.0;
    for (double price : prices) {
        subtotal += price;
    }
    double taxAmount = subtotal * taxRate;
    double total = subtotal + taxAmount;
    return {subtotal, total};
}

int main() {
    const double EPS = 1e-9;

    // Test 1: Empty list -> {0.0, 0.0}
    auto result1 = computePurchaseTotals({}, 0.06);
    assert(std::abs(result1.first - 0.0) < EPS);
    assert(std::abs(result1.second - 0.0) < EPS);

    // Test 2: Single item, zero tax
    auto result2 = computePurchaseTotals({12.95}, 0.0);
    assert(std::abs(result2.first - 12.95) < EPS);
    assert(std::abs(result2.second - 12.95) < EPS);

    // Test 3: Multiple items with 6% tax (as in the snippet)
    std::vector<double> prices3 = {12.95, 24.95, 6.95, 14.95, 3.95};
    auto result3 = computePurchaseTotals(prices3, 0.06);
    double expectedSubtotal = 12.95 + 24.95 + 6.95 + 14.95 + 3.95;
    assert(std::abs(result3.first - expectedSubtotal) < EPS);
    assert(std::abs(result3.second - (expectedSubtotal * 1.06)) < EPS);

    // Test 4: One item with 10% tax
    auto result4 = computePurchaseTotals({100.0}, 0.10);
    assert(std::abs(result4.first - 100.0) < EPS);
    assert(std::abs(result4.second - 110.0) < EPS);

    // Test 5: Three repeated items, 25% tax
    auto result5 = computePurchaseTotals({10.0, 10.0, 10.0}, 0.25);
    assert(std::abs(result5.first - 30.0) < EPS);
    assert(std::abs(result5.second - 37.5) < EPS);

    // Test 6: Prices with many decimal places
    auto result6 = computePurchaseTotals({0.1, 0.2, 0.3}, 0.5);
    assert(std::abs(result6.first - 0.6) < EPS);
    assert(std::abs(result6.second - 0.9) < EPS);

    return 0;
}
