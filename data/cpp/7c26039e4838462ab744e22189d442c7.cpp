Write a C++ function that takes a positive integer `k` and a list of `n` pairs `(a, b)`, where `a` is a positive integer between 1 and 100,000 and `b` is a positive integer (the count of items with that value `a`). The function should return the smallest value `a` such that the cumulative count of items with values ≤ `a` is at least `k`. The input is structured similarly to the code snippet: first an integer `n` (number of types), then `k` (the target rank), followed by `n` pairs. But for a standalone function, you'll receive two vectors: `values` (the distinct `a`s) and `counts` (the corresponding `b`s), plus the integer `k`. Your function must return the smallest `a` that satisfies the condition. Assume `k` is always within the total sum of all counts. Handle cases where `a` values may not be consecutive and may be repeated in the input (if repeated, sum the counts before processing). The function should work for `n` up to 100,000 and large `k` up to 10^9, and each `b` up to 10^9.
#include <cassert>
#include <vector>
#include <cstdint>

// Assume the solution function is defined above
// (omitted here for brevity, but in the actual file include it)

int main() {
    using int64 = std::int64_t;

    // Test 1: basic case
    {
        std::vector<int> vals = {1, 2, 3};
        std::vector<int64> cnts = {1, 2, 3};
        assert(findKthSmallestValue(vals, cnts, 1) == 1);
        assert(findKthSmallestValue(vals, cnts, 2) == 2);
        assert(findKthSmallestValue(vals, cnts, 3) == 2);
        assert(findKthSmallestValue(vals, cnts, 4) == 3);
        assert(findKthSmallestValue(vals, cnts, 6) == 3);
    }

    // Test 2: duplicates in values
    {
        std::vector<int> vals = {5, 5, 5, 3};
        std::vector<int64> cnts = {2, 3, 1, 4};
        // Aggregated: 3->4, 5->6
        assert(findKthSmallestValue(vals, cnts, 1) == 3);
        assert(findKthSmallestValue(vals, cnts, 4) == 3);
        assert(findKthSmallestValue(vals, cnts, 5) == 5);
        assert(findKthSmallestValue(vals, cnts, 10) == 5);
    }

    // Test 3: large counts, k at boundary
    {
        std::vector<int> vals = {100000, 1, 50000};
        std::vector<int64> cnts = {1000000000LL, 1LL, 2LL};
        assert(findKthSmallestValue(vals, cnts, 1) == 1);
        assert(findKthSmallestValue(vals, cnts, 2) == 50000);
        assert(findKthSmallestValue(vals, cnts, 3) == 100000);
        // total = 1000000003
        assert(findKthSmallestValue(vals, cnts, 1000000003LL) == 100000);
    }

    // Test 4: single value
    {
        std::vector<int> vals = {42};
        std::vector<int64> cnts = {7};
        assert(findKthSmallestValue(vals, cnts, 1) == 42);
        assert(findKthSmallestValue(vals, cnts, 7) == 42);
    }

    // Test 5: values with gaps
    {
        std::vector<int> vals = {10, 20, 30};
        std::vector<int64> cnts = {5, 5, 5};
        assert(findKthSmallestValue(vals, cnts, 5) == 10);
        assert(findKthSmallestValue(vals, cnts, 6) == 20);
        assert(findKthSmallestValue(vals, cnts, 10) == 20);
        assert(findKthSmallestValue(vals, cnts, 11) == 30);
        assert(findKthSmallestValue(vals, cnts, 15) == 30);
    }

    // Test 6: all same value multiple times
    {
        std::vector<int> vals = {7, 7, 7};
        std::vector<int64> cnts = {1, 1, 1};
        assert(findKthSmallestValue(vals, cnts, 1) == 7);
        assert(findKthSmallestValue(vals, cnts, 3) == 7);
    }

    return 0;
}
#include <vector>
#include <cstdint>

using int64 = std::int64_t;

// Returns the smallest value a such that the cumulative count of values <= a
// is at least k. The input lists values and their corresponding counts;
// duplicate values are aggregated before processing.
int findKthSmallestValue(const std::vector<int>& values,
                         const std::vector<int64>& counts,
                         int64 k) {
    constexpr int MAX_A = 100000;
    std::vector<int64> freq(MAX_A + 1, 0);

    // Aggregate counts for each value
    for (std::size_t i = 0; i < values.size(); ++i) {
        freq[values[i]] += counts[i];
    }

    // Walk from smallest to largest, accumulating counts
    int64 cumulative = 0;
    for (int a = 1; a <= MAX_A; ++a) {
        cumulative += freq[a];
        if (cumulative >= k) {
            return a;  // k is reached at this value
        }
    }

    // Should never reach here if input is valid (k <= total sum)
    return -1;  // Indicate error; caller should ensure k is valid
}
// The solution approach: first aggregate counts for each distinct value `a` because the input may contain duplicates. Use an array or vector of size 100001 (since `a` is between 1 and 100,000) to accumulate counts. Then iterate `a` from 1 upward, keeping a running cumulative sum. If cumulative sum ≥ `k`, that `a` is the answer. Since `k` is guaranteed to be reachable, the loop will terminate. Edge cases: (1) duplicate `a` values—must sum their counts before comparing. (2) `a` may start from a high value, but iteration from 1 is safe because counts for missing values are zero. (3) Large values: use `long long` for counts and cumulative sum to avoid overflow. Time complexity is O(maxA + n) = O(100,000 + n) per call, which is effectively linear. Space complexity is O(maxA) for the count array.
