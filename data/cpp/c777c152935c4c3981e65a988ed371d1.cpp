// Write a C++ function named `maxNonAdjacentSum` that takes a vector of positive integers and returns the maximum possible sum of a subsequence where no two selected elements are adjacent in the original array. The function must handle empty and single-element arrays gracefully, returning 0 for an empty input and the element itself for a single-element input. You are required to implement the solution using space-optimized dynamic programming with O(1) auxiliary space, and the function should be `const`-correct by taking the input by `const std::vector<int>&`. The signature should be `long long maxNonAdjacentSum(const std::vector<int>& nums)`, returning a `long long` to safely accommodate large sums.

The core idea is a dynamic programming approach where we decide for each element whether to include it in the subsequence or not, subject to the constraint that no two adjacent elements are chosen. We maintain two variables representing the best sum up to the previous position and the best sum up to the position before that. For each element, we compute the maximum of (including the current element plus the best up to two positions back) versus (excluding the current element, which gives the best up to the previous position). Starting with `prev2 = 0` (representing the best sum before the first element) and `prev = nums[0]` (best sum up to index 0), we iterate from index 1 onward. For an empty array, we return 0; for a single-element array, the answer is that element. Time complexity is O(n) for n elements, and auxiliary space is O(1). This approach correctly handles edge cases such as all positive numbers, monotonically increasing or decreasing arrays, and varying lengths.

#include <vector>
#include <algorithm>

// Calculate the maximum sum of a non-adjacent subsequence from a vector of positive integers.
// Returns 0 for an empty input. Uses space-optimized dynamic programming.
long long maxNonAdjacentSum(const std::vector<int>& nums) {
    int n = static_cast<int>(nums.size());
    if (n == 0) return 0;
    if (n == 1) return static_cast<long long>(nums[0]);

    long long prev2 = 0;          // best sum up to index i-2
    long long prev = nums[0];     // best sum up to index i-1 (initially index 0)

    for (int i = 1; i < n; ++i) {
        long long include = static_cast<long long>(nums[i]) + prev2; // take current, skip previous
        long long exclude = prev;                                    // skip current, take previous best
        long long current = std::max(include, exclude);
        prev2 = prev;
        prev = current;
    }

    return prev;
}

#include <cassert>
#include <vector>

// Forward declaration of the solution function (or include the header if separated)
long long maxNonAdjacentSum(const std::vector<int>& nums);

int main() {
    // Basic case from the original snippet: {2,1,4,9} -> best is 2+9=11 or 1+9=10 or 4+? -> 11
    assert(maxNonAdjacentSum({2, 1, 4, 9}) == 11);

    // Empty vector returns 0
    assert(maxNonAdjacentSum({}) == 0);

    // Single element returns that element
    assert(maxNonAdjacentSum({5}) == 5);

    // Two elements: choose the larger one
    assert(maxNonAdjacentSum({3, 7}) == 7);

    // All same values: pick every other element
    assert(maxNonAdjacentSum({4, 4, 4, 4}) == 8);

    // Large numbers to check long long behavior
    assert(maxNonAdjacentSum({1000000, 1, 1000000, 1, 1000000}) == 3000000);

    // Alternating pattern
    assert(maxNonAdjacentSum({1, 2, 3, 4, 5}) == 9); // 1+3+5=9

    // Negative? Not required but ensure works if given positive ints only
    assert(maxNonAdjacentSum({10, 1, 1, 10}) == 20); // 10 + 10

    // Longer sequence with a clear best path
    assert(maxNonAdjacentSum({5, 5, 10, 100, 10, 5}) == 110); // 5 + 100 + 5

    // Input with one large middle value
    assert(maxNonAdjacentSum({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}) == 30); // Sum of even indices: 1+3+5+7+9=25 vs odd: 2+4+6+8+10=30

    return 0;
}
