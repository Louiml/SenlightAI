Write a C++ function that accepts a single positive integer `n` and returns a vector of all its positive divisors in ascending order. The function should handle the case `n == 1` by returning a vector containing only `1`. You may assume the input is always a positive integer, but the function should be robust if called with edge values (e.g., 1, 2, a prime, a perfect square, and a larger composite). Do not include any `main` function in your solution; provide only the free function and necessary headers.

#include <cassert>
#include <vector>

std::vector<int> getDivisors(int n);

int main() {
    assert(getDivisors(1) == std::vector<int>{1});
    assert(getDivisors(2) == std::vector<int>{1, 2});
    assert(getDivisors(7) == std::vector<int>{1, 7});
    assert(getDivisors(12) == std::vector<int>{1, 2, 3, 4, 6, 12});
    assert(getDivisors(16) == std::vector<int>{1, 2, 4, 8, 16});
    assert(getDivisors(36) == std::vector<int>{1, 2, 3, 4, 6, 9, 12, 18, 36});
    assert(getDivisors(100) == std::vector<int>{1, 2, 4, 5, 10, 20, 25, 50, 100});
    return 0;
}

#include <vector>

// Return all positive divisors of n in ascending order.
std::vector<int> getDivisors(int n) {
    std::vector<int> divisors;
    for (int i = 1; i <= n; ++i) {
        if (n % i == 0) {
            divisors.push_back(i);
        }
    }
    return divisors;
}

// The algorithm iterates from `1` up to `n` and checks for divisibility using the modulo operator. For each `i` where `n % i == 0`, we append `i` to the result vector. Since we iterate in increasing order, the resulting vector is automatically sorted ascending. The main edge case is `n == 1`, which yields the single divisor `1` (the loop runs once and finds it). For primes, only `1` and itself are divisors. For perfect squares, like `36`, divisors include `6`, which appears exactly once since we only insert when `i` is a divisor. Time complexity is `O(n)` because we iterate exactly `n` times, and each insertion is `O(1)` amortized; space complexity is `O(d)` where `d` is the number of divisors, but worst-case `d` is `O(n)` (e.g., for `n = 1` or highly composite numbers). No special handling of negative input is needed per the task specification.
