// Write a C++ function `long long sumXorPairwise(const std::vector<long long>& nums)` that takes a vector of `n` non-negative integers (where `n` is at least 1 and can be up to 10^5) and returns the sum of `(a_i XOR a_j)` for all ordered pairs `(i, j)` where `i < j`, modulo `1,000,000,007`. In other words, compute the sum of the bitwise XOR of every unordered pair of distinct elements in the array. The function must handle numbers up to 2^60 - 1.
// The naive approach of iterating over all pairs takes O(n^2) and will be too slow for large n. Instead, we observe that XOR addition can be computed bit by bit independently. For each bit position `k` (from 0 to 59), the contribution of that bit to the total XOR sum is determined by how many numbers in the array have that bit set. If `c` numbers have bit `k` set, then `(n - c)` numbers have it clear. The XOR of a pair contributes `2^k` if and only if exactly one of the two numbers in the pair has that bit set. The number of such unordered pairs is `c * (n - c)`. So for each bit, add `2^k * c * (n - c)` to the answer, all modulo MOD. We can compute `2^k mod MOD` iteratively. Edge cases: when `c == 0` or `c == n`, contribution is 0; when the array has one element, the result is 0. Time complexity: O(60 * n) = O(n) (since 60 is constant), space complexity: O(1) auxiliary plus the input vector.
#include <vector>

// Sum of XOR over all unordered pairs modulo 1e9+7.
// nums: vector of non-negative integers, size >= 1.
long long sumXorPairwise(const std::vector<long long>& nums) {
    const long long MOD = 1000000007LL;
    const int MAX_BITS = 60;
    int n = static_cast<int>(nums.size());

    long long ans = 0;
    long long powerOfTwo = 1; // 2^0 mod MOD

    for (int bit = 0; bit < MAX_BITS; ++bit) {
        long long countOnes = 0;
        for (long long x : nums) {
            if ((x >> bit) & 1LL) {
                ++countOnes;
            }
        }
        long long countZeros = n - countOnes;
        long long pairs = (countOnes % MOD) * (countZeros % MOD) % MOD;
        long long contribution = (powerOfTwo * pairs) % MOD;
        ans = (ans + contribution) % MOD;

        powerOfTwo = (powerOfTwo * 2) % MOD;
    }
    return ans;
}
#include <cassert>
#include <vector>

// Declaration of the function being tested
long long sumXorPairwise(const std::vector<long long>& nums);

int main() {
    assert(sumXorPairwise({1}) == 0); // single element
    assert(sumXorPairwise({1, 2}) == 3); // 1^2 = 3
    assert(sumXorPairwise({0, 0, 0}) == 0);
    assert(sumXorPairwise({1, 1, 1}) == 0); // all same
    assert(sumXorPairwise({1, 2, 3}) == 6); // (1^2)+(1^3)+(2^3)=3+2+1=6
    assert(sumXorPairwise({5, 5, 5}) == 0);
    assert(sumXorPairwise({7, 3, 5, 1}) == (4 + 2 + 6 + 6 + 2 + 4)); // 7^3=4, 7^5=2, 7^1=6, 3^5=6, 3^1=2, 5^1=4 => 24
    assert(sumXorPairwise({1000000000, 999999999}) == (1000000000LL ^ 999999999LL));
    // Large test: all pairs of two numbers with mixed bits
    std::vector<long long> v = {0, 1, 2, 3, 4, 5, 6, 7};
    long long expected = 0;
    for (size_t i = 0; i < v.size(); ++i) {
        for (size_t j = i+1; j < v.size(); ++j) {
            expected = (expected + (v[i] ^ v[j])) % 1000000007LL;
        }
    }
    assert(sumXorPairwise(v) == expected);
    return 0;
}
