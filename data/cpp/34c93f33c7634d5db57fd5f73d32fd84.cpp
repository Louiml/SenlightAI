/*
Given a sequence of integers read from standard input, write a standalone C++ function `long long countWays(const std::vector<long long>& a, long long k)` that returns the number of non-empty contiguous subarrays whose sum is divisible by `k`. The input vector `a` may contain negative numbers, zeros, and duplicate values, and `k` is a positive integer. The result may be large, so return it modulo `1,000,000,007`. The function must handle empty vectors (returning 0) and vectors with all zeros (where every subarray satisfies the condition). You are not required to implement input parsing; the function receives the vector and `k` directly.
*/
#include <vector>
#include <unordered_map>

// Count non-empty contiguous subarrays whose sum is divisible by k, modulo 1,000,000,007.
long long countWays(const std::vector<long long>& a, long long k) {
    const long long MOD = 1000000007LL;
    if (a.empty() || k <= 0) return 0;

    std::unordered_map<long long, long long> freq;
    freq[0] = 1; // prefix sum for empty prefix

    long long prefix = 0;
    long long ways = 0;

    for (long long value : a) {
        prefix = ((prefix + value) % k + k) % k;
        auto it = freq.find(prefix);
        if (it != freq.end()) {
            ways = (ways + it->second) % MOD;
            it->second = (it->second + 1) % MOD;
        } else {
            freq[prefix] = 1;
        }
    }

    return ways;
}
#include <cassert>
#include <vector>

long long countWays(const std::vector<long long>& a, long long k);

int main() {
    // Simple case: [1,2,3], k=3 -> subarrays: [3] (sum 3), [1,2] (sum 3), [1,2,3] (sum 6) => 3
    assert(countWays({1, 2, 3}, 3) == 3);

    // Negative numbers: [-1,2], k=3 -> prefix mods: 0, 2, 1 -> no pairs -> 0
    assert(countWays({-1, 2}, 3) == 0);

    // All zeros: [0,0], k=5 -> every subarray valid: 3 total
    assert(countWays({0, 0}, 5) == 3);

    // Empty vector
    assert(countWays({}, 1) == 0);

    // Single element divisible: [4], k=2 -> subarray [4] sum 4 divisible -> 1
    assert(countWays({4}, 2) == 1);

    // Single element not divisible: [5], k=3 -> 0
    assert(countWays({5}, 3) == 0);

    // Duplicate prefix mods: [1,1,1], k=3 -> prefix mods: 0,1,2,0 -> one pair from two 0s -> 1
    assert(countWays({1, 1, 1}, 3) == 1);

    // Large negative and positive: [1000000000, -1000000000], k=1 -> every subarray divisible -> 3
    assert(countWays({1000000000LL, -1000000000LL}, 1) == 3);

    // Large k (no wrap issues): [1,2,3], k=100 -> only [3]? Actually sums: [1],[2],[3],[1+2=3],[2+3=5],[1+2+3=6] - only sum 3 appears twice? Wait sum 3 from [3] and [1+2] -> 2
    assert(countWays({1, 2, 3}, 100) == 2);

    // Mixed with many mod equal: [3,3,3], k=6 -> prefix mods: 0,3,0,3 -> pairs: two 0s ->1, two 3s ->1 total 2
    assert(countWays({3, 3, 3}, 6) == 2);

    return 0;
}
// A naive approach enumerates all \(O(n^2)\) subarrays and checks each sum, which is too slow for large `n`. The key observation is that a subarray `a[i..j]` has a sum divisible by `k` if and only if the prefix sums `prefix[i-1]` and `prefix[j]` are congruent modulo `k` (i.e., `(prefix[j] - prefix[i-1]) % k == 0`). Therefore, we can compute prefix sums modulo `k`, and for each modulo value, count how many prefix sums share that value. For a given modulo class with `c` prefix sums (including the empty prefix at index -1, which is 0), the number of ways to choose two different prefix positions to form a valid subarray is `c * (c - 1) / 2`. Summing this over all classes gives the answer. We handle negative numbers by adjusting the modulo operation to produce non-negative results (`((sum % k) + k) % k`). Edge cases: empty vector returns 0; `k` positive always; all zeros leads to every prefix sum being 0 mod k, so if there are `n+1` prefix sums all equal 0, the count is `(n+1)*n/2`. Time complexity is `O(n)` and space complexity is `O(k)` for the hash map (or unordered_map). Use `long long` for counts and modulo `1,000,000,007`.
