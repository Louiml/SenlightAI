Given a positive integer `n` and a positive integer `k`, write a C++ function that returns a vector of `(n * k)` integers according to the following construction. If `n` is even, output `n/2` followed by all integers from `n` down to `1`, each repeated `k` times, except that the integer `n/2` is repeated only `k-1` times (since `n/2` was already output once). If `n` is odd and greater than 1, output the integer `(n+1)/2` repeated `k` times, then output `n/2` once, then all integers from `n` down to `1` each repeated `k` times, except exclude the integer `(n+1)/2` entirely, and repeat `n/2` only `k-1` times (since `n/2` was already output once). For `n == 1`, simply output `(n+1)/2` (which is 1) repeated `k` times. The final vector must contain exactly `n*k` elements. The function must be self-contained and return a `std::vector<long long>`; assume all inputs satisfy `n >= 1` and `k >= 1`.
The task is a straightforward simulation of the described output pattern, but careful attention is needed to avoid overcounting and to preserve the exact order. The core idea is to generate the sequence step by step, considering three distinct cases: even `n`, odd `n > 1`, and `n == 1`. For even `n`, we first push `n/2`, then iterate `i` from `n` down to `1`; when `i == n/2`, we push it `k-1` times, otherwise `k` times. This yields `1 + (k-1) + (n-1)*k = n*k` elements. For odd `n > 1`, we push `(n+1)/2` `k` times, then push `n/2` once, then iterate `i` from `n` down to `1`; we skip `i == (n+1)/2`, and if `i == n/2` we push it `k-1` times, otherwise push it `k` times. This yields `k + 1 + (k-1) + (n-2)*k = n*k` elements. For `n == 1`, we simply push 1 `k` times, which equals `n*k`. Edge cases include `n=2` (even, no special overlap beyond `n/2`), `n=3` (odd, `(n+1)/2 = 2`, `n/2 = 1`, so 2 repeated `k`, then 1 once, then from `i=3` down to 1: skip 2, for `i=1` repeat `k-1`, for `i=3` repeat `k`). Also `k=1` is valid; for even `n`, `n/2` appears once at start and zero times in reverse loop, which is correct. Time complexity is `O(n*k)` because we push exactly `n*k` elements. Space complexity is `O(1)` extra beyond the returned vector, which is `O(n*k)` in output size. We use a `std::vector<long long>` to store the result.
#include <vector>
#include <cstddef>

// Generate a vector of length n*k according to the specified pattern.
// n >= 1, k >= 1.
std::vector<long long> generateSequence(long long n, long long k) {
    std::vector<long long> result;
    result.reserve(static_cast<std::size_t>(n * k));

    if (n % 2 == 0) {
        // Even n: start with n/2.
        result.push_back(n / 2);
        // Iterate from n down to 1, repeat each k times except n/2 is repeated k-1 times.
        for (long long i = n; i >= 1; --i) {
            long long repeat = (i == n / 2) ? (k - 1) : k;
            for (long long j = 0; j < repeat; ++j) {
                result.push_back(i);
            }
        }
    } else if (n == 1) {
        // Special case n == 1: just output (n+1)/2 = 1 repeated k times.
        for (long long j = 0; j < k; ++j) {
            result.push_back(1);
        }
    } else {
        // Odd n > 1: first output (n+1)/2 repeated k times.
        for (long long j = 0; j < k; ++j) {
            result.push_back((n + 1) / 2);
        }
        // Then output n/2 once.
        result.push_back(n / 2);
        // Then iterate from n down to 1, skipping (n+1)/2 entirely,
        // and repeat n/2 only k-1 times.
        for (long long i = n; i >= 1; --i) {
            if (i == (n + 1) / 2) {
                continue;
            }
            long long repeat = (i == n / 2) ? (k - 1) : k;
            for (long long j = 0; j < repeat; ++j) {
                result.push_back(i);
            }
        }
    }

    return result;
}
#include <cassert>
#include <vector>
#include <cstddef>

// Declare the function (already defined above; here we repeat for test compilation).
std::vector<long long> generateSequence(long long n, long long k);

int main() {
    // n=1, k=3 -> [1,1,1]
    {
        std::vector<long long> expected = {1, 1, 1};
        assert(generateSequence(1, 3) == expected);
    }
    // n=2, k=2 -> [1, 2, 1] (since n/2=1, then 2 twice, 1 once)
    {
        std::vector<long long> expected = {1, 2, 2, 1};
        assert(generateSequence(2, 2) == expected);
    }
    // n=3, k=2 -> [2,2, 1, 3,3, 1]? Let's compute: (n+1)/2=2 repeated 2 -> 2,2; then n/2=1 once -> 1; then i=3: repeat 2 -> 3,3; i=2: skip; i=1: repeat k-1=1 -> 1. Final: 2,2,1,3,3,1.
    {
        std::vector<long long> expected = {2, 2, 1, 3, 3, 1};
        assert(generateSequence(3, 2) == expected);
    }
    // n=4, k=1 -> [2, 4,3,1] (n/2=2, then i=4..1 each once, i=2 repeat 0 times => skip)
    {
        std::vector<long long> expected = {2, 4, 3, 1};
        assert(generateSequence(4, 1) == expected);
    }
    // n=5, k=1 -> [3, 2, 5,4,1]
    {
        std::vector<long long> expected = {3, 2, 5, 4, 1};
        assert(generateSequence(5, 1) == expected);
    }
    // n=6, k=2 -> [3, 6,6,5,5,4,4,3,2,2,1,1]? Let's compute: n/2=3, then i=6..1, i=3 repeat 1, others repeat 2 => 3, 6,6,5,5,4,4,3,2,2,1,1. That's 1+1+5*2=12 elements.
    {
        std::vector<long long> expected = {3, 6,6,5,5,4,4,3,2,2,1,1};
        assert(generateSequence(6, 2) == expected);
    }
    // n=7, k=2 -> [4,4,3, 7,7,6,6,5,5,4,3,2,2,1,1]? Compute: (n+1)/2=4 repeated 2 -> 4,4; n/2=3 once -> 3; i=7..1: skip 4, i=3 repeat 1, others repeat 2 => 7,7,6,6,5,5,3,2,2,1,1. Actually wait: after 4,4,3, then i=7: two 7s; i=6: two 6s; i=5: two 5s; i=4: skip; i=3: one 3; i=2: two 2s; i=1: two 1s. So total: 4,4,3,7,7,6,6,5,5,3,2,2,1,1. That's 14 elements.
    {
        std::vector<long long> expected = {4,4,3,7,7,6,6,5,5,3,2,2,1,1};
        assert(generateSequence(7, 2) == expected);
    }
    // n=100, k=1: check size and first/last few.
    {
        auto v = generateSequence(100, 1);
        assert(v.size() == 100);
        assert(v[0] == 50);
        assert(v[1] == 100);
        assert(v[2] == 99);
        assert(v[99] == 1);
    }
    return 0;
}
