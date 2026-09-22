// Write a C++ function `double calculateTotalCost(const std::vector<std::pair<std::string, double>>& items)` that takes a vector of pairs, where each pair contains an item name (ignored) and a unit price, and a parallel vector of quantities (also as `double`). However, to keep the task self-contained and simpler, instead of parallel vectors, define the function to accept a `std::vector<std::pair<double, double>>` where each pair is `{quantity, unit_price}`. The function should compute and return the sum of `quantity * unit_price` for all items. The input may be empty, in which case the function should return `0.0`. The function must handle floating-point arithmetic correctly, and the result should be printed with exactly one decimal place when used in the test (but the function itself returns a `double`).

The solution is straightforward: iterate over all pairs in the vector, multiply each `first` (quantity) by each `second` (unit price), and accumulate the results into a `double` sum. Initialize the sum to `0.0` to handle the empty vector case correctly. For floating-point precision, we do not need special handling beyond using `double`; the final result may have rounding errors, but the test uses an epsilon comparison to account for this. Edge cases: empty input (return 0.0), negative quantities or prices (multiplication still works, they are allowed), and large counts (the sum may overflow if numbers are huge, but for typical inputs it's fine). Time complexity is O(n) where n is the number of items; space complexity is O(1) auxiliary.

#include <vector>
#include <utility>

// Compute the total cost as the sum of quantity * unit_price for each item.
// Returns 0.0 for an empty input vector.
double calculateTotalCost(const std::vector<std::pair<double, double>>& items) {
    double total = 0.0;
    for (const auto& item : items) {
        total += item.first * item.second;
    }
    return total;
}

#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// Function under test (paste here for standalone test)
double calculateTotalCost(const std::vector<std::pair<double, double>>& items) {
    double total = 0.0;
    for (const auto& item : items) {
        total += item.first * item.second;
    }
    return total;
}

int main() {
    // Empty vector
    assert(std::fabs(calculateTotalCost({}) - 0.0) < 1e-9);

    // Single item
    assert(std::fabs(calculateTotalCost({{2.0, 3.5}}) - 7.0) < 1e-9);

    // Multiple items
    std::vector<std::pair<double, double>> items = {{2.0, 3.5}, {1.5, 4.0}, {10.0, 0.5}};
    assert(std::fabs(calculateTotalCost(items) - (7.0 + 6.0 + 5.0)) < 1e-9);

    // Negative prices and quantities
    assert(std::fabs(calculateTotalCost({{-2.0, 3.0}, {4.0, -5.0}}) - (-6.0 - 20.0)) < 1e-9);

    // Large numbers (still within double range)
    std::vector<std::pair<double, double>> large = {{1e6, 2.5}, {3e6, 1.5}};
    assert(std::fabs(calculateTotalCost(large) - (2.5e6 + 4.5e6)) < 1e-3);

    // Only zeros
    assert(std::fabs(calculateTotalCost({{0.0, 5.0}, {3.0, 0.0}}) - 0.0) < 1e-9);

    // Mixed signs
    assert(std::fabs(calculateTotalCost({{2.0, -1.0}, {-3.0, 4.0}}) - (-2.0 - 12.0)) < 1e-9);

    return 0;
}
