Write a C++ function `maxWineValue` that takes a vector of integers representing the amount of wine in consecutive jars, and returns the maximum total amount that can be collected under the rule: you may collect from a jar only if you skip at least one jar between collections (i.e., you cannot take from two adjacent jars). However, the collection is not limited to a single pass — you may collect from any subset of jars as long as no two chosen jars are adjacent. The function should handle vectors of length 0 to 10^5, with wine amounts that may be negative (representing spoiled wine you are forced to take if you revisit). You must decide the best subset, possibly selecting no jars if all values are negative, in which case the maximum is 0. Return a `long long` to avoid overflow.
#include <cassert>
#include <vector>

// Declare the function (implementation above)
long long maxWineValue(const std::vector<int>& wine);

int main() {
    // Basic cases
    assert(maxWineValue({7}) == 7);
    assert(maxWineValue({-5}) == 0);
    assert(maxWineValue({}) == 0);

    // Two jars: pick the max (or zero if both negative)
    assert(maxWineValue({3, 5}) == 5);
    assert(maxWineValue({-1, -2}) == 0);
    assert(maxWineValue({-1, 4}) == 4);

    // Classic house robber pattern
    assert(maxWineValue({2, 1, 1, 2}) == 4); // pick 2 + 2
    assert(maxWineValue({1, 2, 3, 1}) == 4); // pick 1 + 3
    assert(maxWineValue({2, 7, 9, 3, 1}) == 12); // 2 + 9 + 1

    // Negative values interspersed
    assert(maxWineValue({-1, 5, -1, 5}) == 10); // pick both 5's
    assert(maxWineValue({-2, -3, -1}) == 0);
    assert(maxWineValue({3, -2, 3}) == 6); // pick 3 and 3, skip -2

    // Large values to test long long
    std::vector<int> big(100000, 1000000000);
    assert(maxWineValue(big) == 50000000000000LL); // sum of every other element

    // All negative
    std::vector<int> neg = {-1, -2, -3, -4};
    assert(maxWineValue(neg) == 0);

    // Mixed with zeros
    assert(maxWineValue({0, 0, 0}) == 0);
    assert(maxWineValue({5, 0, 5}) == 10);

    return 0;
}
#include <vector>
#include <algorithm>

// Returns the maximum total wine collectible from a vector of jar values,
// where no two adjacent jars may both be chosen, and negative values may be skipped entirely.
long long maxWineValue(const std::vector<int>& wine) {
    int n = static_cast<int>(wine.size());
    if (n == 0) return 0;
    
    long long prev2 = 0; // dp[i-2]
    long long prev1 = std::max(0LL, static_cast<long long>(wine[0])); // dp[i-1] for i=1
    
    for (int i = 1; i < n; ++i) {
        long long take = prev2 + wine[i];
        long long skip = prev1;
        long long cur = std::max(skip, take);
        prev2 = prev1;
        prev1 = cur;
    }
    
    return prev1; // after loop, prev1 = dp[n]
}
// This is a classic dynamic programming problem similar to the "house robber" but with a twist: negative values are allowed, so the optimal solution may be to skip all negative jars. The DP state is `dp[i]` = maximum value obtainable from the first `i` jars (indices 0..i-1). Recurrence: for jar `i`, either skip it (`dp[i-1]`) or take it (which means we must skip `i-1`, so `dp[i-2] + wine[i]`). However, since negative values are allowed, taking a negative jar is never beneficial unless it somehow enables a positive later jar, but because adjacency restrictions only block immediate neighbors, taking a negative jar never helps a later jar (you could just skip it and still take `i+1`). So the recurrence is simply `dp[i] = max(dp[i-1], dp[i-2] + wine[i])` with base cases `dp[0]=0`, `dp[1]=max(0, wine[0])`. For length 0, return 0. Time complexity O(n), space O(1) by keeping only two previous DP values. Edge case: all negative numbers → answer 0, which is naturally handled by initializing `dp[0]=0`. For n=1, max(0, wine[0]). Use `long long` because wine values can be up to 10^9 and sum up to 10^14, exceeding 32-bit int.
