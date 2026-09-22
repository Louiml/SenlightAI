Write a C++ function named `maximumEvenSplit` that takes a non-negative integer `finalSum` and returns a vector of distinct positive even integers whose sum equals `finalSum`. The vector must be the lexicographically largest possible sequence (i.e., as many small distinct evens as possible, in ascending order). If `finalSum` is odd or cannot be expressed as a sum of distinct positive even integers, return an empty vector. The function must handle large inputs up to `10^18` efficiently.
The key insight: only even sums can be formed from even integers. For any even `finalSum`, we can greedily pick the smallest available even integers (2, 4, 6, ...) while ensuring that the remaining sum is strictly greater than the next candidate, so we never leave an unusable remainder. Specifically, iterate `i = 2, 4, 6, ...` and if `(sum - i) > i`, then we can safely take `i` and reduce the remaining sum. Otherwise, the remaining sum itself must be the last element (it will be even and distinct from all previously chosen evens, because it is greater than the last `i` that failed the condition). This guarantees a valid partition into distinct evens. For odd `finalSum`, return empty. Edge case: `finalSum = 0`? The problem assumes positive input; if zero, the loop does nothing and returns empty, which is acceptable. Time complexity is O(√finalSum) because we add terms growing by 2, and the number of terms is roughly sqrt(finalSum) in the worst case. Space complexity is O(√finalSum) for the output vector.
#include <vector>

// Returns a vector of distinct positive even integers summing to finalSum.
// If finalSum is odd or cannot be expressed, returns an empty vector.
std::vector<long long> maximumEvenSplit(const long long finalSum) {
    std::vector<long long> result;
    if (finalSum % 2 != 0) {
        return result; // odd sum impossible
    }

    long long remaining = finalSum;
    for (long long candidate = 2; candidate <= remaining; candidate += 2) {
        if ((remaining - candidate) > candidate) {
            remaining -= candidate;
            result.push_back(candidate);
        } else {
            result.push_back(remaining);
            break;
        }
    }
    return result;
}
#include <cassert>
#include <vector>

// (Solution function definition is assumed to be placed above)

int main() {
    // Odd sums return empty
    assert(maximumEvenSplit(1).empty());
    assert(maximumEvenSplit(7).empty());
    assert(maximumEvenSplit(999999999999999999LL).empty());

    // Simple cases
    assert(maximumEvenSplit(2) == std::vector<long long>{2});
    assert(maximumEvenSplit(4) == std::vector<long long>{4});
    assert(maximumEvenSplit(6) == std::vector<long long>{2, 4});
    assert(maximumEvenSplit(8) == std::vector<long long>{2, 6});
    assert(maximumEvenSplit(10) == std::vector<long long>{2, 8});

    // Larger sums
    assert(maximumEvenSplit(28) == std::vector<long long>{2, 4, 6, 16});
    assert(maximumEvenSplit(30) == std::vector<long long>{2, 4, 6, 18});

    // Very large even number (e.g., 10^10)
    std::vector<long long> big = maximumEvenSplit(10000000000LL);
    long long sum = 0;
    for (long long v : big) sum += v;
    assert(sum == 10000000000LL);
    // Check distinctness
    for (size_t i = 1; i < big.size(); ++i) {
        assert(big[i] > big[i-1]);
    }

    return 0;
}
