You are given two arrays: a list of `n` required power levels for tasks, and a list of `m` available air conditioner units, each described by its cooling power and price. A task with required power `R` can be completed by any air conditioner with power at least `R`. Each air conditioner can be used any number of times, but its price is paid per use. Write a function `long long minTotalCost(const std::vector<int>& requiredPowers, const std::vector<std::pair<int,int>>& units)`, where each pair is `(power, price)`. If multiple units share the same power, only the cheapest one matters. The function should return the minimum total cost to satisfy all required power levels in the order they are given (each task independently, not combined). If no air conditioner can satisfy a given required power, the function should return `-1`. The input arrays may be unsorted, and power/price values are positive integers. The total cost may exceed 32-bit integers, so use `long long`.
#include <cassert>
#include <vector>
#include <utility>

// The solution function is assumed to be declared above.
// (In a real test, include the solution code.)

int main() {
    // Basic test: distinct powers and requirements
    {
        std::vector<int> req = {5, 10};
        std::vector<std::pair<int,int>> units = {{10, 100}, {5, 50}, {8, 80}};
        // For 5: cheapest with power>=5 is 50 (power 5)
        // For 10: cheapest with power>=10 is 100 (power 10) total=150
        assert(minTotalCost(req, units) == 150);
    }

    // Duplicate required powers, same unit used multiple times
    {
        std::vector<int> req = {3, 3, 7};
        std::vector<std::pair<int,int>> units = {{3, 10}, {7, 20}};
        // 3+3+7 = 10+10+20 = 40
        assert(minTotalCost(req, units) == 40);
    }

    // Multiple units same power, keep cheapest
    {
        std::vector<int> req = {4};
        std::vector<std::pair<int,int>> units = {{4, 30}, {4, 10}, {4, 20}};
        // Only cheapest power 4 price 10 matters.
        assert(minTotalCost(req, units) == 10);
    }

    // Impractical requirement (too high) -> -1
    {
        std::vector<int> req = {1, 100};
        std::vector<std::pair<int,int>> units = {{1, 5}, {50, 10}};
        // 1 can be met, 100 cannot.
        assert(minTotalCost(req, units) == -1);
    }

    // Unsorted input arrays
    {
        std::vector<int> req = {10, 1, 5};
        std::vector<std::pair<int,int>> units = {{5, 2}, {20, 1}, {1, 9}};
        // req sorted: 1 -> cheapest with power>=1 is min(2,1,9)=1 (power20)
        // req 5 -> cheapest with power>=5 is min(2,1)=1 (power20)
        // req 10 -> cheapest with power>=10 is 1 (power20) total=3
        assert(minTotalCost(req, units) == 3);
    }

    // Empty requirements -> 0
    {
        std::vector<int> req = {};
        std::vector<std::pair<int,int>> units = {{1, 2}};
        assert(minTotalCost(req, units) == 0);
    }

    // Large cost overflow check (using long long)
    {
        std::vector<int> req = {100};
        std::vector<std::pair<int,int>> units = {{100, 1000000000}};
        // cost = 1e9 fits in long long
        assert(minTotalCost(req, units) == 1000000000LL);
    }

    // Edge: only one unit, all requirements met by it
    {
        std::vector<int> req = {1, 2, 3};
        std::vector<std::pair<int,int>> units = {{5, 7}};
        // All three use price 7 each -> 21
        assert(minTotalCost(req, units) == 21);
    }

    // Edge: requirement exactly equal to a unit power that is not the cheapest overall but is cheapest for that power
    {
        std::vector<int> req = {8};
        std::vector<std::pair<int,int>> units = {{8, 4}, {8, 3}, {9, 2}};
        // For 8: available powers 8 and 9, cheapest among them is min(3,2)=2
        assert(minTotalCost(req, units) == 2);
    }

    // Multiple requirements, some require higher power than others, ensure suffix min works
    {
        std::vector<int> req = {2, 9};
        std::vector<std::pair<int,int>> units = {{2, 20}, {5, 10}, {9, 5}};
        // 2 -> cheapest from power>=2: min(20,10,5)=5 (power9)
        // 9 -> cheapest from power>=9: 5 (power9) total=10
        assert(minTotalCost(req, units) == 10);
    }

    return 0;
}
#include <vector>
#include <algorithm>
#include <climits>
#include <cstdint>

// Returns the minimum total cost to satisfy all required power levels,
// or -1 if any required power cannot be met.
long long minTotalCost(const std::vector<int>& requiredPowers,
                       const std::vector<std::pair<int,int>>& units) {
    if (requiredPowers.empty()) return 0;
    if (units.empty()) return -1;

    // Group units by power, keeping the cheapest price for each power.
    std::vector<std::pair<int,int>> uniqueUnits;
    {
        // Sort by power then by price to easily keep cheapest per power.
        std::vector<std::pair<int,int>> sortedUnits = units;
        std::sort(sortedUnits.begin(), sortedUnits.end(),
                  [](const auto& a, const auto& b) {
                      if (a.first != b.first) return a.first < b.first;
                      return a.second < b.second;
                  });
        for (size_t i = 0; i < sortedUnits.size(); ++i) {
            // If this is the first occurrence of this power, keep it.
            if (i == 0 || sortedUnits[i].first != sortedUnits[i-1].first) {
                uniqueUnits.push_back(sortedUnits[i]);
            }
            // Otherwise skip because sorted by price means the first is cheapest.
        }
    }

    // Sort required powers ascending.
    std::vector<int> sortedReq = requiredPowers;
    std::sort(sortedReq.begin(), sortedReq.end());

    // Build suffix-min array over uniqueUnits.
    size_t m = uniqueUnits.size();
    std::vector<int> suffixMin(m + 1, INT_MAX);
    suffixMin[m] = INT_MAX;
    for (size_t i = m; i-- > 0; ) {
        suffixMin[i] = std::min(suffixMin[i+1], uniqueUnits[i].second);
    }

    // Two-pointer over sorted required powers and units.
    long long total = 0;
    size_t j = 0; // index into uniqueUnits
    for (int req : sortedReq) {
        // Advance j until we find a unit with power >= req.
        while (j < m && uniqueUnits[j].first < req) {
            ++j;
        }
        if (j == m) {
            return -1; // No unit powerful enough.
        }
        // Best price from j onward is suffixMin[j].
        total += static_cast<long long>(suffixMin[j]);
    }

    return total;
}
// The solution sorts the required powers in ascending order and processes them with a two-pointer technique. First, group the air conditioners by power, keeping only the minimum price for each distinct power level, then sort these distinct entries by power ascending. For queries in ascending power order, we maintain a pointer `j` over the sorted unit list. For each required power `R`, advance `j` until we find the first unit with power ≥ `R` (if none exists, return `-1` immediately). Among all units from that position onward, the cheapest price is the minimum over the suffix starting at `j`. Precompute a suffix-minimum array `suffixMin` where `suffixMin[j]` = minimum price among units with index ≥ `j`. Then the cost for `R` is `suffixMin[j]`. Sum these costs for all required powers. Because required powers are processed sorted, `j` only moves forward, making the overall loop O(n log n + m log m) due to sorting, with O(n + m) auxiliary space. Edge cases: duplicate required powers, units with same power (only keep cheapest), negative or tiny required power less than the smallest unit power, and required power exceeding all unit powers (return -1).
