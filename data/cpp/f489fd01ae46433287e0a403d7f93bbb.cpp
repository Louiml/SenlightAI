/*
Given an array of up to 40 non-negative integers (each ≤ 10^9), write a C++ function `long long maxModularSubsetSum(const std::vector<int>& nums)` that returns the maximum possible value of `(sum of a non-empty subset) mod 1'000'000'000` (i.e., modulo 10^9). The subset must be non-empty; if all elements are zero or the best subset sum modulo is zero, return 0. The function must handle large arrays efficiently using the meet-in-the-middle technique, since enumerating all 2^40 subsets would be too slow. The result should fit in a `long long`.
*/
#include <vector>
#include <algorithm>
#include <cstdint>

// Returns the maximum possible (sum of a non-empty subset) mod 1'000'000'000.
long long maxModularSubsetSum(const std::vector<int>& nums) {
    const long long MOD = 1000000000LL;
    const int n = static_cast<int>(nums.size());
    if (n == 0) return 0;

    // Split array into two halves.
    const int leftSize = n / 2;
    const int rightSize = n - leftSize;

    // Generate all subset sums for left half.
    std::vector<long long> leftSums;
    leftSums.reserve(1ULL << leftSize);
    for (int mask = 0; mask < (1 << leftSize); ++mask) {
        long long sum = 0;
        for (int i = 0; i < leftSize; ++i) {
            if (mask & (1 << i)) sum += nums[i];
        }
        leftSums.push_back(sum);
    }

    // Generate all subset sums for right half.
    std::vector<long long> rightSums;
    rightSums.reserve(1ULL << rightSize);
    for (int mask = 0; mask < (1 << rightSize); ++mask) {
        long long sum = 0;
        for (int i = 0; i < rightSize; ++i) {
            if (mask & (1 << i)) sum += nums[leftSize + i];
        }
        rightSums.push_back(sum);
    }

    // Sort right sums for binary search.
    std::sort(rightSums.begin(), rightSums.end());

    long long answer = 0;

    // For each left sum, find the best right sum.
    for (long long left : leftSums) {
        // Candidate 1: largest right such that left + right < MOD.
        long long limit = MOD - 1 - left;
        auto it = std::upper_bound(rightSums.begin(), rightSums.end(), limit);
        if (it != rightSums.begin()) {
            --it;
            answer = std::max(answer, left + *it);
        }

        // Candidate 2: largest right overall (may wrap around).
        long long candidate2 = (left + rightSums.back()) % MOD;
        answer = std::max(answer, candidate2);
    }

    return answer;
}
#include <cassert>
#include <vector>
#include <cstdint>

// Assume the solution function is declared above.
long long maxModularSubsetSum(const std::vector<int>& nums);

int main() {
    // Basic cases
    assert(maxModularSubsetSum({5}) == 5);
    assert(maxModularSubsetSum({0}) == 0);
    assert(maxModularSubsetSum({0, 0}) == 0);

    // Simple sum less than MOD
    assert(maxModularSubsetSum({1, 2, 3}) == 6);

    // Sum exactly MOD -> best is MOD-1 (choose MOD-1 or other combo)
    // For {999999999, 1} sum=1000000000, mod=0, but we can choose just 999999999 -> 999999999
    assert(maxModularSubsetSum({999999999, 1}) == 999999999);

    // Wrapping case: {600000000, 600000000} sum=1200000000 mod=200000000, but best is 600000000
    assert(maxModularSubsetSum({600000000, 600000000}) == 600000000);

    // Larger test: all zeros
    std::vector<int> zeros(40, 0);
    assert(maxModularSubsetSum(zeros) == 0);

    // Mixed: want max mod, e.g., {999999998, 999999997, 1} -> best: 999999998+1=999999999
    assert(maxModularSubsetSum({999999998, 999999997, 1}) == 999999999);

    // Random small test with brute force verification for n=10
    {
        std::vector<int> arr = {123, 456, 789, 111, 222, 333, 444, 555, 666, 777};
        long long expected = 0;
        // brute force all non-empty subsets
        int n = (int)arr.size();
        for (int mask = 1; mask < (1 << n); ++mask) {
            long long sum = 0;
            for (int i = 0; i < n; ++i) {
                if (mask & (1 << i)) sum += arr[i];
            }
            expected = std::max(expected, sum % 1000000000LL);
        }
        assert(maxModularSubsetSum(arr) == expected);
    }

    // Check with n=40 and large values (performance test not asserting correctness fully but runs)
    std::vector<int> big(40, 1000000000);
    long long result = maxModularSubsetSum(big);
    assert(result >= 0 && result < 1000000000LL);

    return 0;
}
// The problem asks for the maximum subset sum modulo 10^9. Since n ≤ 40, enumerating all 2^n subsets is infeasible for n=40 (about 1 trillion). Use meet-in-the-middle: split the array into two halves (size n1 = n/2 and n2 = n - n/2). Generate all possible subset sums for each half (without modulo initially, as sums can be up to 40*10^9 = 4*10^10, which fits in long long). Let L be the list of sums from the left half (size 2^n1), and R be the list from the right half (size 2^n2). The total subset sum is `l + r` for any l in L and r in R. We want to maximize `(l + r) mod M` where M = 1'000'000'000. For each l, we need the best r in R. Since modulo wraps around, the maximum possible value is M-1. For a fixed l, the best r is either:
// - The largest r such that `l + r` does not exceed M-1 (i.e., r ≤ M-1-l), giving sum = l+r (which is < M, so modulo is just l+r).
// - Or if no such r exists, the r that, when added to l, gives the smallest remainder after wrapping, i.e., the largest r overall would produce `(l + r) % M` = (l + r - M) which is smaller than any non-wrapping option; but also consider the case where l itself is large and r=0 works if that gives a non-wrapping result. Actually, to be safe, we sort R, then for each l we binary search for the last r ≤ M-1-l. If such r exists, candidate = l + r (since l+r < M). Also, consider the largest r in R (possibly wrapping) - but since we want maximum modulo, the non-wrapping candidate is always >= any wrapping candidate with the same l (because non-wrapping gives l+r, wrapping gives l+r-M, which is smaller). However, there is a subtlety: if no r ≤ M-1-l exists, then every r in R is > M-1-l, so all sums wrap. In that case the best is the smallest wrap, i.e., the smallest r in R (since l+r-M is minimized by smallest r? Actually, we want to maximize (l+r) % M = l+r - M, which increases with r, so largest r gives largest wrapped sum). So in that case consider r = largest element in R. Also, when l itself could be a subset sum from the left half and r=0 is in R (since empty subset from right half gives sum 0), that is always available. So the algorithm: generate L and R, sort R, for each l in L, binary search for the last index with R[idx] <= M-1-l, then candidate1 = l + R[idx] if idx >=0 else 0; candidate2 = l + R.back() (wrapping) but only if candidate2 >= M we take (candidate2 % M) but since we want max modulo, we actually compare candidate1 (which is already < M) and candidate2%M; but candidate1 >= candidate2%M? Not necessarily, so we must compute both properly. The standard method: for each l, let it = lower_bound(R, M-l) (first >= M-l). If it != R.begin(), then get r = R[it-1], candidate = (l+r) % M (this is exactly l+r because l+r < M). If it == R.end(), then all r are < M-l, so the largest r gives the best, candidate = l+R.back() (which is < M). If it == R.begin(), then all r >= M-l, so the best wrapping candidate is l+R.back() % M (since larger r gives larger wrapped sum). Actually to unify: for each l, compute two candidates: r1 = the largest r in R with r <= M-1-l (if exists), candidate1 = l+r1; r2 = the largest r in R overall (R.back()), candidate2 = (l+r2) % M; take max. Since r2 may also be <= M-1-l, candidate1 is at least candidate2 when candidate2 doesn't wrap, but if candidate2 wraps, candidate1 might not exist, so take max of available ones. Also need to consider that the subset must be non-empty; but since we can have l from left and r=0 (empty right) or r from right and l=0, the initial l=0 and r=0 would give sum 0 (empty subset), but we can exclude that by initializing answer to 0 and ensuring we only consider at least one element? Actually, since array can contain zeros, the non-empty subset sum could be 0 if all elements are zero, which is allowed. The problem says maximum value, and empty subset sum 0 is not allowed, but if there is any positive element, the answer will be positive. To be safe, if all sums are 0, answer is 0. So we just take the maximum over all pairs (l,r) except the pair (0,0) that comes from both empty halves? But since we compute candidates for each l in L, and r=0 is in R (empty right), then l from left non-empty gives a valid subset. If l=0 only (empty left), then r from right non-empty gives valid. So we can simply not exclude the (0,0) pair because if we take max, it will be 0, which is acceptable if all elements are zero. However, if there are positive numbers, the max will be >0. So just take max of all candidates (including (0,0)) is fine. Time complexity: generating lists takes O(2^n1 + 2^n2) with n1≈n/2, so O(2^(n/2)) which for n=40 is ~1 million each, fine. Sorting R takes O(2^(n/2) log 2^(n/2)). For each l (2^n1) binary search O(log 2^n2). Total O(2^(n/2) log 2^(n/2)). Space O(2^(n/2)). Edge cases: n=0? But task says array of up to 40, probably non-empty? Assume n>=1. If n=1, L has 2 elements (0 and a[0]), R has 2 elements (0 and a[0]? Actually split: n/2=0, so left half empty, L has only {0}; right half has the single element, R has {0, a[0]}. Then for l=0, best r is a[0]%M. Works.
