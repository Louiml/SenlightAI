Write a C++ function `long long countPairsEqualToTwiceMean(const std::vector<long>& a)` that, given a vector of integers, returns the number of ordered pairs of indices `(i, j)` with `i < j` such that the sum of the two elements equals exactly twice the arithmetic mean of the entire vector. The arithmetic mean is computed as `total / n` (using real-number division), and the target sum is `2 * mean`. For example, if the vector is `[1, 2, 3, 4]`, the total is 10, mean is 2.5, target sum is 5.0. Then pairs with sum 5.0 are (0,3) → 1+4=5 and (1,2) → 2+3=5, so the answer is 2. The function must handle vectors of length at least 1, including cases where no pairs exist, where numbers are large (up to 10^9), and where the mean may be non-integer. Note that floating-point precision must be handled carefully — use `long double` for mean and target computations to avoid errors, and compare using exact equality because the target is derived from exact integer sums divided by `n`, but the sum of two integers is an integer, so if `2*total` is not divisible by `n`, no pairs can exist; in that case the answer is 0 (you can check divisibility with integers before performing floating-point comparisons). However, the problem statement explicitly asks for a floating-point approach; for robustness, you can first check if `(2 * total) % n != 0` and return 0 immediately, otherwise proceed with integer arithmetic. But the function must be written to accept the vector and return the count as a `long long` (since for `n` up to 10^5, the number of pairs can be up to ~5×10^9, which fits in `long long`).
#include <cassert>
#include <vector>
#include <iostream>

// Forward declaration of the function to test (included above in a real project)
long long countPairsEqualToTwiceMean(const std::vector<long>& a);

int main() {
    // Example from problem: [1,2,3,4] → pairs (1,4) and (2,3) → 2
    assert(countPairsEqualToTwiceMean({1, 2, 3, 4}) == 2);

    // Single element: no pairs
    assert(countPairsEqualToTwiceMean({7}) == 0);

    // All equal: [5,5,5] → total=15, mean=5, target=10. Pairs: (0,1), (0,2), (1,2) → 3
    assert(countPairsEqualToTwiceMean({5, 5, 5}) == 3);

    // No pairs: total=1+2=3, mean=1.5, target=3.0 → (1,2) sum 3 → 1 pair
    assert(countPairsEqualToTwiceMean({1, 2}) == 1);

    // Non-integer target: [1,2] sum=3, n=2 → 2*3=6 not divisible by 2? 6%2==0 so yes. Actually target=3. So pair (1,2) works. But test with [1,1,2]? total=4, n=3 → 8%3=2 → 0 pairs
    assert(countPairsEqualToTwiceMean({1, 1, 2}) == 0);

    // Larger test: [1,4,5,0] total=10, mean=2.5, target=5 → pairs sum 5: (1,4) and (0,5) → 2
    assert(countPairsEqualToTwiceMean({1, 4, 5, 0}) == 2);

    // Test with large numbers: [1000000000, 1000000000] total=2e9, n=2, target=2e9 → sum=2e9 → pair → 1
    assert(countPairsEqualToTwiceMean({1000000000L, 1000000000L}) == 1);

    // Negative numbers: [-2, 0, 2] total=0, mean=0, target=0 → pairs summing to 0: (-2,2) → 1
    assert(countPairsEqualToTwiceMean({-2, 0, 2}) == 1);

    // Duplicates with multiple pairings: [1,1,2,2,3] total=9, n=5, mean=1.8, target=3.6 → not integer, 0
    assert(countPairsEqualToTwiceMean({1, 1, 2, 2, 3}) == 0);

    // Ensure vector with all zeros: [0,0,0] → total=0, target=0 → pairs: all unordered pairs → 3
    assert(countPairsEqualToTwiceMean({0, 0, 0}) == 3);

    std::cout << "All tests passed!\n";
    return 0;
}
#include <vector>
#include <unordered_map>
#include <numeric>

// Count ordered pairs (i, j) with i < j such that a[i] + a[j] == 2 * (sum / n)
long long countPairsEqualToTwiceMean(const std::vector<long>& a) {
    const long long n = static_cast<long long>(a.size());
    if (n < 2) return 0;

    long long total = 0;
    for (long x : a) {
        total += static_cast<long long>(x);
    }

    // If 2 * total is not divisible by n, no integer target exists
    if ((2 * total) % n != 0) {
        return 0;
    }

    long long target = (2 * total) / n; // exact integer sum required

    long long count = 0;
    std::unordered_map<long long, long long> freq; // value -> frequency seen so far

    for (long x : a) {
        long long need = target - static_cast<long long>(x);
        auto it = freq.find(need);
        if (it != freq.end()) {
            count += it->second;
        }
        freq[static_cast<long long>(x)]++;
    }

    return count;
}
// The solution uses a frequency map (hash map) to count pairs whose sum equals a precomputed target. First, compute the total sum of all elements as `long double`. Compute the mean as `total / n`, then target as `2 * mean`. Because the sum of two integers is an integer, if `2 * total` is not divisible by `n`, the target is not an integer, so no valid pairs exist and the answer is 0. Otherwise, set `target` as an integer `long long` equal to `2 * total / n` (which is exact when divisible). Then iterate through the vector: for each element `a[i]`, the number of previous elements that pair with it is the count of elements equal to `target - a[i]` seen so far. Use an unordered_map (or map) to store frequencies of seen elements. Since we iterate from left to right and only count pairs where the second element appears later, we avoid double counting and satisfy the `i < j` condition naturally. For each `i`, look up `target - a[i]` in the map, add the existing frequency to the answer, then increment the frequency of `a[i]`. This runs in O(n) average time (or O(n log n) with a std::map) and O(n) space. Edge cases: empty vector is not allowed per problem (n ≥ 1), but if n=1, mean is the element itself, target = 2*element, no pair because no other element, answer 0. Duplicates are handled correctly because each occurrence is counted separately. Large numbers up to 10^9 fit in `long`, but the sum of two such numbers can be up to 2×10^9, which fits in `long long` (use `long long` for calculations to be safe). The integer divisibility check avoids floating-point issues entirely, making the solution robust.
