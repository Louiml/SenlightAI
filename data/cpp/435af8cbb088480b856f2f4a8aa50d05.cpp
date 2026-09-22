Write a C++ function `pair<string, string> construct_hack(int n, int offset, mt19937& rng)` that generates two strings `A` and `B` of equal length `n` over lowercase letters `a`–`z` and `'*'`, such that the sum over all positions `i` of `a_i * b_i * (a_i - b_i)^2` modulo `998244353` is exactly `0`. Here `'a'` represents `offset`, `'b'` represents `offset+1`, ..., `'z'` represents `offset+25`, and `'*'` represents `0`. The function must work for any `n` that is a power of two, at least `2`, and must use a randomized meet-in-the-middle approach with a hash map. It should return a valid pair with high probability; you may assume `rng` is a random number generator seeded externally. The function must be self-contained (using only standard headers) and must not call any external test code.
The core challenge is constructing two length-`n` strings whose weighted sum modulo a prime is zero. A direct brute force is impossible for `n` up to `2^18` (262144). The solution uses meet-in-the-middle: split the string into two halves. Randomly fill the left half with characters, compute the cumulative sum of the contribution for each prefix of the left half, and store in a map the remainder of `sum_l` as key and the smallest index (or `1 + index`) as value. Then iteratively try random characters for the right half positions, maintaining the current sum `sum_r` of the right half. For each new right-half configuration, compute the required left remainder `k = (mod - sum_r) % mod`. If `k` is found in the map and the right-half sum is non-zero (to avoid trivial all-zero solution), we truncate the left half at the stored index (replace the rest with `'*'`), and we have found a valid pair. The algorithm is randomized: with high probability, a suitable match is found in a few iterations because the map covers many distinct remainders. Complexity: building the map takes `O(n/2)` time and `O(n/2)` space; the right-half loop is expected to terminate in `O(1)` map lookups after a few random trials, so total expected time is `O(n)` and space `O(n)`. Edge cases: `n` must be even; if `n` is odd, we can round down. Also ensure the left half is at least length 1. The function returns both strings of length `n` (left half possibly padded with `'*'` after truncation, and right half fully random). The sum modulo `mod` is guaranteed to be 0 by construction.
#include <string>
#include <vector>
#include <unordered_map>
#include <cassert>
#include <random>

// Generate two strings A and B of length n such that
// sum_i a_i * b_i * (a_i - b_i)^2 ≡ 0 (mod 998244353)
// where 'a' maps to offset, 'b' to offset+1, ..., 'z' to offset+25, and '*' to 0.
// n must be even and at least 2.
std::pair<std::string, std::string> construct_hack(int n, int offset, std::mt19937& rng) {
    assert(n >= 2 && n % 2 == 0);
    constexpr int MOD = 998244353;
    
    auto value_of = [&](char c) -> int {
        if (c == '*') return 0;
        return offset + (c - 'a');
    };
    
    auto contribution = [&](int a, int b) -> long long {
        long long diff = a - b;
        return 1LL * a * b * diff * diff;
    };
    
    auto random_char = [&]() -> char {
        int x = std::uniform_int_distribution<int>(0, 26)(rng);
        return (x == 26) ? '*' : static_cast<char>('a' + x);
    };
    
    int M = n / 2;
    std::string A(n, '*'), B(n, '*');
    
    // Fill left half randomly
    for (int i = 0; i < M; ++i) {
        A[i] = random_char();
        B[i] = random_char();
    }
    
    // Map from cumulative sum modulo MOD to (1 + index) for left half
    std::unordered_map<int, int> prefix_map;
    long long sum_l = 0;
    for (int i = 0; i < M; ++i) {
        sum_l += contribution(value_of(A[i]), value_of(B[i]));
        prefix_map[static_cast<int>(sum_l % MOD)] = i + 1; // store 1 + i
    }
    
    // Try random right half configurations
    long long sum_r = 0;
    while (true) {
        int i = std::uniform_int_distribution<int>(M, n - 1)(rng);
        sum_r -= contribution(value_of(A[i]), value_of(B[i]));
        A[i] = random_char();
        B[i] = random_char();
        sum_r += contribution(value_of(A[i]), value_of(B[i]));
        
        int need = static_cast<int>((MOD - (sum_r % MOD)) % MOD);
        auto it = prefix_map.find(need);
        if (it != prefix_map.end() && sum_r != 0) {
            int truncate_at = it->second; // 1 + index, so index = truncate_at - 1
            for (int j = truncate_at; j < M; ++j) {
                A[j] = '*';
                B[j] = '*';
            }
            // recompute sum_l properly (or just recompute all)
            long long final_sum = 0;
            for (int j = 0; j < n; ++j) {
                final_sum += contribution(value_of(A[j]), value_of(B[j]));
            }
            assert(final_sum % MOD == 0);
            break;
        }
    }
    
    return {A, B};
}
#include <cassert>
#include <random>
#include <string>
#include <utility>

// The solution function is declared above; in this test file we include its implementation directly.
// For clarity, we copy it here (in real usage, just link the solution).
// (The full solution code from above is assumed to be present.)

int main() {
    std::mt19937 rng(12345);
    
    auto check_sum = [&](const std::string& A, const std::string& B, int offset) {
        constexpr int MOD = 998244353;
        long long total = 0;
        for (size_t i = 0; i < A.size(); ++i) {
            int a = (A[i] == '*') ? 0 : offset + (A[i] - 'a');
            int b = (B[i] == '*') ? 0 : offset + (B[i] - 'a');
            long long diff = a - b;
            total += 1LL * a * b * diff * diff;
        }
        return total % MOD == 0;
    };
    
    // Test small cases with different offsets
    for (int n : {2, 4, 8, 16}) {
        for (int off : {0, 1, 97}) {
            auto [A, B] = construct_hack(n, off, rng);
            assert(A.size() == static_cast<size_t>(n));
            assert(B.size() == static_cast<size_t>(n));
            assert(check_sum(A, B, off));
        }
    }
    
    // Test larger power-of-two
    {
        int n = 64;
        auto [A, B] = construct_hack(n, 0, rng);
        assert(check_sum(A, B, 0));
    }
    {
        int n = 128;
        auto [A, B] = construct_hack(n, 25, rng);
        assert(check_sum(A, B, 25));
    }
    
    // Ensure not all stars (non-trivial solution)
    {
        int n = 4;
        auto [A, B] = construct_hack(n, 0, rng);
        bool any_star = false;
        for (char c : A) if (c != '*') any_star = true;
        assert(any_star);
    }
    
    return 0;
}
