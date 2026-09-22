// Write a C++ function named `countDistinctPrimeDivisors` that takes a vector of positive integers (each ≤ 10^7) and a vector of query pairs `(l, r)`. For each query, the function must return a vector of integers, where the i-th result is the number of distinct prime numbers `p` (with `2 ≤ p ≤ 10^7`) such that at least one number in the input array is divisible by `p` and whose value `p` lies in the inclusive range `[l, r]`. The input array may contain duplicates, and each number may have repeated prime factors, but each distinct prime divisor is counted only once per number. The queries' `l` and `r` can be any integers; values outside `[2, 10^7]` should be clamped (i.e., treat `l < 2` as 2 and `r > 10^7` as 10^7). If after clamping `l > r`, return 0 for that query.

// The goal is to preprocess the input numbers to count, for each prime `p` up to `10^7`, whether `p` divides at least one input number. Then build a prefix sum array over primes (indexed by prime value) so that any range query `[l, r]` can be answered in O(1) using prefix sums. The main challenge is efficiently finding distinct prime factors for each input number without iterating over all primes per number. Use a sieve to precompute the smallest prime factor (SPF) for every number up to `10^7`. Then for each input number, repeatedly divide by its SPF, and for each distinct prime factor encountered, increment a count array. After processing all numbers, convert the count array into a cumulative prefix sum (where `pref[i] = number of distinct primes ≤ i that divide at least one input number`). Then answer queries by subtracting `pref[l-1]` from `pref[r]` after clamping. Edge cases: numbers equal to 1 have no prime factors; if all input numbers are 1, every query returns 0. Time complexity: O(N log log N) for sieve + O(total prime factors of inputs) for processing, each query O(1). Space complexity: O(10^7) for SPF and prefix arrays.

#include <vector>
#include <algorithm>

// Precompute smallest prime factor (SPF) for all numbers up to LIMIT.
std::vector<int> computeSPF(int limit) {
    std::vector<int> spf(limit + 1);
    for (int i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (static_cast<long long>(i) * i <= limit) {
                for (int j = i * i; j <= limit; j += i) {
                    if (spf[j] == 0) {
                        spf[j] = i;
                    }
                }
            }
        }
    }
    return spf;
}

// Given an array of numbers and queries, for each query return the count of distinct
// prime divisors p (2 ≤ p ≤ 10^7) that divide at least one number and lie in [l, r].
std::vector<int> countDistinctPrimeDivisors(const std::vector<int>& numbers, const std::vector<std::pair<int, int>>& queries) {
    const int LIMIT = 10000000;
    const std::vector<int> spf = computeSPF(LIMIT);

    // boolean array: true if prime p divides at least one input number.
    std::vector<char> present(LIMIT + 1, 0);
    for (int num : numbers) {
        int x = num;
        while (x > 1) {
            int p = spf[x];
            present[p] = 1;
            while (x % p == 0) {
                x /= p;
            }
        }
    }

    // prefix sum: pref[i] = number of distinct primes ≤ i that divide at least one number.
    std::vector<int> pref(LIMIT + 1, 0);
    for (int i = 2; i <= LIMIT; ++i) {
        pref[i] = pref[i - 1] + (present[i] ? 1 : 0);
    }

    std::vector<int> results;
    results.reserve(queries.size());
    for (const auto& q : queries) {
        int l = std::max(q.first, 2);
        int r = std::min(q.second, LIMIT);
        if (l > r) {
            results.push_back(0);
        } else {
            results.push_back(pref[r] - pref[l - 1]);
        }
    }
    return results;
}

#include <cassert>
#include <vector>

int main() {
    // Basic case: numbers = [10, 15] → primes {2,3,5}. Query [1,3] → {2,3} = 2.
    {
        std::vector<int> nums = {10, 15};
        std::vector<std::pair<int,int>> queries = {{1, 3}};
        auto res = countDistinctPrimeDivisors(nums, queries);
        assert(res.size() == 1);
        assert(res[0] == 2);
    }
    // Repeated prime factors: 8 = 2^3, 9 = 3^2 → {2,3}. Query [2,2] → 1.
    {
        std::vector<int> nums = {8, 9};
        std::vector<std::pair<int,int>> queries = {{2, 2}};
        auto res = countDistinctPrimeDivisors(nums, queries);
        assert(res[0] == 1);
    }
    // All numbers are 1 → no primes → any query returns 0.
    {
        std::vector<int> nums = {1, 1, 1};
        std::vector<std::pair<int,int>> queries = {{2, 100}};
        auto res = countDistinctPrimeDivisors(nums, queries);
        assert(res[0] == 0);
    }
    // Prime includes itself: number = 7 → {7}. Query [7,7] → 1.
    {
        std::vector<int> nums = {7};
        std::vector<std::pair<int,int>> queries = {{7, 7}};
        auto res = countDistinctPrimeDivisors(nums, queries);
        assert(res[0] == 1);
    }
    // Query range outside [2,10^7] is clamped: nums = {2,3} → {2,3}, query [0,1] → 0.
    {
        std::vector<int> nums = {2, 3};
        std::vector<std::pair<int,int>> queries = {{0, 1}};
        auto res = countDistinctPrimeDivisors(nums, queries);
        assert(res[0] == 0);
    }
    // Multiple queries: nums = {6, 10} → {2,3,5}. Queries: [2,3]→2, [4,5]→1, [2,5]→3.
    {
        std::vector<int> nums = {6, 10};
        std::vector<std::pair<int,int>> queries = {{2, 3}, {4, 5}, {2, 5}};
        auto res = countDistinctPrimeDivisors(nums, queries);
        assert(res[0] == 2);
        assert(res[1] == 1);
        assert(res[2] == 3);
    }
    // Query with l > r after clamping: nums = {2}, query [8,5] → 0.
    {
        std::vector<int> nums = {2};
        std::vector<std::pair<int,int>> queries = {{8, 5}};
        auto res = countDistinctPrimeDivisors(nums, queries);
        assert(res[0] == 0);
    }
    // Large prime near limit: assume number = 9999991 is prime (let's check minimal).
    // Use 9999991; it is a known prime. Query [9999991,10000000] → 1.
    {
        std::vector<int> nums = {9999991};
        std::vector<std::pair<int,int>> queries = {{9999991, 10000000}};
        auto res = countDistinctPrimeDivisors(nums, queries);
        assert(res[0] == 1);
    }
    return 0;
}
