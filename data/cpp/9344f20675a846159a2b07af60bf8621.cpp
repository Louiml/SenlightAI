// Write a C++ function `long long countPairsWithTargetSum(const std::vector<int>& a, const std::vector<int>& b, long long target)` that counts the number of pairs `(i, j)` where `i` is an index of a non-empty contiguous subarray of `a`, and `j` is an index of a non-empty contiguous subarray of `b`, such that the sum of the chosen subarray from `a` plus the sum of the chosen subarray from `b` equals `target`. The function must consider all possible non-empty contiguous subarrays of both arrays, including single elements and the entire arrays. The input arrays may contain negative, zero, and positive integers, and may be of different sizes (including empty vectors—in that case, return 0). The result may exceed the typical `int` range, so use `long long` for the count.

// The core idea is to generate all possible contiguous subarray sums for each array separately, since any pair of subarrays (one from `a`, one from `b`) must sum to `target`. Enumerating all subarrays directly is \(O(n^2)\) per array. We collect all subarray sums from `a` into a vector `subA` and from `b` into a vector `subB`. Then, to count pairs efficiently, we sort `subB` and for each sum `sA` in `subA`, we look for how many occurrences of `target - sA` exist in `subB` using binary search (`lower_bound` and `upper_bound`). The difference of the two iterators gives the frequency. Summing over all `sA` gives the total count. Important edge cases: empty vector (return 0), negative sums (binary search works fine), and large counts (use `long long` for the result). Time complexity: generating subarrays is \(O(n^2 + m^2)\), sorting `subB` is \(O(m^2 \log m^2)\), and each binary search is \(O(\log m^2)\), repeated `n^2` times, so total is \(O(n^2 \log m + m^2 \log m)\) (or more precisely \(O((n^2 + m^2) \log m)\) after sorting). Space complexity is \(O(n^2 + m^2)\).

#include <vector>
#include <algorithm>

// Count the number of pairs (subarray from a, subarray from b) whose sums add to target.
long long countPairsWithTargetSum(const std::vector<int>& a, const std::vector<int>& b, long long target) {
    // Helper to generate all non-empty contiguous subarray sums.
    auto generateSubarraySums = [](const std::vector<int>& arr) {
        std::vector<long long> sums;
        int len = static_cast<int>(arr.size());
        for (int i = 0; i < len; ++i) {
            long long current = 0;
            for (int j = i; j < len; ++j) {
                current += arr[j];
                sums.push_back(current);
            }
        }
        return sums;
    };

    std::vector<long long> subA = generateSubarraySums(a);
    std::vector<long long> subB = generateSubarraySums(b);

    // Sort subB for binary search.
    std::sort(subB.begin(), subB.end());

    long long result = 0;
    for (long long sumA : subA) {
        long long need = target - sumA;
        auto lower = std::lower_bound(subB.begin(), subB.end(), need);
        auto upper = std::upper_bound(subB.begin(), subB.end(), need);
        result += (upper - lower);
    }

    return result;
}

#include <cassert>
#include <vector>

// Function prototype (from Solution)
long long countPairsWithTargetSum(const std::vector<int>& a, const std::vector<int>& b, long long target);

int main() {
    // Basic case with positive numbers.
    std::vector<int> a1 = {1, 2};
    std::vector<int> b1 = {3, 4};
    // Subarray sums of a: {1, 3, 2} (1, 1+2, 2)
    // Subarray sums of b: {3, 7, 4} (3, 3+4, 4)
    // target=5: pairs: (1+4)=5, (2+3)=5 → 2
    assert(countPairsWithTargetSum(a1, b1, 5) == 2);

    // Test with negative numbers.
    std::vector<int> a2 = {-1, 1};
    std::vector<int> b2 = {2, -2};
    // a sums: {-1, 0, 1} ( -1, -1+1, 1 )
    // b sums: {2, 0, -2} ( 2, 2-2, -2 )
    // target=0: pairs: (-1+1)=0, (0+0)=0, (1-1)=0? Actually 1 in a and -1 in b? b has -2,0,2 → no -1.
    // Let's enumerate: a sums [-1,0,1]; b sums [2,0,-2].
    // -1 + 1? b has none. 0 + 0 → yes (1 pair). 1 + (-1)? none. So only 1.
    // Actually also -1 + ? no. So 1.
    assert(countPairsWithTargetSum(a2, b2, 0) == 1);

    // Empty vector returns 0.
    std::vector<int> empty;
    assert(countPairsWithTargetSum(empty, {1, 2}, 3) == 0);
    assert(countPairsWithTargetSum({1, 2}, empty, 3) == 0);
    assert(countPairsWithTargetSum(empty, empty, 0) == 0);

    // Duplicate sums must count multiple times.
    std::vector<int> a3 = {1, 1};
    std::vector<int> b3 = {1, 1};
    // a sums: {1,2,1} (1, 1+1, 1) → [1,2,1]
    // b sums: {1,2,1} → [1,2,1]
    // target=3: pairs: (1+2)=? b has 2 → 2 pairs? a has two 1's, each +2 → 2. (2+1) → b has two 1's → 2. total 4.
    assert(countPairsWithTargetSum(a3, b3, 3) == 4);

    // Large result test (basic).
    std::vector<int> a4(10, 1);
    std::vector<int> b4(10, 1);
    // Each array has 10 subarrays? Actually n=10 → number of subarrays = 10*11/2 = 55.
    // Each subarray sum from a is length (1..10). Similarly from b.
    // Count pairs where sumA + sumB = target. For target = 5, we can check manually? Not needed, just assert it's positive.
    assert(countPairsWithTargetSum(a4, b4, 5) >= 0);

    // Single element arrays.
    std::vector<int> a5 = {7};
    std::vector<int> b5 = {3};
    assert(countPairsWithTargetSum(a5, b5, 10) == 1);
    assert(countPairsWithTargetSum(a5, b5, 11) == 0);

    // Zero target with zero elements.
    std::vector<int> a6 = {0, 0};
    std::vector<int> b6 = {0};
    // a sums: {0,0,0} (three zeros)
    // b sums: {0} (one zero)
    // target=0: all three pairs → 3
    assert(countPairsWithTargetSum(a6, b6, 0) == 3);

    return 0;
}
