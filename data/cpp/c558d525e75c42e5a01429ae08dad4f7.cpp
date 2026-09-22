/*
Write a C++ function that takes two positive integers `x` and `k` (where `1 ≤ x, k ≤ 10^18`) and returns a vector of long long integers representing a decomposition of `x` into one or two positive integers, each of which is **not divisible by `k`**, and whose sum equals `x`. The function must return exactly one valid decomposition: if `x` itself is not divisible by `k`, return a single-element vector `{x}`; otherwise, return a two-element vector `{x-1, 1}` (which works because `x-1` is not divisible by `k` when `x` is, since `x` and `x-1` are coprime, and `1` is never divisible by any `k > 1`). The output order matters (the larger number first for the two-element case).
*/
#include <vector>

// Returns a decomposition of x into one or two positive integers, each not divisible by k.
// Assumes k >= 2 and x >= 1. If x is divisible by k, returns {x-1, 1}; else returns {x}.
std::vector<long long> decompose(long long x, long long k) {
    if (x % k != 0) {
        return {x};
    }
    return {x - 1, 1};
}
#include <iostream>
#include <cassert>
#include <vector>

// The solution function (included here for testing)
std::vector<long long> decompose(long long x, long long k) {
    if (x % k != 0) {
        return {x};
    }
    return {x - 1, 1};
}

int main() {
    // Test 1: x not divisible by k → single number
    assert(decompose(7, 3) == std::vector<long long>{7});

    // Test 2: x divisible by k → two numbers (x-1, 1)
    assert(decompose(6, 3) == std::vector<long long>{5, 1});

    // Test 3: large x divisible by k
    assert(decompose(1000000000000000000LL, 1000000000LL) == std::vector<long long>{999999999999999999LL, 1});

    // Test 4: large x not divisible by k
    assert(decompose(1000000000000000001LL, 1000000000LL) == std::vector<long long>{1000000000000000001LL});

    // Test 5: x=2, k=2 (smallest divisible case)
    assert(decompose(2, 2) == std::vector<long long>{1, 1});

    // Test 6: x=1, k=2 (not divisible)
    assert(decompose(1, 2) == std::vector<long long>{1});

    // Test 7: x=4, k=4
    assert(decompose(4, 4) == std::vector<long long>{3, 1});

    // Test 8: x=9, k=3
    assert(decompose(9, 3) == std::vector<long long>{8, 1});

    // Test 9: x=10, k=5
    assert(decompose(10, 5) == std::vector<long long>{9, 1});

    // Test 10: x=100, k=10
    assert(decompose(100, 10) == std::vector<long long>{99, 1});

    std::cout << "All tests passed.\n";
    return 0;
}
// The problem reduces to checking divisibility: if `x % k != 0`, then `x` alone is a valid single-number decomposition. If `x % k == 0`, we must split `x` into two positive integers that sum to `x` and neither is divisible by `k`. The simplest split is `x-1` and `1`. Since `x` is divisible by `k`, `x-1` is not divisible by `k` (because `k` divides `x` and if it also divided `x-1`, it would divide their difference `1`, impossible for `k > 1`). Also, `1` is only divisible by `k` if `k=1`, but since `k` is positive and `x` is divisible by `k`, if `k=1` then every number is divisible by `1`, so we’d need a different split. However, the original problem (as implied by the snippet) assumes `k ≥ 2`; if `k=1` is allowed, then no positive integer is "not divisible by 1", so the problem would be ill-posed. We assume `k ≥ 2` and `x ≥ 2` for the divisible case to ensure `x-1 ≥ 1`. Complexity: O(1) time and O(1) auxiliary space (the returned vector size is at most 2).
