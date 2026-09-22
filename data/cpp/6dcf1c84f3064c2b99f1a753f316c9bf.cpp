// Write a standalone C++ function `bool canMakeProfit(const std::vector<int>& prices, int requiredDays)` that takes a non-empty vector of daily stock prices and a positive integer `requiredDays`. The function should return `true` if there exists a strictly increasing subsequence of prices of length at least `requiredDays` (meaning you could buy on each selected day and sell later at a higher price each time), and `false` otherwise. For example, prices `{10, 20, 15, 30}` with `requiredDays = 3` returns `true` because the subsequence `10, 20, 30` is strictly increasing and has length 3. However, `requiredDays = 4` returns `false` because no strictly increasing subsequence of length 4 exists. The function must be efficient for vectors up to 10,000 elements, handle negative prices, duplicates, and a single-element vector.

#include <cassert>
#include <vector>

// Free function declaration from the solution.
bool canMakeProfit(const std::vector<int>& prices, int requiredDays);

int main() {
    // Basic positive case: strictly increasing sequence of length 3 exists.
    assert(canMakeProfit({10, 20, 15, 30}, 3) == true);
    // No increasing subsequence of length 4 exists in a 4-element array with duplicates.
    assert(canMakeProfit({10, 20, 15, 30}, 4) == false);
    // Single element: length 1 only.
    assert(canMakeProfit({7}, 1) == true);
    assert(canMakeProfit({7}, 2) == false);
    // Duplicates break strict increase: all equal only gives length 1.
    assert(canMakeProfit({5, 5, 5, 5}, 2) == false);
    // Negative prices are handled.
    assert(canMakeProfit({-3, -1, -2, 0}, 3) == true);
    // Decreasing sequence gives length 1.
    assert(canMakeProfit({9, 8, 7, 6}, 2) == false);
    // Required days larger than array size.
    assert(canMakeProfit({1, 2, 3}, 4) == false);
    // Non-monotonic but has a long increasing subsequence.
    assert(canMakeProfit({3, 1, 4, 2, 5, 0, 6}, 4) == true);
    // requiredDays of 0 or negative: return false.
    assert(canMakeProfit({1, 2, 3}, 0) == false);
    // Empty vector returns false.
    assert(canMakeProfit({}, 1) == false);
    return 0;
}

#include <vector>
#include <algorithm>
#include <cstddef>

// Returns true if there exists a strictly increasing subsequence of prices
// with length at least requiredDays.
bool canMakeProfit(const std::vector<int>& prices, int requiredDays) {
    const std::size_t n = prices.size();
    if (n == 0 || requiredDays <= 0) return false;
    if (static_cast<std::size_t>(requiredDays) > n) return false;

    // tails[i] = smallest possible tail value of a strictly increasing subsequence of length i+1.
    std::vector<int> tails;
    tails.reserve(n);

    for (const int price : prices) {
        auto it = std::lower_bound(tails.begin(), tails.end(), price);
        if (it == tails.end()) {
            tails.push_back(price);  // Extend the longest subsequence.
        } else {
            *it = price;  // Replace to maintain minimal tail.
        }
    }

    return static_cast<int>(tails.size()) >= requiredDays;
}

// The core problem is computing the length of the longest strictly increasing subsequence (LIS) of the price array, then comparing that length to `requiredDays`. The classic O(n log n) LIS algorithm using a patience-sorting-like approach works here: maintain a temporary array `tails` where `tails[i]` stores the smallest possible tail value of an increasing subsequence of length `i+1`. Iterate over each price; if it is larger than the last element of `tails`, append it, extending the LIS length. Otherwise, use `lower_bound` on the current valid portion of `tails` to find the first element that is not less than the current price, and replace it. This preserves the invariant that `tails` is sorted ascending, allowing binary search. Edge cases: duplicate prices are not allowed in a strictly increasing subsequence, so `lower_bound` correctly replaces equal values; a single element yields LIS length 1. If `requiredDays` is greater than the vector size, it immediately returns `false`. Time complexity is O(n log n) and auxiliary space O(n) for the `tails` vector (can be reduced to O(n) but that's fine).
