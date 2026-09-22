Write a C++ function `long long minimumCostToCompletePrefix(const std::vector<int>& prices)` that takes an array of non-negative integers representing the daily price of a stock over `N` days. The goal is to compute the minimum total cost required to ensure that, by the end of the entire sequence, every prefix of the array (from index 0 to any index `i`) contains at least one day where the stock was bought at the maximum price of that prefix. To achieve this, on any day you may buy the stock at that day's price, but you can only buy if you have not bought at a higher-priced day earlier in the same prefix. The optimal strategy is: scan days from left to right, and whenever you encounter a new all-time high price (strictly greater than any seen so far), you must buy that day. The cost contributed by that buy is the difference between that new high price and each day’s price from the day after the previous high (or from start) up to and including the new high day, summed. Return the total cost as a 64-bit integer. For example, given prices `{3, 1, 2, 5, 4}`, the new highs occur at indices 0 (price 3), 3 (price 5). Cost = (3-3) + (5-3) + (5-1) + (5-2) + (5-5) = 0+2+4+3+0 = 9. If the array is already non-increasing, the answer is 0. The input size can be up to 200,000 and prices up to 1e9, so use efficient algorithms and avoid overflow.

The algorithm processes the price array from left to right, maintaining a variable `last` which is the index of the last day where a new all-time high occurred (initially -1). For each index `i`, if `prices[i]` is a new maximum (i.e., greater than every price before it, including the previous recorded high), we add to the answer the sum over all `j` from `last+1` to `i` of `(prices[i] - prices[j])`. Then set `last = i`. This works because the optimal strategy is to buy exactly on each new record high day; any earlier purchase would be at a lower price and would not cover the requirement for later prefixes. The sum can be computed efficiently using a running prefix sum of prices to avoid an inner loop: the cost for the segment is `(i - last) * prices[i] - (prefix[i+1] - prefix[last+1])`. Edge cases: empty array (return 0), all-equal prices (no new highs except first, cost 0), and very large prices requiring `long long` (or `long long` for the result). Time complexity O(N) for one pass plus prefix sum, space O(N) for prefix sum (can be O(1) if we maintain cumulative sum on the fly). The main tricky part is handling the starting case where the first element is always a new high; also ensure that if prices are strictly decreasing, no further highs occur, so the cost is just the first element minus itself (0). Use `long long` for all arithmetic to avoid overflow.

#include <vector>
#include <cstdint>

// Given a vector of non-negative stock prices, return the minimum total cost
// to buy on every new all-time high day, where each buy contributes the
// difference between that high and all prices from the previous high+1 to that day.
long long minimumCostToCompletePrefix(const std::vector<int>& prices) {
    const int n = static_cast<int>(prices.size());
    if (n == 0) return 0LL;

    // Prefix sums as long long to avoid overflow
    std::vector<long long> prefix(n + 1, 0LL);
    for (int i = 0; i < n; ++i) {
        prefix[i + 1] = prefix[i] + static_cast<long long>(prices[i]);
    }

    int last = -1;          // index of last new high (exclusive)
    long long result = 0LL;
    long long currentMax = -1LL; // maximum price seen so far

    for (int i = 0; i < n; ++i) {
        if (static_cast<long long>(prices[i]) > currentMax) {
            // New high at index i
            // Add cost from last+1 to i
            long long count = i - last; // number of days in segment
            long long sumSegment = prefix[i + 1] - prefix[last + 1];
            result += count * static_cast<long long>(prices[i]) - sumSegment;
            last = i;
            currentMax = prices[i];
        }
    }

    return result;
}

#include <cassert>
#include <vector>

// Function under test (declared above, implemented elsewhere)
long long minimumCostToCompletePrefix(const std::vector<int>& prices);

int main() {
    // Example from the task
    assert(minimumCostToCompletePrefix({3,1,2,5,4}) == 9LL);

    // Empty case
    assert(minimumCostToCompletePrefix({}) == 0LL);

    // Single element
    assert(minimumCostToCompletePrefix({7}) == 0LL);

    // Non-increasing (no new highs after first)
    assert(minimumCostToCompletePrefix({5,4,3,2,1}) == 0LL);

    // Strictly increasing
    assert(minimumCostToCompletePrefix({1,2,3,4}) == (2-1) + (3-1)+(3-2) + (4-1)+(4-2)+(4-3) );
    // Compute manually: new highs at idx0(1), idx1(2), idx2(3), idx3(4)
    // Segment0: (1-1)=0
    // Segment1: (2-2)+(2-1)=0+1=1
    // Segment2: (3-3)+(3-2)+(3-1)=0+1+2=3
    // Segment3: (4-4)+(4-3)+(4-2)+(4-1)=0+1+2+3=6
    // Total = 0+1+3+6=10
    assert(minimumCostToCompletePrefix({1,2,3,4}) == 10LL);

    // Large values to check overflow handling
    std::vector<int> large(200000, 1000000000);
    // All equal: first high at idx0, cost 0
    assert(minimumCostToCompletePrefix(large) == 0LL);

    // Increasing large values
    std::vector<int> inc;
    for (int i = 1; i <= 100000; ++i) inc.push_back(i);
    // Cost formula: for each new high at position i (0-indexed), sum_{j=0..i} (i+1 - (j+1)) = sum_{k=1..i+1} k = (i+1)(i+2)/2
    // Total = sum_{m=1..100000} m(m+1)/2 = 1/2 * sum(m^2+m)
    // We can compute with long long
    long long expected = 0;
    for (long long m = 1; m <= 100000; ++m) expected += m * (m + 1) / 2;
    assert(minimumCostToCompletePrefix(inc) == expected);

    // Mixed pattern
    assert(minimumCostToCompletePrefix({5,1,4,6,2}) == 
           (5-5) + (6-5)+(6-1)+(6-4)+(6-6) + 0? Actually new highs at idx0 (5), idx3 (6). Segment0: 0, Segment3: (6-5)+(6-1)+(6-4)+(6-6)=1+5+2+0=8. Total 8.
    assert(minimumCostToCompletePrefix({5,1,4,6,2}) == 8LL);

    return 0;
}
