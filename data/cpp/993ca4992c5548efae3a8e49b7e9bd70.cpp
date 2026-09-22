// Given four integers, write a C++ function `computeMissingValues` that returns a vector of three integers: the result of subtracting each of the second, third, and fourth smallest inputs from the largest input. Specifically, after sorting the four integers, the largest value `maxVal` is the sum of the three smallest values? No—wait. Actually, the original snippet sorts four numbers and then prints `a[3]-a[0]`, `a[3]-a[1]`, `a[3]-a[2]`. That means: given four numbers, sort them ascending: `x1 ≤ x2 ≤ x3 ≤ x4`. Then output `x4 - x1`, `x4 - x2`, `x4 - x3`. Your task: write a function `vector<long long> differencesOfFour(const array<long long,4>& nums)` that takes four integers (as a fixed-size array or initializer list) and returns a vector of three non-negative integers in that order. The input may contain negative values, zero, duplicates, and very large numbers (up to 10^18). The return values must be computed using 64-bit arithmetic to avoid overflow. Do not print anything; just return the result.

// The solution requires sorting the four input values in ascending order. After sorting, the largest element is at index 3, and the three smallest are at indices 0, 1, and 2. Compute the three differences: `largest - smallest1`, `largest - smallest2`, `largest - smallest3`. These differences are always non-negative because the largest is ≥ each of the others. Duplicate values are handled naturally by sorting; if all four are equal, all differences are zero. Negative inputs are fine because subtraction is well-defined. Edge cases: extremely large positive and negative values; use `long long` (or `int64_t`) to avoid overflow. Sorting four elements can be done with `std::sort` or manually via insertion. The time complexity is O(4 log 4) = O(1) effectively, and space complexity is O(1) auxiliary for the sort (plus the output vector of size 3). Since the input size is fixed at four, the algorithm is constant-time in practice.

#include <array>
#include <vector>
#include <algorithm>

// Given four integers, return {largest - smallest1, largest - smallest2, largest - smallest3}
// after sorting in ascending order.
std::vector<long long> differencesOfFour(const std::array<long long, 4>& nums) {
    std::array<long long, 4> sorted = nums;
    std::sort(sorted.begin(), sorted.end());

    long long largest = sorted[3];
    return {largest - sorted[0], largest - sorted[1], largest - sorted[2]};
}

#include <cassert>
#include <array>
#include <vector>

// Solution function declaration (included here for completeness; normally from header)
std::vector<long long> differencesOfFour(const std::array<long long, 4>& nums);

int main() {
    // Basic case: 1,2,3,4 -> largest 4, differences: 4-1=3, 4-2=2, 4-3=1
    assert(differencesOfFour({1,2,3,4}) == std::vector<long long>({3,2,1}));

    // Unsorted input
    assert(differencesOfFour({4,1,3,2}) == std::vector<long long>({3,2,1}));

    // Negative values: -5, -1, -10, 0 -> sorted: -10,-5,-1,0 -> largest 0, diffs: 10,5,1
    assert(differencesOfFour({-5,-1,-10,0}) == std::vector<long long>({10,5,1}));

    // Duplicates: 5,5,5,5 -> all zeros
    assert(differencesOfFour({5,5,5,5}) == std::vector<long long>({0,0,0}));

    // Duplicates with distinct largest: 2,2,5,2 -> sorted:2,2,2,5 -> diffs:3,3,3
    assert(differencesOfFour({2,2,5,2}) == std::vector<long long>({3,3,3}));

    // Large values: 1000000000000000000, -1000000000000000000, 0, 1
    // sorted: -1e18, 0, 1, 1e18 -> largest=1e18, diffs: 2e18, 1e18, 999999999999999999
    std::vector<long long> result = differencesOfFour({1000000000000000000LL, -1000000000000000000LL, 0, 1});
    assert(result == std::vector<long long>({2000000000000000000LL, 1000000000000000000LL, 999999999999999999LL}));

    return 0;
}
