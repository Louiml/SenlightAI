/*
Write a C++ function named `fallingPeriods` that takes a non-empty vector of integers representing stock prices over consecutive days. For each day `i`, the function must return the number of consecutive days (starting from day `i` itself) until the price first drops below the price on day `i`. If no such drop occurs within the remaining days, the count should extend to the end of the array (including the current day). For example, if prices are `[100, 80, 90, 70]`, the output should be `[1, 2, 1, 1]` because day 0 drops immediately (100→80), day 1 waits until day 3 (80→70, so 2 days), day 2 drops at day 3 (90→70, so 1 day), and day 3 has no future drop so it counts itself as 1. The function must not modify the input vector, and must handle duplicate prices (a drop strictly below the current price, not equal).
*/
#include <vector>

// For each index i, count consecutive days (starting at i) until a strictly
// lower price is found, or the end of the vector is reached.
std::vector<int> fallingPeriods(const std::vector<int>& prices) {
    std::vector<int> result;
    result.reserve(prices.size());

    for (size_t i = 0; i < prices.size(); ++i) {
        int days = 1;  // Count the current day itself
        for (size_t j = i + 1; j < prices.size(); ++j) {
            if (prices[j] < prices[i]) {
                break;  // Found a drop
            }
            ++days;
        }
        result.push_back(days);
    }

    return result;
}
#include <cassert>
#include <vector>

int main() {
    // Example from the task description
    assert(fallingPeriods({100, 80, 90, 70}) == std::vector<int>({1, 2, 1, 1}));

    // All increasing: each day must wait until the end
    assert(fallingPeriods({1, 2, 3, 4}) == std::vector<int>({4, 3, 2, 1}));

    // All decreasing: each day drops immediately
    assert(fallingPeriods({5, 4, 3, 2}) == std::vector<int>({1, 1, 1, 1}));

    // Single element
    assert(fallingPeriods({7}) == std::vector<int>({1}));

    // Duplicates: no drop until a strictly smaller price
    assert(fallingPeriods({3, 3, 3, 1}) == std::vector<int>({3, 2, 1, 1}));

    // Mixed with equal and lower values
    assert(fallingPeriods({10, 10, 5, 6, 4}) == std::vector<int>({2, 1, 2, 1, 1}));
}
// The task is a classic "stock span" variant. A straightforward nested-loop approach works: for each index `i`, initialize a counter to 1 (the current day), then scan forward through the vector while the next price is greater than or equal to the current price, incrementing the counter each step. Stop when either the end is reached or a strictly smaller price is found. This correctly handles the edge case where the current day's price is the last element—the counter stays at 1. Duplicate prices do not break the loop because we only stop on strictly lower values. Time complexity is O(n²) in the worst case (e.g., strictly increasing prices), and O(n) in the best case (all decreasing). Space complexity is O(n) for the output vector, and O(1) auxiliary space beyond that.
