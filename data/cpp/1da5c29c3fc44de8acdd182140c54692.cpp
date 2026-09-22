/*
Write a C++ function that takes two vectors of positive integers, `regular` and `express`, representing the travel costs for each segment on two different train lines, and an integer `expressCost` representing the cost to switch from the regular line to the express line. The function must return a vector of `long long` values where the `i`-th element (0-indexed) is the minimum total cost to travel from the start (before segment 1) to the end of segment `i+1`, given that you start on the regular line at cost 0. You may switch between lines at any segment boundary, but switching from regular to express costs `expressCost` (incurred at the moment of switching, before traveling the express segment), while switching from express to regular is free. You must always travel at least the current segment on one of the two lines. The input vectors have equal size, and you should not modify the original vectors. If the vectors are empty, return an empty vector. The result for each `i` is the minimum of the two possible line states after completing that segment.
*/

#include <vector>
#include <algorithm>
#include <cstddef>

// Returns a vector where the i-th element is the minimum cost to complete
// segment i+1, starting on the regular line at cost 0.
// Switches: regular -> express costs expressCost, express -> regular is free.
std::vector<long long> minimumTravelCosts(
    const std::vector<int>& regular,
    const std::vector<int>& express,
    int expressCost) 
{
    const std::size_t n = regular.size();
    std::vector<long long> results;
    results.reserve(n);

    // Rolling DP: variables for previous segment state.
    long long prevRegular = 0;          // dp[0][0] before any segment
    long long prevExpress = expressCost; // dp[0][1] before any segment (switch at start)

    for (std::size_t i = 0; i < n; ++i) {
        // Compute current segment costs
        long long curRegular = std::min(prevRegular, prevExpress) + regular[i];
        long long curExpress = std::min(prevExpress, prevRegular + expressCost) + express[i];
        results.push_back(std::min(curRegular, curExpress));

        // Update for next iteration
        prevRegular = curRegular;
        prevExpress = curExpress;
    }

    return results;
}

#include <cassert>
#include <vector>
#include <cstddef>

// Assume the solution is included above.

int main() {
    // Example 1: basic
    std::vector<int> reg1 = {2, 3};
    std::vector<int> exp1 = {5, 1};
    assert(minimumTravelCosts(reg1, exp1, 4) == std::vector<long long>({4, 5}));

    // Example 2: switching advantageous
    std::vector<int> reg2 = {10, 1, 1};
    std::vector<int> exp2 = {5, 5, 5};
    assert(minimumTravelCosts(reg2, exp2, 3) == std::vector<long long>({8, 9, 10}));

    // Example 3: never switch express
    std::vector<int> reg3 = {1, 1, 1};
    std::vector<int> exp3 = {100, 100, 100};
    assert(minimumTravelCosts(reg3, exp3, 1) == std::vector<long long>({1, 2, 3}));

    // Example 4: express always better after switch
    std::vector<int> reg4 = {10, 10};
    std::vector<int> exp4 = {1, 1};
    assert(minimumTravelCosts(reg4, exp4, 5) == std::vector<long long>({6, 7}));

    // Example 5: empty input
    assert(minimumTravelCosts({}, {}, 1).empty());

    // Example 6: single segment
    std::vector<int> reg5 = {7};
    std::vector<int> exp5 = {3};
    assert(minimumTravelCosts(reg5, exp5, 2) == std::vector<long long>({5}));

    // Example 7: expressCost large
    std::vector<int> reg6 = {2, 2};
    std::vector<int> exp6 = {1, 1};
    assert(minimumTravelCosts(reg6, exp6, 100) == std::vector<long long>({3, 4}));

    // Example 8: switching later is better
    std::vector<int> reg7 = {1, 5, 1};
    std::vector<int> exp7 = {3, 1, 3};
    assert(minimumTravelCosts(reg7, exp7, 2) == std::vector<long long>({3, 6, 7}));

    // Example 9: all equal costs
    std::vector<int> reg8 = {4, 4};
    std::vector<int> exp8 = {4, 4};
    assert(minimumTravelCosts(reg8, exp8, 0) == std::vector<long long>({4, 8}));

    // Example 10: multiple switches
    std::vector<int> reg9 = {1, 10, 1};
    std::vector<int> exp9 = {10, 1, 10};
    assert(minimumTravelCosts(reg9, exp9, 2) == std::vector<long long>({3, 6, 7}));

    return 0;
}

// The problem is a classic dynamic programming (DP) with two states per step: `dp[i][0]` = minimum cost to finish segment `i` while being on the regular line, and `dp[i][1]` = minimum cost while being on the express line. Initialize `dp[0][0] = 0` (start before segment 1, already on regular) and `dp[0][1] = expressCost` (you could switch to express before any segment, paying the cost) — but note we treat segment indices starting at 1 for convenience. For each segment `i` from 1 to `n`:
// - To be on regular after segment `i`: you can stay on regular from previous regular (`dp[i-1][0] + regular[i]`) or come from express (for free) (`dp[i-1][1] + regular[i]`). So `dp[i][0] = min(dp[i-1][0], dp[i-1][1]) + regular[i]`.
// - To be on express after segment `i`: you can stay on express (`dp[i-1][1] + express[i]`) or switch from regular (pay `expressCost`) (`dp[i-1][0] + expressCost + express[i]`). So `dp[i][1] = min(dp[i-1][1], dp[i-1][0] + expressCost) + express[i]`.
// The answer for segment `i` is `min(dp[i][0], dp[i][1])`. Edge cases: empty input returns empty; positive integers guarantee no overflow in long long for reasonable sizes; expressCost can be any positive int. Complexity: O(n) time, O(1) extra space if we only keep previous state (or O(n) if storing all, but we can compress). For clarity, the reference solution uses a rolling DP with two variables per step, but the provided solution will store a vector for results only.
